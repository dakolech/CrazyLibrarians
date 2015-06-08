#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"
#include <string>

using namespace std;

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tidNadawcy, zegarLamporta, aktualnaLiczbaWolnychMPC;
    };
    
    Bibliotekarz() = delete;
    void wyswietlStan(string info) const;
    
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
