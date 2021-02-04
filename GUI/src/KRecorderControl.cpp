#include "KRecorderControl.h"

  KRecorderControl::KRecorderControl() :QWidget(),
  label_ch_plot("1"),
  btn_ch_plot_next("next channel"),
  btn_save_path("Set Output Path"),
  label_save_path("Current Directory"),
  label_status("Uninitialized"),
#if _WIN32
  btn_init("Initialize[F4]"),
  btn_record("Record[F6]"),
#else
  btn_init("Initialize"),
  btn_record("Record"),
#endif
  label_mode("Mode"),
  label_timer("Timer[sec]"),
  label_interval("Interval"),
  label_time("Sec"),
  label_split_wav("split wav"),
  label_log("logging"),

  LE_timer("5.0"),
  isRecording(false),
  isInited(false),
  record_time(5.0),
  rec_mode(MANUAL),
  wait_thread(nullptr){

    is_setting_time.store(false);
    label_status.setFixedHeight(30);
    btn_record.setEnabled(false);

    combo_interval.setEnabled(false);

    QFont font;
    // Requested 1.20
    // font.setPointSize(36);
    //btn_init.setFont(font);
    //btn_record.setFont(font);
#if _WIN32
    thread_key = new std::thread(&KRecorderControl::KeyboardInput, this);
#endif

    /* TrayIcon */
    //trayIcon = new QSystemTrayIcon(this);
    //auto appIcon = QIcon("../resource/record.ico");
    //this->trayIcon->setIcon(appIcon);
    // Displaying the tray icon
    //QMenu* trayIconMenu = new QMenu(this);
    // Create new, clickable actions
    //QAction* closeAction = new QAction("Close", trayIconMenu);
    // trayIconMenu->addAction(closeAction);
    // trayIcon->setContextMenu(trayIconMenu);
    // this->trayIcon->show();


    /* Layout Section */
    {
      layout_plot_control.addWidget(&label_ch_plot);
      layout_plot_control.addWidget(&btn_ch_plot_next);
      layout_recorder.addLayout(&layout_plot_control);
      layout_recorder.addWidget(&btn_save_path);
      layout_recorder.addWidget(&label_save_path);
      layout_recorder.addWidget(&label_status);
      layout_recorder.addWidget(&btn_init);
      // mode
      label_mode.setAlignment(Qt::AlignCenter);
      layout_mode.addWidget(&label_mode);

      layout_mode.addWidget(&combo_mode);

      layout_mode.setSpacing(0);
      layout_mode.setContentsMargins(0, 0, 0, 0);
      widget_mode.setLayout(&layout_mode);

      layout_recorder.addWidget(&widget_mode);
      // interval 
      label_interval.setAlignment(Qt::AlignCenter);
      label_interval.setEnabled(false);
      layout_interval.addWidget(&label_interval);

      layout_interval.addWidget(&combo_interval);

      layout_interval.setSpacing(0);
      layout_interval.setContentsMargins(0, 0, 0, 0);
      widget_interval.setLayout(&layout_interval);

      layout_recorder.addWidget(&widget_interval);

      //// timer
      layout_timer.addWidget(&label_timer);
      layout_timer.addWidget(&LE_timer);
      LE_timer.setEnabled(false);
      widget_timer.setLayout(&layout_timer);
      layout_recorder.addWidget(&widget_timer);

      layout_timer.setSpacing(0);
      layout_timer.setContentsMargins(0, 0, 0, 0);

      //// scale
      // default
      // per ch

      layout_recorder.addLayout(&layout_scale_ch);

      /*
       *     label      LE
       * 0    0 0       0 1
       * 1    0 2       0 3
       * 2    1 0       1 1
       * 3    1 2       1 3
       * 4    2 0       2 1
       * 5    2 2       2 3
       * */

      //// Scale
      /*
         int temp_i;
         QString temp_qstr;
         for(int i=0;i<max_ch;i++){
         temp_i = (i%2)*2;
         layout_scale_ch.addWidget(&label_scale[i],i/2,temp_i,1,1);
         layout_scale_ch.addWidget(&LE_scale[i],i/2,1 + temp_i,1,1);
         temp_qstr = "ch"+ QString::number(i+1);
         label_scale[i].setText(temp_qstr);
         scale[i]=1;
         LE_scale[i].setText("1.0");
         LE_scale[i].setStyleSheet(_BG_COLOR_3_);

         LE_scale[i].setEnabled(false);
         }
         */
      label_scale.setText("Input Scale");
      std::ifstream ifs(_CONFIG_JSON);

      if (ifs.is_open()) {
        json j = json::parse(ifs);
        scale = j["INPUT"]["SCALE"].get<double>();
        ifs.close();
        LE_scale.setText(QString::number(scale));
      }
      else{
        LE_scale.setText("1.0");
        scale = 1.0;
      }
      layout_scale_ch.addWidget(&label_scale, 0, 0, 1, 1);
      layout_scale_ch.addWidget(&LE_scale, 0, 1, 1, 1);

      out = new WAV*[max_ch];
      //// split
      layout_split_wav.addWidget(&label_split_wav);
      layout_split_wav.addWidget(&check_split_wav);
      do_split_wav = default_split;
      check_split_wav.setChecked(default_split);
      //log
      layout_split_wav.addWidget(&label_log);
      layout_split_wav.addWidget(&check_log);
      check_log.setChecked(true);
      layout_recorder.addLayout(&layout_split_wav);

      // record btn
      layout_recorder.addWidget(&btn_record);
      layout_recorder.addWidget(&label_time);

      // etc
      layout_recorder.setAlignment(Qt::AlignTop);

      widget_recorder.setLayout(&layout_recorder);
      layout_base.addWidget(&widget_plot);
      layout_base.addWidget(&widget_recorder);
      setLayout(&layout_base);

    }

    btn_ch_plot_next.setEnabled(false);
    /* Displayed channel for plot */
    QObject::connect(&btn_ch_plot_next, &QPushButton::clicked, [&]() {
        ch_plot++;
        if (ch_plot >= channels)
        ch_plot = 0;
        label_ch_plot.setText(QString::number(ch_plot+1));
        }
        );
    /* Mode ComboBox */
    combo_mode.addItem("Manual");
    combo_mode.addItem("Timer");
    combo_mode.addItem("Interval");

    QObject::connect(&combo_mode, 
        QOverload<int>::of (&QComboBox::currentIndexChanged),
        [&](int index) {
        switch (index) {
        case MANUAL:
        rec_mode = MANUAL;
        LE_timer.setEnabled(false);
        combo_interval.setEnabled(false);
        break;
        case TIMER:
        rec_mode = TIMER;
        LE_timer.setEnabled(true);
        combo_interval.setEnabled(false);
        break;
        case INTERVAL:
        rec_mode = INTERVAL;
        LE_timer.setEnabled(false);
        combo_interval.setEnabled(true);
        break;
        }
        }
    );

    /* Interval ComboBox */

    ConfigParam param(_CONFIG_JSON);
    sample_rate = param["SAMPLE_RATE"];


    combo_interval.addItem("5-min");
    combo_interval.addItem("30-min");
    combo_interval.addItem("1-hour");
    combo_interval.addItem("2-hour");
    combo_interval.addItem("4-hour");
    combo_interval.addItem("8-hour");
    // default value
    recording_interval = sample_rate * 300;
    QObject::connect(&combo_interval, 
        QOverload<int>::of (&QComboBox::currentIndexChanged),
        [&](int index) {
        switch(index){
        case 0:
        recording_interval= sample_rate * 300;
        break;
        case 1:
        recording_interval=sample_rate * 1800;
        break;
        case 2:
        recording_interval= sample_rate * 3600;
        break;
        case 3:
        recording_interval= sample_rate * 7200;
        case 4:
        recording_interval = sample_rate * 14400;
        case 5:
        recording_interval = sample_rate * 28800;
        break;
        }

        }
    );

    /* Recording */
    QObject::connect(&btn_record, &QPushButton::clicked, [&](){StartRecord();});

    QObject::connect(&btn_init, &QPushButton::clicked, [&]() {
        if(!BuildModule())
        label_status.setText("Initialized");
        else
        label_status.setText("Failed to Initialize");
        });

    /* Set Default file path */ 
    //QDir dir_temp = QDir::current();
    //dir_temp.cdUp();
    //QDir::setCurrent(dir_temp.path());
    //file_path = dir_temp.path();
    //label_save_path.setText(file_path);

    /* Set Output file path */
    QObject::connect(&btn_save_path, &QPushButton::clicked, [&]() {
        file_path = QFileDialog::getExistingDirectory(this, tr("Output Path"),
            nullptr,
            QFileDialog::ShowDirsOnly
            | QFileDialog::DontResolveSymlinks);
        label_save_path.setText(file_path);
        std::ofstream ofs("../config/path.txt");
        if (!ofs.is_open())
        printf("ERROR::Can't open path.json");
        ofs << file_path.toStdString();
        ofs.close();
        });

    /* Load file path*/
    LoadFilePath();
    /* Check whether valid double or not. Only if there is valid double input,
       change timer value. */
    QObject::connect(&LE_timer, &QLineEdit::textChanged,
        [&](const QString& text) {
        bool is_valid;
        double temp_double;
        temp_double = text.toDouble(&is_valid);

        if (is_valid) {
        label_status.setText("timer : " + QString::number(temp_double) + " sec");
        record_time = temp_double;
        }
        }
        );

    /*** max_ch input scale ***/
    /*
       {
       QObject::connect(&LE_scale[0], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
       double temp_double;
       temp_double = text.toDouble(&is_valid);
       if (is_valid) {
       scale[0] = temp_double;
       label.setText("ch 1's scale : " + QString::number(temp_double));
       }
       }
       );
       QObject::connect(&LE_scale[1], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
       double temp_double;
       temp_double = text.toDouble(&is_valid);
       if (is_valid) {
       scale[1] = temp_double;
       label.setText("ch 2's scale : " + QString::number(temp_double));
       }
       }
       );
       QObject::connect(&LE_scale[2], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
       double temp_double;
       temp_double = text.toDouble(&is_valid);
       if (is_valid) {
       scale[2] = temp_double;
       label.setText("ch 3's scale : " + QString::number(temp_double));
       }
       }
       );
       QObject::connect(&LE_scale[3], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
       double temp_double;
       temp_double = text.toDouble(&is_valid);
       if (is_valid) {
       scale[3] = temp_double;
       label.setText("ch 4's scale : " + QString::number(temp_double));
       }
       }
       );
       QObject::connect(&LE_scale[4], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
       double temp_double;
       temp_double = text.toDouble(&is_valid);
       if (is_valid) {
       scale[4] = temp_double;
       label.setText("ch 5's scale : " + QString::number(temp_double));
       }
       }
       );
       QObject::connect(&LE_scale[5], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
       double temp_double;
       temp_double = text.toDouble(&is_valid);
       if (is_valid) {
       scale[5] = temp_double;
       label.setText("ch 6's scale : " + QString::number(temp_double));
       }
       }
       );
       QObject::connect(&LE_scale[6], &QLineEdit::textChanged,
       [&](const QString& text) {
       bool is_valid;
    double temp_double;
    temp_double = text.toDouble(&is_valid);
    if (is_valid) {
      scale[6] = temp_double;
      label.setText("ch 7's scale : " + QString::number(temp_double));
    }
  }
  );
  QObject::connect(&LE_scale[7], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[7] = temp_double;
      label.setText("ch 8's scale : " + QString::number(temp_double));
      }
      }
      );
  QObject::connect(&LE_scale[8], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[8] = temp_double;
      label.setText("ch 9's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[9], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[9] = temp_double;
      label.setText("ch 10's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[10], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[10] = temp_double;
      label.setText("ch 11's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[11], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[11] = temp_double;
      label.setText("ch 12's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[12], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[12] = temp_double;
      label.setText("ch 13's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[13], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[13] = temp_double;
      label.setText("ch 14's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[14], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[14] = temp_double;
      label.setText("ch 15's scale : " + QString::number(temp_double));
      }
      }
      );

  QObject::connect(&LE_scale[15], &QLineEdit::textChanged,
      [&](const QString& text) {
      bool is_valid;
      double temp_double;
      temp_double = text.toDouble(&is_valid);
      if (is_valid) {
      scale[15] = temp_double;
      label.setText("ch 16's scale : " + QString::number(temp_double));
      }
      }
      );
  }
  }
*/
QObject::connect(&LE_scale, &QLineEdit::textChanged,
    [&](const QString& text) {
    bool is_valid;
    double temp_double;
    temp_double = text.toDouble(&is_valid);
    if (is_valid) {
    scale = temp_double;
    label_status.setText("scale : " + QString::number(temp_double));

    std::ifstream ifs(_CONFIG_JSON);

    if (ifs.is_open()) {

    json j = json::parse(ifs);
    j["INPUT"]["SCALE"] = scale;
    ifs.close();

    std::ofstream ofs(_CONFIG_JSON);
    ofs << j.dump(4);
    ofs.close();
    }
    }
    }
);

QObject::connect(&check_split_wav, &QCheckBox::stateChanged,
    [&](int state) {
    // 0 or 2
    switch (state) {
    case 2:
    do_split_wav = true;
    break;
    case 0:
    do_split_wav = false;
    break;
    }
    }
    );

//QObject::connect(&timer, &QTimer::timeout, this, &KRecorderControl::SetTime);
QObject::connect(this, &KRecorderControl::SignalStopTimer, this, &KRecorderControl::StopTimer);
QObject::connect(this, &KRecorderControl::SignalRefreshTimer, this, &KRecorderControl::SetTime);
QObject::connect(this, &KRecorderControl::SignalStartRecord, this, &KRecorderControl::StartRecord);

/* Logger */
/*
check_log.setChecked(true);
try {
  // Create basic file logger (not rotated)
  auto logger = spdlog::basic_logger_mt("logger", "../logs/logs.txt");
  spdlog::set_default_logger(logger);
  spdlog::flush_on(spdlog::level::info);
  spdlog::info("Recorder constructed");
}
catch (const spdlog::spdlog_ex& ex){
  QMessageBox::critical(
      this,
      tr("Log initialization failed"),
      tr(ex.what())
      );
  }
*/
QObject::connect(&check_log, &QCheckBox::stateChanged,
    [&](int state) {
    // 0 or 2
    switch (state) {
    case 2:
  //  spdlog::set_level(spdlog::level::info);
    break;
    case 0:
//    spdlog::set_level(spdlog::level::off);
    break;
    }
    }
    );
}

