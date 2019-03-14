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
 * @file RCSVLogger.cpp
 * @brief Source for a QML CSV file logger for online databases
 * @author Alexandre Reynaud
 * @date 2019-03-14
 */

#include "LoggerUtil.h"
#include "RCSVLogger.h"
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QSysInfo>
#include <QNetworkInterface>
#include <QBluetoothLocalDevice>

namespace QMLLogger{

RCSVLogger::RCSVLogger(QQuickItem* parent) :
        QQuickItem(parent),
        timestampHeader("timestamp"),
        logManager(loadLogManager())
{
    logTime = true;
    logMillis = true;
    toConsole = false;
    precision = 2;

    writing = false;

    serverIp="127.0.0.1";
    port=2000;

    LoggerUtil::androidSyncPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    LoggerUtil::androidSyncPermission("android.permission.READ_EXTERNAL_STORAGE");
}

RCSVLogger::~RCSVLogger(){
    close();
}

void RCSVLogger::updateLocal(){
    foreach (QString path, updates.keys()) {
        if(logManager.contains(path)){
            logManager[path].first()+=CSVLogWriter(path,updates[path]);
        } else {
            logManager[path]={CSVLogWriter(path,updates[path])+1,0};
        }
    }
    saveLogManager();
}

void RCSVLogger::updateRemote(){
    QTcpSocket* socket=new QTcpSocket(this);
    socket->connectToHost(serverIp,(quint16)port);
    socket->waitForConnected();
    foreach (QString path, updates.keys()) {
        if(logManager[path].first()>logManager[path].last()){

            QList<QStringList> temp=CSVReader(path,logManager[path].last());
            temp.append({path});
            QByteArray data;
            QDataStream stream(&data, QIODevice::ReadWrite);
            stream << (qint32)temp.size();
            socket->write(data);
            stream << temp;
            socket->write(data);
            if(socket->waitForBytesWritten()){
                logManager[path].last()=logManager[path].first();
            }
        }

    }
    saveLogManager();

}

int RCSVLogger::CSVLogWriter(QString path, QList<QVariantList> lines){
    QString absPath=absolutePath(path);
    QDir::root().mkpath(QFileInfo(absPath).absolutePath());

    file.setFileName(absPath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        qCritical() << "RCSVLogger::CSVLogWriter(): Could not open file: " << file.errorString();
        return 0;
    }
    else
        writer.setDevice(&file);
        writing = true;

    //Build and dump header if file is empty or newly created
    if(file.pos() == 0){
        writer << buildHeaderString() << "\n";
        writer.flush();
     }

    //Actual writing
    if(file.isOpen()){
        foreach(QVariantList qvl, lines){
                writer << buildLogLine(qvl) << "\n";

        }
        writer.flush();
        file.close();
        writing = false;
        return lines.size();

    }
    else
        qCritical() << "RCSVLogger::CSVLogWriter(): File is not open, valid filename must be provided beforehand.";
        return 0;
}

int RCSVLogger::CSVManagementWriter(QString path, QList<QVariantList> lines){
    QString absPath=absolutePath(path);
    QDir::root().mkpath(QFileInfo(absPath).absolutePath());

    file.setFileName(absPath);
    if(!file.open(QIODevice::WriteOnly)){
        qCritical() << "RCSVLogger::CSVManagementWriter(): Could not open file: " << file.errorString();
        return 0;
    }
    else
        writer.setDevice(&file);

    //Actual writing
    if(file.isOpen()){
        file.seek(0);
        foreach(QVariantList qvl, lines){
            QString line = "";

            //Rest of data
            if(qvl.size() != header.size())
                qWarning() << "RCSVLogger::CSVManagementWriter(): Data and header don't have the same length, log file will not be correct.";
            if(qvl.size() > 0){
                QVariant datum = qvl.at(0);
                line += (datum.type() == QVariant::Double ? QString::number(datum.toReal(), 'f', precision) : datum.toString());
            }
            for(int i = 1; i < qvl.size(); i++){
                QVariant datum = qvl.at(i);
                line += ", " + (datum.type() == QVariant::Double ? QString::number(datum.toReal(), 'f', precision) : datum.toString());
            }
            writer << line << "\n";
        }
        writer.flush();
        file.close();
        return lines.size();
    }
    else
        qCritical() << "RCSVLogger::CSVManagementWriter(): File is not open, valid filename must be provided beforehand.";
        return 0;
}

QList<QStringList> RCSVLogger::CSVReader(QString path, int from){
    QString absPath=absolutePath(path);
    QDir::root().mkpath(QFileInfo(absPath).absolutePath());

    file.setFileName(absPath);
    if(!file.open(QIODevice::ReadOnly)){
        qCritical() << "RCSVLogger::CSVReader(): Could not open file: " << file.errorString();
        return {};
    }

    //Actual reading
    if(file.isOpen()){
        file.seek(from);
        QList<QStringList> lines;
           while (!file.atEnd()) {
               QString line = file.readLine();
               lines.append(line.split(','));
           }
        file.close();
        return lines;
    }
    else
        qCritical() << "RCSVLogger::CSVReader(): File is not open, valid filename must be provided beforehand.";
    return {};
}

QMap<QString,QList<int>> RCSVLogger::loadLogManager(){
    QMap<QString,QList<int>> lm;
    QList<QStringList> lines=CSVReader(logManagerPath,0);
    foreach (QStringList line, lines) {
        QList<int> temp;
        foreach (QString str, line.mid(1)) {
            temp.append(str.toInt());
        }
        lm[line.first()]=temp;
    }
    return lm;
}

void RCSVLogger::saveLogManager(){
    QList<QVariantList> lines;
    foreach (QString path, logManager.keys()) {
        QVariantList line;
        line.append(path);
        line.append(logManager[path].first());
        line.append(logManager[path].last());
        lines.append(line);
    }
    CSVManagementWriter(logManagerPath,lines);
}

QString RCSVLogger::absolutePath(QString path){
    QDir dir(path);
    if(dir.isAbsolute()) {
        qDebug() << "RCSVLogger::log(): Opening " + path + " to log.";
        return path;
    } else{
        qDebug() << "RCSVLogger::log(): Absolute path not given, opening " + path + " to log.";
        return
            #if defined(Q_OS_WIN)
                QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation)
            #else
                QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation)
            #endif
                + "/" + path;
    }
}

