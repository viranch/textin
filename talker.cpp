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
    m_ready = false;
    m_manager = new Network(this);
    m_manager->setCookieJar(new QNetworkCookieJar(this));
}

void Talker::login(QString username, QString password)
{
    disconnect(m_manager, SIGNAL(replyReady(QString)), this, 0);
    connect(m_manager, SIGNAL(replyReady(QString)), this, SLOT(readLogin(QString)));
    m_manager->openUrl(QString(LOGIN_URL), "username="+username+"&password="+password);
}

void Talker::sendText(QString recvr, QString text)
{
    m_manager->openUrl(QString(SEND_URL), QString(SEND_DATA)+"&MobNo="+recvr+"&textArea="+text);
}

void Talker::readLogin(QString data)
{
    m_ready = data.contains("logout");
    emit loginFinished(m_ready);
    if (m_ready) {
        disconnect(m_manager, SIGNAL(replyReady(QString)), this, 0);
        connect(m_manager, SIGNAL(replyReady(QString)), this, SLOT(readSend(QString)));
    }
}

void Talker::readSend(QString data)
{
    emit sendFinished();
}