KRecorderControl::~KRecorderControl() {
  //delete trayIcon;
  ClearBuild();
  if (wait_thread)delete wait_thread;
  delete[] out;
}

int KRecorderControl::BuildModule(){
 // spdlog::info("Build Module");

  ClearBuild();
  ConfigParam param(_CONFIG_JSON);
  ConfigInput input(_CONFIG_JSON);

  widget_plot.ResetShiftSize();

  /* Execption */
  if(!param.IsOpen()){
    QMessageBox::critical( 
        this, 
        tr("KRecorderControl "), 
        tr("Can't Open Config.json File!") );
    return -1;
  }

  /* Configuration */
  sample_rate =param["SAMPLE_RATE"];
  channels = param["CHANNEL"];
  shift_size = param["SHIFT_SIZE"];
  frame_size = param["FRAME_SIZE"];
  device = input.GetDevice();

  temp = new short[channels * shift_size];
  temp_plot = new short[shift_size*4];
  try {
    rt = new RtInput(device, channels, sample_rate, shift_size, frame_size);
  }
  catch (std::runtime_error& e) {
    QMessageBox::critical(
        this,
        tr("RtAudio"),
        tr(e.what())
        );
    return -1;
  }
  if(!do_split_wav)
    out[0] = new WAV(channels, sample_rate);
  else {
    for (int i = 0; i < channels; i++) {
      out[i] = new WAV(1, sample_rate);
    }
  }

  /* build split buffer */
  buf_split = new short* [channels];
  for (int i = 0; i < channels; i++)
    buf_split[i] = new short[shift_size];


  isInited = true;
  btn_record.setEnabled(true);
  btn_ch_plot_next.setEnabled(true);
  /* input scale widgets */
  /*
     for(int i=0;i<channels;i++){
     LE_scale[i].setEnabled(true);
     }
     */
  label_status.setText("Initialized");
  return 0;
}

