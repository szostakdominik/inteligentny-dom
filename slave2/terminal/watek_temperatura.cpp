#include "watek_temperatura.h"

watek_temperatura::watek_temperatura(double a, double b)
{
    this->stara_temperatura = a;
    this->wartosc_koncowa = b;
    this->stop = false;
    this->licznik = 0;
}

void watek_temperatura::run(){

    while ((stara_temperatura > wartosc_koncowa+0.01 || stara_temperatura < wartosc_koncowa-0.01) && !stop){
        if (stara_temperatura > wartosc_koncowa){
            stara_temperatura = stara_temperatura - 0.1;
        }else{
            stara_temperatura = stara_temperatura + 0.1;
        }
        licznik = 0;
        while ((licznik < 50) && (stop == false)){
            QThread::msleep(10);
            licznik++;
        }
        emit obecna_temp(stara_temperatura);
    }
    stop = true;
}

