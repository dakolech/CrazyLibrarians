#ifndef OPCJE
#define OPCJE

#include <string>
#include <map>

using namespace std;

class Opcje {
    Opcje();
    Opcje(Opcje const&)          = delete;
    void operator=(Opcje const&) = delete;

    int liczbaBibliotakrzy,
        liczbaMPC,
        liczbaDostepowDoMPC;

    public:
        static Opcje& pobierzInstancje();

        void zaladujOpcjeProgramu(int argc, char *argv[], int liczbaBibliotakrzy);

        int pobierzLiczbeBibliotekarzy() const;
        int pobierzLiczbeMPC() const;
        int pobierzLiczbeDostepowDoMPC() const;
};

#endif
