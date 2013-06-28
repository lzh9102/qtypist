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

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class AudioPlayer;
class HttpDownloader;

/**
 * @brief Download and play pronunciation audio files.
 */
class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = 0);

    bool hasPhrase(QString phrase, QString lang);

signals:
    
public slots:

    /**
     * @brief Add a url to search for pronunciation files.
     * @param pattern a url pattern containing, using %1 as the phrase placeholder
     * @param lang the language code of the url, empty for all languages
     */
    void addUrlPattern(QString pattern, QString lang);

    void downloadPhrase(QString phrase, QString lang);

    void playPhrase(QString phrase, QString lang);

private slots:

    void downloadPhrase(QString phrase, QString lang, int url_index);

    void downloadComplete(bool success, QString file, QString id);

private:
    QMap<QString, QList<QString> > m_sources; // lang -> url pattern list
    AudioPlayer *m_player;
    HttpDownloader *m_downloader;
    QString m_requestedFile;
    QString getPath(QString phrase, QString lang);
};

#endif // AUDIOMANAGER_H
