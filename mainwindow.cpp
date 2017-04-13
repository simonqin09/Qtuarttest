#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uartconfig.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SendButton->hide();
    ui->ReceiveButton->hide();
    ui->uartoutput->hide();
    ui->uartdevice->setText("/dev/ttymxc");
}

MainWindow::~MainWindow()
{
    delete ui;
    uart_close(this->fd);
}


void MainWindow::on_ReceiveButton_clicked()
{
    str = "";
    msgBox.setText("Received data will show in edit line");
    msgBox.setInformativeText("wait for 10s for receiving");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        memset(this->data_read,0,sizeof(this->data_read));
            for(int i=0;;i++)
            {
                this->len = uart_read(this->fd, this->data_read, sizeof(this->data_read));
                if(this->len > 0)
                {
                    this->data_read[len]='\0';
                    for (int j=0;j<this->len;j++)
                    {
                        if('\n' != this->data_read[len-1])
                            str +=QString(this->data_read[j]);
                    }
                }
                else
                {
                    QMessageBox::warning(NULL, "warning", "can't receive data!");
                    break;
                }

                if('\n' == this->data_read[len-1])
                {
                    ui->uartoutput->setText(str);
                    break;
                }
            }
    }
}

void MainWindow::on_SendButton_clicked()
{
    /*capture uartoutput content QString and change to char[]*/
    this->len = sprintf(this->data_send, ui->uartoutput->text().toLocal8Bit().constData());
    /*judge if uartoutput lineedit is blank*/
    if(this->len == 0)
    {
        /*config default message for sending once lineedit is blank*/
        this->len = sprintf(this->data_send,"this is a test program");
        this->data_send[this->len]='\0';
        /*popup message box to choose*/
        msgBox.setText("iput line is blank, default message as below will be sent");
        msgBox.setInformativeText("this is a test program");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            /*send default message*/
            this->ch = uart_send(this->fd,this->data_send,this->len);
            if(this->ch == -1)
            {
                QMessageBox::warning(NULL, "warning", "send error!");
            }
            ui->label->setText("send default message successfully!");
        }
    }
    else
    {
        /*send input message*/
        if(this->len != -1)
        {
            this->ch = uart_send(this->fd,this->data_send,this->len);
            if(this->ch == -1)
            {
                QMessageBox::warning(NULL, "warning", "send error!");
            }
            ui->label->setText("send input message successfully!");
        }
        else
            QMessageBox::warning(NULL, "warning", "error set data for sending!");
    }
}
void MainWindow::on_SubmitButton_clicked()
{
    /*get uart port device from uartdevice qlineedit*/
    this->len = sprintf(this->uport, ui->uartdevice->text().toLocal8Bit().constData());
    if(this->len != -1)
    {
        this->fd = uart_open(this->uport);
        if(this->fd < 0)
        {
            ui->label->setText("uart port input error!");
        }
        else{
            /* enable RS485 mode */

            if(uart_485(fd) < 0)
            {
                ui->label->setText("failed enable RS485 mode");
            }
            else
            {
                ui->label->setText("RS485 mode enabled");

                /* config uart port, predefined baud rate list:
                B921600,B460800,B230400,B115200,B57600,B38400,B19200,B9600,B4800,B2400,B1200*/
                this->ch = uart_config(fd, B115200);
                if(this->ch == -1)
                {
                    ui->label->setText("uart setup error!");
                }
                else
                {
                    /*modify lable output and show send/receive button and uartoutput line edit*/
                    ui->label->setText("uart setup successfully! Press Send or Receive Button");
                    ui->SendButton->show();
                    ui->ReceiveButton->show();
                    ui->uartoutput->show();
                    ui->uartoutput->setText("input string for send here or default message will be sent");
                 }
             }
        }
    }
}
