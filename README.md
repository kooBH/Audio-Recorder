# Audio-Recorder    
  
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
[Qt6Framework](https://www.qt.io/) : for GUI
[json](https://github.com/nlohmann/json) : for configuration management
[RtAudio](https://www.music.mcgill.ca/~gary/rtaudio/) : for recording 
