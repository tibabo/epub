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
        id: button1
        x : 10
        y : 10
        width: 100
        height: 50
        text: "Open EPUB"
        onClicked: fileDialog.open()
    }

    Text {
        id : meta
        x: button1.x + button1.width + 10
        y: button1.y
        text: "TITLE: " + Epub.title +
              "\n\rCREATOR: " + Epub.creator +
              "\n\rLANGUAGE: " + Epub.language +
              "\n\rPUBLISHER: " + Epub.publisher +
              "\n\rPUBLISH DATE: " + Epub.publishDate
    }
    Text {
        x: meta.x
        y: 100
        width: parent.width - x - 10
        wrapMode: Text.WordWrap
        id: description
        text: Epub.description
    }
    Image {
        id: cover
        source: Epub.cover
        x: button1.x
        y: button1.y + button1.height + 10
        width: button1.width
        fillMode: Image.PreserveAspectFit
        antialiasing: true
    }
}
