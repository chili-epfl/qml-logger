TEMPLATE = lib
TARGET = loggerplugin

CONFIG += qt plugin c++11
CONFIG -= android_install

QT += qml quick bluetooth

QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

TARGET = $$qtLibraryTarget($$TARGET)
uri = Logger

HEADERS += \
    src/LoggerPlugin.h \
    src/SimpleLogger.h

SOURCES += \
    src/LoggerPlugin.cpp \
    src/SimpleLogger.cpp

OTHER_FILES += qmldir

#Install plugin library, qmldir
qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
