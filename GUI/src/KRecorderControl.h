#ifndef _H_K_RECORDER_CONTROL_
#define _H_K_RECORDER_CONTROL_


#define MAX_FILE_BUF 1024

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include "KInput.h" 
#include "KRecordPlot.h"

#include "Config.h"
#include "RtInput.h"
#include "WAV.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <chrono>
#include <atomic>
#include <time.h>
#include <map>

#if _WIN32
// for keyborad input
#include <windows.h>
#endif

//#include <QSystemTrayIcon>
//#include <QMenu>
//#include <QAction>

enum _rec_mode { MANUAL, TIMER, INTERVAL };

class KRecorderControl : public QWidget{
  Q_OBJECT

  private:
    const static bool default_scale = true;
    const static bool default_split = false;

	  QVBoxLayout layout_base;
    KRecordPlotRec widget_plot;
    
  QWidget widget_recorder;


  QVBoxLayout layout_recorder;
  QHBoxLayout  layout_plot_control;
  QLabel label_ch_plot;
  QPushButton btn_ch_plot_next;

  QPushButton btn_save_path;
  QLabel label_save_path;
  // label_stat
  QLabel label_status;
  QPushButton btn_init;

  QWidget widget_mode;
	  QHBoxLayout layout_mode;
	  QLabel label_mode;
	  QComboBox combo_mode;

  QWidget widget_interval;
	  QHBoxLayout layout_interval;
	  QLabel label_interval;
	  QComboBox combo_interval;

   QWidget widget_timer;
    QHBoxLayout layout_timer;
    QLabel label_timer;
    QLineEdit LE_timer;
    
    /* input scaling */
    const static int max_ch = 16;
    QGridLayout layout_scale_ch;
    /*
    // Dynamically allocated by initializer.
    QLabel label_scale[max_ch];
    QLineEdit LE_scale[max_ch];
    
    double scale[max_ch];
    */
    QLabel label_scale;
    QLineEdit LE_scale;
    double scale;

    /* output splitting */
    QHBoxLayout layout_split_wav;
    QLabel label_split_wav;
    QCheckBox check_split_wav;
    QLabel label_log;
    QCheckBox check_log;
    bool do_split_wav;
    short** buf_split;

  QPushButton btn_record;
  QLabel label_time;
  
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
  RtInput *rt;
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
  void StopTimer();
  void StartRecord();
  void SlotToggleRecordnig();

signals:
	/* Should not Stop Timer from another thread. */
  void SignalStopTimer();
  void SignalRefreshTimer();
  void SignalStartRecord();
  void SignalReturnFilePath(QString path);

  public:
#if _WIN32
    void KeyboardInput();
#endif
    void ToggleInteract(bool);
	  KRecorderControl();
	  ~KRecorderControl();
};


#endif
