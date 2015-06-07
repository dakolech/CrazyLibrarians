#ifndef WYJATKI
#define WYJATKI

#include <exception>
#include <string>

using namespace std;

struct BladParsowaniaOpcji : public exception {
    BladParsowaniaOpcji(const char *nazwaProgramu) : nazwaProgramu(nazwaProgramu) { }
    const char *what() const throw() {
        return string("Użycie:   " + nazwaProgramu + " [-opcja] [argument]\n"
             + "Opcje:\n\t"
             + "-h   wyswietl pomoc\n"
             + "\t" + "-b   liczba bibliotekarzy (domyslnie 100)\n"
             + "\t" + "-p   liczba dostępnych MPC (Mechanicznych Poganiaczy Czytelników) (domyslnie 10)\n"
             + "\t" + "-l   ile razy bibliotekarz ma ubiegać się o dostęp do MPC (domyślnie 10)\n"
             + "Przykład: " + nazwaProgramu + " -b 100 -p 10 -l 20\n").c_str();
    }
    private:
        string nazwaProgramu;
};

#endif  /* EXCEPTIONS_HPP */