void KRecorderControl::StartRecord() {
  switch (rec_mode) {
    case MANUAL:
      /* Stop Recording */
      if (isRecording) {
        EnableSetting(true);
        repaint();
        isRecording = false;
        btn_record.setText(text_record);
        printf("Recording stopped.\n");
      //  spdlog::info("Recording stopped");
        rt->Stop();
        rt->Clear();
        if (!do_split_wav) {
          out[0]->Finish();
        }
        else {
          for (int i = 0; i < channels; i++)
            out[i]->Finish();
        }
        rec_thread->join();
        delete rec_thread;

      }
      /* Start Recording */
      else {
        EnableSetting(false);
        repaint();
        isRecording = true;
#if WIN32
        btn_record.setText("Stop[F6]");
#else
        btn_record.setText("Stop");
#endif

        /* Craete output */
        SetFileName();
        if (!do_split_wav)
          out[0]->NewFile(file_name);
        else {
          for (int i = 0; i < channels; i++) {
            strcpy(t_file_name, file_name);
            char* temp_str = strtok(t_file_name, ".");
            sprintf(ch_file_name, "%s_%d.wav", temp_str, i + 1);
            out[i]->NewFile(ch_file_name);
          }
        }
        //spdlog::info("File Created : {}",file_name);
        rt->SetRecordingInf();
        rt->Start();
        rec_thread = new std::thread(&KRecorderControl::Recording, this);
        printf("Recording started.\n");
        //spdlog::info("Recording started");
        label_status.setText(qstring_file_name);
        label_status.setToolTip(qstring_file_name);

        /* Start Stopwatch */
        chrono_start = std::chrono::system_clock::now();

        //timer.start(timer_delay);

      }
      break;
    case TIMER:
      if (!isRecording) {
        label_status.setText("Recording...");
        //force to repaint label
        EnableSetting(false);
        btn_record.setEnabled(false);
        repaint();
        isRecording = true;

        rt->SetRecordingTime(record_time);

        /* Craete output */
        SetFileName();
        if (!do_split_wav)
          out[0]->NewFile(file_name);
        else {
          for (int i = 0; i < channels; i++) {
            strcpy(t_file_name, file_name);
            char* temp_str = strtok(t_file_name, ".");
            sprintf(ch_file_name, "%s_%d.wav", temp_str, i + 1);
            out[i]->NewFile(ch_file_name);
          }
        }
        rt->Start();

        rec_thread = new std::thread(&KRecorderControl::Recording, this);
        rec_thread->detach();

        if (wait_thread)delete wait_thread;

        wait_thread = new std::thread(&KRecorderControl::WaitRecording, this);
        wait_thread->detach();
        /* Start Stopwatch */
        chrono_start = std::chrono::system_clock::now();
        //timer.start(timer_delay);
        btn_record.setEnabled(true);
        btn_record.setText("Stop");
        break;
      }
      /* Already running timered recording -> kill recording */
      else {
        rt->Stop();
      }
    case INTERVAL:
      if (!isRecording) {
        label_status.setText("Recording...");
        //force to repaint label
        EnableSetting(false);
        btn_record.setText("Stop");
        repaint();
        isRecording = true;
        is_interval_running = true;
        rec_thread = new std::thread(&KRecorderControl::IntervalRecording, this, recording_interval);
      }
      else {
        isRecording = false;
        rt->Stop();
        is_interval_running = false;
        EnableSetting(true);
        btn_record.setText(text_record);
        repaint();
        rt->Clear();
      }
      break;
    default:
      break;
  }
}

