# Command Line Interface Recording Programm  

# v1

## How to Use  

+ Windows  
1. Open Command Prompt or Power shell,  
and probe audio devices.    
You need to check  
	1) the device number of device you want to use.  
	2) the number of channels of the device.
	3) the samplerate which the device supports.    
       ( this can be uncorrect)    
```
./AudioProbe.exe
```   
  
2. open ```config.json```  
and set parameters as of the device you want to use.    
If there is ```value``` item in a parameter, modify ```value``` item, not ```options```  
  
3. Record  
```
./Recorder.exe  
```  
4. Stop  
```
Ctrl + C  
```  

# v2
