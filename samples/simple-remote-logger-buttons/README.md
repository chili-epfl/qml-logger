simple-logger-button
====================

Example use of `SimpleLogger` in `qml-logger`. Follow the build instructions in [the
qml-logger README](../../README.md) before trying to run this sample. It is tested with Qt 5.10.1 on the
following:

- Ubuntu 17.10
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

build & run [Ubuntu & macOS & Android]
--------------------------------------

Load the project into QtCreator and press the Run button with the big green arrow.

build & run [Windows]
---------------------

  - Load the project into QtCreator, select the `Release` build (and not the `Debug` and `Profile` build)
  - Press the Run button with the big green arrow once, this will install and launch the app (you may have to enable `Developer mode` in `Settings` -> `Update & Security` -> `For developers` -> `Use developer features`; this windows will pop up anyway if you haven't enabled it already)
  - For all subsequent launches, use the app launcher now installed in the Start Menu

**Important note:** On Windows, the log file will be put under the local data directory of the app since access is not given
to write to any other directory than this within WinRT sandboxing. This will typically be
C:\Users\your-username\AppData\Local\Packages\app-uuid\LocalState. Be careful when using the Run button
in QtCreator, as this will destroy this directory each time the app is launched, taking your potentially
valuable data with it! Instead, launch your app from the Start Menu, which will preserve this directory.
