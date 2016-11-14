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

### `SimpleLogger`

Utility to log strings line by line with optional timestamp and unique device ID. Unless given a full path, this will
dump all log actions to the file with the given name under the Documents directory of the current user, whatever this is
configured as under the specific OS. Every call to the `log(string data)` slot will result in a line as follows in the
log file:

```
    [date & time if enabled] [device info if enabled] data
```

**Parameters:**

>  - **enabled** :            `bool` -      If false, nothing happens when log() is called, default true
>  - **logTime** :            `bool` -      Whether to include the date and time in every log line, default true
>  - **logMillis** :          `bool` -      Whether to include milliseconds in date and time, default true
>  - **logDeviceInfo** :      `bool` -      Whether to include the local unique device info in every log line, default true
>  - **filename** :           `string` -    Desired log filename; if full path is not given, log file will be put in default documents directory
>  - **toConsole** :          `bool` -      Whether to print the log lines to the console for debug purposes, default false

**Slots:**

>  - **log(** `string` data **)** :        Logs the given string to the file as a line
