# AC_Indicator
This program causes the mouse (G403) to flash when the car's RPM reaches maximum.  
**Written by a begginer. So if something happens, I can't take any responsibility.**
  
# Using
- Create `SDK` folder in this.
- Download from [LOGITECH G DEVELOPER LAB](https://www.logitechg.com/en-us/innovation/developer-lab.html).
- Copy `LED\Lib\LogitechLEDLib.lib` and `LED\Include\LogitechLEDLib.h` to `SDK` folder.
- You need to edit is the config.json written as `".../AssettoCorsaArxApplet.exe"` from `%USERPROFILE%\AppData\Local\LGHUB\integrations_config`.
  - Because conflict with G HUB. 
  - From `ledSdk.enable: true` to `ledSdk.enable: false`.
