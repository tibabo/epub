import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import Qt.Reader 1.0


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Epub Metadata reader")

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

    Button {
            width: 100
            height: 50
            text: "Open"
            onClicked: fileDialog.open()
        }
    Text
    {
        x:100
        text: "TITLE: " + MyApi.title +
              "\n\rCREATOR: " + MyApi.creator +
              "\n\rLANGUAGE: " + MyApi.language +
              "\n\rPUBLISHER: " + MyApi.publisher +
              "\n\rPUBLISH DATE: " + MyApi.publishDate

    }
}
