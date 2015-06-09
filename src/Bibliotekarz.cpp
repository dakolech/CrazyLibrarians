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
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    liczbaCzytelnikowDoPonaglenia = rand()%100 + 1;
    Wiadomosc zapytanie(RZADANIE, tidRodzica, 0, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia);

    wyswietlStan("Proszę o dostęp do MPC");
    for(int i = 0; i < LICZBA_BIBLIOTEKARZY; i++) {
        MPI_Send(&zapytanie, sizeof(zapytanie), MPI_BYTE, i, TypWiadomosci::RZADANIE, MPI_COMM_WORLD);
    }

    bool koniec = false;
    int liczbaOdebranychWiadomosci = 0;
    while(!koniec) {
        int flag = 0;
        Wiadomosc odpowiedz;
        MPI_Status status;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        if (flag) {
            MPI_Recv(&odpowiedz, sizeof(odpowiedz), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            liczbaOdebranychWiadomosci++;

            ElementListy nowyElement;
            nowyElement.idProcesu = odpowiedz.tidNadawcy;
            nowyElement.liczbaCzytelnikowDoPonaglenia = odpowiedz.liczbaCzytelnikowDoPonaglenia;
        }

        if (liczbaOdebranychWiadomosci >= LICZBA_BIBLIOTEKARZY)
          koniec = true;
    }
}

void Bibliotekarz::uzywajMPC() {
    wyswietlStan("Używam MPC");
    
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}

void Bibliotekarz::zwolnijMPC() {
    wyswietlStan("Chcę zwolnić swojego MPC (koniec używania)");
    
    Wiadomosc zadanieDostepu(RZADANIE, tidRodzica, 0, wartoscZegaraLamporta, 0);
    // broadcast ŻĄDANIA
    for (int i = 0; i < Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy(); ++i) {
        if (i != tidRodzica) {
            MPI_Send(&zadanieDostepu, sizeof(Wiadomosc), MPI_BYTE, i, TypWiadomosci::RZADANIE, MPI_COMM_WORLD);
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
    Wiadomosc zwolnienieMPC(ZWOLNIENIE_MPC, tidRodzica, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    for (int i = 0; i < Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy(); ++i) {
        if (i != tidRodzica) {
            MPI_Send(&zwolnienieMPC, sizeof(Wiadomosc), MPI_BYTE, i, TypWiadomosci::ZWOLNIENIE_MPC, MPI_COMM_WORLD);
        }
    }
}

void Bibliotekarz::wyswietlStan(string info) const {
    cout << "[Zegar: " << setw(4) << wartoscZegaraLamporta << "]"
         << "[tid: " << setw(4) << tidRodzica << "]"
         << info << "\n";
}
