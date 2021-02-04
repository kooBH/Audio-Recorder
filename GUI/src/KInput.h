#ifndef _H_KINPUT_
#define _H_KINPUT_

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


#include <QString>
#include <QTextBrowser>
#include <QMessageBox>
#include <QCheckBox>

#include "Config.h"
#include "RtBase.h"
#include <map>

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include <iostream>
#include "serial.h"

using std::string;
using std::to_string;
using std::map;
using std::make_pair;

/*
   [           Audio Probe           ]
   [ audio device                |n| ]

   [ Serial Probe][     Set gain     ]
   [ port     |n|][|c|   all input       ]
# Original Code Declared it as 'input'. 
   ['input 1'][dB[n] ]['input 5'][dB[n] ]
   ...
   ['input 4'][dB[n] ]['input 8'][dB[n] ]

   ['real time']( on/off)

   [ 'input file [enter to confirm]']
   [ input.wav                      ]
   */

class  KInput : public QWidget{
  protected:
    // Order(Releasing order) is important.
    QHBoxLayout base_layout;
    QTextBrowser TB_device;

    QWidget left_widget;
    QVBoxLayout left_layout;
    QPushButton btn_reprobe;

    QComboBox combo;

    map<string,unsigned int> map_device;
    QString text_device;	

    QHBoxLayout serial_btn_layout;
    QPushButton btn_serial_reprobe;
    QPushButton btn_setgain;
    map<string,unsigned int> map_serial_device;
    QString text_serial_device;

    QHBoxLayout layout_serial;
    QComboBox combo_serial;
    QCheckBox check_all_ch;
    QLabel label_all_ch;

    QGridLayout layout_ch_gain;

    const static int num_ch=8;
    const static int num_dB = 27;

    QLabel label_ch[num_ch];
    QComboBox combo_gain[num_ch];

    const uint8_t Gain_Value_Table[num_dB] = {
      0xE8,         // -12dB
      0xEA,		  // "-11dB"
      0xEC,         //" -10dB"
      0xEE,           // -9dB,
      0xF0,           // -8dB,
      0xF2,           // -7dB,
      0xF4,           // -6dB,
      0xF6,           // -5dB
      0xF8,           // -4dB
      0xFA,           // -3dB
      0xFC,           // -2dB
      0xFE,           // -1dB
      0x00,           //  0dB
      0x02,           //  1dB
      0x04,           //  2dB
      0x06,           //  3dB
      0x08,           //  4dB
      0x0A,           //  5dB
      0x0C,           //  6dB
      0x0E,           //  7dB
      0x10,           //  8dB
      0x12,           //  9dB
      0x14,           // 10dB
      0x16,           // 11dB
      0x18,           // 12dB
      0x28,           // 20dB
      0x40            // 32dB
    };

    const string Gain_dB_Table[num_dB] = {
      "-12dB",
      "-11dB",
      "-10dB",
      "-9dB",
      "-8dB",
      "-7dB",
      "-6dB",
      "-5dB",
      "-4dB",
      "-3dB",
      "-2dB",
      "-1dB",
      "0dB",
      "1dB",
      "2dB",
      "3dB",
      "4dB",
      "5dB",
      "6dB",
      "7dB",
      "8dB",
      "9dB",
      "10dB",
      "11dB",
      "12dB",
      "20dB",
      "32dB"
    };

