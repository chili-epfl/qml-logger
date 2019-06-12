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
 * @brief Source for a QML CSV file logger with robust data transfer capability
 * @author Alexandre Reynaud
 * @date 2019-03-14
 */

#include "LoggerUtil.h"
#include "RCSVLogger.h"


namespace QMLLogger{

RCSVLogger::RCSVLogger(QQuickItem* parent) :
        QQuickItem(parent),
        absPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/Logs/"),
        logManager(loadLogManager())
{
    logTime = true;
    logMillis = true;
    toConsole = false;
    writing = false;
    precision = 2;
    serverURL="http://127.0.0.1:8000/";
    filename="data.csv";
    header=QStringList();
    manager = new QNetworkAccessManager(this);
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    LoggerUtil::androidSyncPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    LoggerUtil::androidSyncPermission("android.permission.READ_EXTERNAL_STORAGE");
}

RCSVLogger::~RCSVLogger(){
    push();
    delete manager;
}

void RCSVLogger::push(){
    if(updates.size()!=0){
        updateLocal();
        updates.clear();
    }
    updateRemote();
    writing=false;
}

void RCSVLogger::updateLocal(){
    qDebug() << "RCSVLogger::updateLocal(): Writing to local";
    foreach (QString path, updates.keys()) {
        if(logManager.contains(path)){
            logManager[path].first()+=CSVLogWriter(path,updates[path]);
        } else {
            logManager[path]={CSVLogWriter(path,updates[path]), 0};
        }
    }
    saveLogManager();
}

void RCSVLogger::updateRemote(){
    QNetworkRequest request;
    request.setUrl(QUrl(serverURL));
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    QUrlQuery data;

    //Building JSON
    foreach (QString name, logManager.keys()) {
        if(logManager[name].first()>logManager[name].last()){
            QStringList lines=CSVReader(name,logManager[name].last());
            QString pastedLines="";
            foreach (QString line, lines) {
                pastedLines+=line;
            }
            data.addQueryItem(name,pastedLines);
        }
    }
    if (!data.isEmpty()){
        qDebug() << "RCSVLogger::updateRemote(): Writing to remote";

        //Sending to remote
        QNetworkReply* reply=manager->post(request, data.toString(QUrl::FullyEncoded).toUtf8());
        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        //Checking if the transfer was successful
        if(!reply->error()){
            qDebug() << "RCSVLogger::updateRemote(): Write to remote successful";
            foreach (QString path, logManager.keys()) {
                logManager[path].last()=logManager[path].first();
            }
        } else {
            qDebug() << "RCSVLogger::updateRemote(): Write to remote unsuccessful";
        }
        delete reply;
        saveLogManager();
    }
}

int RCSVLogger::CSVLogWriter(QString name, QStringList lines){

    //Setup
    QString path=absolutePath(name);
    QDir::root().mkpath(QFileInfo(path).absolutePath());
    int bytes=0;
    QString line;
    file.setFileName(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        qCritical() << "RCSVLogger::CSVLogWriter(): Could not open file: " << file.errorString();
        return 0;
    }
    else
        writer.setDevice(&file);

    //Actual writing
    if(file.isOpen()){

        //Dump of the header if file is empty or newly created
        if(file.pos() == 0){
            writer << lines[0] << "\n";
            bytes+=lines[0].length()+1;
         }

        //Dump of the log lines
        foreach(QString line, lines.mid(1)){
                writer << line << "\n";
                bytes+=line.length()+1;

        }

        writer.flush();
        file.close();
        return bytes;

    }
    else
        qCritical() << "RCSVLogger::CSVLogWriter(): File is not open, valid filename must be provided beforehand.";
        return 0;
}

int RCSVLogger::CSVManagementWriter(QString name, QStringList lines){

    //Setup
    QString path=absolutePath(name);
    QDir::root().mkpath(QFileInfo(path).absolutePath());
    int bytes=0;
    file.setFileName(path);
    if(!file.open(QIODevice::WriteOnly)){
        qCritical() << "RCSVLogger::CSVManagementWriter(): Could not open file: " << file.errorString();
        return 0;
    }
    else
        writer.setDevice(&file);

    //Actual writing
    if(file.isOpen()){
        file.seek(0);
        foreach(QString line, lines){
            writer << line << "\n";
            bytes+=line.length()+1;
        }
        writer.flush();
        file.close();
        return bytes;
    }
    else
        qCritical() << "RCSVLogger::CSVManagementWriter(): File is not open, valid filename must be provided beforehand.";
        return 0;
}

QStringList RCSVLogger::CSVReader(QString name, int from){

    //Setup
    QString path=absolutePath(name);
    QDir::root().mkpath(QFileInfo(path).absolutePath());
    file.setFileName(path);
    if(!file.open(QIODevice::ReadOnly)){
        qCritical() << "RCSVLogger::CSVReader(): Could not open file: " << file.errorString();
        return {};
    }

    //Actual reading
    if(file.isOpen()){
        QStringList lines;
        file.seek(from);
        while (!file.atEnd()) {
            QString line = file.readLine();
            lines.append(line);
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
    QStringList lines=CSVReader(logManagerPath,0);
    foreach (QString line, lines) {
        QList<int> temp;
        foreach (QString str, line.split(',').mid(1)) {
            temp.append(str.toInt());
        }
        lm[line.split(',').first()]=temp;
    }
    return lm;
}

void RCSVLogger::saveLogManager(){
    QStringList lines;
    foreach (QString name, logManager.keys()) {
        QString line=name+", "+QString::number(logManager[name].first())+", "+QString::number(logManager[name].last());
        lines.append(line);
    }
    CSVManagementWriter(logManagerPath,lines);
}

QString RCSVLogger::absolutePath(QString name){
    return absPath+name;
}

inline QString RCSVLogger::buildLogLine(QVariantList const& data){
    QString line = "";

    //Timestamp
    if(logTime){
        if(logMillis)
            line += QString::number(QDateTime::currentMSecsSinceEpoch());
        else
            line += QString::number(QDateTime::currentSecsSinceEpoch());
    }

    //Rest of the data
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

    //Timestamp header field
    if(logTime)
        headerString += timestampHeader;

    //Rest of the header
    if(header.size() > 0){
        if(logTime)
            headerString += ", ";
        headerString += header.at(0);
    }
    for(int i = 1; i < header.size(); i++)
        headerString += ", " + header.at(i);

    return headerString;
}

void RCSVLogger::log(QVariantList const& data){
    QString line=buildLogLine(data);

    //Debug mode: printing to the console
    if(toConsole){
        qDebug() << line;
        return;
    }

    //Normal Mode: writing to the local CSV database and to the server
    writing = true;
    if(!updates.contains(filename))
        updates[filename]=QStringList(buildHeaderString());
    updates[filename].append(line);
}

void RCSVLogger::setAbsPath(const QString& path){
    if(this->absPath != path){
        if(writing)
            qCritical() << "RCSVLogger::setAbsPath(): absPath cannot be changed while writing.";
        else{
            this->absPath = path;
            emit absPathChanged();
        }
    }
}

void RCSVLogger::setServerURL(const QString& url){
    if(this->serverURL != url){
        if(writing)
            qCritical() << "RCSVLogger::setAbsPath(): serverURL cannot be changed while writing.";
        else{
            this->serverURL = url;
            emit serverURLChanged();
        }
    }
}

}
