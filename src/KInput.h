#ifndef _H_KINPUT_
#define _H_KINPUT_

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QString>
#include <QTextBrowser>
#include <QMessageBox>
#include <QCheckBox>

#include "jsonConfig.h"
#include "RtBase.h"
#include <map>

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include <iostream>

using std::string;
using std::to_string;
using std::map;
using std::make_pair;

class  KInput : public QWidget{
  protected:
    // Order(Releasing order) is important.
    QVBoxLayout base_layout;
    QPushButton btn_probe;
    QHBoxLayout layout_device;
    QLabel label_device;
    QComboBox combo_device;
    QTextBrowser TB_device;

    map<string,unsigned int> map_device;
    QString text_device;	

     // To parse "[num]device_name" -> num
    inline std::string mid_num_str(const std::string& s) {
      std::string::size_type p  = s.find('[');
      std::string::size_type pp = s.find(']', p + 2); 
      return s.substr(p + 1, pp - p - 1);
    }

  public :
    inline KInput();
    inline ~KInput();

  private:
    inline void audioprobe();

};
inline  KInput::KInput() :
  btn_probe("Audio Probe"),
  label_device("Input Device")
{

  // Widget and Layout
  base_layout.setAlignment(Qt::AlignTop);
  base_layout.addWidget(&btn_probe);
  layout_device.addWidget(&label_device);
  layout_device.addWidget(&combo_device);
  base_layout.addLayout(&layout_device);
  base_layout.addWidget(&TB_device);
  setLayout(&base_layout);


  /*** Audio Probe & construct combo_devicebox items ***/
  audioprobe();

  /*** Write result on TextBrowser ***/
  TB_device.setText(text_device);

  /*** Into the serial combo_devicebox ***/
  //deprecated
  /*
     for(auto it = map_serial_device.begin();it !=map_serial_device.end();it++){
     combo_device_serial.addItem( QString::fromStdString(it->first));
     }
     */

  /*** JSON reading ***/
  std::ifstream ifs(_CONFIG_JSON);
  if(!ifs.is_open()){
      QMessageBox::critical( 
      this, 
      tr("KInput"), 
      tr("Can't Open Config.json File!") );
      exit(-1);
      }

  json j = json::parse(ifs);
  ifs.close();

  combo_device.setCurrentIndex(j["input"]["device"].get<int>());


  /*** Slots ***/
  // Combobox
  QObject::connect(&combo_device, &QComboBox::currentTextChanged,
      [&](const QString &item) {
      std::ifstream ifs(_CONFIG_JSON);
      json jj = json::parse(ifs);
      ifs.close();

      jj["input"]["device"] =std::stoi(mid_num_str(item.toStdString())) ;
      std::ofstream ofs(_CONFIG_JSON);
      ofs << jj.dump(4);
      ofs.close();
      }
      );
  /*
     for (int i = 0; i < num_ch; i++) {
     QObject::connect(&combo_device_gain[i], QOverload<int>::of(&QComboBox::currentIndexChanged),
     [&](int index) {ch_gain[i] = index; });
     }
     */


  // Reprobe button
  QObject::connect(&btn_probe, &QPushButton::clicked,
      [&](){
      audioprobe();
      TB_device.setText(text_device);

      }
      );

}// End of Constructor


inline KInput::~KInput(){}

inline void KInput::audioprobe(){
  map_device.clear();

  text_device = " *** Device List *** \n\n";
  // Create an api map.
  std::map<int, std::string> apiMap;
  apiMap[RtAudio::MACOSX_CORE] = "OS-X Core Audio";
  apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
  apiMap[RtAudio::WINDOWS_DS] = "Windows Direct Sound";
  apiMap[RtAudio::WINDOWS_WASAPI] = "Windows WASAPI";
  apiMap[RtAudio::UNIX_JACK] = "Jack Client";
  apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtAudio::LINUX_PULSE] = "Linux PulseAudio";
  apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
  apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";

  RtAudio audio;
  RtAudio::DeviceInfo info;
  text_device.append("Current API : ");
  text_device.append(
      QString::fromStdString(apiMap[audio.getCurrentApi()]));
  text_device.append("\n\n");

  unsigned int devices = audio.getDeviceCount();

  //text_device = "";

  /* Create Widgets */
  for (unsigned int i = 0; i < devices; i++) {
    info = audio.getDeviceInfo(i);
    QString temp_device="[";
    temp_device.append(QString::fromStdString(to_string(i)));
    temp_device.append("]");
   // QString name_dev = QString::fromLocal8Bit(info.name.c_str());
    //QString name_dev = QString::fromUtf8(info.name.c_str());
    temp_device.append(QString::fromStdString(info.name));
   // temp_device.append(name_dev);
   // std::cout<<"device name " << info.name<<"\n";
/* DEBUG Purpose
    char str[40];
    char * cursor;
    memset(str,0,sizeof(char)*40);
    strcpy(str,info.name.c_str());
    cursor = str;

    while(*cursor){
      printf("%02x",*cursor);
      ++cursor;
    }
    printf("\n");
*/
    map_device.insert(make_pair(temp_device.toStdString(),i));

    text_device.append(temp_device);

    text_device.append("\n");
    if (info.probed == false){
      text_device.append("Probe Status = Unsuccessful");
      text_device.append("\n");
    }
    else {
      //  std::cout << "Probe Status = Successful\n";
      text_device.append("Output Channels = ");
      text_device.append(
          QString::fromStdString(to_string(info.outputChannels)));
      text_device.append("\nInput Channels = ");
      text_device.append(
          QString::fromStdString(to_string(info.inputChannels)));
      text_device.append("\n");
      if (info.sampleRates.size() < 1){
        text_device.append("No supported sample rates found!");
      }
      else {
        text_device.append("Supported sample rates = ");
        for (unsigned int j = 0; j < info.sampleRates.size(); j++){
          text_device.append(QString::fromStdString(
                to_string(info.sampleRates[j])
                ));
          text_device.append(" ");
        }
      }
      text_device.append("\n");
    }
    text_device.append("\n");
  }
  text_device.append("\n");

  /*** ReCreate Combobox for input device ***/
  int cnt = combo_device.count();
  int idx = 0;
  //combo_device.clear();
  for(auto it = map_device.begin();it !=map_device.end();it++){
    if(idx < cnt){
      combo_device.setItemText(idx, QString::fromStdString(it->first));
    }
    else{
      combo_device.addItem( QString::fromStdString(it->first));
    }
    idx++;
  }
  for(int i=0;i<cnt-idx;i++)
    combo_device.removeItem(idx);
}

#endif
