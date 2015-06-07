#ifndef EXCEPTIONS_HPP
#define WYJATKI

#include <exception>
#include <string>

using namespace std;

struct WyswietlonoPomoc : public exception {
    WyswietlonoPomoc() { }
    const char *what() const throw() {
        return "";
    }
};

#endif  /* EXCEPTIONS_HPP */

