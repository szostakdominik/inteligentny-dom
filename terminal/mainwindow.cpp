/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>

#define samples 1000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    //do wyswietlania obrazkow
    //zarowki
    obr_zarowki1 = new QLabel;
    obr_zarowki1 = ui->obr_zarowki1;
    zarowka1 = new QPixmap;
    zarowka1->load(":/zarowka_1.png");
    zarowka2 = new QPixmap;
    zarowka2->load(":/zarowka_2.png");
    //okna
    obr_okna1 = new QLabel;
    obr_okna1 = ui->obr_okna1;;
    okno1 = new QPixmap;
    okno1->load(":/okno_1.png");
    okno2 = new QPixmap;
    okno2->load(":/okno_2.png");
    //drzwi
    obr_drzwi1 = new QLabel;
    obr_drzwi1 = ui->obr_drzwi1;
    drzwi1 = new QPixmap;
    drzwi1->load(":/drzwi_1.png");
    drzwi2 = new QPixmap;
    drzwi2->load(":/drzwi_2.png");

    //poczatkowe ustawienia obrazkow:
    obr_zarowki1->setPixmap(*zarowka1);
    obr_okna1->setPixmap(*okno1);
    obr_drzwi1->setPixmap(*drzwi1);

    //tworzenie zmiennych
    m_temp_wysw1 = new QLCDNumber;
    m_temp_wysw2 = new QLCDNumber;
    temp1 = new QDoubleSpinBox;
    temp2 = new QDoubleSpinBox;
    swiatlo1 = new QCheckBox;
    swiatlo2 = new QCheckBox;
    wentylacja1 = new QCheckBox;
    wentylacja2 = new QCheckBox;
    czujnik_ruchu1 = new QTextBrowser;
    czujnik_ruchu2 = new QTextBrowser;
    kontrola_dymu1 = new QTextBrowser;
    kontrola_dymu2 = new QTextBrowser;
    czujnik_drzwi1 = new QTextBrowser;
    czujnik_drzwi2 = new QTextBrowser;
    czujnik_okna1 = new QTextBrowser;
    czujnik_okna2 = new QTextBrowser;
    limit_dymu1 = new QDoubleSpinBox;
    limit_dymu2 = new QDoubleSpinBox;

    //przypisanie zmiennych
    m_temp_wysw1 = ui->master_temp_wysw1;
    m_temp_wysw2 = ui->master_temp_wysw2;
    temp1 = ui->ustaw_temp1;
    temp2 = ui->ustaw_temp2;
    swiatlo1 = ui->wl_swiatlo1;
    swiatlo2 = ui->wl_swiatlo2;
    wentylacja1 = ui->wl_swiatlo1;
    wentylacja2 = ui->wl_swiatlo2;
    czujnik_ruchu1 = ui->czujnik_ruchu1;
    czujnik_ruchu2 = ui->czujnik_ruchu2;
    kontrola_dymu1 = ui->kontrola_dymu1;
    kontrola_dymu2 = ui->kontrola_dymu2;
    czujnik_drzwi1 = ui->czujnik_drzwi1;
    czujnik_drzwi2 = ui->czujnik_drzwi2;
    czujnik_okna1 = ui->czujnik_okna1;
    czujnik_okna2 = ui->czujnik_okna2;
    limit_dymu1 = ui->limit_dymu1;
    limit_dymu2 = ui->limit_dymu2;

    //wstepne ustawienie
    czujnik_ruchu1->setText("Brak ruchu");
    czujnik_ruchu2->setText("Brak ruchu");
    kontrola_dymu1->setText("Poziom OK!");
    kontrola_dymu2->setText("Poziom OK!");
    czujnik_drzwi1->setText("Drzwi zamknięte");
    czujnik_okna1->setText("Okno zamknięte");
    czujnik_drzwi2->setText("Drzwi zamknięte");
    czujnik_okna2->setText("Okno zamknięte");

    //laczenie obiektow
    connect(swiatlo1, SIGNAL(toggled(bool)), this, SLOT(ustaw_swiatlo1()));
    connect(swiatlo2, SIGNAL(toggled(bool)), this, SLOT(ustaw_swiatlo2()));
    connect(wentylacja1, SIGNAL(toggled(bool)), this, SLOT(ustaw_wentylacje1()));
    connect(wentylacja2, SIGNAL(toggled(bool)), this, SLOT(ustaw_wentylacje2()));
    connect(limit_dymu1, SIGNAL(valueChanged(double)), SLOT(poziom_dymu1()));
    connect(limit_dymu2, SIGNAL(valueChanged(double)), SLOT(poziom_dymu2()));

    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    serial_rx = new serial_rx_t;



    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    serial_rx->status = idle;


}




