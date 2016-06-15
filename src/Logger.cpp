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

Logger::Logger(QQuickItem* parent) :
    QQuickItem(parent)
{
    logTime = true;
    logLocalHostName = true;
    logCaller = true;
}

Logger::~Logger(){
    writer.flush();
    file.close();
}

void Logger::setFilename(const QString& filename){
    writer.flush();
    file.close();

    this->filename = filename;

    QDir dir(filename);
    if(dir.isAbsolute())
        qDebug() << "Logger: Opening " + filename + " to log.";
    else{
        qDebug() << "Logger: Absolute path not given, opening " + QDir::homePath() + filename + " to log.";
        QDir::setCurrent(QDir::homePath());
    }

    file.setFileName(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
        qDebug() << "Logger: Could not open file.";
    else
        writer.setDevice(&file);
}

void Logger::log(const QString& data){
    if(file.isOpen())
        writer << data << "\n";
    else
        qDebug() << "Logger: Attempted log() but file is not open, valid filename must be provided beforehand.";
}
