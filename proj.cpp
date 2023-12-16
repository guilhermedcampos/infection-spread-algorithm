/*

O Professor João Caracol está a fazer um estudo para a taskforce do Governo
responsável pelo estudo de doenças transmissíveis em Portugal. A taskforce está
particularmente interessada no tópico da transmissão de doenças entre a
população portuguesa, de forma a estudar os melhores mecanismos de intervenção
para conter a propagação de doenças. Para isso, o Professor João Caracol teve
acesso aos dados da rede social TugaNet, que acredita ser representativa das
reais interacções sociais entre os indivíduos da população portuguesa. Assim, de
forma a estudar o pior caso de propagação de uma dada infecção em Portugal, o
Pro- fessor João Caracol quer perceber qual o maior número de saltos que uma
dada doença pode fazer. No entanto, tendo em conta a densidade das cidades
portuguesas, o Professor João Cara- col decidiu considerar um pressuposto
simplificador: indivíduos que se conhecem mutuamente de forma directa ou
indirecta, ficam infectados instantaneamente.
*/

/*
Input
O ficheiro de entrada contém a informação sobre a rede TugaNet, que é definida
como um grafo dirigido de relações entre dois indivíduos, da seguinte forma: •
Uma linha contendo dois inteiros: o número n de indivíduos (n ≥ 2), e o número
de relações m a indicar (m ≥ 0); • Uma lista em que cada linha i contém dois
inteiros x e y, representando que o indivíduo x conhece o indivíduo y. Quaisquer
inteiros numa linha estão separados por exactamente um espaço em branco, não
contendo qualquer outro carácter, a não ser o fim de linha. Assuma que os grafos
de input são dirigidos (potencialmente) cíclicos.
*/

/*
Output
O programa deverá escrever no output um inteiro s correspondendo ao número
máximo de saltos que uma doença pode fazer na rede TugaNet.
*/

/*

Exemplo 1 Input
7 8
3 4
3 2
4 6
4 5
6 2
6 5
5 7
2 7
Output
4
Exemplo 2 Input
8 9
2 5
3 2
3 7
4 6
5 7
6 5
7 8
8 1
8 5
Output
3
*/

/* The objective is to make the code as fast as possible, for that organize the
 * connections in a matrix, use scc to remove cicles and count them as a single
 * person and use dfs to find the max depth. the dfs has to be iterative,
 * nothing can be recursive*/

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

std::vector<std::vector<int>> createMatrix(int n, int m) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    int x, y;
    for (int i = 0; i < m; i++) {
        std::cin >> x >> y;
        matrix[x - 1][y - 1] = 1;
    }
    return matrix;
}

std::vector<std::vector<int>>
transposeMatrix(const std::vector<std::vector<int>> &matrix) {
    std::vector<std::vector<int>> transposed(
        matrix.size(), std::vector<int>(matrix.size(), 0));
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            transposed[i][j] = matrix[j][i];
        }
    }
    return transposed;
}

int dfs(const std::vector<std::vector<int>> &matrix, int start,
        std::vector<int> &visited) {
    std::stack<int> stack;
    stack.push(start);
    int max = 0;
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        if (visited[current] == 0) {
            visited[current] = 1;
            for (int i = 0; i < matrix.size(); i++) {
                if (matrix[current][i] == 1) {
                    stack.push(i);
                }
            }
            max++;
        }
    }
    return max;
}

void dfsTranspose(const std::vector<std::vector<int>> &transposed, int start,
                  std::vector<int> &visited, std::stack<int> &stack) {
    visited[start] = 1;
    for (int i = 0; i < transposed.size(); i++) {
        if (transposed[start][i] == 1 && visited[i] == 0) {
            dfsTranspose(transposed, i, visited, stack);
        }
    }
    stack.push(start);
}

int scc(const std::vector<std::vector<int>> &matrix) {
    int n = matrix.size();
    std::vector<int> visited(n, 0);
    std::stack<int> stack;

    for (int i = 0; i < n; i++) {
        if (visited[i] == 0) {
            dfsTranspose(transposeMatrix(matrix), i, visited, stack);
        }
    }

    std::fill(visited.begin(), visited.end(), 0);
    int sccCount = 0;

    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        if (visited[current] == 0) {
            dfs(matrix, current, visited);
            sccCount++;
        }
    }

    return sccCount;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> matrix = createMatrix(n, m);

    int sccCount = scc(matrix);
    std::cout << sccCount << std::endl;

    return 0;
}
