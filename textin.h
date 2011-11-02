#ifndef TEXTIN_H
#define TEXTIN_H

#include <QMainWindow>
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
    void on_sendButton_clicked();

private:
    Ui::TextIn *ui;
    SettingsDialog *m_settingsDlg;
    Talker *m_talker;

    void firstrun();
    void login();
    void setEnabled(bool);
};

#endif // TEXTIN_H
