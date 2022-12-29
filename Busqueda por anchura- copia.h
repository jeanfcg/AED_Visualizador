#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <limits>
#include <algorithm>

#define INF std::numeric_limits<int>::max()
#define NONE -1

template<class T> struct Vertex;
template<class T> struct Edge;
template<class T> class Graph;

template<class T> std::ostream& operator<< (std::ostream& os, Vertex<T>& vertex);
template<class T> std::ostream& operator<< (std::ostream& os, Edge<T>& edge);
template<class T> std::ostream& operator<< (std::ostream& os, Graph<T>& graph);

template<class T>
struct Vertex {
    T tag;          // indice del vertice
    int meta;       // metadata: peso, distancia, acumulado, opcion, etc

    // constructor
    Vertex(T tag, int meta = 0) {
        this->tag = tag;
        this->meta = meta;
    }

    // operadores
    bool operator== (const Vertex<T>& other) { // el vertice es igual si sus tags son iguales
        return tag == other.tag;
    }

    friend std::ostream& operator<<<T> (std::ostream& os, Vertex& vertex); // salida estandar
};

template<class T>
std::ostream& operator<< (std::ostream& os, Vertex<T>& vertex) {
    os << vertex.tag;
    return os;
}

template<class T>
struct Edge {
    T start;      // indice del vertice inicial
    T end;        // indice del vertice final
    int meta;       // metadata: peso, distancia, etc
    bool dir;       // true: es dirigido, false: no es dirigido

    // constructor
    Edge(T start, T end, int meta = 0, bool dir = false) {
        this->start = start;
        this->end = end;
        this->meta = meta;
        this->dir = dir;
    }

    Edge(Vertex<T> start, Vertex<T> end, int meta = 0, bool dir = false) {
        this->start = start.tag;
        this->end = end.tag;
        this->meta = meta;
        this->dir = dir;
    }

    bool operator< (const Edge<T>& other) const {
        return meta > other.meta;
    }
/*
    friend bool operator< (const Edge<int>& first, const Edge<int>& second) {
    // la prioridad es mayor con el menor peso
        return (first.meta > second.meta);
    }
*/
    friend std::ostream& operator<<<T> (std::ostream& os, Edge<T>& edge); // salida estandar
};
/*
bool operator< ( Edge<int>& first, Edge<int>& second) { // para cola de prioridades
    // la prioridad es mayor con el menor peso
    return first.operator<(second);
}
*/
template<class T>
std::ostream& operator<< (std::ostream& os, Edge<T>& edge) {
    if (edge.dir)
        os << edge.start << " -> " << edge.end;
    else
        os << edge.start << " -- " << edge.end;
    return os;
}

template<class T>
class Graph {
private:
    std::vector<Vertex<T>> vertices;  // vertices
    std::vector<Edge<int>> edges;       // lados
    std::vector<std::list<Vertex<int>>> adjacents; // cache, lista de lados adyacentes

public:
    // constructor, grafo vacio
    Graph() {}

    // obtener indice
    int index (Vertex<T> vtx) {
        int idx;
        for (idx = 0; idx < vertices.size(); idx++)
            if (vertices[idx] == vtx)
                return idx;
        return NONE; // vertex no found
    }

    // obtener vertice
    Vertex<T> deindex(int idx) {
        return vertices[idx];
    }

    // esta el grafo vacio?
    bool empty() {
        return vertices.empty();
    }

    // insertar vertice al grafo
    void insert (Vertex<T> vertex) {
        if (index(vertex.tag) != NONE) {
            std::cout << "GRAPH ERROR: duplicated vertex";
            exit(1);
        }

        vertices.push_back(vertex);

        if (!adjacents.empty()) // actualiza cache de adyacentes
            adjacents.push_back(std::list<Vertex<int>>());
    }

    // insertar lado al grafo
    void insert (Edge<T> edge) {
        int start = index(edge.start);
        int end = index(edge.end);

        if (start == NONE || end == NONE) {
            std::cout << "ERROR: Graph, vertex no found";
            exit(1);
        }

        edges.push_back(Edge<int>(start, end, edge.meta, edge.dir));

        adjacents.clear(); // libera, cache no valida
    }

    // obtener lados adyacentes, se podan lazos, se toma en cuenta los lados con direccion
    std::list<Vertex<int>> get_adjacents (int vertex) {
        if (vertex < 0 || vertex >= vertices.size()) {
            std::cout << "ERROR: Graph, vertex no found";
            exit(1);
        }

        // devuelve de la cache
        if (!adjacents.empty())
            return adjacents[vertex];

        // crea las listas vacias en la cache
        for (int i = 0; i != vertices.size(); i++)
            adjacents.push_back(std::list<Vertex<int>>());

        // analizamos todos los lados y llenamos cache
        for (auto e = edges.begin(); e != edges.end(); e++) {
            if (e->start != e->end) {
                adjacents[e->start].push_back(Vertex<int>(e->end, e->meta));
                if (!(e->dir))
                    adjacents[e->end].push_back(Vertex<int>(e->start, e->meta));
            }
        }

        return adjacents[vertex];
    }

