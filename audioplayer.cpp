/* qtypist - a typing training program
 *
 * Copyright (C) 2012-2015 Che-Huai Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: https://github.com/lzh9102/qtypist
 */

#include <QProcess>
#include <QFile>
#include "audioplayer.h"
#include "paths.h"

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

bool AudioPlayer::isAvailable()
{
    // try to run mplayer to see if it exists
    QProcess mplayer_proc;
    mplayer_proc.start(Paths::mplayerExecutable());
    if (!mplayer_proc.waitForStarted())
        return false;
    mplayer_proc.waitForFinished();
    return true;
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
    p->m_proc.start(Paths::mplayerExecutable(), params);
}

void AudioPlayer::stopPlayback()
{
    p->m_proc.kill();
    p->m_proc.waitForFinished();
}
