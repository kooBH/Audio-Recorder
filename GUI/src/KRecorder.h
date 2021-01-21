#define MAX_FILE_BUF 1024

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "K/KInput.h" 
#include "K/KParam.h" 
#include "K/KLabel.h" 
#include "K/KPushButton.h"
#include "K/KComboBox.h"

#include "K/KRecordPlot_rec.h"

#include "util/Config.h"
#include "util/RT_Input.h"
#include "util/WAV/WAV.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <chrono>
#include <atomic>
#include <time.h>
#include <map>

#include "verdigris/wobjectdefs.h"
#if _WIN32
// for keyborad input
#include <windows.h>
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

//#include <QSystemTrayIcon>
//#include <QMenu>
//#include <QAction>

enum _rec_mode { MANUAL, TIMER, INTERVAL };

class KRecorder : public KWidget{
	W_OBJECT(KRecorder)

  private:
    const static bool default_scale = true;
    const static bool default_split = false;


	  QVBoxLayout layout_base;
    KRecordPlotRec widget_plot;
    
  KWidget widget_recorder;
  QVBoxLayout layout_recorder;
  QHBoxLayout  layout_plot_control;
    KLabel label_ch_plot;
    KPushButton btn_ch_plot_next;

  KPushButton btn_save_path;
  KLabel label_save_path;
  // label_stat
  KLabel label_status;
  KPushButton btn_init;

  KWidget widget_mode;
	  QHBoxLayout layout_mode;
	  KLabel label_mode;
	  KComboBox combo_mode;

  KWidget widget_interval;
	  QHBoxLayout layout_interval;
	  KLabel label_interval;
	  KComboBox combo_interval;

   KWidget widget_timer;
    QHBoxLayout layout_timer;
    KLabel label_timer;
    QLineEdit LE_timer;
    
    /* input scaling */
    const static int max_ch = 16;
    QGridLayout layout_scale_ch;
    /*
    // Dynamically allocated by initializer.
    KLabel label_scale[max_ch];
    QLineEdit LE_scale[max_ch];
    
    double scale[max_ch];
    */
    KLabel label_scale;
    QLineEdit LE_scale;
    double scale;

    /* output splitting */
    QHBoxLayout layout_split_wav;
    KLabel label_split_wav;
    QCheckBox check_split_wav;
    KLabel label_log;
    QCheckBox check_log;
    bool do_split_wav;
    short** buf_split;

  KPushButton btn_record;
  KLabel label_time;
  
  _rec_mode rec_mode;

  bool isRecording;
  bool isInited;

  QString file_path;
  QString file_path_buf;
  QString qstring_file_name;
  char file_name[MAX_FILE_BUF];
  char t_file_name[MAX_FILE_BUF];
  char ch_file_name[MAX_FILE_BUF];

#if _WIN32
  const QString text_record = "Record[F6]";
#else
const QString text_record = "Record";
#endif

  int shift_size;
  int frame_size;//unused

  int sample_rate;
  int channels;
  int device;

  /* Timer */
  QTimer timer;

  std::atomic<bool> is_setting_time;
  std::chrono::system_clock::time_point chrono_start;
  std::chrono::duration<double> chrono_elapsed;

  /* interval for stopwatch widget updating */
  const int timer_delay = 50;

  short *temp;
  short* temp_plot;
  int ch_plot=0;

  /* Recording */
  RT_Input *rt;
  WAV **out;

  std::thread *rec_thread;
  std::thread *wait_thread;
  std::thread *interval_thread;
  bool is_interval_running;

  double record_time;

  void SetDateTime(){
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(file_name,sizeof(file_name),"%Y-%m-%d_%H-%M-%S.wav",timeinfo);
  }

  void Recording();

  void LoadFilePath();

  int BuildModule();
  /* Wait for RT_Input to stop and Close file & refresh Widgets */
  void WaitRecording();

  /* Timer */
  void TimerRecording(int stock);
  unsigned long recording_timer;

  /* Interval */
  void IntervalRecording(int stock);
  int recording_interval;

  void EnableSetting(bool);
  void SetFileName();

  void ClearBuild();
  /*AfterProcess of Wav file*/
  void AfterFinish();

 
  //QSystemTrayIcon* trayIcon;
#if _WIN32
  std::thread* thread_key;
  bool fF4 = false;
  bool fF6 = false;
  
#endif
public slots:
	void SetTime();
	W_SLOT(SetTime)

		void StopTimer();
	W_SLOT(StopTimer)

    void StartRecord();
  W_SLOT(StartRecord);

public:
	/* Can't Stop Timer from another thread. */
	void SignalStopTimer() 
	W_SIGNAL(SignalStopTimer)

  void SignalRefreshTimer()
  W_SIGNAL(SignalRefreshTimer)

  void SignalStartRecord()
    W_SIGNAL(SignalStartRecord)

  public:
#if _WIN32
    void KeyboardInput();
#endif
	  KRecorder();
	  ~KRecorder();
};


