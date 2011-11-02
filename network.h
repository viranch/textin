#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class Network : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = 0);
    void openUrl(QUrl url, QString data);

signals:
    void ready(QString);

public slots:
    void readReply(QNetworkReply*);

};

#endif // NETWORK_H
