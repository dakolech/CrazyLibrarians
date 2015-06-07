#include "Bibliotekarz.hpp"
#include "Opcje.hpp"

#include <iostream>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tidRodzica(tidRodzica), liczbaDostepnychMPC(Opcje::pobierzInstancje().pobierzLiczbeMPC()) { }

void Bibliotekarz::zajmujSieSoba() const {
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}
