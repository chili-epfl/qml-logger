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
 * @file LoggerUtil.h
 * @brief Logger utilities header
 * @author Ayberk Özgür
 * @date 2016-11-14
 */

#ifndef LOGGERUTIL_H
#define LOGGERUTIL_H

#include<QQuickItem>
#include<QString>

namespace QMLLogger{

/**
 * @brief Logger utilities
 * @singleton
 */
class LoggerUtil : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /**
     * @brief Constant, unique device ID (without spaces) if possible, non-unique ID if not
     */
    Q_PROPERTY(QString uniqueDeviceID CONSTANT READ getUniqueDeviceID)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new LoggerUtil with the given QML parent
     *
     * @param parent The QML parent
     */
    LoggerUtil(QQuickItem* parent = 0);

    /**
     * @brief Destroys this LoggerUtil
     */
    ~LoggerUtil();

    /**
     * @brief Checks and requests Android permission if not given yet
     *
     * @param  permission Permission such as "android.permission.WRITE_EXTERNAL_STORAGE"
     * @return Whether successfully got the permission; true on non-Android platform
     */
    static bool androidSyncPermission(QString const& permission);

    /** @endcond */

public slots:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Gets the unique device ID
     *
     * @return Unique device ID (without spaces) if possible, non-unique ID if not
     */
    static QString getUniqueDeviceID();

    /** @endcond */

};

}

#endif /* LOGGERUTIL_H */
