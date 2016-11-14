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
 * @file CSVLogger.h
 * @brief Header for a QML CSV file logger
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#ifndef CSVLOGGER_H
#define CSVLOGGER_H

#include <QQuickItem>
#include <QString>
#include <QFile>

class CSVLogger : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    Q_PROPERTY(bool logTime WRITE setLogTime READ getLogTime NOTIFY logTimeChanged)
    Q_PROPERTY(bool logMillis WRITE setLogMillis READ getLogMillis NOTIFY logMillisChanged)
    Q_PROPERTY(bool deviceId READ getDeviceId CONSTANT)
    Q_PROPERTY(QString filename WRITE setFilename READ getFilename NOTIFY filenameChanged)
    Q_PROPERTY(bool toConsole MEMBER toConsole)



    Q_PROPERTY(qstringarr header WRITE READ NOTIFY) //setter should give qwarning() << "not editable" after first log

public:

    /**
     * @brief Creates a new CSVLogger with the given QML parent
     *
     * @param parent The QML parent
     */
    CSVLogger(QQuickItem* parent = 0);

    /**
     * @brief Destroys this CSVLogger
     */
    ~CSVLogger();

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

    /**
     * @brief Sets whether to timestamp the log lines
     *
     * @param logTime Whether to include timestamp
     */
    void setLogTime(bool logTime);

    /**
     * @brief Gets whether to timestamp the log lines
     *
     * @return Whether to timestamp the log lines
     */
    bool getLogTime(){ return logTime; }

    /**
     * @brief Sets whether to include milliseconds in timestamp
     *
     * @param logMillis Whether to include milliseconds in timestamp
     */
    void setLogMillis(bool logMillis);

    /**
     * @brief Gets whether to include milliseconds in timestamp
     *
     * @return Whether to include milliseconds in timestamp
     */
    bool getLogMillis(){ return logMillis; }

    /**
     * @brief Gets the unique device ID
     *
     * @return The unique device ID
     */
    QString getDeviceId(){ return deviceId; }

signals:

    /**
     * @brief Emitted when logTime changes
     */
    void logTimeChanged();

    /**
     * @brief Emitted when logMillis changes
     */
    void logMillisChanged();

    /**
     * @brief Emitted when filename changes
     */
    void filenameChanged();

public slots:

    /**
     * @brief Logs given data as one line to file
     *
     * @param data Data to log
     */
    void log(qstringable array log);//const QString& data);

private:

    bool logTime;           ///< Whether to include timestamp when data is logged
    bool logMillis;         ///< Whether to include milliseconds in the timestamp
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

#endif /* CSVLOGGER_H */
