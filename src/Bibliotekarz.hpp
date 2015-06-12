#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include "Definicje.hpp"
#include <list>
#include <string>

using namespace std;

class Bibliotekarz {
    struct Wiadomosc {
        TypWiadomosci typ;
        int tid, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia;

        Wiadomosc() { }
        Wiadomosc(TypWiadomosci typ, int tid, int lamport, int czytelnicy)
            : typ(typ), tid(tid), wartoscZegaraLamporta(lamport), liczbaCzytelnikowDoPonaglenia(czytelnicy) { };
    };

    struct ElementListy {
        int tid, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia;
    };
    
    Bibliotekarz() = delete;
    void obsluzWiadomosci();
    void obsluzWiadomosc(Wiadomosc wiadomosc);
    void kolejkujRzadanie(Wiadomosc wiadomosc);
    void odpowiedzNaRzadanie(int tidAdresata);
    void rozeslijWszystkim(Wiadomosc wiadomosc);
    void usunRzadanieZKolejki(int tid);
    void wyswietlStan(string info) const;
    bool czyMogeWejscDoSekcji() const;
        
    list<ElementListy> lista;
    int tid, wartoscZegaraLamporta;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba();
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
