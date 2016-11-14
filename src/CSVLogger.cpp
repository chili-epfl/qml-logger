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
 * @file CSVLogger.cpp
 * @brief Source for a QML CSV file logger
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#include "CSVLogger.h"

#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QSysInfo>
#include <QNetworkInterface>
#include <QBluetoothLocalDevice>

CSVLogger::CSVLogger(QQuickItem* parent) :
    QQuickItem(parent),
    timestampHeader("timestamp")
{
    logTime = true;
    logMillis = true;
    toConsole = false;

    fileNeedsReopen = false;
    writing = false;
}

CSVLogger::~CSVLogger(){
    writer.flush();
    file.close();
}

inline QString CSVLogger::buildLogLine(QList<QString> const& data){
    QString line = "";

    //Timestamp
    if(logTime){
        if(logMillis)
            line += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        else
            line += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    }

    //Rest of data
    if(data.size() != header.size())
        qWarning() << "CSVLogger::buildLogLine(): Data and header don't have the same length, log file will not be correct.";
    if(data.size() > 0){
        if(logTime)
            line += ", ";
        line += data.at(0);
    }
    for(int i = 1; i < data.size(); i++)
        line += ", " + data.at(i);

    return line;
}

inline QString CSVLogger::buildHeaderString(){
    QString headerString = "";
    if(logTime)
        headerString += timestampHeader;
    if(header.size() > 0){
        if(logTime)
            headerString += ", ";
        headerString += header.at(0);
    }
    for(int i = 1; i < header.size(); i++)
        headerString += ", " + header.at(i);
    return headerString;
}

void CSVLogger::setFilename(const QString& filename){
    if(this->filename != filename){
        writer.flush();
        file.close();

        this->filename = filename;

        fileNeedsReopen = true;
        writing = false;

        emit filenameChanged();
    }
}

void CSVLogger::setLogTime(bool logTime){
    if(this->logTime != logTime){
        if(writing)
            qCritical() << "CSVLogger::setLogTime(): logTime cannot be changed while writing.";
        else{
            this->logTime = logTime;
            emit logTimeChanged();
        }
    }
}

void CSVLogger::setHeader(QList<QString> const& header){
    if(this->header != header){
        if(writing)
            qCritical() << "CSVLogger::setHeader(): header cannot be changed while writing.";
        else{
            this->header = header;
            emit headerChanged();
        }
    }
}

void CSVLogger::log(QList<QString> const& data){
    if(!isEnabled())
        return;

    if(toConsole){
        qDebug() << buildLogLine(data);
        return;
    }

    //File needs re-opening
    if(fileNeedsReopen){
        QDir dir(filename);
        if(dir.isAbsolute())
            qDebug() << "CSVLogger: Opening " + filename + " to log.";
        else{
            filename = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation) + "/" + filename;
            qDebug() << "CSVLogger: Absolute path not given, opening " + filename + " to log.";
            emit filenameChanged();
        }
        QDir::root().mkpath(QFileInfo(filename).absolutePath());

        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
            qCritical() << "CSVLogger: Could not open file.";
            return;
        }
        else
            writer.setDevice(&file);

        fileNeedsReopen = false;
        writing = true;

        //Build and dump header if file is empty or newly created
        if(file.isOpen()){
            if(file.pos() == 0){
                writer << buildHeaderString() << "\n";
                writer.flush();
            }
        }
        else
            qCritical() << "CSVLogger: Attempted log() but file is not open, valid filename must be provided beforehand.";
    }

    //Actual data logging
    if(file.isOpen()){
        writer << buildLogLine(data) << "\n";
        writer.flush();
    }
    else
        qCritical() << "CSVLogger: Attempted log() but file is not open, valid filename must be provided beforehand.";
}
