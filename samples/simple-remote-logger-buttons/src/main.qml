import QtQuick 2.5
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import RLogger 1.0

ApplicationWindow {
    visible: true

    RCSVLogger{
        id: logger
        filename: filenameInput.text
        header: [headerInput.text]
        serverURL: urlInput.text
        logTime: logTimeCheckBox.checked
        logMillis: logMillisCheckBox.checked
        toConsole: toConsoleCheckBox.checked
        precision: precisionInput.text==="" ? 2:precisionInput.text

    }
    Column{

        spacing: 10

        CheckBox{
            id: logTimeCheckBox
            text: "Add a timestamp to each log (ignored if the log file already has a header)"
            checked: true
        }

        CheckBox{
            id: logMillisCheckBox
            text: "Log the timestamp in milliseconds rather than in seconds (if logged)"
            checked: true
        }

        CheckBox{
            id: toConsoleCheckBox
            text: "Log to the console instead"
            checked: false
        }

        Text{
            text: "File Name:"
        }

        TextField{
            id: filenameInput
            text: "example.csv"
        }

        Text{
            text: "URL Of The Server (modifications won't be applied after the first 'log' until a 'push'):"
        }

        TextField{
            id: urlInput
            text: "http://127.0.0.1:8000/"
        }

        Text{
            text: "Header Of The Logs (timestamp excluded; size=1; ignored if the log file already has a header):"
        }

        TextField{
            id: headerInput
            text: "example"
        }

        Text{
            text: "Precision Of Logged Floats (min=0, max=10):"
        }

        TextInput{
            id: precisionInput
            focus: true
            text: "2"
            validator: IntValidator{bottom: 0; top: 10;}
        }

        Button{
            text: "Log 'example_data'"
            onClicked: logger.log(["example_data"])
        }

        Button{
            text: "Log PI (affected by 'Precision')"
            onClicked: logger.log([Math.PI])
        }

        Button{
            text: "Push"
            onClicked: logger.push()
        }
    }
}
