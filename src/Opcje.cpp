#include "Opcje.hpp"
#include "ParserOpcji.hpp"
#include "Wyjatki.hpp"

Opcje::Opcje()
    : liczbaBibliotakrzy(100), liczbaMPC(10), liczbaDostepowDoMPC(10) {}

Opcje& Opcje::pobierzInstancje() {
    static Opcje instancja;

    return instancja;
}

void Opcje::zaladujOpcjeProgramu(int argc, char *argv[]) {
    map<string, string> opcje = ParserOpcji::zczytajOpcjeUruchomienia(argc, argv);

    if (opcje.find("b") != opcje.end())
        liczbaBibliotakrzy = stoul(opcje["b"]);
    if (opcje.find("p") != opcje.end())
        liczbaMPC = stoul(opcje["p"]);
    if (opcje.find("l") != opcje.end())
        liczbaDostepowDoMPC = stoul(opcje["l"]);
}

int Opcje::pobierzLiczbeBibliotekarzy() const { return liczbaBibliotakrzy; }

int Opcje::pobierzLiczbeMPC() const { return liczbaMPC; }

int Opcje::pobierzLiczbeDostepowDoMPC() const { return liczbaDostepowDoMPC; }
