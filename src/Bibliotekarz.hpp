#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include <list>
#include <string>

using namespace std;

class Bibliotekarz {
    /*struct Wiadomosc {
        TypWiadomosci typ;
        int tid, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia;

        Wiadomosc() { }
        Wiadomosc(TypWiadomosci typ, int tid, int lamport, int czytelnicy)
            : typ(typ), tid(tid), wartoscZegaraLamporta(lamport), liczbaCzytelnikowDoPonaglenia(czytelnicy) { };
    };*/
    typedef int Wiadomosc[4];
    
    enum PolaWiadomosci { TYP = 0, TID = 1, LAMPORT = 2, CZYTELNICY = 3, LICZBA_POL = CZYTELNICY + 1 };
    enum TypWiadomosci  { RZADANIE, POTWIERDZENIE, ZWOLNIENIE_MPC };

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
    int tid, wartoscZegaraLamporta, liczbaPotwierdzen;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba();
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
