qml-logger
==========

qml-logger is a file logging plugin that enables logging data to given file from QML. It is tested with Qt 5.6.0 on the
following:

  - Ubuntu 16.04
  - Android 5.0.2 (arm-v7) built with SDK API 19 and NDK r10e on Ubuntu 16.04 host

build
-----

```
    $ mkdir build && cd build
    $ qmake ..
    $ make install
```

This will install the QML plugin inside the Qt sysroot. **Be aware that this is not a sandboxed installation.** See `samples/` for example usage.

QML API
-------

Parameters:

>  - **logTime** :            `bool` - Whether to include the date and time in every log line, default true
>  - **logLocalHostName** :   `bool` - Whether to include the local host name in every log line, default true
>  - **logCaller** :          `bool` - Whether to include the caller object that logged the data in every log line, default true
>  - **filename** :           `string` - Desired log filename; if full path is not given, log file will be put in home directory

Slots:

>  - **log(`string`)** :        Logs the given string to the file as a line
