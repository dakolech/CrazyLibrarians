#ifndef PROCESAPLIKACYJNY
#define PROCESAPLIKACYJNY

#include "IWykonywalny.hpp"

class ProcesAplikacyjny : public IWykonywalny {
    //Bibliotekarz bibliotekarz;
    public:
        ProcesAplikacyjny(int argc, char *argv[]);
        
        void wykonaj();
};

#endif /* PROCESAPLIKACYJNY_H */
