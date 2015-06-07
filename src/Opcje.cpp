#include "Opcje.hpp"
#include "ParserOpcji.hpp"
#include "Wyjatki.hpp"

Opcje::Opcje() : liczbaMPC(10) {}

Opcje& Opcje::pobierzInstancje() {
    static Opcje instancja;

    return instancja;
}

void Opcje::zaladujOpcjeProgramu(int argc, char *argv[], int liczbaBibliotakrzy) {
    map<string, string> opcje = ParserOpcji::zczytajOpcjeUruchomienia(argc, argv);

    if (opcje.find("p") != opcje.end())
        liczbaMPC = stoul(opcje["p"]);
        
    this->liczbaBibliotakrzy = liczbaBibliotakrzy;
}

int Opcje::pobierzLiczbeBibliotekarzy() const { return liczbaBibliotakrzy; }

int Opcje::pobierzLiczbeMPC() const { return liczbaMPC; }
