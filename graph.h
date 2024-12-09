#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <list>

using namespace std;

class Vertex;
class Edge;
class Graph;

typedef struct{
        Vertex* v = nullptr;
        size_t idx = 0;
} Endpoint;

class Graph {
public:
	std::vector<Vertex *> V;
	std::vector<Edge *> E;

	map<string, Vertex *> name_map;

	Edge* add_edge(Vertex* u, Vertex* v);

};

class Vertex {
public:
	size_t id;
	string name;

	// neighbor is an array of neighbours of this vertex
	// edges is an array of edge objects of the same length as edges to enable faster deletion
	vector<Vertex *> neighbors;
	vector<Edge *> edges;

	bool adjacent_to(Vertex* u);
	

	Vertex(size_t _id, string _name)
	: id(_id), name(_name) {};
};

class Edge {
public:
	size_t id;
	// two endpoint vertices and position of the vertices in each others neighbor array
	Endpoint end[2];

	int weight = 1;

	Edge(size_t _id)
	: id(_id) {};
};

inline size_t deg(const Vertex* v){
    return v->neighbors.size();
}
