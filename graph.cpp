#include "graph.h"

Edge* Graph::add_edge(Vertex* u, Vertex* v){
		assert(u != nullptr && v != nullptr);
		assert(!u->adjacent_to(v));
		
		Edge *e = new Edge(E.size());

		e->end[0] = {u, static_cast<size_t>(deg(u))};
		e->end[1] = {v, static_cast<size_t>(deg(v))};

		E.push_back(e);

		u->neighbors.push_back(v);
		v->neighbors.push_back(u);

		u->edges.push_back(e);
		v->edges.push_back(e);

		return e;
}

bool Vertex::adjacent_to(Vertex* u){
		Vertex* v = this;
		// v has smaller neighborhood                                               
		if(deg(v) > deg(u))
			swap(v, u);

		for(Vertex* n: v->neighbors){
			if(n == u)
				return true;
		}
		return false;
	}