void KRecorderControl::Recording() {
  while (rt->IsRunning()) {
    if (rt->data.stock.load() >= shift_size) {
      emit(SignalRefreshTimer());
      rt->GetBuffer(temp);

      for (int i = 0; i < shift_size*3; i++) {
        temp_plot[i] = temp_plot[i+shift_size];
      }
      for (int i = 0; i < shift_size; i++) {
        temp_plot[i+shift_size*3] = temp[i*channels+ch_plot];
      }
      widget_plot.DrawPlot(temp_plot);

      /* input scaleing */

      if (!do_split_wav) {
        if(scale!=1)
          for (int i = 0; i < shift_size * channels; i++)
            temp[i] *= scale;
        out[0]->Append(temp, shift_size * channels);
      }
      else {
        if (scale != 1)
          for (int i = 0; i < shift_size * channels; i++) {
            temp[i] *= scale;
            buf_split[i % channels][i / channels] = temp[i];
          }
        for(int i=0;i<channels;i++)
          out[i]->Append(buf_split[i], shift_size);
      }

    }
  }
}


void KRecorderControl::WaitRecording() {

  while (rt->IsRunning()) { 

    SLEEP(100); 
  }

  /* Release */
  SignalStopTimer();
  if(!do_split_wav)
    out[0]->Finish();
  else {
    for(int i=0;i<channels;i++)
      out[i]->Finish();
  }
  label_status.setText(qstring_file_name);
  label_status.setToolTip(qstring_file_name);
  rt->Clear();
  delete rec_thread;
  isRecording = false;
  EnableSetting(true);
  btn_record.setText(text_record);
}

