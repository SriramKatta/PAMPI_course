/*
 * Copyright (C) 2022 NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#include "timing.h"
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include "util.h"

double dmvm(double *restrict y,
            double *restrict a,
            double *restrict x,
            int N,
            int localN,
            int iter)
{
    double ts, te;
    int rank, size;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunksize = N / size;
    int balance = N % size;
    int prevnode = (rank - 1) % size;
    int nextnode = (rank + 1) % size;
    if (prevnode < 0)
        prevnode = size - 1;

    int chunkstart = rank * (N / size) + MIN(balance, rank);

    ts = MPI_Wtime();
    for (int j = 0; j < iter; j++)
    {
        int currN = localN;
        int currrank = rank;
        for (size_t rot = 0; rot < size; ++rot)
        {
            for (int r = 0; r < localN; r++)
            {
                double temp = 0.0;
                for (int c = chunkstart; c < chunkstart + currN; c++)
                {
                    temp += a[r * N + c] * x[c - chunkstart];
                }
                y[r] = temp;
            }

            chunkstart += currN;
            if (chunkstart >= N)
            {
                chunkstart = 0;
            }

            ++currrank;
            if (currrank == size)
            {
                currrank = 0;
            }
            currN = rowsinrank(currrank, size, N);

            if (rot != (size - 1))
            {
                MPI_Send(x, chunksize + (balance ? 1 : 0), MPI_DOUBLE, prevnode, 0, MPI_COMM_WORLD);
                MPI_Recv(x, chunksize + (balance ? 1 : 0), MPI_DOUBLE, nextnode, 0, MPI_COMM_WORLD, &status);
            }
        }
#ifdef CHECK
        {
            double sum = 0.0;

            for (int i = 0; i < N; i++)
            {
                sum += y[i];
                y[i] = 0.0;
            }
            fprintf(stderr, "Sum: %f\n", sum);
        }
#endif
    }
    te = MPI_Wtime();

    return te - ts;
}
