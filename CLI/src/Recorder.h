#ifndef _H_RECORDER_
#define _H_RECORDER_

/* General */
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>

/* Process */
#include "RtInput.h"
#include "WAV.h"
#include "time.h"

class Recorder{

private:
  /* General */
  std::atomic<bool>flag_recording;

  std::condition_variable cv;
  std::mutex mtx;
  std::unique_lock<std::mutex> *lock;

  /* Process */
  short* raw; 

  // fundamental parameters
  int input_size;
  int device,channels,samplerate,shift_size,frame_size;

  RtAudio audio;
  RtInput *input;
  WAV* output;
  std::string file_path;
  std::string file_name;

  char tmp_str[512];

  bool flag_finish;
  double scale;

  void SetDateTime() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(tmp_str, sizeof(file_name), "%Y-%m-%d_%H-%M-%S.wav", timeinfo);
    file_name = std::string(tmp_str);
  }

 
public:
  inline Recorder(std::string path,int channels,int device,int samplerate,double scale=1.0);
  inline ~Recorder();

  inline void OpenDevice(int device);
  inline void Process();
  inline void Stop();
};

Recorder::Recorder(std::string path, int channels_,int device,int samplerate_, double scale_){
  channels = channels_;
  samplerate = samplerate_;
  // set input_size larget for stable processing
  input_size = 2048;
  shift_size = 128;
  frame_size = 512;
  scale = scale_;
  file_path = path;

  /* General */
  input = nullptr;
  flag_recording.store(false);
  flag_finish = false;

  lock = new std::unique_lock<std::mutex>(mtx);

  /* PROCESS */
  output = new WAV(channels,samplerate);
  raw = new short[channels*shift_size];
}

Recorder::~Recorder() {
  delete[] raw;
  delete input;
  delete output;
  delete lock;
}


void Recorder::OpenDevice(int device_) {
  device= device_;
  input= new RtInput(device,channels,samplerate,shift_size,frame_size,input_size);
  printf("Recorder initialized\n");
}

void Recorder::Process(){

  flag_finish= false;
  flag_recording.store(true);

  SetDateTime();
  input->Start();
  output->NewFile(std::string(file_path +'/'+ file_name).c_str());

  // until resolve all left buffers
  while(flag_recording.load() || input->data.stock.load() > shift_size){
    // enough buffer to read
    if (input->data.stock.load() > shift_size) {
      input->GetBuffer(raw);

      if(scale != 1)
        for (int i = 0; i < shift_size * channels; i++) {
          raw[i] *= scale;
        }

      output->Append(raw,shift_size*channels);
    // not  enough buffer
    }else {
      SLEEP(10);
    }
  }
  input->Stop();
  output->Finish();
  flag_finish = true;
}

void Recorder::Stop(){
  flag_recording.store(false);
  while(! flag_finish){
    SLEEP(100);
  }
}

#endif