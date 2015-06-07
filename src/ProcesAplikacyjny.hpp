#ifndef PROCESAPLIKACYJNY
#define PROCESAPLIKACYJNY

#include "Bibliotekarz.hpp"
#include <memory>

using namespace std;

class ProcesAplikacyjny {
    unique_ptr<Bibliotekarz> bibliotekarz;
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
