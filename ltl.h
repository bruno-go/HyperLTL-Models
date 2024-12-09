#include "graph.h"
#include "petri_net.h"
#include <random>

void latency_model(Petri_Net& PN);

void congestion_model(Petri_Net& PN);

void congestion_query(int n_traces, int con_bound, std::ofstream& out, Petri_Net& PN);