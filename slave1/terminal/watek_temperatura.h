#ifndef WATEK_TEMPERATURA_H
#define WATEK_TEMPERATURA_H

#include <QThread>
#include <QObject>

class watek_temperatura : public QThread
{
    Q_OBJECT
public:
    watek_temperatura(double, double);
    void run();

    double stara_temperatura;
    double wartosc_koncowa;
    bool stop;
    int licznik;

signals:
    void obecna_temp(double);
};

#endif // WATEK_TEMPERATURA_H
