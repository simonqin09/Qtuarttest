#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
class QLineEdit;
class QLable;
class QPushButton;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_ReceiveButton_clicked();

    void on_SendButton_clicked();

    void on_SubmitButton_clicked();

private:
    Ui::MainWindow *ui;
    Ui::QLineEdit *uartdevice;
    Ui::QLineEdit *uartoutput;
    Ui::QLable *lable;
    Ui::QPushButton *SendButton;
    Ui::QPushButton *ReceiveButton;
    QMessageBox msgBox;

    int fd,len,ch;
    char data_send[1024];
    char data_read[1024];
    char uport[20];
    QString str;

};

#endif // MAINWINDOW_H
