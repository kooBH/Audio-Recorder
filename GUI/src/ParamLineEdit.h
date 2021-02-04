#ifndef _H_PARAM_EDIT_
#define _H_PARAM_EDIT_

#include "Config.h"
#include <QLineEdit>
#include <QString>

using std::string;

class ParamLineEdit : public QLineEdit{
  private:
    string path_file;
    string name_algo;
    string key;
    bool is_integer;

  public:
    ParamLineEdit(string _path_file,string _algo, string _key,
        QWidget* parent = nullptr):
      QLineEdit(parent),
      path_file(_path_file),
      name_algo(_algo),
      key(_key),
      is_integer(false){


        std::ifstream ifs(path_file);
        json j = json::parse(ifs);
        j = j[name_algo];

        /* Check whether this is interger item */
        string temp_str;
        for(auto iter : j["__integer__"]){
          temp_str = iter.get<string>();

          if(key.compare(temp_str)==0){
            is_integer = true;
#ifndef NDEBUG
            //         printf("NOTE::%s::%s is INT\n",name_algo.c_str() ,temp_str.c_str() );
#endif
            break;
          }
        }
        if(is_integer){
          setText(QString::number(  static_cast<int>(j[key].get<double>())));
        }
        else{
          setText(QString::number(j[key].get<double>()));
        }
        ifs.close();


        /* Update config file when textChanged */
        /*
           void QLineEdit::textChanged(const QString &text)
           This signal is emitted whenever the text changes. The text argument is the new text.

           Unlike textEdited(), this signal is also emitted when the text is changed programmatically, for example, by calling setText().
         * */
        QObject::connect(this,&ParamLineEdit::textChanged,
            [&](const QString &text){
            bool is_valid;

            if(is_integer){
            int temp_int;
            temp_int =  text.toInt(&is_valid);

            /* Check whether valid integer or not. */
            if(is_valid){
            std::ifstream ifs(path_file);
            json j = json::parse(ifs);
            ifs.close();

            j[name_algo][key] = temp_int;

            std::ofstream ofs(_CONFIG_JSON);
            ofs << j.dump(4);
            ofs.close();


            }


            }else{
              double temp_double;
              temp_double= text.toDouble(&is_valid);

              /* Check whether valid double or not. */
              if(is_valid){
                std::ifstream ifs(path_file);
                json j = json::parse(ifs);
                ifs.close();

                j[name_algo][key] = temp_double;

                std::ofstream ofs(_CONFIG_JSON);
                ofs << j.dump(4);
                ofs.close();
              }


            }

            }
        );
      }

    bool IsInteger(){
      return is_integer;

    }

};

#endif
