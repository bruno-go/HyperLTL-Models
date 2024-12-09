#include <string>

class Config {
public:
	bool debug = false;

	enum class Mode {CONVERT, LATENCY, CONGESTION};
	Mode mode = Mode::CONVERT;
	std::string in_path;
	std::string out_path;
	int max 	  = 1;
	int min 	  = 1;
	int n_traces  = 2;
	int con_bound = 2;
	unsigned long long seed = -1;
};

extern Config cfg;

void read_config(int argc, char**argv);
