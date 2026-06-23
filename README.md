# Philosophers

Implementation of the classic **Dining Philosophers Problem** from the 42 curriculum.

The goal of this project is to understand concurrency, synchronization, shared resources and process/thread management in C.

## About

This project simulates a group of philosophers sitting around a table. Each philosopher alternates between eating, sleeping and thinking. To eat, a philosopher needs two forks, but forks are shared with the philosophers next to them.

The challenge is to correctly manage concurrent execution while avoiding race conditions, deadlocks and inconsistent output.

## Features

- Philosopher simulation with precise timing
- Thread-based implementation for the mandatory part
- Mutexes to protect shared resources
- Death monitoring system
- Optional meal limit
- Synchronized output
- Bonus version using processes and semaphores

## Project Structure

```txt
.
├── Makefile
├── philo
│   ├── main.c
│   ├── auxiliary1.c
│   ├── data_saving.c
│   ├── routine_and_monitor.c
│   └── philo.h
└── philo_bonus
    ├── main_bonus.c
    ├── auxiliary1_bonus.c
    ├── data_saving_bonus.c
    ├── run_bonus.c
    └── philo_bonus.h
Usage

The program receives the following arguments:

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

Arguments:

Argument	Description
number_of_philosophers	Number of philosophers and forks
time_to_die	Maximum time in ms a philosopher can go without eating
time_to_eat	Time in ms a philosopher spends eating
time_to_sleep	Time in ms a philosopher spends sleeping
number_of_times_each_philosopher_must_eat	Optional number of meals required for each philosopher
Compilation

Compile the mandatory version:

make

Run it:

./philo/philo 5 800 200 200

Compile the bonus version:

make bonus

Run it:

./philo_bonus/philo_bonus 5 800 200 200

Clean object files:

make clean

Remove object files and executables:

make fclean

Recompile everything:

make re
Example
./philo/philo 4 410 200 200

Example output:

0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
1 3 has taken a fork
1 3 has taken a fork
1 3 is eating
200 1 is sleeping
200 3 is sleeping

Each line follows this format:

timestamp_in_ms philosopher_id action
Implementation Details
Mandatory

The mandatory version uses:

One thread per philosopher
One mutex per fork
A mutex for printing
A mutex for death/state control
A monitor thread to detect if a philosopher dies
Bonus

The bonus version uses:

One process per philosopher
Semaphores for fork management
Semaphores for printing and death detection
Internal monitor threads inside philosopher processes
What I Learned

Through this project I practiced:

Multithreading in C
Mutexes and semaphores
Process creation with fork
Time management with gettimeofday
Avoiding race conditions
Preventing deadlocks
Cleaning resources correctly
Structuring a concurrent program
Technologies
C
POSIX threads
Mutexes
Semaphores
Processes
Makefile
Notes

This project is part of the 42 curriculum.
It is mainly focused on learning how concurrency works at a low level in C.

If you are also a 42 student, use this repository only as a reference. The best way to learn this project is to understand the synchronization problems and implement your own solution.

Author
Luis Ángel Álvarez Gil
42 Málaga

Luis Ángel Álvarez Gil
42 Málaga
