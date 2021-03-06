#include <QNetworkRequest>
#include "network.h"

#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.106 Safari/535.2"

Network::Network(QObject *parent) :
    QNetworkAccessManager(parent)
{
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(readReply(QNetworkReply*)));
}

void Network::openUrl(QUrl url, QString data="")
{
    QNetworkRequest r(url);
    r.setRawHeader("User-Agent", USER_AGENT);
    r.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    if (data.isEmpty()) {
        get(r);
    } else {
        post(r, QByteArray().append(data));
    }
}

void Network::readReply(QNetworkReply *reply)
{
    QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirect.toString().isEmpty()) {
        emit replyReady(QString(reply->readAll().data()));
    } else {
        openUrl(redirect);
    }
}
