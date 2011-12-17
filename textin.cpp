#include <QSettings>
#include <QPushButton>
#include <QCompleter>
#include "textin.h"
#include "ui_textin.h"

#define MAX_LEN 160

TextIn::TextIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextIn)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Send");
    ui->lenLabel->setText("0/"+QString::number(MAX_LEN));

    m_settingsDlg = new SettingsDialog(this);
    firstrun();

    m_waitingToLogin = false;
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
    if (!s.contains("phone") || !s.contains("passwd")) {
        ui->actionSettings->trigger();
    }
}

void TextIn::setupStatusBar()
{
    m_bar = new QProgressBar(this);
    m_bar->hide();
    ui->statusBar->addPermanentWidget(m_bar);
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
    ui->lenLabel->setEnabled(enabled);
    ui->recvEdit->setEnabled(enabled);
    ui->textEdit->setEnabled(enabled);
    ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled(enabled);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

void TextIn::on_actionSettings_triggered()
{
    if (m_settingsDlg->exec()) {
        login();
    }
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
        setAutoComplete();
        if (m_waitingToLogin) {
            m_waitingToLogin = false;
            on_buttonBox_accepted();
        }
    }
}

void TextIn::sendDone()
{
    saveHistory(m_receivers[m_curr++]);
    setAutoComplete();
    if (m_receivers.size()>1)
        m_bar->setValue(m_curr);
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
    if (!m_talker->isReady()) {
        login();
        m_waitingToLogin = true;
        return;
    }
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

    if (m_receivers.size()>1) {
        m_bar->show();
        m_bar->setMaximum(m_receivers.size());
        m_bar->setValue(m_curr);
    }
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

void TextIn::on_textEdit_textChanged()
{
    QString text = ui->textEdit->toPlainText();
    if (text.length() > MAX_LEN) {
        text = text.left(MAX_LEN);
        ui->textEdit->setPlainText(text);
        ui->textEdit->moveCursor(QTextCursor::End);
    }
    ui->lenLabel->setText(QString::number(text.length())+"/"+QString::number(MAX_LEN));
}

void TextIn::saveHistory(QString num)
{
    QSettings s;
    QStringList hist = s.value("history", QStringList()).toStringList();
    hist.prepend(num);
    hist.removeDuplicates();
    s.setValue("history", hist);
}

void TextIn::setAutoComplete()
{
    QSettings s;
    QStringList hist = s.value("history", QStringList()).toStringList();
    ui->recvEdit->setCompleter(new QCompleter(hist, this));
}
