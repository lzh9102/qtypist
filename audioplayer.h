#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QScopedPointer>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    bool isPlaying() const;

public slots:
    void playAudioFile(QString filename);
    void stopPlayback();
    
signals:
    
public slots:
    
private:
    class Private;
    QScopedPointer<Private> p;
};

#endif // AUDIOPLAYER_H