MainWindow::~MainWindow() {

    delete settings;
    delete serial;
    delete serial_rx;
    delete ui;

}



void MainWindow::openSerialPort() {

    SettingsDialog::Settings p = settings->settings();

    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite)) {

        ui->actionConfigure->setEnabled(false);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionQuit->setEnabled(false);

    } else {

        QMessageBox::critical(this, tr("Error."), serial->errorString());

    }

}



void MainWindow::closeSerialPort() {

    if (serial->isOpen())
        serial->close();

    ui->actionConfigure->setEnabled(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);

}



void MainWindow::writeData(const QByteArray &data) {

        serial->write(data);

}



void MainWindow::readData() {

    QByteArray data = serial->readAll();
    QByteArray komenda1 = "-o11\r";
    QByteArray komenda2 = "-o10\r";
    QByteArray komenda3 = "-d11\r";
    QByteArray komenda4 = "-d10\r";
    QByteArray komenda5 = "-r1\r";
    QByteArray komenda6 = "-o21\r";
    QByteArray komenda7 = "-o20\r";
    QByteArray komenda8 = "-d21\r";
    QByteArray komenda9 = "-d20\r";
    QByteArray komenda10 = "-r2\r";


    for( int i = 0; i < data.size(); i++ ) {



        if( ((uint8_t)data.at(i) == 0x80) ) {

            serial_rx->status = receiving_data;

            serial_rx->frame.clear();
            serial_rx->frame.append(data.at(i));

        }

        else if( serial_rx->status == receiving_data ) {

            serial_rx->frame.append(data.at(i));



            if( serial_rx->frame.size() == serial_rx->frame.at(1) ) {

                union {
                    float word;
                    uint8_t bytes[4];
                } temp;

                temp.bytes[0] = serial_rx->frame.at(3) | ( (serial_rx->frame.at(7) & (0x80 >> 1U)) << 1U );
                temp.bytes[1] = serial_rx->frame.at(4) | ( (serial_rx->frame.at(7) & (0x80 >> 2U)) << 2U );
                temp.bytes[2] = serial_rx->frame.at(5) | ( (serial_rx->frame.at(7) & (0x80 >> 3U)) << 3U );
                temp.bytes[3] = serial_rx->frame.at(6) | ( (serial_rx->frame.at(7) & (0x80 >> 4U)) << 4U );

                serial_rx->status = idle;

            }

        }



        else if( (data.at(i) == '-') && (!serial_rx->status) ) {

            serial_rx->status = receiving_string;
            serial_rx->frame.clear();

        }

        else if( serial_rx->status == receiving_string ) {

            if( data.at(i) != '\r' )
                serial_rx->frame.append(data.at(i));

            else {

                serial_rx->status = idle;
                if(data==komenda1){
                    ui->czujnik_okna1->setText("Okno otwarte");
                }else if(data==komenda2){
                    ui->czujnik_okna1->setText("Okno zamknięte");
                }else if(data==komenda3){
                    ui->czujnik_drzwi1->setText("Drzwi otwarte");
                }else if(data==komenda4){
                    ui->czujnik_drzwi1->setText("Okno zamknięte");
                }else if(data==komenda5){
                    ui->czujnik_ruchu1->setText("Ktoś jest w pokoju");
                    QTimer::singleShot(10000, this, SLOT(brak_ruchu()));
                }else if(data==komenda6){
                    ui->czujnik_okna2->setText("Okno otwarte");
                }else if(data==komenda7){
                    ui->czujnik_okna2->setText("Okno zamknięte");
                }else if(data==komenda8){
                    ui->czujnik_drzwi2->setText("Drzwi otwarte");
                }else if(data==komenda9){
                    ui->czujnik_drzwi2->setText("Drzwi zamknięte");
                }else if(data==komenda10){
                    ui->czujnik_ruchu2->setText("Ktoś jest w pokoju");
                    QTimer::singleShot(10000, this, SLOT(brak_ruchu()));
                }



            }

        }
        else{
            double val;
            QString wartosc = data;
            val = wartosc.toDouble();
            if(val<100){
                if(val<ui->ustaw_temp1->minimum()){
                    stezenie1 = val;
                    if(val>ui->limit_dymu1->value()){
                        ui->kontrola_dymu1->setText("Za wysokie stęzenie dymu!!");
                    }else{
                        ui->kontrola_dymu1->setText("Poziom OK!!!");
                    }
                }else{

                }
            }else if(val>100){
                double val1=val-100.00;
                if(val1<ui->ustaw_temp2->minimum())
                {
                    stezenie2 = val1;
                    if(val1>ui->limit_dymu2->value()){
                        ui->kontrola_dymu2->setText("Za wysokie stęzenie dymu!!");
                    }else{
                        ui->kontrola_dymu2->setText("Poziom OK!!!");
                    }
                }else{

                }
            }
        }



    }

}