void RCSVLogger::close(){
    updateLocal();
    updateRemote();
}

inline QString RCSVLogger::buildLogLine(QVariantList const& data){
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
        qWarning() << "RCSVLogger::buildLogLine(): Data and header don't have the same length, log file will not be correct.";
    if(data.size() > 0){
        if(logTime)
            line += ", ";
        QVariant datum = data.at(0);
        line += (datum.type() == QVariant::Double ? QString::number(datum.toReal(), 'f', precision) : datum.toString());
    }
    for(int i = 1; i < data.size(); i++){
        QVariant datum = data.at(i);
        line += ", " + (datum.type() == QVariant::Double ? QString::number(datum.toReal(), 'f', precision) : datum.toString());
    }

    return line;
}

inline QString RCSVLogger::buildHeaderString(){
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

void RCSVLogger::setFilename(const QString& filename){
    this->filename = filename;
    emit filenameChanged();
}

void RCSVLogger::setLogTime(bool logTime){
    if(this->logTime != logTime){
        if(writing)
            qCritical() << "RCSVLogger::setLogTime(): logTime cannot be changed while writing.";
        else{
            this->logTime = logTime;
            emit logTimeChanged();
        }
    }
}

void RCSVLogger::setHeader(QList<QString> const& header){
    if(this->header != header){
        if(writing)
            qCritical() << "RCSVLogger::setHeader(): header cannot be changed while writing.";
        else{
            this->header = header;
            emit headerChanged();
        }
    }
}

void RCSVLogger::log(QVariantList const& data){
    if(!isEnabled())
        return;

    if(toConsole){
        qDebug() << buildLogLine(data);
        return;
    }
    if(updates.contains(filename)){
        updates[filename].append(data);
    } else {
        updates[filename]={data};
    }
}

}
