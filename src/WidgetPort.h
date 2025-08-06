#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QTextBrowser>

#include <map>
#include <vector>


#include "serial.h"

using std::string;
using std::to_string;
using std::map;
using std::make_pair;
using std::vector;




class WidgetPort : public QWidget {
  Q_OBJECT
private:
  QVBoxLayout layout_base;
  QTextBrowser TB_device;
 QString text_device;	

  QHBoxLayout serial_btn_layout;
  QLabel label_gain_port;
  QPushButton btn_serial_reprobe;
  QPushButton btn_set_gain;
  map<string, unsigned int> map_serial_device;
  QString text_serial_device;

  QHBoxLayout layout_serial;
  QComboBox combo_serial;
  QCheckBox check_all_ch;
  QLabel label_all_ch;

  QGridLayout layout_ch_gain;

  const static int num_ch = 8;
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

  void serial_probe();
  void set_gain();


public:
  WidgetPort();
  ~WidgetPort();

};