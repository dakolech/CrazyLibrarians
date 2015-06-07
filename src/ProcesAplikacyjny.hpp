#ifndef PROCESAPLIKACYJNY
#define PROCESAPLIKACYJNY

#include "IWykonywalny.hpp"

class ProcesAplikacyjny : public IWykonywalny {
    //Bibliotekarz bibliotekarz;
    int tid,
        liczbaBibliotekarzy;
    public:
        ProcesAplikacyjny();
        ~ProcesAplikacyjny();
        
        void zaladuj(int argc, char *argv[]);
        void wykonaj();
        int pobierzTID() const;
};

#endif /* PROCESAPLIKACYJNY_H */
