/*
 * Copyright (C) 2022 NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <float.h>
#include <mpi.h>
#include <math.h>

#include "timing.h"



double integrate(double, double, int);

int main(int argc, char **argv)
{
	double wcs, wce;
	double Pi;
	double res = 0.0;
	double tmp = 0.0;

	double a = 0.0, b = 1.0;

	int slices = 1000000;

#if 0
	wcs = getTimeStamp();
	Pi = integrate(a, b, slices);
	wce = getTimeStamp();
	printf("Pi=%.8lf in %.3lf s \n", Pi, wce - wcs);
#else
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double mya = a + rank * (b - a) / size;
	double myb = mya + (b - a) / size;
	slices /= size;
	Pi = integrate(mya, myb, slices);
	if (rank == 0)
	{

		res = Pi;
		for (int i = 1; i < size; ++i){
			MPI_Recv(&tmp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
			res += tmp;
		}
		printf("Pi=%.8lf\n", Pi);
	}
	else
	{
		MPI_Send(&Pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
#endif
	return EXIT_SUCCESS;
}

double integrate(double a, double b, int SLICES)
{
	double dx = 1.0 / SLICES;
	double sum = 0.0;

	/*

		Your logic to integrate between given interval a to b.
		Declare SLICES here and calculate delta x using a, b and SLICES.
		Iterate over number of slices, calculate the area and sum them.
		Return sum * delta x to get the value of PI.

	*/
	for (size_t i = 0; i < SLICES; i++)
	{
		double x = (i + 0.5) * dx;
		sum += 4.0 * sqrt(1.0 - x * x);
	}

	return sum * dx;
}
