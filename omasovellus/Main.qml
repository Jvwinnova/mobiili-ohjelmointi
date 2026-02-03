import QtQuick 6.5
import QtQuick.Controls 6.5

ApplicationWindow {
    visible: true
    width: 400
    height: 400
    title: qsTr("Random Color Box")

    Rectangle {
        id: colorBox
        width: 200
        height: 200
        anchors.centerIn: parent
        color: "red"
        radius: 10

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Generate random RGB values (0-255)
                var r = Math.floor(Math.random() * 256)
                var g = Math.floor(Math.random() * 256)
                var b = Math.floor(Math.random() * 256)

                // Update the rectangle color
                colorBox.color = Qt.rgba(r/255, g/255, b/255, 1)
            }
        }

        // Smooth color transition
        Behavior on color {
            ColorAnimation { duration: 300 }
        }
    }
}
