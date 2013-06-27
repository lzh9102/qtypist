#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QMap>

class HttpDownloader : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownloader(QObject *parent = 0);

    virtual ~HttpDownloader();

public slots:
    void startDownload(QString url, QString filename);
    void cancelAllDownloads();

signals:
    void downloadFinished(bool success, QString filename);

private slots:
    void slotDownloadFinished(QNetworkReply *reply);

private:
    class DownloadParameters
    {
    public:
        DownloadParameters() { }
        DownloadParameters(QString u, QString f)
            : url(u), filename(f) { }
        QString url;
        QString filename;
    };

    QNetworkAccessManager m_webCtrl;
    QMap<QNetworkReply*, DownloadParameters> m_downloads;
    bool saveToFile(QNetworkReply *reply, QString filename);
};

#endif // HTTPDOWNLOADER_H
