#include <iostream>
#include <cmath>
#include <fstream>
#include <mpi.h>

#define NRUNS 10

bool check_matrix(double *m, double *t, int  n);
bool check_symm(double *m, int n);

int main(int argc, char* argv[])  {
    int i, my_rank, size, r, run;
    int n; // Side length of square matrix
    double *M, *T, *b;
    int rows_per_process;
    double elapsed_time;
    double time2, time1;
    bool symmetry;

    std::fstream tr_file;
    std::fstream sym_file;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (my_rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <number of intervals>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    n = atol(argv[1]);

    if (my_rank == 0) {
        std::cout << "MPI version with processors = " << size << std::endl;
        std::cout << "Size of matrix: " << n << std::endl;
    }

    if ((r = n % size)) {
        if (my_rank == 0) {
            std::cerr << "The size of the matrix " << n << " must divide evenly by the number of processes " << size << std::endl;
        }

        MPI_Finalize();
        return 1;
    }

    rows_per_process = n / size;

    M = new double[n * n];
    b = new double[rows_per_process * n];

    int start = my_rank * rows_per_process;
    int stop = (my_rank + 1) * rows_per_process;

    if(my_rank == 0) {
        T = new double[n * n];

        tr_file.open("Transpose.csv", std::ios::in);
        sym_file.open("Symmetric.csv", std::ios::in);
        if (!tr_file.is_open()) {
            tr_file.open("Transpose.csv", std::ios::out);
            tr_file << "MatSize,Time,ThreadNum,Valid\n";
        }

        if (!sym_file.is_open()) {
            sym_file.open("Symmetric.csv", std::ios::out);
            sym_file << "MatSize,Time,ThreadNum,Valid\n";
        }

        sym_file.close();
        tr_file.close();
    }

    bool local_symmetry = true;

    for(int run = 0; run < NRUNS; run++) {
        if (my_rank == 0) {
            srand(time(NULL));
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < n; j++) {
                    M[i * n + j] = (rand() % 1000) / 10.0;
                }
            }
        }

        // Start of matrix transpose algorithm
        time1 = MPI_Wtime();
        MPI_Bcast(M, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        for (int i = start; i < stop; i++) {
            for (int j = 0; j < n; j++) {
                b[(i - start) * n + j] = M[j * n + i];
            }
        }

        MPI_Gather(b, rows_per_process * n, MPI_DOUBLE, T, rows_per_process * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        time2 = MPI_Wtime();
        elapsed_time = time2 - time1;

        // End of matrix transpose algorithm
        
        if(my_rank == 0) {
            tr_file.open("Transpose.csv", std::ios::out | std::ios::app);
            tr_file << n << "," << elapsed_time << "," << size << "," << check_matrix(M,T, n) << "\n";
            tr_file.close();

            srand(time(NULL));
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < n; j++) {
                    M[i * n + j] = (rand() % 1000) / 10.0;
                }
            }
        }

        // Start of matrix symmetry algorithm
        time1 = MPI_Wtime();

        MPI_Bcast(M, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        local_symmetry = true;

        for (int i = start; i < stop; i++) {
            for (int j = 0; j < n; j++) {
                if (fabs(M[j * n + i] - M[i * n + j]) > 1e-5) {
                    local_symmetry = false;
                }
            }
        }

        MPI_Reduce(&local_symmetry, &symmetry, 1, MPI_CXX_BOOL, MPI_LAND, 0, MPI_COMM_WORLD);

        time2 = MPI_Wtime();
        elapsed_time = time2 - time1;

        // End of matrix symmetry algorithm

        if(my_rank == 0) {
            sym_file.open("Symmetric.csv", std::ios::out | std::ios::app);
            sym_file << n << "," << elapsed_time << "," << size << "," << (check_symm(M, n) == symmetry) << "\n";
            sym_file.close();
        }
    }

    if (my_rank == 0) {
        delete[] T;
    }

    delete[] M;
    delete[] b;

    MPI_Finalize();
    return 0;
}

bool check_matrix(double *m, double *t, int n) {
    bool same = true;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
           if(fabs(t[j * n + i] - m[i * n + j]) > 1e-5) {
            same = false;
            break;
           }
        }
    }

    return same;
}

bool check_symm(double *m, int n) {
    bool symm = true;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
           if(fabs(m[j * n + i] - m[i * n + j]) > 1e-5) {
            symm = false;
            break;
           }
        }
    }

    return symm;
}