import QtQuick 2.5
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import Logger 1.0

ApplicationWindow {
    visible: true

    CSVLogger{
        id: logger
        enabled: enabledCheckbox.checked
        logTime: logTimeCheckBox.checked
        logMillis: logMillisCheckBox.checked
        filename: filenameInput.text
        header: headerInput.text === "" ? [] : headerInput.text.split("\n")
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
            id: toConsoleCheckBox
            text: "Log to console for debug"
            checked: false
        }

        TextField{
            id: filenameInput
            text: "example.csv"
            placeholderText: "Filename"
        }

        Text{
            text: "Header:"
        }

        TextArea{
            id: headerInput
            text: "header1\nheader2\nheader3"
        }

        Text{
            text: "Data:"
        }

        TextArea{
            id: dataInput
            text: "data1\ndata2\ndata3"
        }

        Button{
            text: "Log data"
            onClicked: logger.log(dataInput.text === "" ? [] : dataInput.text.split("\n"))
        }

        Button{
            text: "Close log file"
            onClicked: logger.close()
        }
    }
}
