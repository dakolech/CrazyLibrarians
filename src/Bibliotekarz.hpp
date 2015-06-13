#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

#include <list>
#include <string>

using namespace std;

class Bibliotekarz {
    typedef int Wiadomosc[5];
    
    enum PolaWiadomosci { TYP = 0, TID = 1, LAMPORT = 2, CZYTELNICY = 3, NR_ZWALNIANEGO_MPC = 4, LICZBA_POL = NR_ZWALNIANEGO_MPC + 1 };
    enum TypWiadomosci  { RZADANIE, POTWIERDZENIE, ZWOLNIENIE_MPC };

    struct ElementListy {
        int tid, wartoscZegaraLamporta, liczbaCzytelnikowDoPonaglenia;
    };
    
    Bibliotekarz() = delete;
    void obsluzWiadomosci();
    void obsluzWiadomosc(Wiadomosc wiadomosc);
    int kolejkujRzadanie(Wiadomosc wiadomosc);
    void odpowiedzNaRzadanie(int tidAdresata);
    void rozeslijWszystkim(Wiadomosc wiadomosc);
    void usunRzadanieZKolejki(int tid);
    void wyswietlStan(string info) const;
    bool czyMogeWejscDoSekcji() const;

    list<ElementListy> lista;
    int tid, wartoscZegaraLamporta, liczbaPotwierdzen,
        nrOstatniegoZwolnionegoMPC, nrOczekiwanegoMPC;
    public:
        Bibliotekarz(int tidRodzica);

        void zajmujSieSoba();
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
