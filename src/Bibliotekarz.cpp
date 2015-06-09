#include "Bibliotekarz.hpp"
#include "Opcje.hpp"

#include <openmpi/mpi.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tidRodzica(tidRodzica), liczbaDostepnychMPC(Opcje::pobierzInstancje().pobierzLiczbeMPC()),
      wartoscZegaraLamporta(0), liczbaCzytelnikowDoPonaglenia(0) { }

void Bibliotekarz::zajmujSieSoba() const {
    wyswietlStan("Zajmuję się sobą");
    
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}

void Bibliotekarz::odpowiedzInnymBibliotekarzom() {
    wyswietlStan("Odpowiadam na żądania innych bibliotekarzy");
}

void Bibliotekarz::poprosODostepDoMPC() {
    wyswietlStan("Próbuję dostać się do sekcji krytycznej");
}

void Bibliotekarz::uzywajMPC() {
    wyswietlStan("Używam MPC");
    
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}

void Bibliotekarz::zwolnijMPC() {
    wyswietlStan("Chcę zwolnić swojego MPC (koniec używania)");
    
    // wypełnij obiekt wiadomości danymi
    Wiadomosc wiadomosc;
    wiadomosc.typ = TypWiadomosci::RZADANIE;
    wiadomosc.tidNadawcy = this->tidRodzica;
    wiadomosc.zegarLamporta = ++this->wartoscZegaraLamporta;
    wiadomosc.liczbaCzytelnikowDoPonaglenia = 0;
    // broadcast ŻĄDANIA
    for (int i = 0; i < Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy(); ++i) {
        if (i != tidRodzica) {
            MPI_Send(&wiadomosc, sizeof(Wiadomosc), MPI_BYTE, i, TypWiadomosci::RZADANIE, MPI_COMM_WORLD);
        }
    }
    
    MPI_Status status;
    // czekaj na potwierdzenia od wszystkich
    /* TO DO */
    
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    /* TO DO */
    
    // sekcja krytyczna
    ++this->liczbaDostepnychMPC;
    
    // broadcast informacji o zwolnieniu MPC wraz z nową wartością liczby dostępnych MPC
    wiadomosc.typ = TypWiadomosci::ZWOLNIENIE_MPC;
    wiadomosc.tidNadawcy = this->tidRodzica;
    wiadomosc.zegarLamporta = ++this->wartoscZegaraLamporta;
    wiadomosc.aktualnaLiczbaWolnychMPC = this->liczbaDostepnychMPC;
    for (int i = 0; i < Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy(); ++i) {
        if (i != tidRodzica) {
            MPI_Send(&wiadomosc, sizeof(Wiadomosc), MPI_BYTE, i, TypWiadomosci::ZWOLNIENIE_MPC, MPI_COMM_WORLD);
        }
    }
}

void Bibliotekarz::wyswietlStan(string info) const {
    cout << "[Zegar: " << setw(4) << wartoscZegaraLamporta << "]"
         << "[tid: " << setw(4) << tidRodzica << "]"
         << info << "\n";
}
