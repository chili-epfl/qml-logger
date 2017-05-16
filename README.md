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

### `LoggerUtil`

Singleton object containing various Logger utilities.

**Properties:**

>  - **uniqueDeviceID** : `string [read-only]` - Unique device ID (without spaces) if possible, non-unique device ID if not

### `SimpleLogger`

Utility to log strings line by line with optional timestamp and unique device ID. Unless given a full path, this will
dump all log actions to the file with the given name under the Documents directory of the current user, whatever this is
configured as under the specific OS. Every call to the `log(string data)` slot will result in a line as follows in the
log file:

```
    [timestamp in yyyy-MM-dd HH:mm:ss.zzz format if enabled] [unique device ID if enabled] data
```

**Properties:**

>  - **enabled** :            `bool` -      If false, nothing happens when `log()` is called, default true
>  - **logTime** :            `bool` -      Whether to include the timestamp in every log line, default true
>  - **logMillis** :          `bool` -      Whether to include milliseconds in date and time, default true
>  - **logDeviceInfo** :      `bool` -      Whether to include the local unique device info in every log line, default true
>  - **filename** :           `string` -    Desired log filename; if full path is not given, log file will be put in default documents directory
>  - **toConsole** :          `bool` -      Whether to print the log lines to the console for debug purposes, default false

**Slots:**

>  - **log(** `string` data **)** :        Logs the given string to the file as a line

### `CSVLogger`

Utility to log CSV data line by line with optional timestamp. Unless given a full path, this will dump all log actions
to the file with the given name under the Documents directory of the current user, whatever this is configured as under
the specific OS. At the first call to the `log(list<string> data)` slot, if the log file is empty or newly created, a
header will be dumped to the file as follows:

```
    timestamp if enabled, header[0], header[1], ..., header[N - 1]
```

After that, every call to the `log(list<string> data)` slot will result in a line as follows in the
log file:

```
    timestamp in yyyy-MM-dd HH:mm:ss.zzz format if enabled, data[0], data[1], ..., data[N - 1]
```

**Properties:**

>  - **enabled** :      `bool` -            If false, nothing happens when `log()` is called, default `true`
>  - **header** :       `list<string>` -    Header fields (excluding timestamp), cannot be changed after a call to `log()` until a call to `close()`, default `[]`
>  - **logTime** :      `bool` -            Whether to include the timestamp in every log line as the first field, cannot be changed after a call to `log()` until a call to `close()`, default `true`
>  - **logMillis** :    `bool` -            Whether to include milliseconds in date and time, default `true`
>  - **filename** :     `string` -          Desired log filename; if full path is not given, log file will be put in default documents directory
>  - **toConsole** :    `bool` -            Whether to print the log lines to the console for debug purposes, default `false`
>  - **precision** :    `int` -             Number of decimal places for printing floating point numbers, default `2`

**Slots:**

>  - **log(** `list<var>` data **)** :      Logs the given data to the file as a line with comma separated fields
>  - **close()** :                          Closes the log file    
