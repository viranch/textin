#ifndef TALKER_H
#define TALKER_H

#include <QNetworkAccessManager>
#include "network.h"

class Talker : public QObject
{
    Q_OBJECT
public:
    explicit Talker(QObject *parent = 0);
    void login(QString username, QString password);
    void sendText(QString recvr, QString text);
    inline bool status() { return m_status; }

signals:
    void loginFinished(bool);
    void sendFinished();

public slots:
    void readReply(QString);

private:
    bool m_status;
    Network *m_manager;
};

#endif // TALKER_H
