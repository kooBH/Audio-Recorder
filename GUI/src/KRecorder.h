#ifndef _H_K_RECORDER_
#define _H_K_RECORDER_

#include <QWidget>
#include <QScreen>
#include <QTabWidget>
#include <QApplication>
#include <QGuiApplication>

#include "KRecorderControl.h"
#include "KParam.h"
//#include "KInput.h"

class KRecorder : public QWidget {
  Q_OBJECT
  private:
    QHBoxLayout layout_main;
    QTabWidget widget_tab;
    KRecorderControl widget_recorder;
    KInput widget_input;
    KParam widget_param;

    bool isRecording;

public : 
  KRecorder();
  
  void ToggleRecorderInteract(bool);

  public slots:
  void SlotToggleRecording();
  void SlotReturnFilePath(QString path,double elapsed);


signals:
  void SignalToggleRecording();
  void SignalRecordFinished(QString flle_path,double elapsed);

};

#endif
