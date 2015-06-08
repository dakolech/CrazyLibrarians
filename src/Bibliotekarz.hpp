#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"
#include <list>

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tidNadawcy, zegarLamporta, aktualnaLiczbaWolnychMPC, liczbaCzytelnikowDoPonaglenia;
    };

    struct Element {
        int idProcesu, liczbaCzytelnikowDoPonaglenia;
        bool pozwolilem;
    }
    Bibliotekarz() = delete;
    
    int liczbaDostepnychMPC, tidRodzica, wartoscZegaraLamporta, liczbaBibliotekarzy,
        liczbaCzytelnikowDoPonaglenia;
    std::list<Wiadomosc> kolejka;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba() const;
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
