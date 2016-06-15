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
 * @file Logger.h
 * @brief QML wrapper for Logger
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <QQuickItem>
#include <QString>
#include <QFile>

class Logger : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    Q_PROPERTY(bool logTime MEMBER logTime)
    Q_PROPERTY(bool logMillis MEMBER logMillis)
    Q_PROPERTY(bool logDeviceInfo MEMBER logDeviceInfo)
    Q_PROPERTY(QString filename WRITE setFilename READ getFilename)

public:

    /**
     * @brief Creates a new Logger with the given QML parent
     *
     * @param parent The QML parent
     */
    Logger(QQuickItem* parent = 0);

    /**
     * @brief Destroys this Logger
     */
    ~Logger();

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
    QString getFilename(){
        return filename;
    }

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
    bool filenameChanged;   ///< Filename changed and file needs reopening

    QFile file;             ///< Log file
    QTextStream writer;     ///< Log file writer

    QString deviceId;       ///< Unique device ID
};

#endif /* LOGGER_H */