/* Slot */
void KRecorderControl::SetTime() {
  if (!is_setting_time.load()) {
    is_setting_time.store(true);
    chrono_elapsed = std::chrono::system_clock::now() - chrono_start;
    label_time.setText(QString::number(chrono_elapsed.count(), 'g'));
    is_setting_time.store(false);
  }
}

/* Slot */
void KRecorderControl::StopTimer() {
  timer.stop();
}

void KRecorderControl::EnableSetting(bool flag) {
  btn_init.setEnabled(flag);
  combo_interval.setEnabled(flag);
  combo_mode.setEnabled(flag);
  btn_save_path.setEnabled(flag);
  LE_timer.setEnabled(flag && (rec_mode == TIMER));
}

void KRecorderControl::SetFileName() {
  SetDateTime();
  /* Execption for "" */
  if(file_path.isEmpty())
    file_path_buf = QString(file_name);
  else
    file_path_buf = file_path + "/" + QString(file_name);

  qstring_file_name = file_path_buf;

  /* QString.toLocal8Bit() is OS independent routine, so far.
   * Worked well on Windows. */
  strcpy(file_name, file_path_buf.toLocal8Bit());
}

void KRecorderControl::ClearBuild(){
  if (isInited) {
    delete rt;
    delete[] temp;
    delete[] temp_plot;

    for (int i = 0; i < channels; i++)
      delete[] buf_split[i];
    delete[] buf_split;

    if (!do_split_wav)
      delete out[0];
    else
      for (int i = 0; i < channels; i++)
        delete out[i];

    for(int i=0;i<channels;i++){
      //LE_scale[i].setEnabled(false);
      ;
    }
  }
}

