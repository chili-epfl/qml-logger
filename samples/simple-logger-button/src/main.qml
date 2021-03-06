import QtQuick 2.5
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import Logger 1.0

ApplicationWindow {
    visible: true

    SimpleLogger{
        id: logger
        enabled: enabledCheckbox.checked
        logTime: logTimeCheckBox.checked
        logMillis: logMillisCheckBox.checked
        logDeviceInfo: logDeviceInfoCheckBox.checked
        filename: filenameInput.text
        toConsole: toConsoleCheckBox.checked
    }

    Column{

        spacing: 5

        CheckBox{
            id: enabledCheckbox
            text: "Enabled"
            checked: true
        }

        CheckBox{
            id: logTimeCheckBox
            text: "Log time"
            checked: true
        }

        CheckBox{
            id: logMillisCheckBox
            text: "Log milliseconds as well in time (needs time enabled)"
            checked: true
        }

        CheckBox{
            id: logDeviceInfoCheckBox
            text: "Log local unique device ID"
            checked: true
        }

        CheckBox{
            id: toConsoleCheckBox
            text: "Log to console for debug"
            checked: false
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
