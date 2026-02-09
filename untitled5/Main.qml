import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 420
    height: 360
    title: "Music Player"

    MediaPlayer {
        id: player
        audioOutput: AudioOutput {
            id: audioOut
            volume: volumeSlider.value
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Button {
            text: "Pick Music File"
            onClicked: fileDialog.open()
        }

        RowLayout {
            spacing: 10
            Button { text: "Play"; onClicked: player.play() }
            Button { text: "Pause"; onClicked: player.pause() }
            Button { text: "Stop"; onClicked: player.stop() }
        }

        // 🎵 Progress slider (Qt 6–safe)
        Slider {
            Layout.fillWidth: true
            from: 0
            to: player.duration
            value: pressed ? value : player.position

            onMoved: {
                if (pressed)
                    player.position = value
            }
        }

        // 🔊 Volume slider
        RowLayout {
            spacing: 10
            Label { text: "Volume"; Layout.preferredWidth: 60 }
            Slider {
                id: volumeSlider
                Layout.fillWidth: true
                from: 0
                to: 1
                value: 0.5
            }
        }

        // ⏱ Time display
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: formatTime(player.position) + " / " +
                  formatTime(player.duration)
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select Audio File"
        nameFilters: ["Audio files (*.mp3 *.wav *.ogg *.m4a)"]

        onAccepted: {
            player.source = selectedFile
            player.play()
        }
    }

    function formatTime(ms) {
        if (ms <= 0) return "0:00"
        var s = Math.floor(ms / 1000)
        return Math.floor(s / 60) + ":" +
               (s % 60 < 10 ? "0" : "") + (s % 60)
    }
}

