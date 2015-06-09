#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"
#include <list>
#include <string>

using namespace std;

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tidNadawcy, zegarLamporta, aktualnaLiczbaWolnychMPC,
            liczbaCzytelnikowDoPonaglenia;
    };

    struct ElementListy {
        int idProcesu, liczbaCzytelnikowDoPonaglenia;
    };

    Bibliotekarz() = delete;
    void wyswietlStan(string info) const;
    
    list<ElementListy> lista;
    int liczbaDostepnychMPC, tidRodzica, wartoscZegaraLamporta,
        liczbaCzytelnikowDoPonaglenia, iloscPotwierdzen;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba() const;
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
        void obsluzWiadomosci();
        void obsluzWiadomosc(Wiadomosc wiadomosc);
        void osbluzRzadanie(Wiadomosc wiadomosc);
};

#endif
