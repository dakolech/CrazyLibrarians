#include "ParserOpcji.hpp"
#include "Wyjatki.hpp"

#include <unistd.h>
#include <iostream>

map<string, string> ParserOpcji::zczytajOpcjeUruchomienia(int argc, char *argv[]) {
    char tmp;
    std::map<string, string> opcje;

    if (argc < 3)
        wyswietlPomoc(argv[0]);

    while ((tmp = getopt(argc, argv, "b:p:")) != -1) {
        switch (tmp) {
            case 'b': case 'p':
                opcje[string(&tmp)] = string(optarg);
                break;
            default:
                wyswietlPomoc(argv[0]);
                break;
        }
    }

    return opcje;
}

void ParserOpcji::wyswietlPomoc(char *programName) {
    cout << "Użycie:   " << programName << " [-opcja] [argument]\n"
         << "Opcje:\n\t"
         << "-h   wyswietl pomoc\n"
         << "\t" << "-b   liczba bibliotekarzy\n"
         << "\t" << "-p   liczba dostępnych MPC (Mechanicznych Poganiaczy Czytelników)\n"
         << "Przykład: " << programName << " -b 100 -p 10\n";

    throw WyswietlonoPomoc();
}
