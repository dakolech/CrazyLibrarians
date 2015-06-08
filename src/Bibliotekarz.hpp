#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"

using namespace std;

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tidNadawcy, zegarLamporta, aktualnaLiczbaWolnychMPC;
    };
    
    Bibliotekarz() = delete;
    
    int liczbaDostepnychMPC, tidRodzica, wartoscZegaraLamporta;
    public:
        Bibliotekarz(int tidRodzica);
        
        void zajmujSieSoba() const;
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
