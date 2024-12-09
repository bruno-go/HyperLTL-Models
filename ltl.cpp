#include "ltl.h"
#include <vector>
#include <sstream>
#include "query_strings.h"

void latency_model(Petri_Net& PN){

    Place* counter = new Place(PN.T.size(), "Latency_Counter");
    PN.P.push_back(counter);
    // for each transition add an arc to the latency counter
    for(Transition* t: PN.T){
        PN.add_arc(t, counter, PN.rng.sample());
    }
}

void congestion_model(Petri_Net& PN){
    std::stringstream buf;

    for(Transition* t: PN.T){
        buf.str(std::string());
        buf.clear();

        buf << "Congestion_" << t->name;

        Place* congestion = new Place(PN.P.size(), buf.str());
        congestion->type = Place::Type::CONGESTION;
        PN.P.push_back(congestion);

        PN.add_arc(t, congestion, PN.rng.sample());
    }
}


void congestion_query(int n_traces, int con_bound, std::ofstream& out, Petri_Net& PN){

    std::vector<Place*> graph_places;

    for(auto& p: PN.P){
        if(p->type == Place::Type::GRAPH)
            graph_places.push_back(p);
    }

    int max_idx = max(0, (int) graph_places.size() - 1);
    std::uniform_int_distribution<int> dist(0, max_idx);

    int source = dist(PN.rng.gen);
    int target;
    do{
        target = dist(PN.rng.gen);
    }while(target == source);

    // place a token at the source
    graph_places[source]->marking = 1;
    

    Qwriter writer(out);

    writer.write(Query::header);

    writer.iwrite_line(Query::property_set);
    writer.iwrite_line(Query::open("property"));

    std::stringstream id_name;
    id_name  << "congestion_model_"<< graph_places[source]->name << "_to_" << graph_places[target]->name << "_" << PN.rng.seed;
    writer.write(Query::id(id_name.str()));
    writer.write(Query::description("HyperLTL Congestion"));

    writer.iwrite_line(Query::open("formula"));

    for(int i = 1; i <= n_traces; i++){
        writer.iwrite_line(Query::exists_path(i));
    }

    writer.iwrite_line(Query::open("finally"));
    writer.iwrite_line(Query::open("conjunction"));

    for(int i = 1; i <= n_traces; i++){
        writer.iwrite_line(Query::open("integer-eq"));
        writer.iwrite_line(Query::path_scope(i));
        writer.iwrite_line(Query::open("tokens-count"));
        writer.write(Query::place(graph_places[target]->name));
        writer.close(2);
        writer.write(Query::int_constant(1));
        writer.close();
    }

    std::vector<Place*> congestion_places;

    for(auto& p : PN.P){
        if(p->type == Place::Type::CONGESTION){
            congestion_places.push_back(p);
        }
    }

    for(auto& cp : congestion_places){
        writer.iwrite_line(Query::open("integer-lt"));
        writer.iwrite_line(Query::open("integer-sum"));
        for(int i = 1; i <= n_traces; i++){
            writer.iwrite_line(Query::path_scope(i));
            writer.iwrite_line(Query::open("tokens-count"));
            writer.write(Query::place(cp->name));
            writer.close(2); // token + path-scope
        }
        writer.close(); // sum
        writer.write(Query::int_constant(con_bound));
        writer.close(); // lt
    }

    writer.close_all();
}