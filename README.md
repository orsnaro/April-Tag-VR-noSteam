> [!CAUTION]
> MOVED TO AZURE REPOS platform due to GitHub's data/Quota/bandwidth limitations: [Azure repo link](https://dev.azure.com/Reality-SimSuit/Reality-SimSuit/_git/April-Tag-VR-noSteam)

# 🚀 VISIT THE NEW [OSC noSteam v1.0 Branch](https://github.com/orsnaro/April-Tag-VR-noSteam/tree/ATT_OSC_noSteam) 

#### This fork will be more directed to work with Reality SimSuit project and Willie's ExitSuit 


# ORSNARO Important Notes:

> [!WARNING]  
> This branch is for the experimental Apriltag VR Tracker noSteam Version (many files are built in debug config)

> [!WARNING]  
> READ THIS Before Using The [detailed Build Guide](detailed-build-guide) with This Fork:

* This is built using `Visual Studio 17.10 preview 2` and didn't work for me on `vs17.9`
* ensure that VS IDE has Those Workloads At least: `Desktop development with C++`,`Universal Windows Platform development`
* other dev Env specs: `Win10 Pro 10.0.1.19.045 build 19045 X64`, `cmake 3.27.1`, `cmder` or `powershell 7.4.1`
* dont forget to do the `liblo.dll` copy step to `install\` at end for OSC and the whole exe to work! (the detailed tutorial lacks this step)


> [!TIP]  
> most of the logs for SimSuit,OSC and noSteam feature related events will be  written to:

* `C:\VS2022_repos\April-Tag-VR-noSteam\install\logs\simSuitAprilLog.txt`. 
* The rest of Apiltag Tracker logs will be in `C:\VS2022_repos\April-Tag-VR-noSteam\install\logs\*.log`

# Start of original README.md <sub>(with minor adds to build section)</sub>
-------

### (OUTDATED) This guide is also available in the following languages:

- [汉语 (Simplified Chinese)](/%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87%E6%95%99%E7%A8%8B%EF%BC%88SimplifiedChineseTutorial%EF%BC%89.md)

### [>>Full Tutorial Here<<](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/wiki)

For the full setup guide, click on the link above to access the wiki!

### Having questions or issues?

Ask in the help channel of our discord server: https://discord.gg/g2ctkXB4bb

## April-Tag-VR-FullBody-Tracker

Full-body tracking in VR using AprilTag markers.

This is my second attempt at creating a full-body tracking system using fiducial markers. This should enable people to get fullbody tracking for free, using only a phone and some cardboard. It is possible to get pretty good tracking with trackers of sizes as small as 10cm and a PS eye camera of 640x480 resolution. Increasing the marker size or using a higher resolution and faster phone camera further improves tracking.

**NOTE: THIS IS A FREE AND OPEN SOURCE PROJECT. YOU DO NOT NEED DRIVER4VR FOR THIS!**

To use, you will have to make three trackers - one for each leg and one for hips. Using only leg trackers will not work in VRChat!

This version uses the much more accurate AprilTag system and includes many improvements to make the system easier to use, such as a GUI interface and a more straightforward  calibration.

If you have any issues or encounter any bugs, feel free to open an issue on github or message me on discord: https://discord.gg/g2ctkXB4bb

The program can be downloaded from the [releases](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/releases) tab.

![demo](images/demo.gif)

Beatsaber demo: https://youtu.be/Akps-dH0EeA

#### Short setup video:
I am too bad at making actual tutorials, but I did record a short video of me setting up everything. It's not a replacement for the tutorial below, but it may help you understand some of the steps better.

https://youtu.be/ncN8Vw_0DrE

#### I also have a ko-fi!

https://ko-fi.com/ju1ce

Donations are apreciated but not necessary!

### Build instructions:

**NOTE: THIS IS ONLY FOR DEVELOPERS. IF YOU ONLY WANT TO USE APRILTAGTRACKERS AND NOT WRITE CODE, THE TUTORIAL IS ON THE [WIKI](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/wiki)**

The project is a CMake project. You should be able to build it either using CMake and your favourite IDE/compiler, or some IDEs already support opening cmake projects directly.

#### (NOT TESTED IN THIS FORK)Linux prerequisites
```
sudo apt-get update -y
sudo apt-get install -y build-essential tar curl zip unzip pkg-config autoconf libudev-dev freeglut3-dev libgtk-3-dev libsecret-1-dev libgcrypt20-dev libsystemd-dev ffmpeg
```
OpenCV gstreamer backend
```
sudo apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```


#### Windows prerequisites
Open in Visual Studio 17.10 Preview 2 [(Includes a fix)](https://github.com/microsoft/vcpkg/issues/31565#issuecomment-1723267213), and use the Visual Studio Command Prompt.

#### Clone and build
```
git clone https://github.com/orsnaro/April-Tag-VR-noSteam
cd April-Tag-VR-noSteam
cmake -B build
cmake --build build --config Release --target install
copy .\liblo\x64-Debug-vs2022win64generator\Debug\liblo.dll .\install\
```

 #### detailed build guide
That should be it! [A more detailed guide is up](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/blob/master/WindowsBuild_Simple.md), and we are always there to help on the (discord server)[https://discord.gg/g2ctkXB4bb]!

#### Troubleshooting

If anything goes wrong when building, please notify us in the discord/via a github issue!

## Sources
Olson, Edwin. "AprilTag: A robust and flexible visual fiducial system." 2011 IEEE International Conference on Robotics and Automation. IEEE, 2011.

https://github.com/AprilRobotics/apriltag

WxWidgets: A Cross-Platform GUI Library

https://github.com/wxWidgets/wxWidgets
