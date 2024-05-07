#include "Algorithms.h"

#include "../data_structures/MutablePriorityQueue.h"
/*
std::vector<Vertex*> NearestNeighbour(Graph * graph , Vertex * startVertex) {
    int numVertices = graph->getNumVertex();

    // Start with any vertex as the initial vertex
    Vertex* currentVertex = startVertex;

    // Initialize a set to keep track of visited vertices
    std::unordered_set<Vertex * > visited;
    visited.insert(startVertex);

    std::vector<Vertex*> path;
    path.push_back(startVertex);

    // Repeat until all vertices are visited
    for (int i = 0; i < numVertices - 1; ++i) {
        double minDistance = std::numeric_limits<double>::max();
        Vertex* nextVertex = nullptr;

        // Find the nearest unvisited neighbor
        for (Edge* edge : currentVertex->getAdj()) {
            Vertex* neighbor = edge->getDestination();
            if (!visited.count(neighbor) && edge->getWeight() < minDistance) {
                minDistance = edge->getWeight();
                nextVertex = neighbor;
            }
        }

        // Move to the nearest unvisited neighbor
        if (nextVertex != nullptr) {
            visited.insert(nextVertex);
            currentVertex = nextVertex;
            path.push_back(currentVertex);
        }
    }

    // Return to the starting vertex to complete the tour
    std::unordered_set<Edge *> ajdEdjes = currentVertex->getAdj();

    path.push_back(startVertex);

    return path;
}

*/

std::vector<Vertex*> PrimMst(const Graph* graph , Vertex * sourceVertex){

    Clock clock1;

    clock1.start();

    std::vector<Vertex *> path;

    std::unordered_map<int, Vertex *> vertexSet = graph->getVertexSet();

    for(std::pair<int , Vertex *> pair : vertexSet){
        Vertex * v = pair.second;
        v->setDist(INF);
        v->setVisited(false);
    }

    MutablePriorityQueue<Vertex> vertexQueue;

    for(std::pair<int , Vertex *> pair : vertexSet){
        Vertex * v = pair.second;
        vertexQueue.insert(v);
    }

    sourceVertex->setDist(0);

    while (!vertexQueue.empty()){
        Vertex * curr = vertexQueue.extractMin();
        if (curr->isVisited()) continue;
        curr->setVisited(true);
        path.push_back(curr);
        // Update distances and paths to adjacent vertices
        std::unordered_map<int , Edge *> edgeSet = curr->getAdj();
        for(std::pair<int , Edge *> edgePair : edgeSet){
            Edge * e = edgePair.second;

            Vertex * neighbor = e->getDestination();

            if (neighbor->getDist() > e->getWeight() && !neighbor->isVisited()) {
                neighbor->setDist(e->getWeight());
                vertexQueue.decreaseKey(neighbor);
            }
        }
    }

    double time = clock1.elapsed();

    std::cout << "Prim took " << time << "second" << std::endl;
    return path;
}


/* A heurística de "Vizinho Mais Próximo" (N.N - Nearest Neighbor) é
   um algoritmo de busca heurística utilizado para encontrar um caminho aproximadamente mais curto em um grafo, especialmente em problemas de TSP (Traveling Salesman Problem). A ideia básica do algoritmo é começar de um nó inicial e, em cada etapa, selecionar o nó mais próximo que ainda não foi visitado.

    Aqui está uma descrição geral do algoritmo:

    Escolha um nó inicial como o nó atual.
    Enquanto houver nós não visitados:
    a. Encontre o vizinho mais próximo do nó atual que ainda não foi visitado.
    b. Adicione esse vizinho ao caminho.
    c. Marque o vizinho como visitado e faça dele o nó atual.
    Retorne ao nó inicial para completar o ciclo.
 */

/*Triangulolar inequality : the least distance path to reach a vertex j from i is always to reach j directly from i
   rather than go through some vertex k.
   dist(i, j ) <= dist(i,k) + dist(k , j)
   1. Algorithm: select a root vertex
   2. Find a minimum spanning tree
   3. Do preorder walk of T. and return Hamilton Cycle
 *
 *
*/

std::vector<Edge *> TriangularApproximationHeuristic(Graph * graph , Vertex * source , Vertex * dest){

    std::vector<Vertex * > path = PrimMst(graph , source);

    std::vector<Vertex *> preorderWalk;
    std::unordered_set<Vertex *> visited;
    std::stack<Vertex *> stack;
    stack.push(source);

    while (!stack.empty()) {
        Vertex *current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {
            preorderWalk.push_back(current);
            visited.insert(current);
            std::unordered_map<int, Edge *> map = current->getAdj();
            for (std::pair<int , Edge *> pair_ : current->getAdj()) {
                Edge * edge = pair_.second;
                stack.push(edge->getDestination());
            }
        }
    }

    // Step 3: Define a tour that visits all vertices in the graph using the order obtained in step 2
    std::unordered_set<Vertex *> visitedNodes;
    std::vector<Edge *> optimalRoute;
    for (size_t i = 0; i < preorderWalk.size(); ++i) {
        Vertex *current = preorderWalk[i];
        Vertex *next = (i + 1 < preorderWalk.size()) ? preorderWalk[i + 1] : preorderWalk[0];

        // Check if the next vertex is already visited
        if (visitedNodes.find(next) == visitedNodes.end()) {
            // Find the edge connecting current and next vertices
            std::unordered_map<int, Edge *> map = current->getAdj();
            for (std::pair<int , Edge *> pair_ : current->getAdj()) {
                Edge * edge = pair_.second;
                if (edge->getDestination() == next) {
                    optimalRoute.push_back(edge);
                    break;
                }
            }
            visitedNodes.insert(current);
        }
    }

    return optimalRoute;
}

