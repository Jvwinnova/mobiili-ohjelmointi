#include "player.h"
#include <QFileInfo>
#include <QDebug>

Player::Player(QObject *parent) : QObject(parent)
{
    m_audioOutput = new QAudioOutput(this);
    m_player.setAudioOutput(m_audioOutput);

    connect(&m_player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
    connect(&m_player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
}

void Player::play() {
    if (m_player.source().isEmpty()) {
        qDebug() << "No song loaded!";
        return;
    }
    qDebug() << "Playing:" << currentSong();
    m_player.play();
}

void Player::pause() { m_player.pause(); }
void Player::stop()  { m_player.stop(); }

qreal Player::volume() const { return m_audioOutput->volume(); }
void Player::setVolume(qreal vol) { m_audioOutput->setVolume(vol); emit volumeChanged(); }

qreal Player::position() const { return m_player.position(); }
void Player::setPosition(qreal pos) { m_player.setPosition(static_cast<qint64>(pos)); emit positionChanged(); }

qreal Player::duration() const { return m_player.duration(); }
QString Player::currentSong() const { return m_player.source().toString(); }

void Player::setCurrentSong(const QString &song)
{
    if (song.isEmpty()) {
        qDebug() << "No file selected!";
        return;
    }

    QUrl url(song);

    if (!url.isValid()) {
        qDebug() << "Invalid URL:" << song;
        return;
    }

    if (url.isLocalFile()) {
        QFileInfo fi(url.toLocalFile());
        if (!fi.exists()) {
            qDebug() << "File does not exist:" << fi.absoluteFilePath();
            return;
        }
        qDebug() << "Selected file path:" << fi.absoluteFilePath();
        qDebug() << "File name:" << fi.fileName();
        qDebug() << "File size:" << fi.size() << "bytes";
    }

    qDebug() << "MediaPlayer URL:" << url.toString();
    m_player.setSource(url);
    emit currentSongChanged();
}
