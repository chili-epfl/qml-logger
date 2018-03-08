qml-logger
==========

qml-logger is a file logging plugin that enables logging data to given file from QML. It is tested with Qt 5.10.0 on the
following:

  - Ubuntu 16.04
  - Android 6.0.0 (arm-v7) built with SDK API 23 and NDK r10e on Ubuntu 16.04 host

  See [samples/](samples/) for example uses.

  See [DOCUMENTATION.md](DOCUMENTATION.md) for the API.

  build
  -----

  ```
      $ mkdir build && cd build
      $ qmake ..
      $ make install
  ```

  This will install the QML plugin inside the Qt sysroot. **Be aware that this is not a sandboxed installation.**

  build documentation
  -------------------

  ```
      $ doxygen
      $ moxygen --output DOCUMENTATION.md xml
  ```
