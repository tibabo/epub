import QtQuick 2.12

Item {
    property alias title : name.text
    property alias value : input.text
    property alias input : input

    Text {
        id: name
    }

    TextInput {
        id: input
        x: name.width + 10
    }
    height: name.height
}
