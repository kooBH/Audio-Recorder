#ifndef _H_K_RECORDER_
#define _H_K_RECORDER_

#include <QWidget>
//#include <QScreen>
#include <QTabWidget>

#include "KRecorderControl.h"
#include "KParam.h"
#include "KInput.h"

class KRecorder : public QWidget {
  private:

    QHBoxLayout layout_main;
    QTabWidget widget_tab;
    KRecorderControl w1;
    KInput w2;
    KParam w3;

public : 
  KRecorder() {
    widget_tab.setFixedSize(QSize(400,580));
    widget_tab.addTab(&w1, "Rec");
    widget_tab.addTab(&w2, "Input");
    widget_tab.addTab(&w3, "Param");
    layout_main.addWidget(&widget_tab);
    setLayout(&layout_main);

    /* Start at Right Top Corner */
    // QSize size = widget_main.sizeHint();
    // QRect rec = QApplication::desktop()->screenGeometry();
    // int width = rec.width();
    // widget_main.move(width - size.width(), 0);

      setStyleSheet("\
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
  }
    


};

#endif
