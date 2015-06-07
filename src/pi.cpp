#include <stdlib.h>
#include <stdio.h>
#include <openmpi/mpi.h>
#include <math.h>

#include <iostream>
#include "Opcje.hpp"
#include "Wyjatki.hpp"

using namespace std;

#define POINTS 10000

#define ROOT 0
#define MSG_TAG 100

int main(int argc, char **argv) {
    int size,tid;
    int R;

    MPI_Init(&argc, &argv); 

    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &tid );
    
    try {
        Opcje::pobierzInstancje().zaladujOpcjeProgramu(argc, argv);
        if (tid == 0) {
            cout << "Bibliotekarzy: " << Opcje::pobierzInstancje().pobierzLiczbeBibliotekarzy() << "\n"
                 << "MPC: " << Opcje::pobierzInstancje().pobierzLiczbeMPC() << "\n"
                 << "Dostepow do MPC: " << Opcje::pobierzInstancje().pobierzLiczbeDostepowDoMPC() << "\n";
        }
    } catch (BladParsowaniaOpcji &e) {
        if (tid == 0)
            cerr << e.what();
        
        MPI_Finalize();
        return 0;
    }

    srand( tid * time(NULL) );

    int res,
        points_per_process = POINTS / size;

    if ( tid == 0 ) {
        MPI_Status status;
        int i, temp_sum = 0;
        long double pi;
    
        for (i = 1; i < size; ++i) {
            MPI_Recv( &res, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            temp_sum += res;
        }

        pi = 4.*temp_sum/POINTS;
        printf("Przybliżenie pi po zebraniu danych od %d procesów wynosi %llf\n", i, pi);
    } else {
        int i, nk = 0;
        long double x, y;
        for (i = 0; i < points_per_process; ++i) {
            x = ((long double)rand() / (RAND_MAX)) * 2 - 1;
            y = ((long double)rand() / (RAND_MAX)) * 2 - 1;
            if (x*x + y*y <= 1) {
                nk++;
            }
        }
        MPI_Send( &nk, 1, MPI_INT, ROOT, MSG_TAG, MPI_COMM_WORLD );
    }

    MPI_Finalize();
}
