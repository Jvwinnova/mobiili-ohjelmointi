import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import CalculatorEnums 1.0

ApplicationWindow {
    id: window
    visible: true
    title: "Laskin"

    minimumWidth: 240
    minimumHeight: 400

    // ---------------- Responsive base properties ----------------
    property real baseWidth: width
    property real baseHeight: height
    property real baseFontSize: Math.min(baseWidth, baseHeight) * 0.05
    property real buttonSpacing: baseWidth * 0.01
    property real marginSize: baseWidth * 0.02
    property real displayHeight: baseHeight * 0.12
    property real drawerWidth: baseWidth * 0.65

    // ---------------- Background Image ----------------
    background: Image {
        id: mainBackground
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "" // initially empty
        visible: source !== "" && source !== undefined
        z: -1

        onStatusChanged: {
            console.log("BG status:", status, "source:", source)
            if (status === Image.Error) {
                console.log("BG load error:", errorString)
            }
        }
    }

    // ---------------- Drawer ----------------
    Drawer {
        id: drawer
        width: drawerWidth
        height: parent.height
        edge: Qt.LeftEdge

        Rectangle {
            anchors.fill: parent
            color: darkModeSwitch.checked ? Qt.rgba(0,0,0,0.8) : Qt.rgba(1,1,1,0.8)
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: marginSize
            spacing: buttonSpacing

            Label {
                text: "Copyright © 2026 Jere Vainio"
                font.pixelSize: baseFontSize * 0.6
                color: darkModeSwitch.checked ? "white" : "black"
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: buttonSpacing
                Label {
                    text: "Dark Mode"
                    font.pixelSize: baseFontSize * 0.6
                    color: darkModeSwitch.checked ? "white" : "black"
                }
                Switch {
                    id: darkModeSwitch
                    checked: false
                }
            }

            Button {
                text: "Set Background Image"
                font.pixelSize: baseFontSize * 0.6
                Layout.fillWidth: true
                onClicked: fileDialog.open()
            }

            FileDialog {
                id: fileDialog
                title: "Select Background Image"
                nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
                onAccepted: {
                    console.log("FileDialog accepted, file:", fileDialog.file)
                    if (fileDialog.file && fileDialog.file !== "") {
                        mainBackground.source = fileDialog.file
                        mainBackground.visible = true
                        console.log("Set background source to:", fileDialog.file)
                    } else {
                        console.log("No valid file selected.")
                    }
                }
            }

        }
    }

    // ---------------- Header ----------------
    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: buttonSpacing

            ToolButton {
                text: "☰"
                font.pixelSize: baseFontSize
                onClicked: drawer.open()
            }

            Label {
                text: "Scientific Calculator"
                font.pixelSize: baseFontSize
                Layout.alignment: Qt.AlignCenter
            }
        }
    }

    // ---------------- Main Content ----------------
    Item {
        id: mainContent
        anchors.fill: parent
        anchors.margins: marginSize

        ColumnLayout {
            anchors.fill: parent
            spacing: buttonSpacing

            // ---------------- Display ----------------
            Rectangle {
                id: displayRect
                Layout.fillWidth: true
                Layout.preferredHeight: displayHeight
                radius: 6
                color: darkModeSwitch.checked ? Qt.rgba(0,0,0,0.6) : Qt.rgba(1,1,1,0.6)

                Text {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: parent.width * 0.03
                    text: calculator.display
                    font.pixelSize: baseFontSize * 1.1
                    color: darkModeSwitch.checked ? "white" : "black"
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideLeft
                }
            }

            // ---------------- Memory Buttons ----------------
            GridLayout {
                Layout.fillWidth: true
                columns: 4
                rowSpacing: buttonSpacing
                columnSpacing: buttonSpacing

                CalcButton { text: "MC"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.clearMemory() }
                CalcButton { text: "MR"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.readMemory() }
                CalcButton { text: "MS"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.setMemory() }
                CalcButton { text: "M+"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.addToMemory() }
            }

            // ---------------- Scientific Buttons ----------------
            GridLayout {
                Layout.fillWidth: true
                columns: 4
                rowSpacing: buttonSpacing
                columnSpacing: buttonSpacing

                CalcButton { text: "√";   font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.unaryOperatorClicked(Calculator.SquareRoot) }
                CalcButton { text: "x²";  font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.unaryOperatorClicked(Calculator.Square) }
                CalcButton { text: "1/x"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.unaryOperatorClicked(Calculator.Inverse) }
                CalcButton { text: "n!";  font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.unaryOperatorClicked(Calculator.Factorial) }
                CalcButton { text: "xʸ";  font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.multiplicativeOperatorClicked(Calculator.Power) }
                CalcButton { text: "±";   font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.changeSignClicked() }
                CalcButton { text: "⌫";   font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.backspaceClicked() }
            }

            // ---------------- Main Keypad ----------------
            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 4
                rowSpacing: buttonSpacing
                columnSpacing: buttonSpacing

                // Row 1
                CalcButton { text: "7"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("7") }
                CalcButton { text: "8"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("8") }
                CalcButton { text: "9"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("9") }
                CalcButton { text: "÷"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.multiplicativeOperatorClicked(Calculator.Division) }

                // Row 2
                CalcButton { text: "4"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("4") }
                CalcButton { text: "5"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("5") }
                CalcButton { text: "6"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("6") }
                CalcButton { text: "×"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.multiplicativeOperatorClicked(Calculator.Multiplication) }

                // Row 3
                CalcButton { text: "1"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("1") }
                CalcButton { text: "2"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("2") }
                CalcButton { text: "3"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("3") }
                CalcButton { text: "−"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.additiveOperatorClicked(Calculator.Subtraction) }

                // Row 4
                CalcButton { text: "C"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.clear() }
                CalcButton { text: "0"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.digitClicked("0") }
                CalcButton { text: "."; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.pointClicked() }
                CalcButton { text: "+"; font.pixelSize: baseFontSize; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: calculator.additiveOperatorClicked(Calculator.Addition) }

                // Row 5 - Spanning buttons
                CalcButton {
                    text: "CA"
                    Layout.columnSpan: 2
                    font.pixelSize: baseFontSize
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onClicked: calculator.clearAll()
                }

                CalcButton {
                    text: "="
                    Layout.columnSpan: 2
                    font.pixelSize: baseFontSize * 1.1
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onClicked: calculator.equalClicked()
                }
            }
        }
    }
}
