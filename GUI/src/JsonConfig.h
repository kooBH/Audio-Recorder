#ifndef _H_JSON_CONFIG_
#define _H_JSON_CONFIG_

/* JsonConfig.h
  parse json file and put it into map type
*/

#include <iostream>
//https://stackoverflow.com/questions/41236982/how-to-iterate-over-variadic-function-with-stdstring-arguments
#include <vector>
#include <map>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using std::string;
using std::map;
using std::vector;

/*
 * usage :
 * ConfigJson config("config.json",{"WPE","POST_GAMMA","TAB_DELAY"})
 * */

template <typename T>
class JsonConfig{
  protected:
    map <string,T> data;
    string file_path;
   	vector<string> args;
    bool is_open;

  public:
    inline JsonConfig();
  	inline JsonConfig(string file_path);
    inline JsonConfig(string file_path, vector<string> );
    inline ~JsonConfig();
    inline void Load();
    inline void Print();
    inline map<string,T> GetData();
    inline string GetName();

    /* Config["KEY"] -> return Value */
    T& operator[](string key){
     return data[key];
    }

    bool IsOpen(){return is_open;}
};

template<typename T>
inline JsonConfig<T>::JsonConfig(string _file_path,vector<string> _args):
  JsonConfig<T>(_file_path)
{
  args = _args;
  Load();
}

template<typename T>
inline JsonConfig<T>::JsonConfig(string _file_path) :
  JsonConfig<T>(){
    file_path = _file_path ;
  }

template<typename T>
inline JsonConfig<T>::JsonConfig(){
  is_open = false;
}

template<typename T>
inline JsonConfig<T>::~JsonConfig(){

}

template<typename T>
inline void JsonConfig<T>::Load(){
  std::ifstream ifs(file_path);
  json j = json::parse(ifs);
  auto it = args.begin();

  string name = *it;
#ifndef NDEBUG
  std::cout<<"JsonConfig::Load("<<name<<")\n";
#endif
  it++;
  while(it!=args.end()){
  data[*it] = j[name][*it].template get<T>();
  it++;
  
  }
  ifs.close();
}

template<typename T>
inline void JsonConfig<T>::Print(){
  for( auto const& it : data)
    std::cout<<it.first<<" : " << it.second<<"\n";
}

template<typename T>
inline map<string,T> JsonConfig<T>::GetData(){
  return data;
}

template<typename T>
inline string JsonConfig<T>::GetName(){
  return args[0];
}

#endif
