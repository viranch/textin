#ifndef TEXTIN_H
#define TEXTIN_H

#include <QMainWindow>
#include <QProgressBar>
#include "settingsdialog.h"
#include "talker.h"

namespace Ui {
    class TextIn;
}

class TextIn : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextIn(QWidget *parent = 0);
    ~TextIn();

private slots:
    void on_actionSettings_triggered();
    void loginDone(bool);
    void sendDone();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_textEdit_textChanged();

private:
    Ui::TextIn *ui;
    SettingsDialog *m_settingsDlg;
    Talker *m_talker;
    QProgressBar *m_bar;
    QStringList m_receivers;
    int m_curr;

    void firstrun();
    void setupStatusBar();
    void login();
    void setEnabled(bool);
};

#endif // TEXTIN_H
