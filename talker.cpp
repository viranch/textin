#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "talker.h"

#define LOGIN_URL "http://site5.way2sms.com/Login1.action"
#define SEND_URL "http://site5.way2sms.com/quicksms.action"
#define SEND_DATA "HiddenAction=instantsms&Action=455dasv556"

Talker::Talker(QObject *parent) :
    QObject(parent)
{
    m_status = false;
    m_manager = new Network(this);
    m_manager->setCookieJar(new QNetworkCookieJar(this));
    connect(m_manager, SIGNAL(ready(QString)), this, SLOT(readReply(QString)));
}

void Talker::login(QString username, QString password)
{
    m_manager->openUrl(QString(LOGIN_URL), "username="+username+"&password="+password);
}

void Talker::sendText(QString recvr, QString text)
{
    m_manager->openUrl(QString(SEND_URL), QString(SEND_DATA)+"&MobNo="+recvr+"&textArea="+text);
}

void Talker::readReply(QString data)
{
    if (!status()) {
        m_status = data.contains("logout");
        emit loginFinished(status());
    }
    else {
        emit sendFinished();
    }
}
