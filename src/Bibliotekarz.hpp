#ifndef BIBLIOTEKARZ
#define BIBLIOTEKARZ

class Bibliotekarz {
    Bibliotekarz() = delete;
    
    int liczbaDostepnychMPC, tidRodzica;
    public:
        Bibliotekarz(int tidRodzica);
        
        void zajmujSieSoba() const;
        void odpowiedzInnymBibliotekarzom();
        void poprosODostepDoMPC();
        void uzywajMPC();
        void zwolnijMPC();
};

#endif
