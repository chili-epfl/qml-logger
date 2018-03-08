/*
 * Copyright (C) 2016 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file SimpleLogger.h
 * @brief Header for a simple line-by-line logger
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#ifndef SIMPLELOGGER_H
#define SIMPLELOGGER_H

#include <QQuickItem>
#include <QString>
#include <QFile>

namespace QMLLogger{

/**
 * @brief Utility to log strings line by line with optional timestamp and unique device ID.
 *
 * Unless given a full path, this will dump all log actions to the file with the given name under the Documents
 * directory of the current user, whatever this is configured as under the specific OS. Every call to the
 * `log(string data)` slot will result in a line as follows in the log file:
 *
 * ```
 *     [timestamp in yyyy-MM-dd HH:mm:ss.zzz format if enabled] [unique device ID if enabled] data
 * ```
 */
class SimpleLogger : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Desired log filename; if full path is not given, log file will be put in default documents directory */
    Q_PROPERTY(QString filename WRITE setFilename READ getFilename NOTIFY filenameChanged)

    /** @brief Whether to include the timestamp in every log line, default true */
    Q_PROPERTY(bool logTime MEMBER logTime)

    /** @brief Whether to include milliseconds in date and time, default true */
    Q_PROPERTY(bool logMillis MEMBER logMillis)

    /** @brief Whether to include the local unique device info in every log line, default true */
    Q_PROPERTY(bool logDeviceInfo MEMBER logDeviceInfo)

    /** @brief Whether to print the log lines to the console for debug purposes, default false */
    Q_PROPERTY(bool toConsole MEMBER toConsole)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new SimpleLogger with the given QML parent
     *
     * @param parent The QML parent
     */
    SimpleLogger(QQuickItem* parent = 0);

    /**
     * @brief Dumps remaining logs to file and destroys this SimpleLogger
     */
    ~SimpleLogger();

    /**
     * @brief Sets the file name; puts file to home directory if full path is not given
     *
     * @param filename The new filename, or full path
     */
    void setFilename(const QString& filename);

    /**
     * @brief Gets the filename
     *
     * @return The filename
     */
    QString getFilename(){ return filename; }

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when filename changes
     */
    void filenameChanged();

    /** @endcond */

public slots:

    /**
     * @brief Logs given data as one line to file
     *
     * @param data Data to log
     */
    void log(const QString& data);

private:

    bool logTime;           ///< Whether to include time when data is logged
    bool logMillis;         ///< Whether to include milliseconds when logging time
    bool logDeviceInfo;     ///< Whether to include local unique device info when data is logged
    QString filename;       ///< Log's filename or full path
    bool fileNeedsReopen;   ///< Filename changed and file needs reopening

    QFile file;             ///< Log file
    QTextStream writer;     ///< Log file writer

    bool toConsole;         ///< Log to console instead of file for debug purposes

    QString deviceId;       ///< Unique device ID

    /**
     * @brief Builds the log line info prefix
     *
     * @return Log line info prefix
     */
    QString linePrefix();

};

}

#endif /* SIMPLELOGGER_H */
