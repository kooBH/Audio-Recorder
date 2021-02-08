#include "KRecorder.h"

KRecorder::KRecorder() :QWidget() {
  widget_tab.setFixedSize(QSize(400, 580));
  widget_tab.addTab(&widget_recorder, "Recorder");
  widget_tab.addTab(&widget_input, "Input");
  widget_tab.addTab(&widget_param, "Param");
  layout_main.addWidget(&widget_tab);
  setLayout(&layout_main);

  /* Start at Right Top Corner */
  // QSize size = widget_main.sizeHint();
  // QRect rec = QApplication::desktop()->screenGeometry();
  // int width = rec.width();
  // widget_main.move(width - size.width(), 0);

  setStyleSheet("\
      QWidget{background:rgb(226, 228, 175); border: 1px solid black;}\
      QLabel{background:white;border: 1px solid black;}\
      QPushButton{color:black;}\
      QComboBox{color:black;}\
      QTextBrowser{color:black;}\
      QLabel:disabled{color:gray;}\
      QPushButton:disabled{color:gray;}\
      QComboBox:disabled{color:gray;}\
      QTextBrowser:disabled{color:gray;}\
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

void KRecorder::SlotReturnFilePath(QString path) {
  emit(SignalRecordFinished(path));
}

  

