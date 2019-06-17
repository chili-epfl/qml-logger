/*
 * Copyright (C) 2019 EPFL
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
 * @file RCSVLogger.h
 * @brief Header of a QML CSV file logger with robust data transfer capability
 * @author Alexandre Reynaud
 * @date 2019-03-14
 */

#ifndef RCSVLOGGER_H
#define RCSVLOGGER_H

#include <QQuickItem>
#include <QNetworkAccessManager>
#include <QtNetwork>

namespace QMLLogger {

/**
 * @brief Utility to generate logs, save them locally, then send them to a remote server in a robust fashion.
 *
 * This will dump all logs in the 'asbPath'+'filename' CSV file.
 * At the first call to the 'log(list<string> data)' slot, if the log file is empty
 * or newly created, a header will be dumped to the file as follows:
 *
 * '''
 *    'timestamp' if enabled, header[0], header[1], ..., header[N - 1]
 * '''
 *
 * After that, every call to the 'log(list<string> data)' slot will result in a line as follows in the log file:
 *
 * '''
 *     timestamp if enabled, data[0], data[1], ..., data[N - 1]
 * '''
 */
    class RCSVLogger : public QQuickItem {
        /* *INDENT-OFF* */
        Q_OBJECT
        /* *INDENT-ON* */

        /** @brief Desired CSV log file name (must end with '.csv'); the file will be put in the 'absPath' directory; default 'data.csv' */
        Q_PROPERTY(QString filename MEMBER filename)

        /** @brief Whether to include milliseconds in date and time, default 'true' */
        Q_PROPERTY(bool logMillis MEMBER logMillis)

        /** @brief Whether to print the log lines to the console for debug purposes, default 'false' */
        Q_PROPERTY(bool toConsole MEMBER toConsole)

        /** @brief Header fields (excluding timestamp); is ignored if the log file already has a header; default '[]' */
        Q_PROPERTY(QStringList header MEMBER header)

        /** @brief Whether to include the timestamp in every log line as the first field; is ignored if the log file already has a header; default 'true' */
        Q_PROPERTY(bool logTime MEMBER logTime)

        /** @brief Number of decimal places for printing floating point numbers, default '2'*/
        Q_PROPERTY(int precision MEMBER precision)

        /** @brief URL of the reception server; cannot be changed after a call to 'log()' until a call to 'push()'; default '127.0.0.1:8000' */
        Q_PROPERTY(QString serverURL MEMBER serverURL WRITE setServerURL READ getServerURL NOTIFY serverURLChanged)

        /** @brief Absolute path to the log storage; cannot be changed after a call to 'log()' until a call to 'push()'; default '/Documents/Logs' */
        Q_PROPERTY(QString absPath WRITE setAbsPath READ getAbsPath NOTIFY absPathChanged)
    public:
        /** @cond DO_NOT_DOCUMENT */

        /**
         * @brief Creates a new RCSVLogger with the given QML parent
         * @param parent The QML parent
         */
        RCSVLogger(QQuickItem* parent = nullptr);

        /**
         * @brief Destroys the logger
         */
        ~RCSVLogger();

        /**
         * @brief Sets the URL of the reception server; has no effect after the first log()
         * @param url The new URL
         */
        void setServerURL(const QString& url);

        /**
         * @brief Gets the URL of the reception server
         * @return The URL
         */
        QString getServerURL(){ return serverURL; }

        /**
         * @brief Sets the absolute path to the log storage; has no effect after the first log()
         * @param path The new path
         */
        void setAbsPath(const QString& path);

        /**
         * @brief Gets the absolute path to the log storage
         * @return The path
         */
        QString getAbsPath(){ return absPath; }

        /** @endcond */

    signals:

        /** @cond DO_NOT_DOCUMENT */

        /**
         * @brief Emitted when serverURL changes
         */
        void serverURLChanged();

        /**
         * @brief Emitted when absPath changes
         */
        void absPathChanged();

        /** @endcond */

    public slots:

        /**
         * @brief Logs given data as one entry
         * @param data Data to log; must conform to the header format if meaningful log is desired
         */
        void log(QVariantList const& data);

        /**
         * @brief Updates the local CSV database and the server, then frees the logger from constraints (serverURL and asbPath can be changed)
         */
        void push();

    private:

        QString absPath;               ///< Absolute path to the log storage (default = '/Documents/Logs/')
        QString filename;              ///< Name of the CSV file used to store the logs (relative path)
        QStringList header;         ///< Header to dump as the first line

        bool writing;                  ///< Log is being written

        QFile file;                    ///< Log file
        QTextStream writer;            ///< Log file writer

        bool logTime;                  ///< Whether to include timestamp as the first field when data is logged
        bool logMillis;                ///< Whether to include milliseconds in the timestamp
        bool toConsole;                ///< Whether to log to the console instead of the file and server (for debug purposes)
        int precision;                 ///< Precision of floats in the logs

        const QString timestampHeader="timestamp"; ///< Timestamp header field

        const QString logManagerPath="LogManager.csv"; ///< Name of the log manager (must end with '.csv')

        QString serverURL; ///< URL of the reception server
        QMap<QString,QList<int>> logManager; ///< Map linking each file to the number of lines they contain locally and remotely
        QMap<QString,QStringList> updates; ///< Map linking each file to the lines yet to be written locally

        QNetworkAccessManager *manager; ///< Network manager

        /**
         * @brief Builds the header string
         * @return Header string, including the timestamp as the first field if logTime is true
         */
        QString buildHeaderString();

        /**
         * @brief Builds the log row
         * @param data Data to log
         * @return Log row
         */
        QString buildLogLine(QVariantList const& data);

        /**
         * @brief Updates the local logs
         */
        void updateLocal();

        /**
         * @brief Updates the remote database to match the local one
         */
        void updateRemote();

        /**
         * @brief Loads (or creates if non-existant) the log manager
         * @return The log manager
         */
        QMap<QString,QList<int>> loadLogManager();

        /**
         * @brief Saves the current state of the log manager
         */
        void saveLogManager();

        /**
         * @brief Loads all lines of a given CSV file, starting from a given row number
         * @param name Name of the file
         * @param from First read byte
         * @return List of all read rows
         */
        QStringList CSVReader(QString name, int from);

        /**
         * @brief Writes all given lines at the end of the CSV file with the given name (meant for the log files)
         * @param name Name of the file
         * @param lines List of the rows to write
         * @return Amount of bytes written
         */
        int CSVLogWriter(QString name, QStringList lines);

        /**
         * @brief Writes all given lines at the beginning of the CSV file with the given name (meant for the log manager)
         * @param name Name of the file
         * @param lines List of the rows to write
         * @return Amount of bytes written
         */
        int CSVManagementWriter(QString name, QStringList lines);

        /**
         * @brief Transforms the given file name into an absolute path to the corresponding file
         * @param name Name of the file
         * @return Absolute path to the file
         */
        QString absolutePath(QString name);
    };

}
#endif // RCSVLOGGER_H
