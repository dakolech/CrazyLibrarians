#include "ProcesAplikacyjny.hpp"
#include "Wyjatki.hpp"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    ProcesAplikacyjny p;
    try {
        p.zaladuj(argc, argv);
        p.wykonaj();
    } catch (BladParsowaniaOpcji &e) {
        // tylko 1 proces (o tid == 0) wypisuje komunikat
        if (p.pobierzTID() == 0)
            cerr << e.what();
    }
}
