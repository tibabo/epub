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
        title: "Please choose an Epub file"
        folder: shortcuts.home

        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrl)
            Epub.openFile(fileDialog.fileUrl);
        }
        onRejected: {
            console.log("Canceled")
        }
        nameFilters: [ "Epub (*.epub)" ]
    }

    Button {
        width: 100
        height: 50
        text: "Open EPUB"
        onClicked: fileDialog.open()
    }

    Text {
        x:100
        text: "TITLE: " + Epub.title +
              "\n\rCREATOR: " + Epub.creator +
              "\n\rLANGUAGE: " + Epub.language +
              "\n\rPUBLISHER: " + Epub.publisher +
              "\n\rPUBLISH DATE: " + Epub.publishDate
    }
}
