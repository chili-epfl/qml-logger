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
 * @file LoggerUtil.cpp
 * @brief Logger utilities header
 * @author Ayberk Özgür
 * @date 2016-11-14
 */

#include "LoggerUtil.h"

#include<QSysInfo>
#include<QBluetoothLocalDevice>
#include<QNetworkInterface>

#ifdef ANDROID
    #include <QtAndroid>
#endif

namespace QMLLogger{

LoggerUtil::LoggerUtil(QQuickItem* parent) : QQuickItem(parent){ }

LoggerUtil::~LoggerUtil(){ }

QString LoggerUtil::getUniqueDeviceID(){
    QString deviceId = QSysInfo::prettyProductName();

    QString macAddr("");

    //Try Bluetooth first, it is more reliable than WiFi and Ethernet
    for(QBluetoothHostInfo const& info : QBluetoothLocalDevice::allDevices()){
        QString hwAddr(info.address().toString());
        if(hwAddr != "00:00:00:00:00:00" && hwAddr != "02:00:00:00:00:00"){
            macAddr = hwAddr;
            break;
        }
    }

    //Try WiFi/Ethernet next
    if(macAddr == "")
        for(QNetworkInterface const& interface : QNetworkInterface::allInterfaces())
            if(!(interface.flags() & QNetworkInterface::IsLoopBack)){
                QString hwAddr = interface.hardwareAddress();
                qDebug() << hwAddr;
                if(hwAddr != "00:00:00:00:00:00"){
                    macAddr = hwAddr;
                    break;
                }
            }

    #ifdef ANDROID

        //Try /sys/class/net/wlan0/address for Android 6
        if(macAddr == ""){
            QFile wifiMacAddressFile("/sys/class/net/wlan0/address");
            if(wifiMacAddressFile.open(QIODevice::ReadOnly | QIODevice::Text)){
                QString hwAddr = wifiMacAddressFile.readLine();
                hwAddr = hwAddr.replace("\n","");
                wifiMacAddressFile.close();
                if(hwAddr != "00:00:00:00:00:00" && hwAddr != "02:00:00:00:00:00")
                    macAddr = hwAddr;
            }
        }
    #endif

    if(macAddr != "")
        deviceId += " @ " + macAddr;
    else
        qWarning() << "LoggerUtil::uniqueDeviceID(): Couldn't get any MAC address of device, device ID won't be unique!";

    deviceId.replace(" ", "_");
    return deviceId;
}

bool LoggerUtil::androidSyncPermission(QString const& permission){
#ifdef ANDROID
    QtAndroid::PermissionResult permissionResult = QtAndroid::checkPermission(permission);
    if(permissionResult == QtAndroid::PermissionResult::Denied){
        QtAndroid::requestPermissionsSync(QStringList() << permission);
        permissionResult = QtAndroid::checkPermission(permission);
        if(permissionResult == QtAndroid::PermissionResult::Denied){
            qCritical() << "LoggerUtil::androidSyncPermission(): Could not get permission, logging will not work!";
            return false;
        }
    }
    return true;
#else
    Q_UNUSED(permission);
    return true;
#endif
}

}