void KRecorderControl::IntervalRecording(int stock){
  //printf("Run interval Thread\n");
  // run recording thread
  rt->Start();

  //Create
  chrono_start = std::chrono::system_clock::now();
  SetFileName();
  if (!do_split_wav)
    out[0]->NewFile(file_name);
  else {
    for (int i = 0; i < channels; i++) {
      strcpy(t_file_name, file_name);
      char* temp_str = strtok(t_file_name, ".");
      sprintf(ch_file_name, "%s_%d.wav", temp_str, i + 1);
      out[i]->NewFile(ch_file_name);
    }
  }
  long long elap =0;

  while (rt->IsRunning()) {
    if (rt->data.stock >= shift_size) {
      emit(SignalRefreshTimer());
      // cut audio input for every given second  
      elap += shift_size;
      //std:cout << elap << std::endl;;
      // TODO

      rt->GetBuffer(temp);


      /* Plot */
      for (int i = 0; i < shift_size * 3; i++) {
        temp_plot[i] = temp_plot[i + shift_size];
      }
      for (int i = 0; i < shift_size; i++) {
        temp_plot[i + shift_size * 3] = temp[i * channels + ch_plot];
      }
      widget_plot.DrawPlot(temp_plot);

      /* input scaleing */
      if (!do_split_wav) {
        if (scale != 1)
          for (int i = 0; i < shift_size * channels; i++)
            temp[i] *= scale;
        out[0]->Append(temp, shift_size * channels);
      }
      else {
        if (scale != 1)
          for (int i = 0; i < shift_size * channels; i++) {
            temp[i] *= scale;
            buf_split[i % channels][i / channels] = temp[i];
          }
        for(int i=0;i<channels;i++)
          out[i]->Append(buf_split[i], shift_size);
      }

      if (elap >= stock) {
        //std::cout << "cut " << stock << "\n";
        elap = 0;

        // CUT
        if (!do_split_wav)
          out[0]->Finish();
        else
          for (int i = 0; i < channels; i++)
            out[i]->Finish();

        //Create
        chrono_start = std::chrono::system_clock::now();
        SetFileName();
        if (!do_split_wav)
          out[0]->NewFile(file_name);
        else {
          for (int i = 0; i < channels; i++) {
            strcpy(t_file_name, file_name);
            char* temp_str = strtok(t_file_name, ".");
            sprintf(ch_file_name, "%s_%d.wav", temp_str, i + 1);
            out[i]->NewFile(ch_file_name);
          }
        }
      }
    }
  }

  if (!do_split_wav)
    out[0]->Finish();
  else
    for (int i = 0; i < channels; i++)
      out[i]->Finish();
  printf("STOP\n");
}

