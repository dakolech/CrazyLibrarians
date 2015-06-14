#include "Bibliotekarz.hpp"
#include "Opcje.hpp"

#include <openmpi/mpi.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tid(tidRodzica), wartoscZegaraLamporta(0), liczbaPotwierdzen(0), nrOczekiwanegoMPC(-1) {
    stanMPC.resize(Opcje::pobierzInstancje().pobierzLiczbeMPC());
    fill(stanMPC.begin(), stanMPC.end(), true);
}

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

void Bibliotekarz::poprosODostepDoMPC() {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    int liczbaCzytelnikowDoPonaglenia = rand()%100 + 1;
    
    wyswietlStan("Proszę o dostęp do MPC (do ponaglenia: " + to_string(liczbaCzytelnikowDoPonaglenia) + ")");
    
    // dodaj własne ŻĄDANIE do kolejki (chcę zabrać MPC)
    Wiadomosc rzadanie = { RZADANIE, tid, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia };
    
    // broadcast ŻĄDANIA
    this->rozeslijWszystkim(rzadanie);
    
    // czekaj na potwierdzenia od wszystkich
    while (this->liczbaPotwierdzen != LICZBA_BIBLIOTEKARZY - 1)
        obsluzWiadomosci();

    this->liczbaPotwierdzen = 0;
    
    wyswietlStan("Zebrałem wszystkie potwierdzenia, czekam na zwolnienie MPC nr " + to_string(this->nrOczekiwanegoMPC));
    
    this->nrOczekiwanegoMPC = kolejkujRzadanie(rzadanie) % Opcje::pobierzInstancje().pobierzLiczbeMPC();
    // sprawdzenie, czy jestem już na szczycie swojej kolejki, jeśli nie, to czekanie
    // na odbiór wiadomości o zwolnieniu sekcji krytycznej
    do {
        obsluzWiadomosci();
    } while (!czyMogeWejscDoSekcji());
    
    Wiadomosc zabranieMPC = { ZABRANIE_MPC, tid, ++wartoscZegaraLamporta, 0, this->nrOczekiwanegoMPC };
    this->rozeslijWszystkim(zabranieMPC);
}

void Bibliotekarz::uzywajMPC() {
    wyswietlStan("\033[31mUŻYWAM MPC NR " + to_string(this->nrOczekiwanegoMPC) + "\033[m");
    
    int microseconds = rand()%490000 + 10000;
    usleep(microseconds);
    ++wartoscZegaraLamporta;
}

void Bibliotekarz::zwolnijMPC() {
    wyswietlStan("\033[32mZWALNIAM MPC NR " + to_string(this->nrOczekiwanegoMPC) + "\033[m");
    
    // usuń własne żądanie z kolejki
    usunRzadanieZKolejki(tid);
    // broadcast ZWOLNIENIA MPC
    Wiadomosc zwolnienie = { ZWOLNIENIE_MPC, tid, ++wartoscZegaraLamporta, 0, this->nrOczekiwanegoMPC };
    this->rozeslijWszystkim(zwolnienie);
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
            break;
        case ZABRANIE_MPC:
            stanMPC[wiadomosc[PolaWiadomosci::NR_MPC]] = false;
            break;
        case ZWOLNIENIE_MPC:
            usunRzadanieZKolejki(wiadomosc[PolaWiadomosci::TID]);
            stanMPC[wiadomosc[PolaWiadomosci::NR_MPC]] = true;
            break;
    }
    wartoscZegaraLamporta = max(wartoscZegaraLamporta, wiadomosc[PolaWiadomosci::LAMPORT]) + 1;
}

int Bibliotekarz::kolejkujRzadanie(Wiadomosc w) {
    if (lista.empty()) {
        lista.push_front(ElementListy{w[TID], w[LAMPORT], w[CZYTELNICY]});
        return 0;
    } else {
        int i = 0;
        for (auto it = lista.begin(); it != lista.end(); ++it, ++i) {
            if (it->wartoscZegaraLamporta > w[LAMPORT] ||
            
               (it->wartoscZegaraLamporta == w[LAMPORT] &&
                it->liczbaCzytelnikowDoPonaglenia > w[CZYTELNICY]) ||
            
               (it->wartoscZegaraLamporta == w[LAMPORT] &&
                it->liczbaCzytelnikowDoPonaglenia == w[CZYTELNICY] &&
                it->tid > w[TID]))
                {
                    lista.insert(it, ElementListy{w[TID], w[LAMPORT], w[CZYTELNICY]});
                    return i;
                }
        }
        lista.push_back(ElementListy{w[TID], w[LAMPORT], w[CZYTELNICY]});
        return i;
    }
}

void Bibliotekarz::odpowiedzNaRzadanie(int tidAdresata) {
    Wiadomosc w = { POTWIERDZENIE, tid, ++wartoscZegaraLamporta, 0 };
    MPI_Send(w, PolaWiadomosci::LICZBA_POL, MPI_INT, tidAdresata, POTWIERDZENIE, MPI_COMM_WORLD);
}

void Bibliotekarz::rozeslijWszystkim(Wiadomosc wiadomosc) {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    for (int i = 0; i < LICZBA_BIBLIOTEKARZY; ++i) {
        if (i != tid) {
            MPI_Send(wiadomosc, PolaWiadomosci::LICZBA_POL, MPI_INT, i, wiadomosc[PolaWiadomosci::TYP], MPI_COMM_WORLD);
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
    int i = 0;
    for (auto iterator = lista.begin();
         i < Opcje::pobierzInstancje().pobierzLiczbeMPC() &&
         iterator != lista.end();
         ++i, ++iterator)
    {
        if (iterator->tid == tid && stanMPC[this->nrOczekiwanegoMPC])
            return true;
    }
    return false;
}

void Bibliotekarz::wyswietlStan(string info) const {
    const int LICZBA_BIBLIOTEKARZY = Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy();
    
    cout << setw(4) << wartoscZegaraLamporta << " | "
         << "tid: " << setw(4) << tid << " | "
         << "kolejka: ";

    if (lista.size() > 0) {
        for (auto &e : lista)
            cout << setw(2) << e.tid << " ";
    }
    
    cout << string(3*(LICZBA_BIBLIOTEKARZY - lista.size()), ' ') << "| " << info << "\n";
}







