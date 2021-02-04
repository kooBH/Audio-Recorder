#ifndef _H_PARAM_COMBO_
#define _H_PARAM_COMBO_

#include <QObject>

#include <QComboBox>
#include <QWidget>

#include "Config.h"

using namespace std;

class ParamComboBox : public QComboBox {
  private:
    std::string key;
    std::string file_path;

  public:
    ParamComboBox(std::string _file_path, std::string _key,QWidget *parent = nullptr) : QComboBox(parent) {
      key = _key;
      file_path = _file_path;

      QObject::connect(this, &ParamComboBox::currentTextChanged,
          [&](const QString &item) {
#ifndef NDEBUG
//          cout <<"NOTE::"<<key <<" : "<<item.toStdString() << endl;
#endif
          std::ifstream ifs(file_path);
          json j = json::parse(ifs);
          j["PARAM"][key]["VALUE"] = stoi(item.toStdString());
          std::ofstream ofs(file_path);
          ofs << j.dump(4);
          ofs.close();
          }
          );
    }

    ~ParamComboBox(){};

};

#endif