    // obtener matriz de adyacencias, se podan lados repetidos mayores
    std::vector<std::vector<int>> get_matrix() {
        // matriz vacia
        std::vector<std::vector<int>> matrix;
        // matriz nxn puesto a INF, n = numero de vertices
        for (int i = 0; i < vertices.size(); i++)
            matrix.push_back(std::vector<int>(vertices.size(), INF));
        // usamos la cache de adyacentes porque ya esta podado los lazos y direcciones
        for (int i = 0; i < vertices.size(); i++) {
            std::list<Vertex<int>> adjs = get_adjacents(i);
            for (auto v = adjs.begin(); v != adjs.end(); v++) {
                int j = v->tag;
                if (v->meta < matrix[i][j])
                    matrix[i][j] = v->meta;
            matrix[i][i] = 0;
            }
        }
        return matrix;
    }

    // algoritmo de busquedad por profundidad
    Graph<T> alg_DFS(int initial) {
        if (initial < 0 || initial >= vertices.size()) {
            std::cout << "ERROR: Graph, vertex no found";
            exit(1);
        }

        std::vector<int> costs(vertices.size(), INF);       // costos
        std::vector<bool> ready(vertices.size(), false);    // vertices procesados
        std::vector<int> starts(vertices.size());           // nodos iniciales, nodos padres
        std::stack<Edge<int>> stack;                        // pila vacia

        costs[initial] = -1;
        stack.push(Edge<int>(initial, initial));
        for(int i = 0; i < vertices.size(); i++)
            starts[i] = i;

        while (!stack.empty()) {
            Edge<int> edge = stack.top();
            stack.pop();
            if (!ready[edge.end]) {
                ready[edge.end] = true;
                costs[edge.end] = costs[edge.start] + 1;
                starts[edge.end] = edge.start;
                std::list<Vertex<int>> adjs = get_adjacents(edge.end);
                for (auto e = adjs.begin(); e != adjs.end(); e++)
                    stack.push(Edge<int>(edge.end, e->tag));
            }
        }

        Graph<T> tree;
        tree.vertices = vertices;
        for (int i = 0; i < vertices.size(); i++) {
            tree.vertices[i].meta = costs[i];
            if (starts[i] != i)
                tree.edges.push_back(Edge<int>(starts[i], i, 1, true));
        }

        return tree;
    }

    // algoritmo de busquedad por anchura
    Graph<T> alg_BFS(int initial) {
        if (initial < 0 || initial >= vertices.size()) {
            std::cout << "ERROR: Graph, vertex no found";
            exit(1);
        }

        std::vector<int> costs(vertices.size(), INF);       // costos
        std::vector<bool> ready(vertices.size(), false);    // vertices procesados
        std::vector<int> starts(vertices.size());           // nodos iniciales, nodos padres
        std::queue<Edge<int>> queue;                        // cola vacia

        costs[initial] = -1;
        queue.push(Edge<int>(initial, initial));
        for(int i = 0; i < vertices.size(); i++)
            starts[i] = i;

        while (!queue.empty()) {
            Edge<int> edge = queue.front();
            queue.pop();
            if (!ready[edge.end]) {
                ready[edge.end] = true;
                costs[edge.end] = costs[edge.start] + 1;
                starts[edge.end] = edge.start;
                std::list<Vertex<int>> adjs = get_adjacents(edge.end);
                for (auto e = adjs.begin(); e != adjs.end(); e++)
                    queue.push(Edge<int>(edge.end, e->tag));
            }
        }

        Graph<T> tree;
        tree.vertices = vertices;
        for (int i = 0; i < vertices.size(); i++) {
            tree.vertices[i].meta = costs[i];
            if (starts[i] != i)
                tree.edges.push_back(Edge<int>(starts[i], i, 1, true));
        }

        return tree;
    }

