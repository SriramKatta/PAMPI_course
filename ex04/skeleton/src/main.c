/*
 * Copyright (C) NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "likwid-marker.h"
#include "parameter.h"
#include "solver.h"
#include "timing.h"

#include <mpi.h>

int main(int argc, char **argv)
{
    double startTime, endTime;
    int rank;
    Parameter params;
    Solver solver;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    initParameter(&params);

    if (argc != 2)
    {
        printf("Usage: %s <configFile>\n", argv[0]);
        MPI_Finalize();
        exit(EXIT_SUCCESS);
    }

    readParameter(&params, argv[1]);
    if (rank == 0)
        printParameter(&params);

    initSolver(&solver, &params, 2);
    startTime = getTimeStamp();
    solve(&solver);
    endTime = getTimeStamp();
    getResult(&solver);

    if (rank == 0)
        printf("Walltime %.2fs\n", endTime - startTime);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
