#include "httpdownloader.h"
#include <QFile>

#define BUFFER_SIZE 1024

HttpDownloader::HttpDownloader(QObject *parent)
    : QObject(parent)
{
    connect(&m_webCtrl, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotDownloadFinished(QNetworkReply*)));
}

HttpDownloader::~HttpDownloader()
{
    cancelAllDownloads();
}

void HttpDownloader::startDownload(QString url, QString filename)
{
    DownloadParameters params(url, filename);
    QNetworkReply *reply = m_webCtrl.get(QNetworkRequest(url));
    m_downloads[reply] = params;
}

void HttpDownloader::cancelAllDownloads()
{
    QList<QNetworkReply*> replies = m_downloads.keys();
    foreach (QNetworkReply *reply, replies) {
        reply->abort();
        reply->deleteLater();
    }
    m_downloads.clear();
}

void HttpDownloader::slotDownloadFinished(QNetworkReply *reply)
{
    DownloadParameters params = m_downloads[reply];
    bool success = !reply->error();
    if (success)
        success = saveToFile(reply, params.filename);
    m_downloads.remove(reply);
    reply->deleteLater();
    emit downloadFinished(success, params.filename);
}

bool HttpDownloader::saveToFile(QNetworkReply *reply, QString filename)
{
    QFile localFile(filename);
    if (!localFile.open(QIODevice::WriteOnly))
        return false;
    char buffer[BUFFER_SIZE];
    int nread;
    while ((nread = reply->read(buffer, BUFFER_SIZE)) >= 0) {
        localFile.write(buffer, nread);
    }
    return true;
}