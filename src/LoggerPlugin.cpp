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
 * @file LoggerPlugin.cpp
 * @brief Implementation of the object that exposes the Logger plugin components as QML objects
 * @author Ayberk Özgür
 * @date 2016-06-15
 */

#include "LoggerPlugin.h"

#include "LoggerUtil.h"
#include "SimpleLogger.h"
#include "CSVLogger.h"
#include "RCSVLogger.h"

namespace QMLLogger{

void LoggerPlugin::registerTypes(const char* uri){
    qmlRegisterSingletonType<LoggerUtil>(uri, 1, 0, "LoggerUtil",
                                               [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                   Q_UNUSED(qmlEngine)
                                                   Q_UNUSED(jsEngine)
                                                   return new LoggerUtil();
                                               });
    qmlRegisterType<SimpleLogger>(uri, 1, 0, "SimpleLogger");
    qmlRegisterType<CSVLogger>(uri, 1, 0, "CSVLogger");
    qmlRegisterType<RCSVLogger>(uri, 1, 0, "RCSVLogger");

}

}
