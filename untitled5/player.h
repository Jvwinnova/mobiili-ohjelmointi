#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(qreal position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qreal duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString currentSong READ currentSong WRITE setCurrentSong NOTIFY currentSongChanged)

public:
    explicit Player(QObject *parent = nullptr);

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    qreal volume() const;
    void setVolume(qreal vol);

    qreal position() const;
    void setPosition(qreal pos);

    qreal duration() const;

    QString currentSong() const;
    void setCurrentSong(const QString &song);

signals:
    void volumeChanged();
    void positionChanged();
    void durationChanged();
    void currentSongChanged();

private:
    QMediaPlayer m_player;
    QAudioOutput *m_audioOutput;
};

#endif // PLAYER_H

