import QtQuick 6.4
import QtQuick.Controls 6.4

Button {
    id: root

    property color baseColor: "#E0E0E0"
    property color textColor: "#111"
    property color pressedColor: Qt.darker(baseColor, 1.15)

    font.pixelSize: 16
    font.bold: true

    // 🔹 Scale animation on press
    scale: root.down ? 1.08 : 1.0
    Behavior on scale {
        NumberAnimation {
            duration: 120
            easing.type: Easing.OutQuad
        }
    }

    background: Rectangle {
        radius: 8
        color: root.down ? root.pressedColor : root.baseColor
        border.color: Qt.darker(color, 1.25)
        border.width: 1
    }

    contentItem: Text {
        anchors.centerIn: parent
        text: root.text
        font: root.font
        color: root.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    implicitWidth: 48
    implicitHeight: 48
}
