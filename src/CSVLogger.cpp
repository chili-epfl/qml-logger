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

CSVLogger::CSVLogger(QQuickItem* parent) : QQuickItem(parent){
    logTime = true;
    logMillis = true;

    fileNeedsReopen = false;

    toConsole = false;

    devid
}

CSVLogger::~CSVLogger(){
    writer.flush();
    file.close();
}

inline QString CSVLogger::linePrefix(){
    QString res = "";
    if(logTime){
        if(logMillis)
            res += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        else
            res += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    }
    return res;
}

void CSVLogger::setLogTime(bool logTime){
    if(this->logTime != logTime){
        this->logTime = logTime;
        emit logTimeChanged();
    }
}

void CSVLogger::setLogMillis(bool logMillis){
    if(this->logMillis != logMillis){
        this->logMillis = logMillis;
        emit logMillisChanged();
    }
}

void CSVLogger::setFilename(const QString& filename){
    if(this->filename != filename){
        writer.flush();
        file.close();

        this->filename = filename;

        fileNeedsReopen = true;

        emit filenameChanged();
    }
}

void CSVLogger::log(const QString& data){
    if(!isEnabled())
        return;

    if(toConsole){
        qDebug() << linePrefix() + data;
        return;
    }

    //File needs re-opening
    if(fileNeedsReopen){
        QDir dir(filename);
        if(dir.isAbsolute())
            qDebug() << "Logger: Opening " + filename + " to log.";
        else{
            filename = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation) + "/" + filename;
            qDebug() << "Logger: Absolute path not given, opening " + filename + " to log.";
            emit filenameChanged();
        }
        QDir::root().mkpath(QFileInfo(filename).absolutePath());

        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
            qCritical() << "Logger: Could not open file.";
            return;
        }
        else
            writer.setDevice(&file);


        //// DUMP HEADER


        fileNeedsReopen = false;
    }

    //Actual data logging
    if(file.isOpen()){
        writer << linePrefix() << data << "\n";
        writer.flush();
    }
    else
        qCritical() << "Logger: Attempted log() but file is not open, valid filename must be provided beforehand.";
}
