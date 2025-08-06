#include "KRecorder.h"

KRecorder::KRecorder() :QWidget() {
  // disable window maximization
  this->setMaximumSize(QSize(400, 580));

  this->setWindowFlags(Qt::WindowTitleHint |Qt::WindowCloseButtonHint);
  widget_tab.setFixedSize(QSize(400, 580));
  widget_tab.addTab(&widget_recorder, "Control");
  widget_tab.addTab(&widget_input, "Deivce");
  widget_tab.addTab(&widget_param, "Params");
  widget_tab.addTab(&widget_port, "AMEMS");
  layout_main.addWidget(&widget_tab);

  label_footer.setText("");
  label_footer.setText("<a href=\"https://github.com/kooBH/Audio-Recorder\" style=\"color:black\"; >Contact</a>");
  label_footer.setTextInteractionFlags(Qt::TextBrowserInteraction);
  label_footer.setOpenExternalLinks(true);  
  label_footer.setObjectName("footerLabel");

  layout_footer.addWidget(&label_footer);
  layout_footer.setAlignment(Qt::AlignRight);
  layout_main.addLayout(&layout_footer);
  setLayout(&layout_main);

  /* Start at Right Top Corner */
  /*
   QScreen* screen = QGuiApplication::primaryScreen();
   QRect  screenGeometry = screen->geometry();
   QSize size = this->sizeHint();
   int width = screenGeometry.width();
   this->move(width - size.width(), 0);
   */
  setStyleSheet("\
      QWidget{background:rgb(240, 245, 255);}\
      QLabel{background:white;border: 1px solid black;}\
      QLabel#footerLabel { background: rgba(0, 0, 0, 0); color: black; border: 0px }\
      QPushButton{color:black;background:rgb(220,230,255);}\
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

  

