TEMPLATE = app

QT += qml quick

SOURCES += src/main.cpp

RESOURCES += qml.qrc

android {
    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target
    export(INSTALLS)

    ANDROID_EXTRA_LIBS = $$[QT_INSTALL_QML]/Logger/libloggerplugin.so
}
