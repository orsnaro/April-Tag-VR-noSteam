## This repo has Windows builds for @yoyobuae's "work" and "personal" forks of ApriltagVRTracker. 

The difference from main program is some tracking improvements and fixes.
Since @yoyobuae doesn't compile for Windows and these fixes will likely never hit Master due to refactoring, I've compiled and released the windows executables to save you the trouble.

### User note : If you just want to run this program on [Windows just use an exe from Releases.](https://github.com/Skyrion9/April-Tag-VR-FullBody-Tracker/releases)
Features of __work__ branch:
Optimized build flags, updated exposure script.
Improved tracker calibration, reduces tracker misalignment especially noticable with dodecahedron or cubic tracker setups. (@yoyobuae)
Privacy mode for camera view. (@yoyobuae)
Full list of changes : https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/compare/master...yoyobuae:April-Tag-VR-FullBody-Tracker:work

I recommend you read through entire FAQ if you decide to build yourself.

Git fails to install vcpkg to source folder. The workaround is simple, if you've a working Vcpkg that comes with VS2022 on a Windows system, it should be used instead. VS installer takes good care of vcpkg anyway.
NOTE: This repo only contains fixes for branches __work__ and __personal__, but if the branch you want includes Vcpkg good chance this'll fix it.

Compile steps for Windows:

__1-__  Download and install VS 2022, check enable building for C/C++ desktop applications during installation. I've built with 17.7.5, but PREVIEW version is recommended for __personal__ branch, as it fixes a bug in vcpkg but may cause other issues for the other branches. Disregard this if your version is >= 17.8

__2-__  On Github, "Code > Open in visual studio" to clone this repo. Wait until it's done.

__3-__ In VS 2022, Git > Manage branches and double click on the remote branch you want to use. Also click Delete and Regenerate cache afterwards. 

__4-__ Let it rip. Hopefully it'll build without errors. When you get output : ```CMake generation finished```, proceed to next step. If you got CCX compiler errors after the line ```Vcpkg install - done``` ignore this. VS is trying to use an incorrect install build configuration (of its own making).

__5-__ Hit View > Terminal and paste the following code:
```
cmake -B build
cmake --build build --config Release --target install
```

You can find the program in "Install" folder if it compiled successfully.

## Notes
This is only tested on @yoyobuae 's __work__ and __personal__ branches on Windows 10. Other branches with Linux specific apps/services might fail, some don't even ship with vcpkg.
As to what causes the error, git seems to fail to download vcpkg, likely something about how cmake and git interacts since manually cloning works fine.

## FAQ (Probably?)
__1-__ Why did you create an entire readme for a simple fix?
There are still ongoing fixes from Microsoft's side, which you won't find out about until you run into an error. Those have been pending for months. This workaround deviates from the intended method also, and @yoyobuae compiles for Linux only. I wanted an updated ApriltagVRTracker, found a good branch and decided to build it.

Do note my builds are not endorsed by the original developers. I recommend you try the original releases first.

__2-__ I still can't compile
Either use a precompiled exe from Release, or make sure to follow each step exactly, Without alternating to different IDEs etc. I've included builds for multiple architectures. Pay attention to Output window and fix anything missing. It's usually a vcpkg bug. In case of missing directory errors, double check your Windows Environment path and add any missing paths that show up in the output. Remember to restart VS and regenerate cache so path changes are recognized.

__2.1-__ If you've an error related to Powershell, I recommend installing Powershell 7. Add it to your User ENV path (Regardless of installation ENV setting. As it adds to System path, but I had to manually add it to User ENV.) . Sometimes VS installer clears up your path.

Same path addition should fix the default Powershell if you want to skip PS7.

