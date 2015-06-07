#ifndef EXCEPTIONS_HPP
#define WYJATKI

#include <exception>
#include <string>

using namespace std;

struct WyswietlonoPomoc : public exception {
    WyswietlonoPomoc(const char *nazwaProgramu) : nazwaProgramu(nazwaProgramu) { }
    const char *what() const throw() {
        return string("Użycie:   " + nazwaProgramu + " [-opcja] [argument]\n"
             + "Opcje:\n\t"
             + "-h   wyswietl pomoc\n"
             + "\t" + "-b   liczba bibliotekarzy\n"
             + "\t" + "-p   liczba dostępnych MPC (Mechanicznych Poganiaczy Czytelników)\n"
             + "Przykład: " + nazwaProgramu + " -b 100 -p 10\n").c_str();
    }
    private:
        string nazwaProgramu;
};

#endif  /* EXCEPTIONS_HPP */

