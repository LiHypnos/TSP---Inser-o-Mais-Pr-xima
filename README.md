### README.md

```markdown
# TSP Heurística de Inserção Barata - Dinâmica

Este projeto implementa uma solução heurística para o **Problema do Caixeiro Viajante (TSP - Traveling Salesperson Problem)** utilizando o método de **inserção dinâmica barata**. A solução é otimizada para encontrar um ciclo que minimiza as distâncias entre pontos no plano, representados por coordenadas cartesianas (x, y).

---

## Autores

- **Elian Fernando Simões Costa**  
- **Clarisse Lacerda Pimentel**

---

## Objetivo

Resolver o TSP a partir de uma entrada no formato **TSP** (com cabeçalho e coordenadas dos pontos) e encontrar uma rota que passe por todos os pontos com custo aproximado mínimo, utilizando a heurística de inserção dinâmica.

Além disso, a implementação mede o tempo de processamento e salva a rota gerada em um arquivo de saída.

---

## Características

1. **Leitura do Formato TSP**:  
   O programa aceita um arquivo com cabeçalho no formato:
   ```txt
   TYPE : TSP
   DIMENSION : <número de pontos>
   EDGE_WEIGHT_TYPE : EUC_2D
   NODE_COORD_SECTION
   <id> <x> <y>
   <id> <x> <y>
   ...
   EOF
   ```

2. **Etapas da Heurística**:
   - Seleção inicial de três pontos formando o maior triângulo.
   - Inserção dinâmica dos pontos restantes no ciclo, utilizando uma fila de prioridade para escolher a inserção mais barata.

3. **Saída**:
   - Exibe no terminal a maior aresta do ciclo e o tempo de processamento.
   - Gera o arquivo `saida.txt` com o ciclo no formato:
     ```txt
     v_1 v_2 v_3 ... v_1
     ```

---

## Funcionalidades

- **Cálculo da Distância**:  
  Utiliza a fórmula da distância euclidiana:
  \[
  d = \sqrt{(x_2 - x_1)^2 + (y_2 - y_1)^2}
  \]

- **Seleção do Maior Triângulo**:  
  Escolhe três pontos que formam o maior triângulo com base na área calculada.

- **Inserção Dinâmica**:  
  Insere pontos no ciclo com base no menor aumento de custo.

- **Medida de Desempenho**:  
  Calcula o tempo total de execução do algoritmo.

---

## Como Compilar

Certifique-se de ter um compilador C++ (como o `g++`) instalado. Para compilar o programa, execute o comando:

```bash
g++ -o tsp_insercao dinamica_tsp.cpp
```

---

## Como Executar

Após compilar, você pode executar o programa redirecionando um arquivo de entrada no formato **TSP**:

```bash
./tsp_insercao < entrada.tsp
```

Ou digitando os dados manualmente no terminal, no formato especificado.

### Exemplo de Entrada:

```txt
TYPE : TSP
DIMENSION : 5
EDGE_WEIGHT_TYPE : EUC_2D
NODE_COORD_SECTION
1 0.0 0.0
2 100.0 0.0
3 100.0 100.0
4 0.0 100.0
5 50.0 50.0
EOF
```

### Exemplo de Saída (Terminal):

```txt
Maior aresta do ciclo: 100 / tempo: 0.0023 s
```

### Arquivo de Saída (`saida.txt`):

```txt
v_1 v_2 v_5 v_3 v_4 v_1
```

---

## Estrutura do Código

### Principais Estruturas e Funções:

1. **Struct `xy`**:
   Representa os pontos com coordenadas (x, y).

2. **Struct `Insercao`**:
   Representa as opções de inserção na fila de prioridade:
   - Ponto a ser inserido.
   - Posição no ciclo após a qual o ponto será inserido.
   - Custo da inserção.

3. **Função `calcularDistancia`**:
   Calcula a distância euclidiana entre dois pontos.

4. **Função `encontrarMaiorTriangulo`**:
   Identifica três pontos que formam o maior triângulo.

5. **Função `tspInsercaoBarata`**:
   Resolve o TSP utilizando a heurística de inserção dinâmica.

6. **Função `main`**:
   - Lê os dados de entrada.
   - Calcula a matriz de distâncias.
   - Executa a heurística para gerar o ciclo.
   - Salva os resultados no arquivo e exibe métricas no terminal.

---

## Requisitos

- **Compilador C++** (compatível com C++11 ou superior).
- **Arquivo de entrada** no formato TSP.

---

## Desempenho

O tempo de execução depende do número de pontos. A solução foi projetada para ser eficiente com a abordagem de:
- Seleção inicial de três pontos.
- Inserção dinâmica usando fila de prioridade.


- Atualmente o código tem uma complexidade de O(n^3) já que:
Cálculo da Matriz de Distâncias: O(n^2).
Seleção do Maior Triângulo: O(n^3).
Inserção Dinâmica: O(n^2log⁡n).


## fontes: 

Livro: "Introduction to Algorithms" (Cormen, Leiserson, Rivest, Stein)
    Este é um livro padrão de algoritmos que cobre análise de complexidade de várias técnicas, incluindo heurísticas e métodos aproximados.
    Capítulo sobre análise de algoritmos e problemas NP-completos seria especialmente relevante.

Artigo: "Heuristics for the Traveling Salesman Problem" (Lawler et al., 1985)
    Este artigo discute diferentes heurísticas para resolver o TSP, incluindo métodos de inserção como o seu. Ele fornece insights sobre complexidade e desempenho dessas abordagens.

Fonte online: GeeksforGeeks
    O site contém explicações claras sobre algoritmos relacionados ao TSP e análise de complexidade.
    Exemplo relevante: TSP Using Nearest Neighbor and Dynamic Programming.
