
## Prerequistes
1. [Zoom Account](https://zoom.us)
2. [Zoom Meeting SDK App Type](https://marketplace.zoom.us)
2. Authentication token signed by 
    1. Client ID
    2. Client Secret
3. Meeting Recording Token (Optional)
 ======================================================================

## Getting Started

### Download Linux Meeting SDK from marketplace.zoom.us

1 decompress the downloaded `zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx.tar` and copy the files in the decompressed folder to these sample-app folders

 -  `h` to `demo/include/h`
 - `qt_libs` to `demo/lib/zoom_meeting_sdk/qt_libs`
 - 4 of the `lib******.so` files to `demo/lib/zoom_meeting_sdk/lib******.so`
 - copy `translation.json` to `demo/lib/zoom_meeting_sdk/json`
 - softlink  `libmeetingsdk.so` to `libmeetingsdk.so.1` within `demo/lib/zoom_meeting_sdk/` . You can use the command `ln -s libmeetingsdk.so libmeetingsdk.so.1` to do so.

2 Execute `cmake -B build` in demo/ folder . After successful` cmake`, the `demo/build/` folder will be generated.

3 Execute `make` in the `/demo/build/` directory to compile

### Notes for using the demo:
1. The sdk auth token, password, meeting number etc.. needs to be written in the config.txt file.
The [recording_token](https://developers.zoom.us/docs/meeting-sdk/apis/#operation/meetingLocalRecordingJoinToken) when entered, will allow you to recording with additional actions from host.

`GetVideoRawData` and `GetAudioRawData` are boolean control variables. You can search for them in the sample code to understand the flow.

```

meeting_number: "1234567890"
token: "xxxxxxxx.yyyyyyyyyyyyyyyyyy.zzzzzzzzzzzzzzzzz"
meeting_password: "123456"
recording_token: ""
GetVideoRawData: "true"
GetAudioRawData: "true"

```
2. You need to fill in `withoutloginParam.userName = "";`  in `meeting_sdk_demo.cpp`. `meeting_sdk_demo.cpp` is the starting point of the demo app.
3. Once init sdk and auth sdk are successfully executed, only then you can join and leave meeting.
4. Once you have successfully joined a meeting, subscribing to raw audio and raw video requires either
    1. either host/co-host/local-recording rights or 
    2. meeting recording token
5. You will need a file named `~\.config.us\zoomus.conf` before you can access raw audio in an environment without soundcard, such as docker and WSL environment. The sample zoomus.conf file is generated in the `setup-pulseaudio.sh` files

## CMakeLists.txt
If you have additional cpp and/or .h files, you might need to be included in this CMakeLists.txt

```
add_executable(meetingSDKDemo 
              ${CMAKE_SOURCE_DIR}/meeting_sdk_demo.cpp
              ${CMAKE_SOURCE_DIR}/init_auth_sdk_workflow.cpp
              ${CMAKE_SOURCE_DIR}/meeting_sdk_util.cpp
              ${CMAKE_SOURCE_DIR}/init_auth_sdk_workflow.h
              ${CMAKE_SOURCE_DIR}/meeting_sdk_util.h
              ${CMAKE_SOURCE_DIR}/RegressionTestRawdataRender.cpp
              ${CMAKE_SOURCE_DIR}/RegressionTestRawdataRender.h
              ${CMAKE_SOURCE_DIR}/custom_ui_eventSink.h
              ${CMAKE_SOURCE_DIR}/custom_ui_eventSink.cpp
              ${CMAKE_SOURCE_DIR}/MeetingReminderEventListener.h
              ${CMAKE_SOURCE_DIR}/MeetingReminderEventListener.cpp
              )
```

Similarly for libraries

```
target_link_libraries(meetingSDKDemo gcc_s gcc)
target_link_libraries(meetingSDKDemo meetingsdk)
target_link_libraries(meetingSDKDemo glib-2.0)
target_link_libraries(meetingSDKDemo pthread)
```

# Docker




==============================================================

## Ubuntu 



#### Install necessary dependencies
```
apt-get update && apt-get install -y build-essential cmake

apt-get install -y --no-install-recommends --no-install-suggests \
    libx11-xcb1 \
    libxcb-xfixes0 \
    libxcb-shape0 \
    libxcb-shm0 \
    libxcb-randr0 \
    libxcb-randr0 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-xtest0 
 ```
 #### optional libraries
``` 
apt-get install -y --no-install-recommends --no-install-suggests \
    libdbus-1-3 \
    libglib2.0-0 \
    libgbm1 \
    libxfixes3 \
    libgl1 \
    libdrm2 \
    libgssapi-krb5-2 
```


#### CURL related dependencies
```
apt-get install libcurl4-openssl-dev \
    openssl \
    ca-certificates \
    pkg-config 
```

#### Install Pulseaudio
```
apt-get install -y  pulseaudio pulseaudio-utils 
```

#### if you are getting error about <SDL2/SDL.h>
```
apt-get install libegl-mesa0 libsdl2-dev g++-multilib
```




## Addition requirement for raw audio data in docker / headless environment with no soundcard
Additional file in ~/.config/zoomus.conf

[General]
system.audio.type=default

## Dockerfiles


Dockerfile targetting different distros are provided in this sample app as well (Dockerfile-centos, Dockerfile-Ubuntu .....)
Currently this is tested on 
- Centos 9 (functionality where custom function of fetching JWT Token from Web Service does not work, main SDK function works fully)
- Centos 8
- Ubuntu 22

## Run these in /demo directory


#### Centos 8
docker build -t msdk-5.16.5-on-centos8-compact -f Dockerfile-Centos8/Dockerfile .
docker run -it --rm msdk-5.16.5-on-centos8-compact

#### Centos 9
docker build -t msdk-5.16.5-on-centos9-compact -f Dockerfile-Centos9/Dockerfile .
docker run -it --rm msdk-5.16.5-on-centos9-compact

#### Ubuntu 22
docker build -t msdk-5.16.5-on-ubuntu-compact -f Dockerfile-Ubuntu/Dockerfile .
docker run -it --rm msdk-5.16.5-on-ubuntu-compact


## Pulseaudio

The configuration for pulseaudio has been provided for you as shell scripts.

The setup is done via `setup-pulseaudio.sh` and `setup-pulssaudio-centos.sh`, this need to be run prior to running this project in a docker environment.

The script 
- starts the pulseaudio service, 
- creates a virtual speaker, 
- a virtual microphone, and 
- add a zoomus.conf file in the docker environment.