#if _WIN32
void KRecorderControl::KeyboardInput() {
  bool run = true;

  while (run) {
    //F4
    if (GetKeyState(VK_F4) < 0 && !fF4){
      fF4 = true;
      BuildModule();
      //std::cout << "F4 pressed" << std::endl;
    }
    if (GetKeyState(VK_F4) >= 0 && fF4){
      //  std::cout << "F4 Released" << std::endl;
      fF4 = false;
    }
    //F6
    if (GetKeyState(VK_F6) < 0 && !fF6){
      fF6 = true;
      //spdlog::info("F6 Pressed");

      //std::cout << "F6 pressed" << std::endl;
      // Build Module every time.
      /*
         if (!isInited) {
         BuildModule();
         SLEEP(500);
         }
         */
      if (isRecording) {
        emit(SignalStartRecord());
      }
      else {
        BuildModule();
        SLEEP(500);
        emit(SignalStartRecord());
      }
    }
    if (GetKeyState(VK_F6) >= 0 && fF6){
      //  std::cout << "F6 Released" << std::endl;
      fF6 = false;
    }
    //Exit
    /*
       if ((GetKeyState(VK_F4) & 0x8000))
       {
       std::cout << "F4 pressed" << std::endl;
       run_main = false;
       run = false;
       }
       */
    SLEEP(10);
  }
}
#endif

void KRecorderControl::LoadFilePath() {
  std::ifstream ifs("../config/path.txt");
  if (!ifs.is_open())
    printf("ERROR::Can't open path.json");
  std::string tmp_str;
  std::getline(ifs, tmp_str);
  this->file_path = QString::fromUtf8(tmp_str.c_str());
  label_save_path.setText(file_path);
  //std::cout << j["PATH"].get<std::string>() << std::endl;
  ifs.close();
}

