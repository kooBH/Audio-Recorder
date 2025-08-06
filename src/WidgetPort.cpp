#include "WidgetPort.h"


WidgetPort::WidgetPort(): btn_serial_reprobe("Search Ports"),
label_gain_port("AMEMS Port"),
btn_set_gain("SetGain"),
label_all_ch("all by one"),
all_in_one(true) {
  layout_base.setAlignment(Qt::AlignLeft|Qt::AlignTop); 

	for (int i = 0; i < num_ch; i++)
		ch_gain[i] = 0;

 /* Serial Probe */
  layout_serial.addWidget(&label_gain_port);
  layout_serial.addWidget(&combo_serial);
  layout_serial.addWidget(&label_all_ch);
  layout_serial.addWidget(&check_all_ch);
  check_all_ch.setChecked(true);	  
  /* Gain Control */
  layout_base.addLayout(&layout_serial);

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

  layout_base.addLayout(&layout_ch_gain);

  serial_btn_layout.addWidget(&btn_serial_reprobe);
  serial_btn_layout.addWidget(&btn_set_gain);
  layout_base.addLayout(&serial_btn_layout);

  layout_base.addWidget(&TB_device);



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

  // set_gain button
  QObject::connect(&btn_set_gain, &QPushButton::clicked,
      [&](){
      set_gain();
      TB_device.setText(text_device);
      }
      );

  QObject::connect(&btn_serial_reprobe, &QPushButton::clicked,
      [&](){
      serial_probe();
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


  this->setLayout(&layout_base);


  // Init TB
  serial_probe();
  TB_device.setText(text_device);


}
WidgetPort::~WidgetPort() {

}


void WidgetPort::serial_probe() {
  text_device = " *** Device List *** \n\n";

  vector<serial::PortInfo> devices_found = serial::list_ports();

  vector<serial::PortInfo>::iterator iter = devices_found.begin();
  //printf("Found %lu Devices\n", devices_found.size());
  //printf("=======================\n");
  map_serial_device.clear();
  int i = 0;
  while (iter != devices_found.end()) {
    serial::PortInfo device = *iter++;
    //if (strcmp("n/a", device.hardware_id.c_str())){
    {
      map_serial_device.insert(make_pair(device.port.c_str(), i));
      i++;
      text_device.append("\nPort = ");
      //text_device.append(QString::fromStdString(device.port.c_str())); 
      text_device.append(QString::fromLocal8Bit(device.port.c_str())); 
      text_device.append("\nDescription = ");
      //text_device.append(QString::fromStdString(device.description.c_str())); 
      text_device.append(QString::fromLocal8Bit(device.description.c_str())); 
      text_device.append("\nHardware ID = ");
      //text_device.append(QString::fromStdString(device.hardware_id.c_str()));
      text_device.append(QString::fromLocal8Bit(device.hardware_id.c_str()));
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
void WidgetPort::set_gain() {

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

