#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <variant>
#include <random>
#include <fstream>

using namespace std;

class Place;
class Transition;
class Arc;
class Petri_Net;

class Random_Gen{
public:
	std::mt19937_64 gen;
	uint_fast64_t seed;
	int min = 1;
	int max = 5;
	std::uniform_int_distribution<int> dis;

	Random_Gen(uint_fast64_t _seed, int _min, int _max){
		min = _min;
		max = _max;
		if(_seed == (uint_fast64_t) -1){
			random_device rd;
			seed = rd();
		}
		else{
			seed = _seed;
		}
		gen.seed(seed);

		dis = std::uniform_int_distribution<int>(min, max);
	}

	Random_Gen() : 
		Random_Gen(-1, 1, 5) {}

	int sample(){
		return dis(gen);
	}
	
};

class Petri_Net {
public:
	std::vector<Place *> P;
	std::vector<Transition *> T;

	Random_Gen rng;

	void add_place_from_vertex(Vertex* v);

    void add_arc(Place* p, Transition* t, int weight = 1);
    void add_arc(Transition* t, Place* p, int weight = 1);
};


class Place {
public:
	enum class Type {DEFAULT, GRAPH, ADDED, CONGESTION};

	size_t id;
	string name;
	int marking = 0;
	Type   type = Type::DEFAULT;

	vector<Arc> out_arcs;
    vector<Arc>  in_arcs;

	Place(size_t _id, string _name)
	: id(_id), name(_name) {};

	void print_tpn(std::ostream& out);
};

class Transition {
public:
	size_t id;
	string name;


	vector<Arc> out_arcs; // self = src
    vector<Arc>  in_arcs; // self = dst

	Transition(size_t _id, string _name)
	: id(_id), name(_name) {};

	void print_tpn(std::ostream& out);
};

class Arc{
public:
    std::variant<Place*, Transition*> src;
    std::variant<Place*, Transition*> dst;

	int weight = 1;

    Arc(std::variant<Place*, Transition*> _src, std::variant<Place*, Transition*> _dst, int _weight)
    : src(_src), dst(_dst), weight(_weight) {};

	void print_tpn(std::ostream& out, std::string label);

	std::string get_src_out();
	std::string get_dst_out();
};

Petri_Net net_from_graph(Graph& G);

void output_as_tpn(std::string& out_path, Petri_Net& PN);


