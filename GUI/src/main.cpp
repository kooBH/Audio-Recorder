#include <Qt>
#include <QApplication>
#include <QIcon>

#include "KRecorder.h"


#ifdef _WIN32
#include <Windows.h>
#endif


int main(int argc, char* argv[]){

  //path to dll 
  QCoreApplication::addLibraryPath(".");

  //path to so
  QCoreApplication::addLibraryPath("../lib");
  //QCoreApplication::addLibraryPath("../lib/platfrominputcontexts");

  QApplication app(argc,argv);

  printf("Loading....\n");
  KRecorder recorder;

  app.setWindowIcon(QIcon("../resource/record.ico"));

  recorder.show();


#ifdef _WIN32
	//ShowWindow(::GetConsoleWindow(), SW_HIDE);
  ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
	
/*
  int id = QFontDatabase::addApplicationFont(QString::fromStdString(_HOME_DIR)  
           + QString::fromStdString("/resource/SDMiSaeng.ttf"));      
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);                           
        QFont SerifFont(family);
	app.setFont(SerifFont);
*/  


  return app.exec();
}
