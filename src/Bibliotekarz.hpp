#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"
#include <list>

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tidNadawcy, zegarLamporta, aktualnaLiczbaWolnychMPC, liczbaCzytelnikowDoPonaglenia;
    };

    struct ElementKolejki {
        int idProcesu, liczbaCzytelnikowDoPonaglenia;
        bool pozwolilem;
    };

    Bibliotekarz() = delete;
    
    int liczbaDostepnychMPC, tidRodzica, wartoscZegaraLamporta, liczbaBibliotekarzy,
        liczbaCzytelnikowDoPonaglenia;
    std::list<ElementKolejki> kolejka;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba() const;
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
