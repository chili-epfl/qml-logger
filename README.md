qml-logger
==========

qml-logger is a file logging plugin that enables logging data to given file from QML. It is tested with Qt 5.10.1 on the
following:

- Ubuntu 17.10
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

See [samples/](samples/) for example uses.

See [doc/index.html](doc/index.html) for the API.

build [Linux & macOS]
---------------------

```
  $ mkdir build && cd build
  $ qt-install-dir/qt-version/target-platform/bin/qmake ..
  $ make install
```

This will install the QML plugin inside the Qt sysroot, which you must have write access to. **Be aware that this is not a sandboxed installation.**

build [Android]
---------------

```
  $ export ANDROID_NDK_ROOT=/path-to-android-ndk/
  $ mkdir build && cd build
  $ qt-install-dir/qt-version/target-platform/bin/qmake ..
  $ make install
```

This will install the QML plugin inside the Qt sysroot, which you must have write access to. **Be aware that this is not a sandboxed installation.**

build [Windows]
---------------

Run the following in the `Developer Command Prompt for VS 2017`:

```
  > "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
  > mkdir build
  > cd build
  > qt-install-root\qt-version\target-platform\bin\qmake ..
  > nmake
  > nmake install
```

This will install the QML plugin inside the Qt sysroot, which you must have write access to. **Be aware that this is not a sandboxed installation.**

build documentation
-------------------

Install dependencies:
```
  $ apt install doxygen doxyqml
```

Then, generate the documentation:
```
  $ doxygen
```

RCSVLogger
-----------

Utilization
------------
'''
	Initialization:
		RCSVLogger{

        		id: logger
        		filename: "data.csv" (dafault) --- Name of the CSV log file (it is placed in the "absPath" directory)
        		header: [] (default) --- Header of the logs (excluding "timestamp")
        		serverURL: "http://127.0.0.1:8000/" (default) --- URL of the reception server	
			logTime: true (default) --- Whether to include a timestamp at the beginning of each log or not
    			logMillis: true (default) --- In the case timestamps are added to the logs, whether to express them in milliseconds or seconds
    			toConsole: false (default) --- Whether to print the logs in the console instead of saving them (for debugging purposes) or not
    			precision: 2 (default) --- Precision of log-saved floats
			absPath: "/.../Documents/Logs" (default) --- Absolute path to the local CSV database
		}
	Log Writing:
		logger.log([data]) --- "data" must match the inputed header
'''
Limitations
------------
'''
	RCSVLogger will work with any header but, taking into account the structure of the SQLite3 database waiting on the other side,
	it is preferable to use one of the following formats:
		-["game_id","user_id","iter","robot_id","x","y","theta"], logTime=true --- for the table "Position"
		-["game_id","user_id","zone_id","iter","robot_id","event","state"], logTime=true --- for the table "DiscreteEvent"
		-["game_id","user_id","zone_id","iter","robot_id","event","value"], logTime=true --- for the table "ContinuousEvent"
		-["game_id","name"], logTime=false --- for the table "Game"
		-["user_id","name"], logTime=false --- for the table "User"
		-["zone_id","name"], logTime=false --- for the table "Zone"
'''
