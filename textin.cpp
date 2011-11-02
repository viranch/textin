#include <QSettings>
#include "textin.h"
#include "ui_textin.h"

TextIn::TextIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextIn)
{
    ui->setupUi(this);

    m_settingsDlg = new SettingsDialog(this);
    firstrun();

    m_talker = new Talker(this);
    connect(m_talker, SIGNAL(loginFinished(bool)), this, SLOT(loginDone(bool)));
    connect(m_talker, SIGNAL(sendFinished()), this, SLOT(sendDone()));
    login();
}

TextIn::~TextIn()
{
    delete ui;
}

void TextIn::firstrun()
{
    QSettings s;
    if (!s.contains("phone")) {
        ui->actionSettings->trigger();
    }
}

void TextIn::login()
{
    setEnabled(false);
    QSettings s;
    m_talker->login(s.value("phone").toString(), s.value("passwd").toString());
    ui->statusBar->showMessage("Logging in...");
}

void TextIn::setEnabled(bool enabled)
{
    ui->textEdit->setEnabled(enabled);
    ui->sendButton->setEnabled(enabled);
}

void TextIn::on_actionSettings_triggered()
{
    if (m_settingsDlg->exec())
        login();
}

void TextIn::loginDone(bool success)
{
    if (!success) {
        ui->statusBar->showMessage("Login failed. (Phone number/password invalid)");
        ui->actionSettings->trigger();
    }
    else {
        setEnabled(true);
        ui->statusBar->showMessage("Ready.");
    }
}

void TextIn::sendDone()
{
    ui->statusBar->showMessage("Sent");
}

void TextIn::on_sendButton_clicked()
{
    ui->statusBar->showMessage("Sending...");
    m_talker->sendText(ui->recvEdit->text().trimmed(), ui->textEdit->toPlainText());
}