    // algoritmo de Prim
    Graph<T> alg_Prim(int initial) {
        if (initial < 0 || initial >= vertices.size()) {
            std::cout << "ERROR: Graph, vertex no found";
            exit(1);
        }

        std::vector<int> costs(vertices.size(), INF);       // costos
        std::vector<bool> ready(vertices.size(), false);    // vertices procesados
        std::vector<int> starts(vertices.size());      // nodos iniciales, nodos padres
        std::priority_queue<Edge<int>> queue;                             // cola vacia


        costs[initial] = 0;
        queue.push(Edge<int>(initial, initial));
        for(int i = 0; i < vertices.size(); i++)
            starts[i] = i;

        while (!queue.empty()) {
            Edge<int> edge = queue.top();
            queue.pop();
            if (!ready[edge.end]) {
                ready[edge.end] = true;
                costs[edge.end] = edge.meta;
                starts[edge.end] = edge.start;
                std::list<Vertex<int>> adjs = get_adjacents(edge.end);
                for (auto e = adjs.begin(); e != adjs.end(); e++)
                    queue.push(Edge<int>(edge.end, e->tag, edge.meta + e->meta));
            }
        }

        Graph<T> tree;
        tree.vertices = vertices;
        for (int i = 0; i < vertices.size(); i++) {
            tree.vertices[i].meta = costs[i];
            if (starts[i] != i)
                tree.edges.push_back(Edge<int>(starts[i], i, 1, true));
        }

        return tree;
    }
/*
    // algoritmo de Kruskal
    std::vector<Edge> alg_Kruskall() {

        // cola de prioridad vacia
        std::priority_queue<Edge> queue;

        // agregamos todos los lados a la cola, descartamos los lados dirigidos porque no cumplen con grafo conexo entre todos los vertices
        for (auto e = edges.begin(); e!= edges.end; e++)
            if (!e->dir)
                queue.push(*e);

        // contador de lados, ce sera igual a (nvertices - 1) como maximo
        int ce = 0;

        // contenedor para los subgrafos disyuntos
        std::list<std::list<Edge>> subgraphs;

        while (ce < vertices.size() && !queue.empty()) {
            std::Edg


        }





    }
*/
    // algoritmo de Floyd Warshall
    std::vector<std::vector<int>> alg_Floyd_Warshall() { //Marshall

        std::vector<std::vector<int>> matrix = get_matrix();

        int cn = vertices.size();

        for(int k = 0; k < cn; k++)
            for(int i = 0; i < cn; i++)
                for(int j = 0; j < cn; j++){
                    int dt;
                    // evitar desbordamiento con INF
                    if (matrix[i][k] == INF || matrix[k][j] == INF)
                        dt = INF;
                    else
                        dt = matrix[i][k] + matrix[k][j];
                    if(matrix[i][j] > dt)
                        matrix[i][j] = dt;
                }

        return matrix;
    }

    // crear archivo Dot para matrices
    void toDot(std::string f, std::vector<std::vector<int>> matrix) {
        std::fstream file;
        file.open(f, std::ios_base::out);
        if (file.is_open()) {
            file << "digraph {" << std::endl;
            file << "\tbgcolor = transparent;" << std::endl;
            int n = matrix.size();
            file << "\tNode [shape=none; label=<" << std::endl;
            file << "\t\t<TABLE>" << std::endl;
            for (int i = 0; i < n; i++) {
                file << "\t\t\t<TR>" << std::endl;
                for (int j = 0; j < n; j++) {
                    std::string value = matrix[i][j] == INF ? "INF" : std::to_string(matrix[i][j]);
                    file << "\t\t\t\t<TD> " << value << " </TD>" << std::endl;
                }
                file << "\t\t\t</TR>" << std::endl;
            }

            file << "\t\t</TABLE>>]" << std::endl;
            file << "}" << std::endl;
        }
    }

    // crear archivo dot
    void toDot(std::string f, Graph<T> tree = Graph<T>()) {
        std::fstream file;
        file.open(f, std::ios_base::out);
        if (file.is_open()) {
            file << "digraph {" << std::endl;
            file << "\tbgcolor = transparent;" << std::endl;
            // vertex
            for (int i = 0; i < vertices.size(); i++) {
                std::string meta;
                if (!tree.empty()) {
                    meta = tree.vertices[i].meta == INF ? "INF" : std::to_string(tree.vertices[i].meta);
                    meta = " : " + meta;
                }
                file << "\tnode" << i << " [label=\"" << vertices[i].tag << meta <<  "\"; style=filled; fillcolor=blue; color=blue; fontcolor=white]" << std::endl;
            }
            // edges
            for (int i = 0; i < edges.size(); i++) {
                std::string dir = edges[i].dir ? "forward" : "none";
                file << "\tnode" << edges[i].start << " -> node" << edges[i].end << " [dir=" << dir << "; label=" << edges[i].meta << "];" << std::endl;
            }
            // tree
            for (int i = 0; i < tree.edges.size(); i++) {
                if (tree.edges[i].start != tree.edges[i].end) {
                    std::string dir = tree.edges[i].dir ? "forward" : "none";
                    file << "\tnode" << tree.edges[i].start << " -> node" << tree.edges[i].end << " [dir=" << dir << "; color=red];" << std::endl;
                }
            }
            file << "}" << std::endl;
        }
    }


    friend std::ostream& operator<<<T> (std::ostream& os, Graph<T>& g);

};


template <class T>
std::ostream& operator<< (std::ostream& os, Graph<T>& graph) {
    os << "Graph {" << std::endl;
    for (auto v = graph.vertices.begin(); v != graph.vertices.end(); v++)
        os << "    " << *v << ";" << std::endl;
    for (auto e = graph.edges.begin(); e != graph.edges.end(); e++){
        os << "    " << *e << ";" << std::endl;
    }
    os << "}";
    return os;
}

#endif // GRAPH_H
