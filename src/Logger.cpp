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
 * @file Logger.cpp
 * @brief Implementation of the QML wrapper for Logger
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#include "Logger.h"

#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QSysInfo>
#include <QNetworkInterface>
#include <QBluetoothLocalDevice>

Logger::Logger(QQuickItem* parent) : QQuickItem(parent){
    logTime = true;
    logMillis = false;
    logDeviceInfo = true;

    fileNeedsReopen = false;

    toConsole = false;

    /*
     * Build device ID string
     */

    deviceId = "[" + QSysInfo::prettyProductName();

    QString macAddr("");

    //Try Bluetooth first, it is more reliable than WiFi and Ethernet
    for(QBluetoothHostInfo& info : QBluetoothLocalDevice::allDevices()){
        QString hwAddr(info.address().toString());
        if(hwAddr != "00:00:00:00:00:00" && hwAddr != "02:00:00:00:00:00"){
            macAddr = hwAddr;
            break;
        }
    }

    //Try WiFi/Ethernet next
    if(macAddr == "")
        for(QNetworkInterface& interface : QNetworkInterface::allInterfaces())
            if(!(interface.flags() & QNetworkInterface::IsLoopBack)){
                QString hwAddr = interface.hardwareAddress();
                qDebug() << hwAddr;
                if(hwAddr != "00:00:00:00:00:00"){
                    macAddr = hwAddr;
                    break;
                }
            }

    if(macAddr != "")
        deviceId += " @ " + macAddr;
    else
        qWarning() << "Logger: Couldn't get any MAC address of device, device ID won't be unique!";

    deviceId += "] ";
}

Logger::~Logger(){
    writer.flush();
    file.close();
}

inline QString Logger::linePrefix(){
    QString res = "";
    if(logTime){
        if(logMillis)
            res += "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "] ";
        else
            res += "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "] ";
    }
    if(logDeviceInfo)
        res += deviceId;
    return res;
}

void Logger::setLogTime(bool logTime){
    if(this->logTime != logTime){
        this->logTime = logTime;
        emit logTimeChanged();
    }
}

void Logger::setLogMillis(bool logMillis){
    if(this->logMillis != logMillis){
        this->logMillis = logMillis;
        emit logMillisChanged();
    }
}

void Logger::setLogDeviceInfo(bool logDeviceInfo){
    if(this->logDeviceInfo != logDeviceInfo){
        this->logDeviceInfo = logDeviceInfo;
        emit logDeviceInfoChanged();
    }
}

void Logger::setFilename(const QString& filename){
    if(this->filename != filename){
        writer.flush();
        file.close();

        this->filename = filename;

        fileNeedsReopen = true;

        emit filenameChanged();
    }
}

void Logger::log(const QString& data){
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
            QString docDir = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation);
            qDebug() << "Logger: Absolute path not given, opening " + docDir + "/" + filename + " to log.";
            QDir::setCurrent(docDir);
        }

        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
            qCritical() << "Logger: Could not open file.";
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
        qCritical() << "Logger: Attempted log() but file is not open, valid filename must be provided beforehand.";
}
