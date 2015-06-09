#include "Bibliotekarz.hpp"
#include "Opcje.hpp"

#include <openmpi/mpi.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tid(tidRodzica), liczbaDostepnychMPC(Opcje::pobierzInstancje().pobierzLiczbeMPC()),
      wartoscZegaraLamporta(0), liczbaCzytelnikowDoPonaglenia(0), liczbaPotwierdzen(0) { }

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
    
    wyswietlStan("Proszę o dostęp do MPC");
    // dodaj własne ŻĄDANIE do kolejki (chcę zabrać MPC)
    liczbaCzytelnikowDoPonaglenia = rand()%100 + 1;
    this->obsluzRzadanie(tid, liczbaCzytelnikowDoPonaglenia);
    // broadcast ŻĄDANIA
    Wiadomosc zapytanie(RZADANIE, tid, 0, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia);
    this->rozeslijWszystkim(zapytanie);
    // czekaj na potwierdzenia od wszystkich
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    /* TO DO */
    
    // sekcja krytyczna
    --this->liczbaDostepnychMPC;
    
    // broadcast informacji o zabraniu MPC wraz z nową wartością liczby dostępnych MPC
    Wiadomosc zabranieMPC(ZABRANIE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    for (int i = 0; i < LICZBA_BIBLIOTEKARZY; ++i) {
        if (i != tid) {
            MPI_Send(&zabranieMPC, sizeof(Wiadomosc), MPI_BYTE, i, TypWiadomosci::ZABRANIE_MPC, MPI_COMM_WORLD);
        }
    }
}

void Bibliotekarz::uzywajMPC() {
    wyswietlStan("Używam MPC");
    
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}

void Bibliotekarz::zwolnijMPC() {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    
    wyswietlStan("Chcę zwolnić swojego MPC (koniec używania)");
    // dodaj własne ŻĄDANIE do kolejki (chcę zwolnić MPC)
    this->obsluzRzadanie(tid, 0);
    // broadcast ŻĄDANIA
    Wiadomosc rzadanieDostepu(RZADANIE, tid, 0, wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(rzadanieDostepu);    
    // czekaj na potwierdzenia od wszystkich
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    while (!czyMogeWejscDoSekcji()) {
        obsluzWiadomosci();
    }
    
    // sekcja krytyczna
    ++this->liczbaDostepnychMPC;
    
    // broadcast informacji o zwolnieniu MPC wraz z nową wartością liczby dostępnych MPC
    Wiadomosc zwolnienieMPC(ZWOLNIENIE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(zwolnienieMPC);
}

void Bibliotekarz::wyswietlStan(string info) const {
    cout << "[Zegar: " << setw(4) << wartoscZegaraLamporta << "]"
         << "[tid: " << setw(4) << tid << "]"
         << info << "\n";
}

void Bibliotekarz::obsluzWiadomosci() {
    int flag = 0;
    Wiadomosc odpowiedz;
    MPI_Status status;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        MPI_Recv(&odpowiedz, sizeof(odpowiedz), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        obsluzWiadomosc(odpowiedz);
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
}

void Bibliotekarz::obsluzWiadomosc(Wiadomosc wiadomosc) {
    switch (wiadomosc.typ) {
        case RZADANIE:
            obsluzRzadanie(wiadomosc.tid, wiadomosc.liczbaCzytelnikowDoPonaglenia);
            break;
        case POTWIERDZENIE:
            //pamietać o wyzerowaniu
            ++liczbaPotwierdzen;
            break;
        case ZABRANIE_MPC:
            liczbaDostepnychMPC = wiadomosc.aktualnaLiczbaWolnychMPC;
            break;
        case ZWOLNIENIE_MPC:
            liczbaDostepnychMPC = wiadomosc.aktualnaLiczbaWolnychMPC;
            break;
        }
    wartoscZegaraLamporta = max(wartoscZegaraLamporta, wiadomosc.zegarLamporta) + 1;
}

void Bibliotekarz::obsluzRzadanie(int tid, int liczbaCzytelnikowDoPonaglenia) {
    if (lista.empty()) {
        lista.push_front(ElementListy{tid, liczbaCzytelnikowDoPonaglenia});
    } else {
        for (auto it = lista.begin(); it != lista.end(); ++it) {
            if (it->liczbaCzytelnikowDoPonaglenia >  liczbaCzytelnikowDoPonaglenia ||
               (it->liczbaCzytelnikowDoPonaglenia == liczbaCzytelnikowDoPonaglenia &&
                it->tid > tid)) {
                    lista.insert(it, ElementListy{tid, liczbaCzytelnikowDoPonaglenia});
                    return;
                }
        }
        lista.push_back(ElementListy{tid, liczbaCzytelnikowDoPonaglenia});
    }
}

void Bibliotekarz::rozeslijWszystkim(Wiadomosc wiadomosc) {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    for (int i = 0; i < LICZBA_BIBLIOTEKARZY; ++i) {
        if (i != tid) {
            MPI_Send(&wiadomosc, sizeof(Wiadomosc), MPI_BYTE, i, wiadomosc.typ, MPI_COMM_WORLD);
        }
    }
}

bool Bibliotekarz::czyMogeWejscDoSekcji() const {
    
}
