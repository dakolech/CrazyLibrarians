#include "Bibliotekarz.hpp"
#include "Opcje.hpp"

#include <iostream>
#include <iomanip>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tidRodzica(tidRodzica), liczbaDostepnychMPC(Opcje::pobierzInstancje().pobierzLiczbeMPC()),
      wartoscZegaraLamporta(0) { }

void Bibliotekarz::zajmujSieSoba() const {
    cout << "[Zegar: " << setw(4) << wartoscZegaraLamporta << "]"
         << "[tid: " << setw(4) << tidRodzica << "]"
         << "Zajmuję się sobą\n";
    
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}
