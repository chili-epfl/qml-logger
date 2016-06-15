import QtQuick 2.5
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import Logger 1.0

Window {
    visible: true

    width: 640
    height: 480

    Logger{
        id: logger
        logTime: logTimeCheckBox.checked
        logMillis: logMillisCheckBox.checked
        logLocalHostName: logLocalHostNameCheckBox.checked
        filename: filenameInput.text
    }

    Column{

        spacing: 5

        CheckBox{
            id: logTimeCheckBox
            text: "Log time"
            checked: true
        }

        CheckBox{
            id: logMillisCheckBox
            text: "Log milliseconds as well in time (needs time enabled)"
            checked: false
        }

        CheckBox{
            id: logLocalHostNameCheckBox
            text: "Log local host name"
            checked: true
        }

        TextField{
            id: filenameInput
            text: "example.log"
            placeholderText: "Filename"
        }

        Button{
            text: "Log 'example_data'"
            onClicked: logger.log("example_data")
        }

        Button{
            text: "Log 'another_example_data'"
            onClicked: logger.log("another_example_data")
        }
    }
}
