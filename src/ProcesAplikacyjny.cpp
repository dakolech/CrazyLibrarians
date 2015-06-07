#include "ProcesAplikacyjny.hpp"
#include "Opcje.hpp"

ProcesAplikacyjny::ProcesAplikacyjny(int argc, char *argv[]) {
    Opcje::pobierzInstancje().zaladujOpcjeProgramu(argc, argv);
}

void ProcesAplikacyjny::wykonaj() {
    return;
}
