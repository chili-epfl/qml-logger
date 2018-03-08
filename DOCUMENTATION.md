# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`QMLLogger`](#namespaceQMLLogger) | 

# namespace `QMLLogger` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`QMLLogger::CSVLogger`](#classQMLLogger_1_1CSVLogger) | Utility to log CSV data line by line with optional timestamp.
`class `[`QMLLogger::LoggerUtil`](#classQMLLogger_1_1LoggerUtil) | Logger utilities.
`class `[`QMLLogger::SimpleLogger`](#classQMLLogger_1_1SimpleLogger) | Utility to log strings line by line with optional timestamp and unique device ID.

# class `QMLLogger::CSVLogger` 

```
class QMLLogger::CSVLogger
  : public QQuickItem
```  

Utility to log CSV data line by line with optional timestamp.

Unless given a full path, this will dump all log actions to the file with the given name under the Documents directory of the current user, whatever this is configured as under the specific OS. At the first call to the `log(list<string> data)` slot, if the log file is empty or newly created, a header will be dumped to the file as follows:

```cpp
timestamp if enabled, header[0], header[1], ..., header[N - 1]
```

After that, every call to the `log(list<string> data)` slot will result in a line as follows in the log file:

```cpp
timestamp in yyyy-MM-dd HH:mm:ss.zzz format if enabled, data[0], data[1], ..., data[N - 1]
```

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`{property} QString `[`filename`](#classQMLLogger_1_1CSVLogger_1a9db30c672dff6b43760bbcb1811a8a91) | Desired log filename; if full path is not given, log file will be put in default documents directory.
`{property} bool `[`logTime`](#classQMLLogger_1_1CSVLogger_1a2850689779d0c326606f6506a27371a9) | Whether to include the timestamp in every log line as the first field, cannot be changed after a call to `[log()](#classQMLLogger_1_1CSVLogger_1a73ea1e5c7366c5f0b0e4dd36aa8c973c)` until a call to `[close()](#classQMLLogger_1_1CSVLogger_1a77f7a8148fd1556c6342c84ef45232c0)`, default `true`
`{property} bool `[`logMillis`](#classQMLLogger_1_1CSVLogger_1a51bd24a4ac8e9e0e3779812ab76af333) | Whether to include milliseconds in date and time, default `true`
`{property} bool `[`toConsole`](#classQMLLogger_1_1CSVLogger_1af599b63efe8319311d9a5655c3b75569) | Whether to print the log lines to the console for debug purposes, default `false`
`{property} int `[`precision`](#classQMLLogger_1_1CSVLogger_1a8f0273e02d9eb41699bc2e86d983f2bc) | Number of decimal places for printing floating point numbers, default `2`
`{property} QList< QString > `[`header`](#classQMLLogger_1_1CSVLogger_1aeb90c315493d8fc581fdb40a61ca8867) | Header fields (excluding timestamp), cannot be changed after a call to `[log()](#classQMLLogger_1_1CSVLogger_1a73ea1e5c7366c5f0b0e4dd36aa8c973c)` until a call to `[close()](#classQMLLogger_1_1CSVLogger_1a77f7a8148fd1556c6342c84ef45232c0)`, default `[]`
`{slot} public void `[`log`](#classQMLLogger_1_1CSVLogger_1a73ea1e5c7366c5f0b0e4dd36aa8c973c)`(QVariantList const & data)` | Logs given data as one entry.
`{slot} public void `[`close`](#classQMLLogger_1_1CSVLogger_1a77f7a8148fd1556c6342c84ef45232c0)`()` | Closes the log file.

## Members

#### `{property} QString `[`filename`](#classQMLLogger_1_1CSVLogger_1a9db30c672dff6b43760bbcb1811a8a91) 

Desired log filename; if full path is not given, log file will be put in default documents directory.

Log's filename or full path.

#### `{property} bool `[`logTime`](#classQMLLogger_1_1CSVLogger_1a2850689779d0c326606f6506a27371a9) 

Whether to include the timestamp in every log line as the first field, cannot be changed after a call to `[log()](#classQMLLogger_1_1CSVLogger_1a73ea1e5c7366c5f0b0e4dd36aa8c973c)` until a call to `[close()](#classQMLLogger_1_1CSVLogger_1a77f7a8148fd1556c6342c84ef45232c0)`, default `true`

Whether to include timestamp as the first field when data is logged.

#### `{property} bool `[`logMillis`](#classQMLLogger_1_1CSVLogger_1a51bd24a4ac8e9e0e3779812ab76af333) 

Whether to include milliseconds in date and time, default `true`

Whether to include milliseconds in the timestamp.

#### `{property} bool `[`toConsole`](#classQMLLogger_1_1CSVLogger_1af599b63efe8319311d9a5655c3b75569) 

Whether to print the log lines to the console for debug purposes, default `false`

Log to console instead of file for debug purposes.

#### `{property} int `[`precision`](#classQMLLogger_1_1CSVLogger_1a8f0273e02d9eb41699bc2e86d983f2bc) 

Number of decimal places for printing floating point numbers, default `2`

Number of decimal places to print to the log for floats.

#### `{property} QList< QString > `[`header`](#classQMLLogger_1_1CSVLogger_1aeb90c315493d8fc581fdb40a61ca8867) 

Header fields (excluding timestamp), cannot be changed after a call to `[log()](#classQMLLogger_1_1CSVLogger_1a73ea1e5c7366c5f0b0e4dd36aa8c973c)` until a call to `[close()](#classQMLLogger_1_1CSVLogger_1a77f7a8148fd1556c6342c84ef45232c0)`, default `[]`

Header to dump on the first line.

#### `{slot} public void `[`log`](#classQMLLogger_1_1CSVLogger_1a73ea1e5c7366c5f0b0e4dd36aa8c973c)`(QVariantList const & data)` 

Logs given data as one entry.

#### Parameters
* `data` Data to log, must conform to the header format if meaningful log is desired

#### `{slot} public void `[`close`](#classQMLLogger_1_1CSVLogger_1a77f7a8148fd1556c6342c84ef45232c0)`()` 

Closes the log file.

# class `QMLLogger::LoggerUtil` 

```
class QMLLogger::LoggerUtil
  : public QQuickItem
```  

Logger utilities.

This class is **singleton**, you can call `ThisClass.anyFunction()` and use `ThisClass.anyProperty` directly.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`{property} QString `[`uniqueDeviceID`](#classQMLLogger_1_1LoggerUtil_1a8492385c3b09756b44023881e9a9dcc6) | Constant, unique device ID (without spaces) if possible, non-unique ID if not.

## Members

#### `{property} QString `[`uniqueDeviceID`](#classQMLLogger_1_1LoggerUtil_1a8492385c3b09756b44023881e9a9dcc6) 

Constant, unique device ID (without spaces) if possible, non-unique ID if not.

# class `QMLLogger::SimpleLogger` 

```
class QMLLogger::SimpleLogger
  : public QQuickItem
```  

Utility to log strings line by line with optional timestamp and unique device ID.

Unless given a full path, this will dump all log actions to the file with the given name under the Documents directory of the current user, whatever this is configured as under the specific OS. Every call to the `log(string data)` slot will result in a line as follows in the log file:

```cpp
[timestamp in yyyy-MM-dd HH:mm:ss.zzz format if enabled] [unique device ID if enabled] data
```

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`{property} QString `[`filename`](#classQMLLogger_1_1SimpleLogger_1acb7d62bf0a704dd23b7d4a2c5f782c32) | Desired log filename; if full path is not given, log file will be put in default documents directory.
`{property} bool `[`logTime`](#classQMLLogger_1_1SimpleLogger_1a3df298d037c9d5a5ffe39d8ded8b426d) | Whether to include the timestamp in every log line, default true.
`{property} bool `[`logMillis`](#classQMLLogger_1_1SimpleLogger_1adbcd5344afe7a5ff58e762a4b34328a4) | Whether to include milliseconds in date and time, default true.
`{property} bool `[`logDeviceInfo`](#classQMLLogger_1_1SimpleLogger_1a580ca4d5c8d731506e29c78aab899f41) | Whether to include the local unique device info in every log line, default true.
`{property} bool `[`toConsole`](#classQMLLogger_1_1SimpleLogger_1a4aa4bab85434ff1a7ef8ca5556ba5351) | Whether to print the log lines to the console for debug purposes, default false.
`{slot} public void `[`log`](#classQMLLogger_1_1SimpleLogger_1ab7ead4ea661f64090a3cd26ea2c89ece)`(const QString & data)` | Logs given data as one line to file.

## Members

#### `{property} QString `[`filename`](#classQMLLogger_1_1SimpleLogger_1acb7d62bf0a704dd23b7d4a2c5f782c32) 

Desired log filename; if full path is not given, log file will be put in default documents directory.

Log's filename or full path.

#### `{property} bool `[`logTime`](#classQMLLogger_1_1SimpleLogger_1a3df298d037c9d5a5ffe39d8ded8b426d) 

Whether to include the timestamp in every log line, default true.

Whether to include time when data is logged.

#### `{property} bool `[`logMillis`](#classQMLLogger_1_1SimpleLogger_1adbcd5344afe7a5ff58e762a4b34328a4) 

Whether to include milliseconds in date and time, default true.

Whether to include milliseconds when logging time.

#### `{property} bool `[`logDeviceInfo`](#classQMLLogger_1_1SimpleLogger_1a580ca4d5c8d731506e29c78aab899f41) 

Whether to include the local unique device info in every log line, default true.

Whether to include local unique device info when data is logged.

#### `{property} bool `[`toConsole`](#classQMLLogger_1_1SimpleLogger_1a4aa4bab85434ff1a7ef8ca5556ba5351) 

Whether to print the log lines to the console for debug purposes, default false.

Log to console instead of file for debug purposes.

#### `{slot} public void `[`log`](#classQMLLogger_1_1SimpleLogger_1ab7ead4ea661f64090a3cd26ea2c89ece)`(const QString & data)` 

Logs given data as one line to file.

#### Parameters
* `data` Data to log

Generated by [Moxygen](https://sourcey.com/moxygen)