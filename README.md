# Disease Spread Analysis with Tarjan's SCC and DFS

This repository presents a C++ implementation to analise the potential impact of disease spread within social networks,along with a version (`max-connections-chrono.cpp`) that measures the execution time for each input. The goal is to determine the worst-case scenario for disease propagation, considering instant transmission between individuals who share direct or indirect connections. Our solution consists in an adapted iterative Tarjan's algorithm for finding Strongly Connected Components (SCC). This enables the identification of socially interconnected groups. Complemented by a Depth-First Search (DFS), we determine the maximum number of connections within the identified components in the social graph. This approach ultimately leads to a comprehensive understanding of the worst-case disease spread scenario.
For a deeper understanding, our algorithm integrates elements from graph theory, emphasizing the significance of SCC in modeling social structures and DFS for quantifying connections within these structures. 

## Getting Started

Clone the repository to your local machine using the following command in your terminal or command prompt:

```bash
git clone https://github.com/guilhermedcampos/disease-spread-algorithm/
```

### Prerequisites

- C++ compiler (g++)
- Make

### Build and Run

To build the executable for the main algorithm and execute it for each input file in the `testsTime` folder, run the following commands:

```bash
make
```

To clean the generated output files from the testsTime folder, use:

```bash
make clean
```
### Test Data

The repository includes a total of 19 tests located in the testsTime folder and the folder tests/ (includes 2 tests that were given to us) along with their expected output. 
Each test  in testsTime/ includes inputs that increment the number of vertex's of the graph by 500. The number of relations also increases with each test

To generate additional tests, use the following command:

```bash
./gen_supermarble <V> <E> <SubN> <m> <M> <seed> > testfile
```

```bash
circular.sh V > testfile
```

To execute the algorithm on a specific test input, run the command after compiling:

```bash
/max-connections.out < testfile
``` 

### Contents

    - src/max-connections.cpp: Main algorithm implementation in c++ language.
    - src/max-connections-chrono.cpp: Algorithm version that measures execution time.
    - src/gen_supermarble.cpp: Auxiliar tool to generate additional test inputs.
    - makefile: Makefile for building and running (`max-connections-chorno.cpp`) on all the inputs files in testsTime.
    - relatorio.pdf: Document explaining the main problem, summarizing the solution, and drawing conclusions from both practical and theoretical analyses.

### Conclusion

Theoretical analysis suggests that the algorithm has a complexity of O(V*(V+E)) due DFS having a complexity of O(V+E) and then being called on the number of SCC's found, worst case scenario being all vertexes. 
Practical analysis is further detailed in the relatorio.pdf.

