# N-Queen Problem Solver: Algorithm Comparison

This project implements and compares three distinct search and optimization algorithms to solve the classic **N-Queen Problem**: placing $N$ non-attacking queens on an $N \times N$ chessboard.

The three algorithms compared are **Iterative Deepening Search (IDS)**, **Hill Climbing (HC)**, and a **Genetic Algorithm (GA)**.

---

## Algorithms Implemented

All three algorithms use a `vector<int>` to represent the board, where the index is the row and the value is the column of the queen. This ensures no two queens are in the same row or column.

### 1. Iterative Deepening Search (IDS)
A complete search strategy that sequentially executes **Depth-Limited Search (DLS)** with increasing depth limits from $0$ to $N-1$.

* **Initial State:** The board is initialized with $N$ queens in a specific pattern (e.g., `{0, 2, 4, ...N/2, 1, 3, 5, ...N-1}`).
* **Search:** DLS expands from the current row, searching for a column that is safe from attack by preceding queens.

### 2. Random-Restart Hill Climbing (HC)
A local search technique that attempts to minimize the number of attacking queen pairs.

* **Strategy:** Starts with a random board and iteratively finds the swap of a pair of numbers (columns) that results in the smallest number of attacks.
* **Restart:** The process repeats until no swap can reduce the number of attacks or a zero-attack configuration is found. It restarts with a new random board up to $100,000$ times or until a solution is found.

### 3. Genetic Algorithm (GA)
A metaheuristic search inspired by the process of natural selection.

* **Population:** Starts with $1000$ random individuals.
* **Selection:** Parents are randomly chosen from the **top $5\%$** of the population (those with the lowest attack count).
* **Crossover:** Uses **Cycle Crossover** to generate two offspring from two parents.
* **Mutation:** Each offspring has a **$0.5$ probability** of mutation, which involves randomly swapping two columns.
* **Success:** Continues for generations until an individual with zero attacks is found.

---

## Execution

To run the program, use the following command:

```bash
cd "[file path]" && g++ Nqueen.cpp -std=c++11 -o main && "[file path]/main"