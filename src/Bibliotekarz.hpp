#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"
#include <list>
#include <string>

using namespace std;

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tid, zegarLamporta, aktualnaLiczbaWolnychMPC,
            liczbaCzytelnikowDoPonaglenia;

        Wiadomosc() { }
        Wiadomosc(TypWiadomosci typ, int tid, int MPCs, int lamport, int czytelnicy)
            : typ(typ), tid(tid), aktualnaLiczbaWolnychMPC(MPCs), zegarLamporta(lamport), 
              liczbaCzytelnikowDoPonaglenia(czytelnicy) { };
    };

    struct ElementListy {
        int tid, liczbaCzytelnikowDoPonaglenia;
    };
    
    Bibliotekarz() = delete;
    void obsluzWiadomosci();
    void obsluzWiadomosc(Wiadomosc wiadomosc);
    void kolejkujRzadanie(int tid, int liczbaCzytelnikowDoPonaglenia);
    void odpowiedzNaRzadanie(int tidAdresata);
    void rozeslijWszystkim(Wiadomosc wiadomosc);
    void wyswietlStan(string info) const;
    bool czyMogeWejscDoSekcji() const;
        
    list<ElementListy> lista;
    int liczbaDostepnychMPC, tid, wartoscZegaraLamporta,
        liczbaCzytelnikowDoPonaglenia, liczbaPotwierdzen;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba();
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
