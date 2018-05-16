#include "watek_ruch.h"

watek_ruch::watek_ruch()
{
    this->stop = false;
    licznik = 0;
}

void watek_ruch::run(){
    jaki_ruch = "Wykryto ruch";
    emit opis_ruchu(jaki_ruch);
    licznik = 0;
    while ((licznik < 300) && (stop == false)){
        QThread::msleep(10);
        licznik++;
    }
    jaki_ruch = "Brak ruchu";
    emit opis_ruchu(jaki_ruch);
    stop = true;
}
