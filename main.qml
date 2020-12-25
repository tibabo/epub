import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import Qt.Reader 1.0




Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home

        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrl)
            MyApi.openFile(fileDialog.fileUrl);
        }
        onRejected: {
            console.log("Canceled")
        }
        nameFilters: [ "Epub (*.epub)" ]
    }



    property int someValue: MyApi.someProperty

    Component.onCompleted: {
        console.log(MyApi.doSomething())
    }

    Button {
            text: "Open"
            onClicked: fileDialog.open()
        }
}
