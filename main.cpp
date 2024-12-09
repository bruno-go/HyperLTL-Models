#include "input.h"
#include "graph.h"
#include "petri_net.h"
#include "ltl.h"
#include <fstream>
#include <iostream>


int main(int argc, char* argv[]){

    read_config(argc, argv);
    Graph G;
    
    get_input_graph(cfg.in_path, G);

    Petri_Net PN = net_from_graph(G);
    
    PN.rng = Random_Gen(cfg.seed, cfg.min, cfg.max);

    switch(cfg.mode){
        case Config::Mode::LATENCY:
            latency_model(PN);
            break;
        case Config::Mode::CONGESTION:{
            congestion_model(PN);
            if(!cfg.out_path.empty()){
                std::ofstream fp;
                fp.open(cfg.out_path + "_query.xml");
                congestion_query(cfg.n_traces, cfg.con_bound, fp, PN);
            }else{
                std::cerr << "[error] query generation requires specifying output path" << std::endl;
            }
            }
            break;
        default:
            break;
    }
    output_as_tpn(cfg.out_path, PN);
    exit(0);
}