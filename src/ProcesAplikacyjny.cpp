#include "ProcesAplikacyjny.hpp"
#include "Opcje.hpp"

#include <openmpi/mpi.h>

ProcesAplikacyjny::ProcesAplikacyjny() : tid(0), liczbaBibliotekarzy(0) { }

ProcesAplikacyjny::~ProcesAplikacyjny() {
    MPI_Finalize();
}

void ProcesAplikacyjny::zaladuj(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &liczbaBibliotekarzy);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);

    Opcje::pobierzInstancje().zaladujOpcjeProgramu(argc, argv, liczbaBibliotekarzy);
}

void ProcesAplikacyjny::wykonaj() {
    return;
}

int ProcesAplikacyjny::pobierzTID() const {
    return tid;
}
