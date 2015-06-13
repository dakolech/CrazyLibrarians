#include "Bibliotekarz.hpp"
#include "Opcje.hpp"

#include <openmpi/mpi.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tid(tidRodzica), wartoscZegaraLamporta(0), liczbaPotwierdzen(0) { }

void Bibliotekarz::zajmujSieSoba() {
    wyswietlStan("Zajmuję się sobą");
    
    int microseconds = rand()%490000 + 10000;
    usleep(microseconds);
    ++wartoscZegaraLamporta;
}

void Bibliotekarz::odpowiedzInnymBibliotekarzom() {
    wyswietlStan("Odpowiadam na żądania innych bibliotekarzy");
    
    this->obsluzWiadomosci();
    ++wartoscZegaraLamporta;
}

/*void Bibliotekarz::poprosODostepDoMPC() {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    liczbaCzytelnikowDoPonaglenia = rand()%100 + 1;
    
    string info = "Proszę o dostęp do MPC (do ponaglenia: " + to_string(liczbaCzytelnikowDoPonaglenia) + ")";
    wyswietlStan(info);
    // dodaj własne ŻĄDANIE do kolejki (chcę zabrać MPC)
    lista.push_back(ElementListy{tid, liczbaCzytelnikowDoPonaglenia});
    // broadcast ŻĄDANIA
    Wiadomosc zapytanie(RZADANIE, tid, 0, ++wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia);
    this->rozeslijWszystkim(zapytanie);
    // czekaj na potwierdzenia od wszystkich
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    liczbaPotwierdzen = 0;
    
    wyswietlStan("Zebrałem wszystkie potwierdzenia (do zabrania)");
    
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    do {
        obsluzWiadomosci();
    } while (!czyMogeWejscDoSekcji() || liczbaDostepnychMPC == 0);
    
    // sekcja krytyczna
    wyswietlStan("Jestem w sekcji krytycznej (zabrałem MPC)");
    
    Wiadomosc alert(ZARAZ_ZABIORE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(alert);
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    liczbaPotwierdzen = 0;
    
    wyswietlStan("Zmieniam wartość zmiennej: liczbaDostepnychMPC (--)");
    --this->liczbaDostepnychMPC;
    
    // broadcast informacji o zabraniu MPC wraz z nową wartością liczby dostępnych MPC
    Wiadomosc zabranieMPC(ZABRANIE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(zabranieMPC);
}*/

void Bibliotekarz::poprosODostepDoMPC() {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    int liczbaCzytelnikowDoPonaglenia = rand()%100 + 1;
    
    wyswietlStan("Proszę o dostęp do MPC (do ponaglenia: " + to_string(liczbaCzytelnikowDoPonaglenia) + ")");
    
    // dodaj własne ŻĄDANIE do kolejki (chcę zabrać MPC)
    Wiadomosc wlasneRzadanie = { RZADANIE, tid, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia };
    kolejkujRzadanie(wlasneRzadanie);
    
    // broadcast ŻĄDANIA
    Wiadomosc rzadanie = { RZADANIE, tid, ++wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia };
    this->rozeslijWszystkim(rzadanie);
    
    // czekaj na potwierdzenia od wszystkich
    while (this->liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1)
        obsluzWiadomosci();

    this->liczbaPotwierdzen = 0;
    
    wyswietlStan("Zebrałem wszystkie potwierdzenia (do zabrania)");
    
    /*
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    do {
        obsluzWiadomosci();
    } while (!czyMogeWejscDoSekcji() || liczbaDostepnychMPC == 0);
    
    // sekcja krytyczna
    wyswietlStan("Jestem w sekcji krytycznej (zabrałem MPC)");
    
    Wiadomosc alert(ZARAZ_ZABIORE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(alert);
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    liczbaPotwierdzen = 0;
    
    wyswietlStan("Zmieniam wartość zmiennej: liczbaDostepnychMPC (--)");
    --this->liczbaDostepnychMPC;
    
    // broadcast informacji o zabraniu MPC wraz z nową wartością liczby dostępnych MPC
    Wiadomosc zabranieMPC(ZABRANIE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(zabranieMPC);
    */
}

void Bibliotekarz::uzywajMPC() {
    wyswietlStan("Używam MPC");
    
    int microseconds = rand()%490000 + 10000;
    usleep(microseconds);
    ++wartoscZegaraLamporta;
}

