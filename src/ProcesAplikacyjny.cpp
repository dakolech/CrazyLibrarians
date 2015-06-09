#include "ProcesAplikacyjny.hpp"
#include "Opcje.hpp"

#include <time.h>
#include <openmpi/mpi.h>

ProcesAplikacyjny::ProcesAplikacyjny() : tid(0), liczbaBibliotekarzy(0) { }

ProcesAplikacyjny::~ProcesAplikacyjny() {
    MPI_Finalize();
}

void ProcesAplikacyjny::zaladuj(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &liczbaBibliotekarzy);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);

    srand(time(NULL) + tid);
    Opcje::pobierzInstancje().zaladujOpcjeProgramu(argc, argv, liczbaBibliotekarzy);
    
    bibliotekarz.reset(new Bibliotekarz(tid));
}

void ProcesAplikacyjny::wykonaj() {
    while (true) {
        bibliotekarz->zajmujSieSoba();
        bibliotekarz->odpowiedzInnymBibliotekarzom();
        bibliotekarz->poprosODostepDoMPC();
        bibliotekarz->uzywajMPC();
        bibliotekarz->zwolnijMPC();
    }
}

int ProcesAplikacyjny::pobierzTID() const {
    return tid;
}
