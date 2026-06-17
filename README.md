# CPU-Scheduler_simulator

This project is a high-fidelity simulation of a CPU task scheduler and a multi-tier memory hierarchy. It models the performance impact of memory latency on task execution time using a Round Robin scheduling algorithm and a FIFO cache policy.

## Features
* **Round Robin Scheduling:** Processes tasks with a fixed quantum of 3 cycles.
* **Hierarchical Memory:** Simulates L1, L2, and L3 caches with specific capacity, latency, and promotion logic.
* **Performance Tracking:** Dynamically calculates total CPU cycles, including stall cycles caused by memory misses.
* **FIFO Eviction:** Implements the First-In-First-Out policy for all cache tiers with a cascade eviction mechanism.

## Requirements
* C++ Compiler (g++, clang++, or MSVC)
* Standard C++11 or higher

## How to Compile and Run
1. **Clone the repository:**
```bash
git clone <your-repo-url>
cd <repo-name>
How to Compile and Run
Clone the repository:
```

```Bash
git clone <your-repo-url>
cd <repo-name>
Compile the code:
Using g++ (recommended):
```
```Bash
g++ -o simulator main.cpp
Run the simulator:
You can run the simulator using the default input.txt file or specify your own:
```
```Bash
./simulator          # Uses default input.txt
./simulator my_input.txt # Uses custom input file
```
## Input File Format
The simulator expects a plain text file with the following format:

Plaintext
TASK T1 BURST 35 MEM M1 M2 M3 ...
TASK T2 BURST 10 MEM M1 M34 ...
## Project Structure
* main.cpp: Contains the scheduler engine, cache logic, and memory hierarchy.

* input.txt: The input data used for simulation testing.

* README.md: This documentation file.