__2.2-__ [There's a bug where Vcpkg can't install/find Msys](https://github.com/microsoft/vcpkg/issues/31565#issuecomment-1723267213). If your VS 2022 version is older than 17.8, you'll have this error. It is however fixed in 17.8 Preview 2 or newer. 
The branch "personal" has this issue, likely because it's updated with the original master branch. "work" had no issues with Msys and compiled successfully on 17.7.5 with the given steps.


__3-__ This is bad for x y z reasons
You're probably right, but it works and as far as dependencies go I prefer to use what's already installed on my system due to size concerns.

__4-__ How does this fix it?
This stops enforcing source vcpkg and uses your own if you've one installed and set in Windows Environment, doesn't affect other OSes, so it is inconsequential to the build process as CMake will recognize and use VS 2022's vcpkg.

__5-__ I've an error, how can I get help?
If you can't reproduce the same error on official branches, and believe it to be something related to the workaround we're using, you can create an Issue here. Double check your steps and make sure you've read through all of this.

Otherwise try ApriltagVR's discord. You'll get better help there. Reminder @yoyobuae 's branches are only tested on Linux, and they won't provide build help for Windows.

__6-__ Why are your files large?
~~They contain crypto miners~~ 
My builds utilize [optimized compiler flags that increase speed at a size cost](https://github.com/Skyrion9/April-Tag-VR-FullBody-Tracker/commit/d00a0411700d79b631ed62507400e0edd9c46e3f), have reduced function calls and may improve performance. This build is larger than the 0.7.1 official build by about 11 MB for *work* and 22 MB for *personal*. 
It might reduce the CPU overhead and it's nice to have for weaker CPUs like mine. The only downside is increased file size. Especially helpful for multi-instance multi-camera setups.
I've ~5% CPU usage with previews disabled on a ryzen 1600 with AVX2 and 2 trackers actively being tracked. 



__7-__ Wow you've read this far? Here's a little reward for you.
```
@@echo off
start "" "%~dp0AprilTagTrackers.exe"
call "%~dp0\utilities\set_exposure.bat"
exit
```
Script to start Apriltag and apply IP Webcam settings simultaneously.
Create a notepad file, copy paste the above code in, rename it to "APTGVR_IPCam.bat" file and move it next to AprilTagTrackers.exe and run it as Administrator (Make a shortcut and in properties > shortcut > advanced enable it)

It'll automatically apply your parameters configured within utilities\set_exposure.bat and launch APTVR, so make sure to update that bat file with your IP Camera parameters first!

Also updated the exposure script along the way, which is more verbose now instead of disappearing on you.


### Original Readme below, also follow the wiki page on the original master branch to set it up.


## This guide is also available in the following languages:

- ![汉语 (Simplified Chinese)](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/blob/master/%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87%E6%95%99%E7%A8%8B%EF%BC%88SimplifiedChineseTutorial%EF%BC%89.md)

## [>>Full Tutorial Here<<](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/wiki)

For the full setup guide, click on the link above to access the wiki!

## Having questions or issues?

Ask in the help channel of our discord server: https://discord.gg/g2ctkXB4bb

# April-Tag-VR-FullBody-Tracker

Full-body tracking in VR using AprilTag markers.

This is my second attempt at creating a full-body tracking system using fiducial markers. This should enable people to get fullbody tracking for free, using only a phone and some cardboard. It is possible to get pretty good tracking with trackers of sizes as small as 10cm and a PS eye camera of 640x480 resolution. Increasing the marker size or using a higher resolution and faster phone camera further improves tracking.

**NOTE: THIS IS A FREE AND OPEN SOURCE PROJECT. YOU DO NOT NEED DRIVER4VR FOR THIS!**

To use, you will have to make three trackers - one for each leg and one for hips. Using only leg trackers will not work in VRChat!

This version uses the much more accurate AprilTag system and includes many improvements to make the system easier to use, such as a GUI interface and a more straightforward  calibration.

If you have any issues or encounter any bugs, feel free to open an issue on github or message me on discord: https://discord.gg/g2ctkXB4bb

The program can be downloaded from the [releases](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/releases) tab.

![demo](images/demo.gif)

Beatsaber demo: https://youtu.be/Akps-dH0EeA

### Short setup video:
I am too bad at making actual tutorials, but I did record a short video of me setting up everything. It's not a replacement for the tutorial below, but it may help you understand some of the steps better.

https://youtu.be/ncN8Vw_0DrE

## Build instructions:

**NOTE: THIS IS ONLY FOR DEVELOPERS. IF YOU ONLY WANT TO USE APRILTAGTRACKERS AND NOT WRITE CODE, THE TUTORIAL IS ON THE [WIKI](https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker/wiki)**

The project is a CMake project. You should be able to build it either using CMake and your favourite IDE/compiler, or some IDEs already support opening cmake projects directly.

### Linux prerequisites
```
sudo apt-get update -y
sudo apt-get install -y build-essential tar curl zip unzip pkg-config autoconf libudev-dev freeglut3-dev libgtk-3-dev libsecret-1-dev libgcrypt20-dev libsystemd-dev ffmpeg
```
OpenCV gstreamer backend
```
sudo apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```


### Windows prerequisites
Open in Visual Studio, or use the Visual Studio Command Prompt.


### Clone and build
```
git clone https://github.com/ju1ce/April-Tag-VR-FullBody-Tracker
cd April-Tag-VR-FullBody-Tracker
cmake -B build
cmake --build build --config Release --target install
```

That should be it! In case you try it before a more detailed guide is up, we are always there to help on the discord server! (link above)

### Troubleshooting

If anything goes wrong when building, please notify us in the discord/via a github issue!

## Sources
Olson, Edwin. "AprilTag: A robust and flexible visual fiducial system." 2011 IEEE International Conference on Robotics and Automation. IEEE, 2011.

https://github.com/AprilRobotics/apriltag

WxWidgets: A Cross-Platform GUI Library

https://github.com/wxWidgets/wxWidgets
