# Audio-Recorder

## State  
GUI : WIP  
CLI : done  
  
## Clone   
  
Need to clone recursively.  
```
git clone --recursive https://github.com/koobh/Audio-Recorder.git
```
If already clonded,  
```
git submodule init  
git submodule update
```  
  
## Build  
Use CMake.  

```
	 Audio_reocrder/<CLI or GUI>/build$ cmake ..  
```   
then, if you are in Ubuntu, there will be ```Makefile``` or if you are in Windows, there will be viusal stuidio project.   
  
## OS    
+ ubuntu 20.04 LTS : 64-bit   
+ Windows 10 : 64-bit  

## Notice
|NAME|LICENSE|USEAGE|
|---|---|---|
[Qt6Framework](https://www.qt.io)|[LGPL](https://www.qt.io/terms-conditions/)|GUI, text encoding
[json](https://github.com/nlohmann/json)|[MIT](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT)| configuration management  
[RtAudio](https://www.music.mcgill.ca/~gary/rtaudio/) |[license](https://www.music.mcgill.ca/~gary/rtaudio/license.html)| recording   
