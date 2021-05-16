# AC_Indicator
This program causes the mouse (G403) to flash when the car's RPM reaches revolution.  
  
# Using
- Create `SDK` folder in this, then download from [LOGITECH G DEVELOPER LAB](https://www.logitechg.com/en-us/innovation/developer-lab.html) and copy `LED\Lib\LogitechLEDLib.lib`, `LED\Include\LogitechLEDLib.h` to `SDK` folder.
- As it is in conflict with G HUB, what you need to edit is the JSON file written as `".../AssettoCorsaArxApplet.exe"` in config.json in `C:\Users\USERNAME\AppData\Local\LGHUB\integrations_config`, `ledSdk.enable: true` to `ledSdk.enable: false`.
