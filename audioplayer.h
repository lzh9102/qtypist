/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: http://code.google.com/p/qtypist
 */

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

    static bool isAvailable();

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