/*void Bibliotekarz::zwolnijMPC() {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    
    wyswietlStan("Chcę zwolnić swojego MPC (koniec używania)");
    // dodaj własne ŻĄDANIE do kolejki (chcę zwolnić MPC)
    this->kolejkujRzadanie(tid, 0);
    // broadcast ŻĄDANIA
    Wiadomosc rzadanieDostepu(RZADANIE, tid, 0, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(rzadanieDostepu);    
    // czekaj na potwierdzenia od wszystkich
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    liczbaPotwierdzen = 0;
    
    wyswietlStan("Zebrałem wszystkie potwierdzenia (do zwolnienia)");
    
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    do {
        obsluzWiadomosci();
    } while (!czyMogeWejscDoSekcji());
    
    // sekcja krytyczna
    wyswietlStan("Jestem w sekcji krytycznej (zwalnianie)");
    Wiadomosc alert(ZARAZ_ZWOLNIE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(alert);
    while (liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1) {
        obsluzWiadomosci();
    }
    liczbaPotwierdzen = 0;
    
    wyswietlStan("Zmieniam wartość zmiennej: liczbaDostepnychMPC (++)");
    ++this->liczbaDostepnychMPC;
    
    // broadcast informacji o zwolnieniu MPC wraz z nową wartością liczby dostępnych MPC
    Wiadomosc zwolnienieMPC(ZWOLNIENIE_MPC, tid, liczbaDostepnychMPC, ++wartoscZegaraLamporta, 0);
    this->rozeslijWszystkim(zwolnienieMPC);
}*/

void Bibliotekarz::zwolnijMPC() {
    wyswietlStan("ATRAPA Zwalniam swojego MPC");
}

void Bibliotekarz::obsluzWiadomosci() {
    int flag = 0;
    Wiadomosc odpowiedz;
    MPI_Status status;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        MPI_Recv(&odpowiedz, PolaWiadomosci::LICZBA_POL, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        obsluzWiadomosc(odpowiedz);
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
}

void Bibliotekarz::obsluzWiadomosc(Wiadomosc wiadomosc) {
    switch (wiadomosc[PolaWiadomosci::TYP]) {
        case RZADANIE:
            kolejkujRzadanie(wiadomosc);
            odpowiedzNaRzadanie(wiadomosc[PolaWiadomosci::TID]);
            break;
        case POTWIERDZENIE:
            ++this->liczbaPotwierdzen;
        case ZWOLNIENIE_MPC:
            usunRzadanieZKolejki(wiadomosc[PolaWiadomosci::TID]);
            break;
    }
    wartoscZegaraLamporta = max(wartoscZegaraLamporta, wiadomosc[PolaWiadomosci::LAMPORT]) + 1;
}

void Bibliotekarz::kolejkujRzadanie(Wiadomosc w) {
    if (lista.empty()) {
        lista.push_front(ElementListy{w[TID], w[LAMPORT], w[CZYTELNICY]});
    } else {
        for (auto it = lista.begin(); it != lista.end(); ++it) {
            if (it->wartoscZegaraLamporta > w[LAMPORT] ||
            
               (it->wartoscZegaraLamporta == w[LAMPORT] &&
                it->liczbaCzytelnikowDoPonaglenia > w[CZYTELNICY]) ||
            
               (it->wartoscZegaraLamporta == w[LAMPORT] &&
                it->liczbaCzytelnikowDoPonaglenia == w[CZYTELNICY] &&
                it->tid > w[TID]))
                {
                    lista.insert(it, ElementListy{w[TID], w[LAMPORT], w[CZYTELNICY]});
                    return;
                }
        }
        lista.push_back(ElementListy{w[TID], w[LAMPORT], w[CZYTELNICY]});
    }
}

void Bibliotekarz::odpowiedzNaRzadanie(int tidAdresata) {
    Wiadomosc w = {POTWIERDZENIE, tid, ++wartoscZegaraLamporta, 0};
    MPI_Send(&w, PolaWiadomosci::LICZBA_POL, MPI_INT, tidAdresata, POTWIERDZENIE, MPI_COMM_WORLD);
}

void Bibliotekarz::rozeslijWszystkim(Wiadomosc wiadomosc) {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    for (int i = 0; i < LICZBA_BIBLIOTEKARZY; ++i) {
        if (i != tid) {
            MPI_Send(&wiadomosc, PolaWiadomosci::LICZBA_POL, MPI_INT, i, wiadomosc[PolaWiadomosci::TYP], MPI_COMM_WORLD);
        }
    }
}

void Bibliotekarz::usunRzadanieZKolejki(int tid) {
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if (it->tid == tid) {
            lista.erase(it);
            break;
        }
    }
}

bool Bibliotekarz::czyMogeWejscDoSekcji() const {
    ElementListy el = lista.front();
    if (el.tid == tid)
        return true;
    return false;
}

void Bibliotekarz::wyswietlStan(string info) const {
    cout << setw(4) << wartoscZegaraLamporta << " | "
         << "tid: " << setw(4) << tid << " | "
         << info << "\n";
}
