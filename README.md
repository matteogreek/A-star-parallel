# A-star-parallel
This is an academic project for the High Performance Computing course at the University of Trento, taught by Prof. Sandro Fiore.

## Project Goal
The focus of this project is to utilize parallel computing techniques to optimize the performance of the A* algorithm and to evaluate the results using benchmarking metrics. The parallel implementation of the A* algorithm will be developed in C language and by employing the MPI library. The algorithm will be implemented and tested on the [HPC@Unitrento](https://sites.google.com/unitn.it/hpc/) cluster to determine the impact of parallelism on the computational efficiency.

## Setup the environment
To gain access to the HPC cluster, you must possess a cluster account that is authorized. An authorized cluster account is a UniTN account that has been granted the necessary permissions. Access to the frontend node is achieved through the use of the SSH protocol and can be done either within the University network or remotely via the Internet using the VPN service.

1. Connect to the VPN.
2. Test server reachability by pinging it.
  ```
  ping hpc2.unitn.it
  ```
3. Connect via SSH. 
  ```
  ssh <username>@hpc2.unitn.it
  ```

After having connected to the login node you can run `./test.sh` to automatically perform benchamrks on a set of input mazes. 
The default input files can be found inside the `inputs` folder. The results will be stored inside the `benchmarks` directory.

## Results


| ![SpeedUp figure](https://github.com/matteogreek/A-star-parallel/blob/main/benchmarks/speedup.png) | ![Efficiency figure](https://github.com/matteogreek/A-star-parallel/blob/main/benchmarks/efficiency.png) | 
|:--:| :--:| 
| *Graph representing the algorithm speedup* |*Graph representing the algorithm efficiency* |

