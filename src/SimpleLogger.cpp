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
 * @file SimpleLogger.cpp
 * @brief Source for a simple line-by-line logger
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#include "SimpleLogger.h"

#include <QDir>
#include <QStandardPaths>
#include <QDateTime>

#include "LoggerUtil.h"

namespace QMLLogger{

SimpleLogger::SimpleLogger(QQuickItem* parent) : QQuickItem(parent){
    logTime = true;
    logMillis = true;
    logDeviceInfo = true;
    toConsole = false;

    fileNeedsReopen = false;

    deviceId = "[" + LoggerUtil::getUniqueDeviceID() + "] ";

    LoggerUtil::androidSyncPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    LoggerUtil::androidSyncPermission("android.permission.READ_EXTERNAL_STORAGE");
}

SimpleLogger::~SimpleLogger(){
    writer.flush();
    file.close();
}

inline QString SimpleLogger::linePrefix(){
    QString res = "";
    if(logTime){
        if(logMillis)
            res += "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") + "] ";
        else
            res += "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "] ";
    }
    if(logDeviceInfo)
        res += deviceId;
    return res;
}

void SimpleLogger::setFilename(const QString& filename){
    if(this->filename != filename){
        writer.flush();
        file.close();

        this->filename = filename;

        fileNeedsReopen = true;

        emit filenameChanged();
    }
}

void SimpleLogger::log(const QString& data){
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
            qDebug() << "SimpleLogger::log(): Opening " + filename + " to log.";
        else{
            filename = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation) + "/" + filename;
            qDebug() << "SimpleLogger::log(): Absolute path not given, opening " + filename + " to log.";
            emit filenameChanged();
        }
        QDir::root().mkpath(QFileInfo(filename).absolutePath());

        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
            qCritical() << "SimpleLogger::log(): Could not open file: " << file.errorString();
            return;
        }
        else
            writer.setDevice(&file);

        fileNeedsReopen = false;
    }

    //Actual data logging
    if(file.isOpen()){
        writer << linePrefix() << data << "\n";
        writer.flush();
    }
    else
        qCritical() << "SimpleLogger::log(): File is not open, valid filename must be provided beforehand.";
}

}
