#ifndef PROCESAPLIKACYJNY
#define PROCESAPLIKACYJNY

class ProcesAplikacyjny {
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
