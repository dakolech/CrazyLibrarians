#include "ParserOpcji.hpp"
#include "Wyjatki.hpp"

#include <unistd.h>
#include <iostream>

map<string, string> ParserOpcji::zczytajOpcjeUruchomienia(int argc, char *argv[]) {
    char tmp;
    std::map<string, string> opcje;

    if (argc < 3)
        throw BladParsowaniaOpcji(argv[0]);

    while ((tmp = getopt(argc, argv, "p:")) != -1) {
        switch (tmp) {
            case 'p':
                opcje[string(&tmp)] = string(optarg);
                break;
            default:
                throw BladParsowaniaOpcji(argv[0]);
                break;
        }
    }

    return opcje;
}
