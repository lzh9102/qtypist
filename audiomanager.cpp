/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: https://github.com/lzh9102/qtypist
 */

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include "audiomanager.h"
#include "audioplayer.h"
#include "httpdownloader.h"
#include "paths.h"

#define DEFAULT_TIMEOUT 1000

AudioManager::AudioManager(QObject *parent) :
    QObject(parent),
    m_player(new AudioPlayer(this)),
    m_downloader(new HttpDownloader(this))
{
    connect(m_downloader, SIGNAL(downloadFinished(bool,QString,QString)),
            this, SLOT(downloadComplete(bool,QString,QString)));
}

void AudioManager::addUrlPattern(QString pattern, QString lang)
{
    m_sources[lang].push_back(pattern);
}

void AudioManager::downloadPhrase(QString phrase, QString lang)
{
    if (!hasPhrase(phrase, lang))
        downloadPhrase(phrase, lang, 0);
}

bool AudioManager::hasPhrase(QString phrase, QString lang)
{
    return QFile::exists(getPath(phrase, lang));
}

void AudioManager::playPhrase(QString phrase, QString lang)
{
    if (hasPhrase(phrase, lang)) {
        m_player->playAudioFile(getPath(phrase, lang));
        m_requestedFile = "";
    } else {
        m_requestedFile = getPath(phrase, lang);
        downloadPhrase(phrase, lang);
    }
}

void AudioManager::downloadPhrase(QString phrase, QString lang, int url_index)
{
    QList<QString>& urls = m_sources[lang];
    if (url_index < urls.size()) {
        QString url = urls[url_index].arg(phrase);
        QString path = getPath(phrase, lang);
        qDebug() << QString("audio manager: start downloading %1 to %2")
                    .arg(url).arg(path);
        QDir().mkpath(QDir(path).absoluteFilePath("..")); // create parent directory
        m_downloader->startDownload(url, path
                   , QString("%1\n%2\n%3").arg(phrase).arg(lang).arg(url_index));
    }
}

void AudioManager::downloadComplete(bool success, QString file, QString id)
{
    QStringList components = id.split("\n");
    Q_ASSERT(components.size() == 3);
    QString phrase = components[0];
    QString lang = components[1];
    int url_index = components[2].toInt();

    if (success) {
        qDebug() << QString("download success: %1").arg(file);
        if (m_requestedFile == file)
            playPhrase(phrase, lang);
    } else {
        downloadPhrase(phrase, lang, url_index + 1); // try next url
    }
}

QString AudioManager::getPath(QString phrase, QString lang)
{
    QDir basedir(Paths::audioCachePath());
    return basedir.absoluteFilePath(lang + "/" + phrase);
}