void MainWindow::handleError(QSerialPort::SerialPortError error) {

    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error."), serial->errorString());
        closeSerialPort();
    }

}

void MainWindow::brak_ruchu(){
    ui->czujnik_ruchu1->setText("brak ruchu");
}

void MainWindow::on_wl_swiatlo1_toggled(bool checked)
{
    if(checked){
        writeData("-s11\r");
    }else{
        writeData("-s10\r");
    }
}

void MainWindow::on_wl_wentylacja1_toggled(bool checked)
{
    if(checked){
        writeData("-w11\r");
    }else{
        writeData("-w10\r");
    }
}

void MainWindow::on_wl_swiatlo2_toggled(bool checked)
{
    if(checked){
        writeData("-s21\r");
    }else{
        writeData("-s20\r");
    }
}

void MainWindow::on_wl_wentylacja2_toggled(bool checked)
{
    if(checked){
        writeData("-w21\r");
    }else{
        writeData("-w20\r");
    }
}

void MainWindow::on_limit_dymu1_editingFinished()
{
    if(stezenie1>ui->limit_dymu1->value()){
        ui->kontrola_dymu1->setText("Za wysokie stężenie!!");
    }else{
        ui->kontrola_dymu1->setText("Poziom OK!!");
    }
}

void MainWindow::on_limit_dymu2_editingFinished()
{
    if(stezenie2>ui->limit_dymu2->value()){
        ui->kontrola_dymu2->setText("Za wysokie stężenie!!");
    }else{
        ui->kontrola_dymu2->setText("Poziom OK!!");
    }
}

void MainWindow::on_ustaw_temp1_editingFinished()
{
    double value = ui->ustaw_temp1->value();
    QString valuestring = QString::number(value);
    QByteArray array = valuestring.toUtf8();
    writeData(array);
    if (watek_temp1){
        if (watek_temp1->stop == false){
            watek_temp1->stop = true;
            watek_temp1->wait();
            delete watek_temp1;
            watek_temp1 = NULL;
        }
    }
    watek_temp1 = new watek_temperatura(ui->master_temp_wysw1->value(),value);
    connect(watek_temp1, SIGNAL(obecna_temp(double)), ui->master_temp_wysw1, SLOT(display(double)));
    watek_temp1->start();
}

void MainWindow::on_ustaw_temp2_editingFinished()
{
    double value = ui->ustaw_temp2->value()+100.00;
    QString valuestring = QString::number(value);
    QByteArray array = valuestring.toUtf8();
    writeData(array);
    if (watek_temp1){
        if (watek_temp1->stop == false){
            watek_temp1->stop = true;
            watek_temp1->wait();
            delete watek_temp1;
            watek_temp1 = NULL;
        }
    }
    watek_temp1 = new watek_temperatura(ui->master_temp_wysw2->value(),value-100.00);
    connect(watek_temp1, SIGNAL(obecna_temp(double)), ui->master_temp_wysw2, SLOT(display(double)));
    watek_temp1->start();
}