    int ch_gain[num_ch];
    bool all_in_one;
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
    inline void serialprobe();
    inline void setgain();

};
inline  KInput::KInput() :
  btn_reprobe("Audio Probe"),
  btn_serial_reprobe("Search Ports"),
  btn_setgain("SetGain"),
  label_all_ch("all by one"),
  all_in_one(true)
{
	for (int i = 0; i < num_ch; i++)
		ch_gain[i] = 0;

  /*** Serial Probe & construct combobox items ***/
  serialprobe();

  /*** Audio Probe & construct combobox items ***/
  audioprobe();

  /*** Write result on TextBrowser ***/
  TB_device.setText(text_device);

  /*** Into the serial combobox ***/
  //deprecated
  /*
     for(auto it = map_serial_device.begin();it !=map_serial_device.end();it++){
     combo_serial.addItem( QString::fromStdString(it->first));
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

  /**** Set Initial item ***/
  combo.setCurrentIndex(j["INPUT"]["DEVICE"].get<int>());


  /*** left layout ***/

  /* Audio Probe */
  left_layout.addWidget(&btn_reprobe);

  left_layout.addWidget(&combo);

  /* Serial Probe */
  layout_serial.addWidget(&combo_serial);
  layout_serial.addWidget(&label_all_ch);
  layout_serial.addWidget(&check_all_ch);
  check_all_ch.setChecked(true);	  
  left_layout.addLayout(&layout_serial);
  /* Gain Control */



  for(int j=0;j<num_ch;j++)
    for (int i = 0; i < num_dB; i++) {
      combo_gain[j].addItem(QString::fromStdString(Gain_dB_Table[i]));
    }
  for (int i = 0; i < num_ch; i++) {
    label_ch[i].setText("input " + QString::number(i + 1));
    layout_ch_gain.addWidget(&label_ch[i],i%4,2*(int)(i/4));
    layout_ch_gain.addWidget(&combo_gain[i],i%4,2*(int)(i/4)+1);
  }

  // default false
  for (int i = 1; i < num_ch; i++) {
    combo_gain[i].setEnabled(false);
  }
  label_ch[0].setText("all inputs");

  left_layout.addLayout(&layout_ch_gain);

  serial_btn_layout.addWidget(&btn_serial_reprobe);
  serial_btn_layout.addWidget(&btn_setgain);

  left_layout.addLayout(&serial_btn_layout);

  left_widget.setLayout(&left_layout);
  left_widget.setFixedWidth(260);

  /*** base loyout ***/
  //base_layout.addWidget(&left_widget,0,0,1,1);
  base_layout.addWidget(&left_widget);
  left_layout.setAlignment(Qt::AlignTop);
  //TB_device.setFixedHeight(400);
  //TB_device.setBaseSize(300,400);
  //TB_device.setFixedWidth(400);
  //horizontal scrollbar
  //TB_device.setLineWrapColumnOrWidth(QTextEdit::NoWrap);

  //base_layout.addWidget(&TB_device,0,1,6,2);
  base_layout.addWidget(&TB_device);

  base_layout.setAlignment(Qt::AlignTop);
  //setBaseSize(400,500);
  setLayout(&base_layout);



  /*** Slots ***/
  // Combobox
  QObject::connect(&combo, &QComboBox::currentTextChanged,
      [&](const QString &item) {
      std::ifstream ifs(_CONFIG_JSON);
      json jj = json::parse(ifs);
      ifs.close();

      jj["INPUT"]["DEVICE"] =std::stoi(mid_num_str(item.toStdString())) ;
      std::ofstream ofs(_CONFIG_JSON);
      ofs << jj.dump(4);
      ofs.close();
      }
      );
  /*
     for (int i = 0; i < num_ch; i++) {
     QObject::connect(&combo_gain[i], QOverload<int>::of(&QComboBox::currentIndexChanged),
     [&](int index) {ch_gain[i] = index; });
     }
     */

  QObject::connect(&combo_gain[0], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {

      ch_gain[0] = index;
      if(all_in_one)
		  for(int i=1;i<num_ch;i++){
			  combo_gain[i].setCurrentIndex(index);
			  ch_gain[i] = index;
      }

      });
  QObject::connect(&combo_gain[1], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[1] = index; });
  QObject::connect(&combo_gain[2], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[2] = index; });
  QObject::connect(&combo_gain[3], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[3] = index; });
  QObject::connect(&combo_gain[4], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[4] = index; });
  QObject::connect(&combo_gain[5], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[5] = index; });
  QObject::connect(&combo_gain[6], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[6] = index; });
  QObject::connect(&combo_gain[7], QOverload<int>::of(&QComboBox::currentIndexChanged),
      [&](int index) {ch_gain[7] = index; });

  // Reprobe button
  QObject::connect(&btn_reprobe, &QPushButton::clicked,
      [&](){
      audioprobe();
      TB_device.setText(text_device);

      }
      );

  // setgain button
  QObject::connect(&btn_setgain, &QPushButton::clicked,
      [&](){
      setgain();
      TB_device.setText(text_device);
      }
      );

  QObject::connect(&btn_serial_reprobe, &QPushButton::clicked,
      [&](){
      serialprobe();
      TB_device.setText(text_device);
      }
      );
  QObject::connect(&check_all_ch, &QCheckBox::stateChanged,
      [&](int state) {
      // 0 or 2
      switch (state) {
      case 2:
      all_in_one = true;
      break;
      case 0:
      all_in_one = false;
      break;
      }
      for (int i = 1; i < num_ch; i++) {
      combo_gain[i].setEnabled(!all_in_one);
      }
      if (all_in_one) {
      label_ch[0].setText("every input");
      }
      else {
      label_ch[0].setText("input 1");
      }
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
  int cnt = combo.count();
  int idx = 0;
  //combo.clear();
  for(auto it = map_device.begin();it !=map_device.end();it++){
    if(idx < cnt){
      combo.setItemText(idx, QString::fromStdString(it->first));
    }
    else{
      combo.addItem( QString::fromStdString(it->first));
    }
    idx++;
  }
  for(int i=0;i<cnt-idx;i++)
    combo.removeItem(idx);
}

inline void KInput::serialprobe(){  
  text_device = " *** Device List *** \n\n";

  vector<serial::PortInfo> devices_found = serial::list_ports();

  vector<serial::PortInfo>::iterator iter = devices_found.begin();
  //printf("Found %lu Devices\n", devices_found.size());
  //printf("=======================\n");
  map_serial_device.clear();
  int i = 0;
  while (iter != devices_found.end()) {
    serial::PortInfo device = *iter++;
    if (strcmp("n/a", device.hardware_id.c_str())){
      map_serial_device.insert(make_pair(device.port.c_str(), i));
      i++;
      text_device.append("\nPort = ");
      text_device.append(QString::fromStdString(device.port.c_str())); 
      text_device.append("\ndescription = ");
      text_device.append(QString::fromStdString(device.description.c_str())); 
      text_device.append("\nhardware ID = ");
      text_device.append(QString::fromStdString(device.hardware_id.c_str()));
      text_device.append("\n"); 
      /*
         printf(
         "Port name : %s\nDescription :%s\nID : %s\n=======================\n",
         device.port.c_str(), device.description.c_str(),
         device.hardware_id.c_str());
         */

    }
  }
  combo_serial.clear();
  for(auto it = map_serial_device.begin();it !=map_serial_device.end();it++){
    combo_serial.addItem( QString::fromStdString(it->first));
  }
}

inline void KInput::setgain(){
	int i = 0;
	
  unsigned long baud = 38400;
  string port = combo_serial.currentText().toStdString();
 
  try{
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000)); 
    //  cout << "Is the serial port open?";
	if (my_serial.isOpen())
	{
		// Get the Test string
		//int count = 0;

		uint8_t data_write[10];
		uint8_t data_read[10];

		uint8_t checksum = 0;
		size_t available;
		data_write[0] = 0x55;

		if (all_in_one) {
			for (i = 1; i < 9; i++)
				ch_gain[i] = ch_gain[0];
			}

      for ( i = 1; i < 9; i++) {
        data_write[i] = Gain_Value_Table[ch_gain[i]];
        checksum += data_write[i];
      }
      data_write[9] = checksum;

      memset(data_read, 0, sizeof(uint8_t) * 10);
      my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
      // cout << "Available : " << my_serial.available() << endl;
      size_t bytes_wrote = my_serial.write(data_write, 10);
      //  cout << "bytes_wrote " << bytes_wrote << endl;

      available = my_serial.available();
      //  cout << "Available : " << available << endl;

      size_t string_read = my_serial.read(data_read, 10);
      // cout << "Available : " << my_serial.available() << endl;

      //  cout << "string_read : " << string_read << endl;

      text_device = "";

      if (data_read[0] == 0x55)
      {//printf("Read 55\n");
        text_device.append("INFO::Changed gain to \n");
        for(int i=0;i<num_ch;i++){
          text_device.append("output ");
          text_device.append(QString::number(i+1));;
          text_device.append(" to ");
          text_device.append(Gain_dB_Table[ch_gain[i]].c_str());
          text_device.append("dB\n");
        }

      }
      else
      {
        //printf("Read not 55 : 0x%02X\n",data_read[0]);
        printf("ERROR::Fail to change value of gain :0x%02X\n",data_read[0]);
        text_device.append("ERROR::Fail to change value of gain\n");
      }
      //  cout << " Yes." << endl;
    }
    else {
      text_device.append("error! port is not open\n");
      //return -1;
    }
  }
  catch (std::exception const& e){
    text_device.append("error! port is not found\n");
  }
}

#endif
