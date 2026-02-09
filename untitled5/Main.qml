import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 400
    height: 360
    title: "Music Player"

    Column {
        anchors.centerIn: parent
        spacing: 12

        Button {
            text: "Pick Music File"
            onClicked: fileDialog.open()
        }

        Row {
            spacing: 10
            Button { text: "Play"; onClicked: player.play() }
            Button { text: "Pause"; onClicked: player.pause() }
            Button { text: "Stop"; onClicked: player.stop() }
        }

        // Progress Slider
        Slider {
            id: progressSlider
            width: 360
            from: 0
            to: player.duration
            value: player.position

            onMoved: {
                if (pressed) player.position = value
            }

            Connections {
                target: player
                function onPositionChanged(pos) {
                    if (!progressSlider.pressed) progressSlider.value = pos
                }
                function onDurationChanged(dur) {
                    progressSlider.to = dur
                }
            }
        }

        // Volume Slider
        Row {
            spacing: 10
            Text { text: "Volume"; width: 60; horizontalAlignment: Text.AlignRight }
            Slider {
                id: volumeSlider
                width: 300
                from: 0
                to: 1
                value: 0.5
                onValueChanged: player.volume = value
            }
        }

        Text {
            id: songName
            text: player.currentSong
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

        Text {
            id: timeDisplay
            text: Math.floor(player.position / 1000) + " / " +
                  Math.floor(player.duration / 1000) + " sec"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select Audio File"
        nameFilters: ["Audio files (*.mp3 *.wav *.ogg *.m4a)"]

        onAccepted: {
            if (!selectedFile) {
                console.log("No file selected")
                return
            }

            console.log("File selected:", selectedFile)
            player.currentSong = selectedFile

        }
    }


    Timer {
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            if (!progressSlider.pressed) {
                progressSlider.value = player.position
            }
            timeDisplay.text = Math.floor(player.position / 1000) + " / " +
                               Math.floor(player.duration / 1000) + " sec"
        }
    }
 }
