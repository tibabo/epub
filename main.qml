import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import Qt.Reader 1.0


Window {
    id: mainWindow
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
            buttonSave.visible = true;
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

    InputItem {id:input1; x: button1.x + button1.width + 10; y: button1.y; title:"TITLE:"; value: Epub.title; input.onTextEdited:{Epub.title = value}}
    InputItem {id:input2; x: input1.x; y: input1.y + input1.height; title:"CREATOR:"; value: Epub.creator; input.onTextEdited:{Epub.creator = value}}
    InputItem {id:input3; x: input1.x; y: input2.y + input2.height; title:"PUBLISHER:"; value: Epub.publisher; input.onTextEdited:{Epub.publisher = value}}
    InputItem {id:input4; x: input1.x; y: input3.y + input3.height; title:"LANGUAGE:"; value: Epub.language; input.onTextEdited:{Epub.language = value}}
    InputItem {id:input5; x: input1.x; y: input4.y + input4.height; title:"DATE:"; value: Epub.publishDate; input.onTextEdited:{Epub.publishDate = value}}


    Text {
        x: input1.x
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

    Dialog {
        id: dialog
        title: "Overwrite epub ?"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true
        onAccepted: Epub.save()
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2
    }

    Button {
        id: buttonSave
        x : 10
        y : cover.y + cover.height + 10
        visible: false
        width: 100
        height: 50
        text: "Save EPUB"
        onClicked:{
            dialog.open()
        }
    }
}
