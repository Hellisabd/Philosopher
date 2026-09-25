*This project has been created as part of the 42 curriculum by bgrosjea.*

# Philosophers

> I never thought philosophy would be so deadly.

## Description

**Philosophers** is an introduction to multithreading in C, based on the classic
[dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).

One or more philosophers sit at a round table with a bowl of spaghetti in the
middle. There is one fork between each pair of neighbours, so there are as many
forks as philosophers. Each philosopher cycles through three states:

**eating → sleeping → thinking → eating → …**

To eat, a philosopher must hold both the fork on their left and the fork on
their right. If a philosopher goes `time_to_die` milliseconds without starting a
meal, they die and the simulation stops. The goal is to keep everyone alive, with
no data races, no deadlocks, and no overlapping output.

This repository contains the **mandatory part** (`philo/`):

- each philosopher is a separate **thread** (`pthread_create`);
- each fork is protected by its own **mutex**;
- a separate **supervisor thread** watches every philosopher and reports a death
  within a few milliseconds.

## Instructions

### Compilation

```sh
cd philo
make        # builds ./philo with cc -Wall -Wextra -Werror
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

### Usage

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Meaning |
| --- | --- |
| `number_of_philosophers` | Number of philosophers, and of forks (max 200). |
| `time_to_die` (ms) | A philosopher dies if they have not started eating within this time since their last meal (or the start). |
| `time_to_eat` (ms) | Time spent eating, holding two forks. |
| `time_to_sleep` (ms) | Time spent sleeping. |
| `number_of_times_each_philosopher_must_eat` (optional) | The simulation stops once every philosopher has eaten this many times. |

All arguments must be positive integers. Invalid input prints an error and the
program exits with status `1`.

### Output

Every state change is printed as:

```
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```

Each philosopher's lines are coloured to make the logs easier to read.

### Examples

```sh
./philo 1 800 200 200       # one fork only: the philosopher dies at 800 ms
./philo 5 800 200 200       # nobody should die
./philo 5 800 200 200 7     # stops once everyone has eaten 7 times
./philo 4 410 200 200       # nobody should die
./philo 4 310 200 100       # a philosopher dies
```

## Technical choices

- **Shared state:** everything lives in a single `t_phil` struct passed to every
  thread; there are no global variables.
- **Mutexes:**
  - `f_m[n]`: one mutex per fork;
  - `print_m`: stops log lines from overlapping;
  - `t_m`: protects each philosopher's last meal timestamp;
  - `alive_check`: protects the `alive` flag that ends the simulation;
  - `start`: holds all threads until every thread has been created, so they
    start at the same time;
  - `mutex` / `init_sup`: give each thread its ID and protect the meal counter.
- **Avoiding deadlock:** philosophers with odd IDs wait a moment before
  starting, so neighbours don't all grab their first fork at the same time.
- **Precise sleeping:** `ft_sleep` loops on `gettimeofday` rather than calling
  one long `usleep`, and checks the `alive` flag on each pass, so a thread stops
  soon after a death.
- **Death detection:** the supervisor thread keeps scanning
  `time_since_last_meal` for every philosopher. When one runs out of time it sets
  `alive = false` and prints `died`. Every philosopher checks that flag before
  each action.
- **Single philosopher:** handled separately (`one_phil`): they take their only
  fork, wait `time_to_die`, and die.

## Resources

- [Dining philosophers problem, Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming, LLNL](https://hpc-tutorials.llnl.gov/posix/)
- `man pthread_create`, `man pthread_mutex_lock`, `man gettimeofday`
- [Valgrind Helgrind / DRD](https://valgrind.org/docs/manual/hg-manual.html) and
  `-fsanitize=thread` for finding data races

### AI usage

AI (Claude) was used to write this README from the subject. The program itself (design, threading logic, synchronisation) was
written without AI.
