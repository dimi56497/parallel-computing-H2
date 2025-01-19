<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT LOGO -->
<br />
<div align="center">

<h3 align="center">Homework 2: Parallelizing matrix operations using MPI.</h3>

  <p align="center">
    This project is a homework delivery regarding the second homework of parallel computing.
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#task-statement">Task Statement</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#csv-usage">CSV Usage</a></li>
  </ol>
</details>



## Task Statement
In this assignment, you will explore explicit parallelization techniques using Message Passing Interface (MPI) by implementing a matrix transpose operation. You will benchmark and analyze the perfor- mance of this approach, comparing its efficiency and scalability with the parallel approaches of the first homework. Consider a matrix M of size n × n, where n is a power of two.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With
* ![Cpp][Cpp]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started
⚠️ Be careful, this project has been written in C++ and compiled using **gcc version `9.1.0`** and ***mpi version `3.2.1`***, use the same compiler as I don't assure the same output results using another version. When using this project two csv files will be created in which results will be saved.
The pbs included will run all the tests to be able to compute a weak scaling. By deaufult when launching the programm the simulation will executed 10 times.

### Prerequisites

Ensure that you have installed gcc version `9.1.0`
  ```sh
  g++ --version 
  ```
Ensure that you have installed mpi version `3.2.1`

If you want to run this program on the cluster make sure you have loaded gcc version `9.1.0` and mpi version `3.2.1` for gcc


### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/dimi56497/parallel-computing-H2.git
   ```
2. Copy the src directory on your HPC working dir
   ```sh
   scp -r src username@hpc.unitn.it:workingdir/
   ```
3. Copy the pbs file on your HPC working dir
   ```sh
   scp matJob.pbs username@hpc.unitn.it:workingdir/
   ```
4. Enter the HPC
   ```sh
   ssh username@hpc.unitn.it
   ```

5. Insert your working directory in `matJob.pbs`
   ```sh
   cd /home/username/dir; # Select your working directory
   ```
6. Run the pbs using the pbs scheduler
   ```sh
    qsub matJob.pbs
   ```
7. If you want to use an interactive session use this command
    ```sh
    qsub -I -q short_cpuQ -l select=1:ncpus=32:mpiprocs=32:mem=200mb
    ```
8. To run the program localy and on an interactive session
    ```sh
    mpicxx src/main.cpp -o code.out
    mpirun -np proc_number ./code.out mat_size_n
    ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## CSV Usage

The csv files contains:
- MatSize: size of matrix
- Time: execution time taken
- ThreadNumber: number of threads used (only present when using OpenMP)
- Valid: validity of data

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Dimitri Corraini - dimitri.corraini@studenti.unitn.it

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[Cpp]: https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white