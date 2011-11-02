#include <QSettings>
#include <QPushButton>
#include "textin.h"
#include "ui_textin.h"

TextIn::TextIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextIn)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Send");
    m_bar = new QProgressBar(this);
    m_bar->hide();
    ui->statusBar->addPermanentWidget(m_bar);

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
    ui->label->setEnabled(enabled);
    ui->label_2->setEnabled(enabled);
    ui->recvEdit->setEnabled(enabled);
    ui->textEdit->setEnabled(enabled);
    ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled(enabled);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
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
        ui->recvEdit->setFocus();
        ui->statusBar->showMessage("Ready.");
    }
}

void TextIn::sendDone()
{
    m_bar->setValue(++m_curr);
    if (m_curr < m_receivers.size()) {
        m_talker->sendText(m_receivers[m_curr], ui->textEdit->toPlainText());
    } else {
        setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
        ui->buttonBox->button(QDialogButtonBox::Close)->show();
        if (m_curr == m_receivers.size())
            ui->statusBar->showMessage("Sent");
    }
}

void TextIn::on_buttonBox_accepted()
{
    m_curr = 0;
    m_receivers.clear();
    m_receivers << ui->recvEdit->text().trimmed().split(',', QString::SkipEmptyParts);

    if (m_receivers.size() == 0) {
        return;
    }

    setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Close)->hide();
    ui->buttonBox->button(QDialogButtonBox::Cancel)->show();

    for (int i=0; i<m_receivers.size(); i++) {
        m_receivers[i] = m_receivers.at(i).trimmed();
    }
    ui->statusBar->showMessage("Sending...");
    m_bar->show();

    m_bar->setMaximum(m_receivers.size());
    m_bar->setValue(m_curr);
    m_talker->sendText(m_receivers[m_curr], ui->textEdit->toPlainText());
}

void TextIn::on_buttonBox_rejected()
{
    if (ui->buttonBox->button(QDialogButtonBox::Close)->isVisible()) {
        close();
    } else if (m_curr != m_receivers.size()-1) {
        m_curr = m_receivers.size();
        m_bar->hide();
        ui->statusBar->showMessage("Cancelled.");
    }
}
