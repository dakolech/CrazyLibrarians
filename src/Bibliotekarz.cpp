#include "Bibliotekarz.hpp"
#include "Opcje.hpp"
#define MSG_TAG 100

#include <iostream>
#include <openmpi/mpi.h>
#include <unistd.h>

Bibliotekarz::Bibliotekarz(int tidRodzica)
    : tidRodzica(tidRodzica), liczbaDostepnychMPC(Opcje::pobierzInstancje().pobierzLiczbeMPC()), liczbaBibliotekarzy(Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy()) { }

void Bibliotekarz::zajmujSieSoba() const {
    int microseconds = rand()%4900000 + 100000;
    usleep(microseconds);
}

void Bibliotekarz::poprosODostepDoMPC() {
    liczbaCzytelnikowDoPonaglenia = rand()%100 + 1;
    Wiadomosc zapytanie;
    zapytanie.typ = RZADANIE;
    zapytanie.tidNadawcy = tidRodzica;
    zapytanie.zegarLamporta = wartoscZegaraLamporta;
    zapytanie.aktualnaLiczbaWolnychMPC = liczbaDostepnychMPC;
    zapytanie.liczbaCzytelnikowDoPonaglenia = liczbaCzytelnikowDoPonaglenia;

    for(int i=0; i<liczbaBibliotekarzy; i++) {
        MPI_Send(&zapytanie, sizeof(zapytanie), MPI_BYTE, i, MSG_TAG, MPI_COMM_WORLD);
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

            ElementKolejki nowyElement;
            nowyElement.idProcesu = odpowiedz.tidNadawcy;
            nowyElement.liczbaCzytelnikowDoPonaglenia = odpowiedz.liczbaCzytelnikowDoPonaglenia;
            nowyElement.pozwolilem = false;

            for( list<ElementKolejki>::iterator iter=kolejka.begin(); iter != kolejka.end(); iter++ )
                if ((iter->liczbaCzytelnikowDoPonaglenia > liczbaCzytelnikowDoPonaglenia &&
                        iter->pozwolilem == false)
                    ||
                    (iter->liczbaCzytelnikowDoPonaglenia == liczbaCzytelnikowDoPonaglenia &&
                        iter->idProcesu > tidRodzica && iter->pozwolilem == false))
                  kolejka.insert(iter, nowyElement);
        }

        if (liczbaOdebranychWiadomosci >= liczbaBibliotekarzy)
          koniec = true;
    }

}
