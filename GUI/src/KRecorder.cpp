#include "KRecorder.h"

KRecorder::KRecorder() :QWidget() {
  // disable window maximization
  this->setMaximumSize(QSize(400, 580));

  this->setWindowFlags(Qt::WindowTitleHint |Qt::WindowCloseButtonHint);
  widget_tab.setFixedSize(QSize(400, 580));
  widget_tab.addTab(&widget_recorder, "Recorder");
  widget_tab.addTab(&widget_input, "Input");
  widget_tab.addTab(&widget_param, "Param");
  layout_main.addWidget(&widget_tab);
  setLayout(&layout_main);

  /* Start at Right Top Corner */
   QScreen* screen = QGuiApplication::primaryScreen();
   QRect  screenGeometry = screen->geometry();
   QSize size = this->sizeHint();
   int width = screenGeometry.width();
   this->move(width - size.width(), 0);

  setStyleSheet("\
      QWidget{background:rgb(226, 228, 175);}\
      QLabel{background:white;border: 1px solid black;}\
      QPushButton{color:black;}\
      QComboBox{color:black;background:white}\
      QTextBrowser{color:black;}\
      QLabel:disabled{color:gray;}\
      QPushButton:disabled{color:gray;}\
      QComboBox:disabled{color:gray;}\
      QTextBrowser:disabled{color:gray;}\
      QCheckBox:indicator{background:rgb(210, 53, 50);border: 1px solid;}\
      QCheckBox:indicator:checked{background:rgb(79,214,130); border: 1px solid;}\
      \
      ");

  isRecording = false;
  QObject::connect(this, &KRecorder::SignalToggleRecording, &widget_recorder, &KRecorderControl::SlotToggleRecordnig);
  QObject::connect(&widget_recorder, &KRecorderControl::SignalReturnFilePath, this, &KRecorder::SlotReturnFilePath);
}

void KRecorder::ToggleRecorderInteract(bool flag) {
  widget_recorder.ToggleInteract(flag);
}

void KRecorder::SlotToggleRecording() {
 isRecording = !isRecording;
 emit(SignalToggleRecording());
}

void KRecorder::SlotReturnFilePath(QString path,double chrono_elapsed) {
  emit(SignalRecordFinished(path,chrono_elapsed));
}

  

