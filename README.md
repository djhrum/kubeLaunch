# KubeLaunch
Simple launcher for kubectl and not only for that.

## Note
This utility was created to make my routine work more comfortable. 

Designed on the principles of the minimum necessary functionality.

## Description
This utility provide you to run many pre-configured instances of kubectl without additional console windows and aggrigare all the output into a single log window.

Utility running in the system tray. And all actions you can do in tray-menu.

## Features
* Run or stop instance of kubectl
* Look logs
* Look and change config
* Automatic restart of all running instances every 1 minute


![Main menu](https://github.com/djhrum/kubeLaunch/raw/master/tray-menu.png)

![Settings window](https://github.com/djhrum/kubeLaunch/raw/master/settings.png)

## Configuration

The config file is located in the user's home directory, with name **kubelaunch.ini**.

The config file should contain:
```
[system]
kubepath=%Full Path To Kubectl%
kuberestart=%Auto Restart Flag (false or true)%
[list]
%Name Of Param1%=%List Of Kubectl Arguments%
%Name Of Param2%=%List Of Kubectl Arguments%
....
```

If config file does not exist, KubeLauch will be creat it.

You can find sample config into release archive.
