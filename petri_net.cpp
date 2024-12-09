#include "graph.h"
#include "petri_net.h"
#include <iostream>
#include <sstream>

void Petri_Net::add_arc(Place* p, Transition* t, int weight){
    p->out_arcs.push_back({p, t, weight});
    t-> in_arcs.push_back({p, t, weight});
}

void Petri_Net::add_arc(Transition* t, Place* p, int weight){
    t->out_arcs.push_back({t, p, weight});
    t-> in_arcs.push_back({t, p, weight});
}

void Petri_Net::add_place_from_vertex(Vertex* v){
    std::string name = v->name;
    /* in the tapn format, names are not allowed to be just numbers
       places inherit name from graphml file, but these mostly use just numbers
       if names start with a numeric character they are padded with a 'P'
    */
    if(name[0] >= '0' && name[0] <= '9'){
        name = "P" + name;
    }

    Place* p = new Place(v->id, name);
    p->type = Place::Type::GRAPH;
    P.push_back(p);
}


Petri_Net net_from_graph(Graph& G){
    Petri_Net PN;

    for(Vertex* v: G.V){
        PN.add_place_from_vertex(v);
    }

    // uses source -> target definition in file as directedness (the graph files themselves do not define directedness)
    // as the places have identical ids to the vertices we can use them directly
    std::stringstream buf; 
    for(Edge* e: G.E){
        Place* p = PN.P[e->end[0].v->id];
        Place* q = PN.P[e->end[1].v->id];


        buf.str(std::string());
        buf.clear();

        buf << "T" << PN.T.size() / 2;
        Transition* t = new Transition(PN.T.size(), buf.str());
        PN.T.push_back(t);

        buf << "_R";
        Transition* t_r = new Transition(PN.T.size(), buf.str());
        PN.T.push_back(t_r);

        PN.add_arc(p, t, 1);
        PN.add_arc(t, q, 1);

        PN.add_arc(q, t_r, 1);
        PN.add_arc(t_r, p, 1);

    }

    //std::cout << "net has " << PN.P.size() << " places, " << PN.T.size() << " transitions\n";
    return PN;
}

void Place::print_tpn(std::ostream& out){
    out << "<place displayName=\"true\" id=\""<<"P"<< id <<"\" initialMarking=\""<< marking <<"\" invariant=\"&lt; inf\" name=\""<< name <<"\" nameOffsetX=\"0\" nameOffsetY=\"0\" positionX=\"0\" positionY=\"0\"/>" << std::endl;
    return;
}

void Transition::print_tpn(std::ostream& out){
    out << "<transition angle=\"0\" displayName=\"true\" id=\""<<"T"<< id <<"\" infiniteServer=\"false\" name=\""<< name <<"\" nameOffsetX=\"0\" nameOffsetY=\"0\" player=\"0\" positionX=\"0\" positionY=\"0\" priority=\"0\" urgent=\"false\"/>" << std::endl;
    return;
}

std::string Arc::get_src_out(){
    std::stringstream out;
    if(std::holds_alternative<Place*>(src)){
        out << "P" << std::get<Place*>(src)->id;
    }else{
        out << "T" << std::get<Transition*>(src)->id;
    }
    return out.str();
}

std::string Arc::get_dst_out(){
    std::stringstream out;
    if(std::holds_alternative<Place*>(dst)){
        out << "P" << std::get<Place*>(dst)->id;
    }else{
        out << "T" << std::get<Transition*>(dst)->id;
    }
    return out.str();
}

void Arc::print_tpn(std::ostream& out, std::string label){
    if(std::holds_alternative<Place*>(src)){
        out << "<arc id=\""<< label <<"\" inscription=\"[0,inf)\" nameOffsetX=\"0\" nameOffsetY=\"0\" source=\""<< get_src_out() <<"\" target=\""<< get_dst_out() <<"\" type=\"timed\" weight=\""<< weight <<"\"/>" << std::endl;
    }
    else{
        out << "<arc id=\""<< label <<"\" inscription=\"1\" nameOffsetX=\"0\" nameOffsetY=\"0\" source=\""<< get_src_out() <<"\" target=\""<< get_dst_out() <<"\" type=\"normal\" weight=\""<< weight <<"\"/>" << std::endl;
    }
    return;
}

void write_output_tpn(std::ostream& out, Petri_Net& PN){
    std::string header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<pnml xmlns=\"http://www.informatik.hu-berlin.de/top/pnml/ptNetb\">\n  <net active=\"true\" id=\"TAPN1\" type=\"P/T net\">";
    out << header << std::endl;
    for(Place* p : PN.P){
        p->print_tpn(out);
    }
    for(Transition* t: PN.T){
        t->print_tpn(out);
    }

    size_t arc_count = 0;
    std::stringstream buf; 

    for(Place* p : PN.P){
        for(auto& arc: p->out_arcs){
            buf.str(std::string());
            buf.clear();

            buf << "A" << arc_count++;
            arc.print_tpn(out, buf.str());
        }
    }
    for(Transition* t: PN.T){
        for(auto& arc: t->out_arcs){
            buf.str(std::string());
            buf.clear();

            buf << "A" << arc_count++;
            arc.print_tpn(out, buf.str());
        }
    }

    out << "</net>\n<feature isGame=\"false\" isTimed=\"false\"/>\n</pnml>" << std::endl;
}


void output_as_tpn(std::string& out_path, Petri_Net& PN){
    if(out_path.empty()){
        write_output_tpn(std::cout, PN);
    }else{
        std::ofstream fp;
        fp.open(out_path);
        if(!fp){
            std::cerr << "invalid output file path" << std::endl;
            exit(1);
        }
        write_output_tpn(fp, PN);
    }
}