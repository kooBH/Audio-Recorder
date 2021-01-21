#include <Qt>
#include <QApplication>
#include <QDesktopWidget>
#include <QTabWidget>

#include "K/KRecorder.h"

#ifdef _WIN32
#include <Windows.h>
#endif

class RApplication :public QApplication {
private:
  KWidget widget_main;
  QHBoxLayout layout_main;
  QTabWidget widget_tab;
  KRecorder w1;
  KInput w2;
  KParam w3;
  
public:
  RApplication(int& argc, char** argv);
  ~RApplication();

};

RApplication::RApplication(int& argc, char** argv) :QApplication(argc, argv) {
  w1.setStyleSheet(_BG_COLOR_2_);
  widget_tab.setFixedSize(QSize(400,580));
  widget_tab.addTab(&w1, "Rec");
  widget_tab.addTab(&w2, "Input");
  widget_tab.addTab(&w3, "Param");
  layout_main.addWidget(&widget_tab);
  widget_main.setLayout(&layout_main);
  widget_main.setStyleSheet(_BG_COLOR_1_);
  widget_main.show();

  /*Start at Right Top Corner */
  QSize size = widget_main.sizeHint();
  QRect rec = QApplication::desktop()->screenGeometry();
  int width = rec.width();

  widget_main.move(width - size.width(), 0);
}

RApplication::~RApplication() {
  
}


int main(int argc, char* argv[]){
  printf("Loading....\n");

  //path to dll 
  QCoreApplication::addLibraryPath(".");

  //path to so
  QCoreApplication::addLibraryPath("../lib");
  //QCoreApplication::addLibraryPath("../lib/platfrominputcontexts");

  RApplication app(argc,argv);
  app.setWindowIcon(QIcon("../resource/record.ico"));
#ifdef _WIN32
	ShowWindow(::GetConsoleWindow(), SW_HIDE);
  //ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
	
/*
  int id = QFontDatabase::addApplicationFont(QString::fromStdString(_HOME_DIR)  
           + QString::fromStdString("/resource/SDMiSaeng.ttf"));      
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);                           
        QFont SerifFont(family);
	app.setFont(SerifFont);
*/  

/* Since default Text Color of MacOS is white - not sure -. 
 * Set Text Color of Each Component by SetStyleSheet()
 * */
#ifdef __APPLE__
  app.setStyleSheet("\
      QLabel{color:black;}\
      QPushButton{color:black;}\
      QComboBox{color:black;}\
      QTextBrowser{color:black;}\
      QLabel:disabled{color:gray;}\
      QPushButton:disabled{color:gray;}\
      QComboBox:disabled{color:gray;}\
      QTextBrowser:disabled{color:gray;}\
      \
      ");
#endif
  app.exec();
 
  return 0;
}
