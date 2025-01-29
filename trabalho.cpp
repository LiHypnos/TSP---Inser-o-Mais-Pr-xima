//Autores: Elian Fernando Simões Costa / Clarisse Lacerda Pimentel 

#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

struct xy {
    float x, y;
};

struct Insercao {
    int ponto;  // Ponto a ser inserido
    int posicao;  // Posição no ciclo após a qual o ponto será inserido
    float custo;  // Custo da inserção

    // Comparador para a heap (menor custo tem prioridade)
    bool operator<(const Insercao& other) const {
        return custo > other.custo;
    }
};

// Definições globais
const double PI = 3.141592;
const double RRR = 6378.388;  // Raio médio da Terra em km

// Função auxiliar para converter graus + minutos em radianos
double converterParaRadianos(double coordenada) {
    int deg = round(coordenada);  // Parte inteira (graus)
    double min = coordenada - deg;  // Parte decimal (minutos)
    return PI * (deg + 5.0 * min / 3.0) / 180.0;
}

// Função para calcular a distância entre dois pontos (Euclidiana ou Geográfica)
float calcularDistancia(const xy& ponto1, const xy& ponto2, bool geo) {
    if (geo) {
        // Conversão para radianos
        double lat1 = converterParaRadianos(ponto1.x);
        double lon1 = converterParaRadianos(ponto1.y);
        double lat2 = converterParaRadianos(ponto2.x);
        double lon2 = converterParaRadianos(ponto2.y);

        // Cálculo da distância geodésica
        double q1 = cos(lon1 - lon2);
        double q2 = cos(lat1 - lat2);
        double q3 = cos(lat1 + lat2);
        return (int)(RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
    }

    // Distância Euclidiana padrão
    return sqrt(pow(ponto1.x - ponto2.x, 2) + pow(ponto1.y - ponto2.y, 2));
}

// Função para encontrar os três pontos que formam o maior triângulo
vector<int> encontrarMaiorTriangulo(const vector<xy>& pontos) {
    int n = pontos.size();
    float maiorArea = 0;
    vector<int> triangulo(3);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                float area = abs(
                    pontos[i].x * (pontos[j].y - pontos[k].y) +
                    pontos[j].x * (pontos[k].y - pontos[i].y) +
                    pontos[k].x * (pontos[i].y - pontos[j].y)) / 2.0;

                if (area > maiorArea) {
                    maiorArea = area;
                    triangulo = {i, j, k};
                }
            }
        }
    }
    return triangulo;
}

// Função para resolver o TSP usando a heurística de inserção dinâmica otimizada
vector<int> tspInsercaoBarata(const vector<xy>& pontos, vector<vector<float>>& dist, vector<int>& ids, float& maiorAresta) {
    int n = pontos.size();
    vector<bool> visitado(n, false);

    // Passo 1: Encontra o maior triângulo para iniciar o ciclo
    vector<int> ciclo = encontrarMaiorTriangulo(pontos);
    for (int i : ciclo) visitado[i] = true;

    // Passo 2: Configurar a heap para gerenciar as inserções
    priority_queue<Insercao> heap;
    for (int i = 0; i < n; i++) {
        if (visitado[i]) continue;

        for (size_t j = 0; j < ciclo.size(); j++) {
            size_t prox = (j + 1) % ciclo.size();
            float custo = dist[ciclo[j]][i] + dist[i][ciclo[prox]] - dist[ciclo[j]][ciclo[prox]];
            heap.push({i, (int)j, custo});
        }
    }

    // Passo 3: Inserir os pontos restantes dinamicamente
    while ((int)ciclo.size() < n) {
        Insercao melhor = heap.top();
        heap.pop();

        // Verifica se o ponto já foi visitado
        if (visitado[melhor.ponto]) continue;

        // Insere o ponto no ciclo
        ciclo.insert(ciclo.begin() + melhor.posicao + 1, melhor.ponto);
        visitado[melhor.ponto] = true;

        // Atualiza a maior aresta
        maiorAresta = max(maiorAresta, melhor.custo);

        // Atualiza a heap apenas para os pontos ainda não visitados
        for (int i = 0; i < n; i++) {
            if (visitado[i]) continue;

            size_t prox = (melhor.posicao + 2) % ciclo.size();
            float custo = dist[ciclo[melhor.posicao]][i] + dist[i][ciclo[prox]] - dist[ciclo[melhor.posicao]][ciclo[prox]];
            heap.push({i, melhor.posicao, custo});
        }
    }

    // Converte o ciclo para vetor usando os IDs originais
    vector<int> caminho;
    for (int indice : ciclo) caminho.push_back(ids[indice]);

    // Calcula a aresta final entre o último e o primeiro vértice do ciclo
    float custoUltimaAresta = dist[ciclo.back()][ciclo.front()];
    maiorAresta = max(maiorAresta, custoUltimaAresta);
    // Fecha o ciclo corretamente
    caminho.push_back(caminho[0]);

    return caminho;

}

int main() {
    string lixo;
    vector<xy> pontos;
    vector<int> ids;
    int id;
    float x, y;
    bool geo = false;
    while (getline(cin, lixo)) {
        if (lixo == "EDGE_WEIGHT_TYPE: GEO") {
            geo = true;  // Encontra o tipo de peso de aresta GEO
        }
        if (lixo == "NODE_COORD_SECTION") {
            break;  // Encontra a seção de coordenadas e para
        }
    }
    while (cin >> id >> x >> y) {
        ids.push_back(id);
        pontos.push_back({x, y});
    }
    clock_t inicio = clock(); // começa a calcular o tempo de processamento
    int n = pontos.size();
    if (n < 3) {
        cout << "É necessário pelo menos 3 pontos!" << endl;
        return 1;
    }

    // Calcula a matriz de distâncias
    vector<vector<float>> dist(n, vector<float>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                dist[i][j] = calcularDistancia(pontos[i], pontos[j], geo);
            }
        }
    }

    // Executa o algoritmo TSP de inserção dinâmica
    float maiorAresta = 0;
    vector<int> caminho = tspInsercaoBarata(pontos, dist, ids, maiorAresta);
    clock_t fim = clock(); // termina o tempo de processamento
    
   //Grava o caminho no arquivo "saida.txt"
    ofstream saida("saida.txt");
    for (size_t i = 0; i < caminho.size(); i++) {
        saida << "v_" << caminho[i];
        if (i < caminho.size() - 1) {
            saida << " ";
        }
    }
     double duracao = double(fim - inicio) / CLOCKS_PER_SEC;
    // Exibe a maior aresta
    cout << "Maior aresta do ciclo: " << (int) maiorAresta << " / tempo: "<< duracao<< " s" << endl;

    return 0;
}
