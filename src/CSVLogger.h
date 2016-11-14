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

    Q_PROPERTY(QString filename WRITE setFilename READ getFilename NOTIFY filenameChanged)
    Q_PROPERTY(bool logTime WRITE setLogTime READ getLogTime NOTIFY logTimeChanged)
    Q_PROPERTY(bool logMillis MEMBER logMillis)
    Q_PROPERTY(bool toConsole MEMBER toConsole)
    Q_PROPERTY(QList<QString> header WRITE setHeader READ getHeader NOTIFY headerChanged)

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
     * @brief Sets whether to log the timestamp as the first field, has no effect after the first log()
     *
     * @param logTime Whether to log the timestamp as the first field
     */
    void setLogTime(bool logTime);

    /**
     * @brief Gets whether the timestamp is being logged
     *
     * @return Whether the timestamp is being logged
     */
    bool getLogTime(){ return logTime; }

    /**
     * @brief Sets the new header to be dumped to the log file on its first open if it's empty
     *
     * @param header New header
     */
    void setHeader(QList<QString> const& header);

    /**
     * @brief Gets the current header
     *
     * @return The current header
     */
    QList<QString> getHeader(){ return header; }

signals:

    /**
     * @brief Emitted when filename changes
     */
    void filenameChanged();

    /**
     * @brief Emitted when logTime changes
     */
    void logTimeChanged();

    /**
     * @brief Emitted when the header changes
     */
    void headerChanged();

public slots:

    /**
     * @brief Logs given data as one entry
     *
     * @param data Data to log, must conform to the header format if meaningful log is desired
     */
    void log(QList<QString> const& data);







    //void clasdasdasdasdose();












private:

    QString filename;              ///< Log's filename or full path
    QList<QString> header;         ///< Header to dump on the first line

    bool fileNeedsReopen;          ///< Filename changed and file needs reopening
    bool writing;                  ///< Log is being written

    QFile file;                    ///< Log file
    QTextStream writer;            ///< Log file writer

    bool logTime;                  ///< Whether to include timestamp as the first field when data is logged
    bool logMillis;                ///< Whether to include milliseconds in the timestamp
    bool toConsole;                ///< Log to console instead of file for debug purposes

    const QString timestampHeader; ///< Timestamp header field string

    /**
     * @brief Builds and gets the header string
     *
     * @return Header string, including the timestamp as the first field if logTime is true
     */
    QString buildHeaderString();

    /**
     * @brief Builds and gets the log row
     *
     * @brief data Data to log
     * @return Log row
     */
    QString buildLogLine(QList<QString> const& data);

};

#endif /* CSVLOGGER_H */
