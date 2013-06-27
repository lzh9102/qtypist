#include <QProcess>
#include "audioplayer.h"

class AudioPlayer::Private
{
public:
    QProcess m_proc;
};

AudioPlayer::AudioPlayer(QObject *parent) :
    QObject(parent), p(new Private)
{
}

AudioPlayer::~AudioPlayer()
{
    stopPlayback();
}

bool AudioPlayer::isPlaying() const
{
    return p->m_proc.state() != QProcess::NotRunning;
}

void AudioPlayer::playAudioFile(QString filename)
{
    if (isPlaying())
        stopPlayback();
    QStringList params;
    params.append(filename);
    p->m_proc.start("mplayer", params);
}

void AudioPlayer::stopPlayback()
{
    p->m_proc.kill();
}
