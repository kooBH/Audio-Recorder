#include <fstream>

#include "json.hpp"
using json = nlohmann::json;
#include "Recorder.h"

#if _WIN32
// for keyborad input
#include <windows.h>
#endif

#if _WIN32
  void KeyboardInput();
#endif

/* Glbal Params */
Recorder* recorder=nullptr;
bool fF4 = false;
bool fF6 = false;
bool isRecording = false;
int device     = 0;
int channels   = 1;
int samplerate  = 16000;
int shift_size = 2048;
double scale = 1.0;

int main(){

  /* Init */
  std::ifstream ifs("config.json");
  json j = json::parse(ifs);
  ifs.close();

  device     = j["device"].get<int>();
  channels   = j["channels"].get<int>();
  samplerate = j["samplerate"].get<int>();
  shift_size = j["shift_size"].get<int>();
  scale      = j["scale"].get<double>();

  recorder = new Recorder("../", channels, device, samplerate, scale);

  recorder->Process();
		
	return 0;
}

#if _WIN32
void KeyboardInput() {
  bool run = true;

  while (run) {
    //F4
    if (GetKeyState(VK_F4) < 0 && !fF4) {
      fF4 = true;
      recorder->OpenDevice();
      //std::cout << "F4 pressed" << std::endl;
    }
    if (GetKeyState(VK_F4) >= 0 && fF4) {
      //  std::cout << "F4 Released" << std::endl;
      fF4 = false;
    }
    //F6
    if (GetKeyState(VK_F6) < 0 && !fF6) {
      fF6 = true;

      if (isRecording) {
        recorder->Stop();
        isRecording = false;
      }
      else {
        recorder->Process();
        isRecording = true;
      }
    }
    if (GetKeyState(VK_F6) >= 0 && fF6) {
      //  std::cout << "F6 Released" << std::endl;
      fF6 = false;
    }
    //Exit
    /*
       if ((GetKeyState(VK_F4) & 0x8000))
       {
       std::cout << "F4 pressed" << std::endl;
       run_main = false;
       run = false;
       }
       */
    SLEEP(10);
  }
}
#endif