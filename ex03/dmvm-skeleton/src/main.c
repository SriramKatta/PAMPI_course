/*
 * Copyright (C) 2022 NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "allocate.h"
#include "timing.h"
#include "util.h"
#include <mpi.h>

extern double dmvm(double *restrict y,
                   const double *restrict a,
                   const double *restrict x,
                   int N,
                   int localN,
                   int iter);

int main(int argc, char **argv)
{

    size_t bytesPerWord = sizeof(double);
    size_t N = 0;
    size_t iter = 1;
    double *a, *x, *y;
    double t0, t1;
    double walltime;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {

        if (argc != 3)
        {
            printf("Usage: %s <N> <iter>\n", argv[0]);
            exit(EXIT_SUCCESS);
        }
    }

    N = atoi(argv[1]);
    iter = atoi(argv[2]);

    int balance = N % size;
    int chunkstart = rank * (N / size) + MIN(balance, rank);
    int localN = rowsinrank(rank, size, N);

    a = (double *)allocate(ARRAY_ALIGNMENT, N * localN * bytesPerWord);
    x = (double *)allocate(ARRAY_ALIGNMENT, localN + 1 * bytesPerWord);
    y = (double *)allocate(ARRAY_ALIGNMENT, localN + 1 * bytesPerWord);

    // initialize arrays
    for (int i = 0; i < localN; i++)
    {
        x[i] = (double)i;
        y[i] = 0.0;

        for (int j = 0; j < N; j++)
        {
            a[i * N + j] = (double)j + i;
        }
    }

    walltime = dmvm(y, a, x, N, localN, iter);

    double flops = (double)2.0 * N * N * iter;
    // # iterations, problem size, flop rate, walltime
    if (rank == 0)
    {
        printf("%zu %zu %.2f %.2f\n", iter, N, 1.0E-06 * flops / walltime, walltime);
    }
    MPI_Finalize();

    return EXIT_SUCCESS;
}
