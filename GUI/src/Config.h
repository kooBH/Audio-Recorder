#ifndef _H_GET_CONFIG_
#define _H_GET_CONFIG_

#include <fstream>
#include <string>

#include "JsonConfig.h"

using json = nlohmann::json;
using std::string;

/* Need to be implemented by itself 
 * Since data types are not equal.
 * */
class ConfigInput{
  private:
    bool is_open;

  public:
    bool real_time;
    int device;
    /* json can't handle utf properly. so Input file will be 
	   dealed with plain text file */
	//std::string input_file;
    double scale;
    ConfigInput(string _path_config){
      //input_file= _path_config;
      is_open = false;
      Load();

    }
    void Load(){
      std::ifstream ifs(input_file);

      if(ifs.is_open())
        is_open = true;
      else
        is_open = false;

      json j = json::parse(ifs);
      real_time = j["INPUT"]["REAL_TIME"].get<bool>();
      device = j["INPUT"]["DEVICE"].get<int>();
      //input_file = j["INPUT"]["INPUT_FILE"].get<std::string>();
      scale = j["INPUT"]["SCALE"].get<double>();
      ifs.close();
    }
    /* Since ConfigInput has multiple member types.. */
    bool IsRealTime(){return real_time;}
    int GetDevice(){return device;}
    //std::string GetInputFile(){return input_file;}
};

class ConfigParam : public JsonConfig<int> {
  public:
    ConfigParam(string _file_path) 
		: JsonConfig(_file_path){

		args = { "PARAM",
			"CHANNEL",
			"FRAME_SIZE",
			"SHIFT_SIZE",
			"SAMPLE_RATE"};

      Load();
    }
    void Load() {
      std::ifstream ifs(file_path);
      
      /* Error Handdling */
      if(ifs.is_open())
        is_open = true;
      else
        is_open = false;


      json j = json::parse(ifs);
      auto it = args.begin();

      string name = *it;
      it++;
      while(it!=args.end()){
        data[*it] = j[name][*it]["VALUE"].get<int>();
        it++;
      }
      ifs.close();
    };
};

#endif
