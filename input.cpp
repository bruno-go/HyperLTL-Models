#include <iostream>
#include <regex>
#include <unordered_map>
#include "input.h"

enum class state {XML_HEAD, GRAPH_ML, GRAPH, NODES, NODE_TAG_OPEN, EDGES_OR_END, EDGE_TAG_OPEN};

void trim(std::string& str){
	constexpr char whitespace[] = " \n\t\r\v\f";
	const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos){
        str = "";
		return;
	}

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    str = str.substr(strBegin, strRange);
}
void from_graphml(std::istream& in, Graph &G) {
	//std::ios::sync_with_stdio(false); better performance but annoying for debugging at times
	string line;
	
	std::regex header( R"(<\?xml[^>]*>\s*<graphml[^>]*>)");
	std::regex graph_tag( R"(<graph edgedefault=\"undirected\">)");
	std::regex node_start( R"(<node id=\"([^"]+)\">)");
	std::regex node_end( R"(<\/node>)");

	std::regex edge_start( R"(<edge source=\"([^"]+)\" target=\"([^"]+)\">)");
	std::regex edge_end( R"(<\/edge>)");

	state current = state::XML_HEAD;

	std::unordered_map<std::string, size_t> node_id_to_idx;

	// parse the input
	while (getline(in, line)) {
		match:
		trim(line);
		std::smatch m;
		switch (current){
		
		case state::XML_HEAD:{
			if(std::regex_search(line, m, header)){
				line = m.suffix();
				current = state::GRAPH;
				if(line.length() > 0)
					goto match;
			}
			break;
		}
		case state::GRAPH:{
			if(std::regex_search(line, m, graph_tag)){
				line = m.suffix();
				current = state::NODES;
				if(line.length() > 0)
					goto match;
			}
			break;
		}
		case state::NODES:{
			if(std::regex_search(line, m, node_start)){
				//add check that node with id does not already exist
				auto node_key = m[1].str();
				size_t idx = node_id_to_idx.size();
				node_id_to_idx[node_key] = idx;
				Vertex* v = new Vertex(idx, node_key);
				G.V.push_back(v);


				line = m.suffix();
				current = state::NODE_TAG_OPEN;
				if(line.length() > 0)
					goto match;
			}
			if(std::regex_search(line, m, edge_start)){
				//add check that edge with id does not already exist
				auto source_id  = m[1].str();
				auto target_id  = m[2].str();

				size_t source_idx = node_id_to_idx[source_id];
				size_t target_idx = node_id_to_idx[target_id];

				Vertex* v = G.V[source_idx];
				Vertex* u = G.V[target_idx];

				if(!u->adjacent_to(v)){
					G.add_edge(v, u);
				}
				else{
					//ignore for now
				}
				
				line = m.suffix();
				current = state::EDGE_TAG_OPEN;
				if(line.length() > 0)
					goto match;
			}
			break;
		}
		case state::NODE_TAG_OPEN:{
			if(std::regex_search(line, m, node_end)){
				line = m.suffix();
				current = state::NODES;
				if(line.length() > 0)
					goto match;
			}
			break;
		}

		case state::EDGES_OR_END:{
			if(std::regex_search(line, m, edge_start)){

				//add check that edge with id does not already exist
				auto source_id  = m[1].str();
				auto target_id  = m[2].str();

				size_t source_idx = node_id_to_idx[source_id];
				size_t target_idx = node_id_to_idx[target_id];

				Vertex* v = G.V[source_idx];
				Vertex* u = G.V[target_idx];
				
				if(!u->adjacent_to(v)){
					G.add_edge(v, u);
				}
				else{
					// ignore for now
				}

				line = m.suffix();
				current = state::EDGE_TAG_OPEN;
				if(line.length() > 0)
					goto match;
			}
			break;
		}

		case state::EDGE_TAG_OPEN:{
			if(std::regex_search(line, m, edge_end)){
				line = m.suffix();
				current = state::EDGES_OR_END;
				if(line.length() > 0)
					goto match;
			}
			break;
		}

		default:
			break;
		}
		
	}

	if(current != state::EDGES_OR_END){
		std::cerr << "[error] could not parse graphml file" << std::endl;
		exit(1);
	}
}

void get_input_graph(std::string& in_path, Graph& G){
	 if(in_path.empty()){
        from_graphml(std::cin, G);
    }else{
        std::ifstream fp;
        fp.open(in_path);
        if(!fp){
            std::cerr << "invalid input file path" << std::endl;
            exit(1);
        }
        from_graphml(fp, G);
    }
}
