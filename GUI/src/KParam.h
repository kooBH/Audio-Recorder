#ifndef _H_KPARAM_
#define _H_KPARAM_

#include <QGridLayout>

#include <QWidget>
#include <QLabel>
#include "ParamComboBox.h"
#include "ParamLineEdit.h"

#include "Config.h"

class KParam : public QWidget{

  private:
    QGridLayout layout;
    //ConfigParam param_s;
    vector<tuple<QLabel *, ParamComboBox *>> cb;
    vector<tuple<QLabel*, ParamLineEdit*>> param_algo;
    vector<QLabel*> title;

    int r,c;
    int max_row;

    QString style_label;
  public :

    inline KParam();
    inline ~KParam();
    inline void AddAlgo(JsonConfig<double> config);
    inline void AddIntParam(JsonConfig<int> config);
   };

inline KParam::KParam(){
      r=0;
      c=0;
      max_row =14;

      layout.setAlignment(Qt::AlignLeft|Qt::AlignTop); 

      /*** JSON Reading ***/
      std::ifstream ifs(_CONFIG_JSON);

      if(!ifs.is_open()){
        QMessageBox::critical( 
            this, 
            tr("KParam"), 
            tr("Can't Open Config.json File!") );
        exit(-1);

      }

      json j = json::parse(ifs);
      json param = j["PARAM"];

      /* Dynamic combobox for PARAM */
      r=0;
      QLabel *temp_title = new QLabel(QString::fromStdString("Base"));
      layout.addWidget(temp_title,r,c,1,1);
      title.push_back(temp_title);
      r++;
      for (json::iterator it = param.begin(); it != param.end(); ++it) {
        QLabel *t1 = new QLabel(QString::fromStdString(it.key()));
        t1->setFixedHeight(40);
        t1->setStyleSheet(style_label);
        t1->setAlignment(Qt::AlignCenter);
        ParamComboBox*t2 = new ParamComboBox(_CONFIG_JSON, it.key() );


        /* Iterate on "OPTIONS "*/
        json options = it.value().at("OPTIONS");
        for (auto it2 = options.begin(); it2 != options.end(); ++it2) {
          t2->addItem(QString::fromStdString(std::to_string((int)it2.value()) ));
        }
        /* Set Default iten as in json */
        const int index = t2->findText(QString::fromStdString(
              std::to_string((int)it.value().at("VALUE")))); 
        if(index >= 0) 
          t2->setCurrentIndex(index);

        cb.push_back(std::make_tuple(t1, t2));

        /**/
        layout.addWidget(t1, r, c, 1, 1);
        layout.addWidget(t2, r+1, c, 1, 1);
        r=r+2;
      }
      c++;

      /*
      ConfigVAD vad;
      AddAlgo(vad);

      ConfigWPE wpe;
      AddAlgo(wpe);

      ConfigStereo_AEC stereo_aec;
      AddAlgo(stereo_aec);

      ConfigMLDR mldr;
      AddAlgo(mldr);

      ConfigCam Cam;
      AddIntParam(Cam);
      */
      ifs.close();
      
      //  setFixedSize(600,400);
      setLayout(&layout);
}

inline void KParam::AddAlgo(JsonConfig<double> config){
      r=0;
      QLabel *temp_title = new QLabel(QString::fromStdString(config.GetName()));
      layout.addWidget(temp_title,r,c,1,1);
      title.push_back(temp_title);
      r++;

      for(auto it : config.GetData()) {
        QLabel *t1;
        ParamLineEdit *t2;
        t1 = new QLabel(QString::fromStdString(it.first));
        t1->setFixedHeight(40);
        t1->setStyleSheet(style_label);
        t1->setAlignment(Qt::AlignCenter);

        t2 = new ParamLineEdit(_CONFIG_JSON,config.GetName(),it.first);
        
        // Is Integer Parameter ? 
        if(t2->IsInteger())
          t1->setText("[int]" + t1->text());

        param_algo.push_back(std::make_tuple(t1, t2));
        layout.addWidget(t1,r,c,1,1);
        layout.addWidget(t2,r+1,c,1,1);

        r=r+2;
      }
      c++;
}

inline void KParam::AddIntParam(JsonConfig<int> config){
      r=0;
      QLabel *temp_title = new QLabel(QString::fromStdString(config.GetName()));
      layout.addWidget(temp_title,r,c,1,1);
      title.push_back(temp_title);
      r++;

      for(auto it : config.GetData()) {
        QLabel *t1;
        ParamLineEdit *t2;
        t1 = new QLabel(QString::fromStdString(it.first));
        t1->setFixedHeight(40);
        t1->setStyleSheet(style_label);
        t1->setAlignment(Qt::AlignCenter);

        t2 = new ParamLineEdit(_CONFIG_JSON,config.GetName(),it.first);
        // Is Integer Parameter ? 
        if(t2->IsInteger())
          t1->setText("[int]" + t1->text());
        
        param_algo.push_back(std::make_tuple(t1, t2));
        layout.addWidget(t1,r,c,1,1);
        layout.addWidget(t2,r+1,c,1,1);

        r=r+2;
      }
      c++;
}

KParam::~KParam(){
      for (auto &t : cb) {
        delete std::get<0>(t);
        delete std::get<1>(t);
      }
      for (auto &t : param_algo) {
        delete std::get<0>(t);
        delete std::get<1>(t);
      }
      for(auto &t : title)
        delete t;
}
#endif
