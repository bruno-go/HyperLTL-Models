#include <iostream>
#include <string>

#include <argp.h>

#include "config.h"

/* globals */
Config cfg;

enum {
	OPT_FILE_IN = 20000,
	OPT_FILE_OUT,
	OPT_MODE,
	OPT_SEED,
	OPT_MAX,
	OPT_MIN,
	OPT_TRACES,
	OPT_CONGESTION,
	OPT_DEBUG
};

static error_t parse_opt (int key, char *arg, struct argp_state *state);

static struct argp_option options[] = {
	{"in", 		OPT_FILE_IN, "path", 0, "set input file path"},
	{"out", 	OPT_FILE_OUT, "path", 0, "set output file path"},
	{"mode",	OPT_MODE, "mode", 0,				"Modes convert, latency, congestion"},
	{"seed",	OPT_SEED, "[uint_64]", 0,				"Set seed [default: random]"},
	{"debug",	OPT_DEBUG, 0, 0,				"Enable debug prints"},
	{"max",	OPT_MAX, "[int]", 0,				"Set maximum random edge weight"},
	{"min",	OPT_MIN, "[int]", 0,				"Set minimum random edge weight"},
	{"traces", OPT_TRACES, "[int]", 0,			"Set number of traces [default: 2]"},
	{"bound", OPT_CONGESTION, "[int]", 0,			"Set congestion bound [default: 2]"},
	{ 0 }
};
static struct argp argp = {options, parse_opt, 0, 0};

static Config::Mode str_to_mode(std::string str) {
	if (str == "convert")
		return Config::Mode::CONVERT;
	else if (str == "latency")
		return Config::Mode::LATENCY;
	else if (str == "congestion")
		return Config::Mode::CONGESTION;

	std::cerr << "Unknown mode: " << str << std::endl;
	exit(1);
}

static error_t parse_opt (int key, char *arg, struct argp_state *state){
	switch (key) {
	case OPT_FILE_IN:
		cfg.in_path = (std::string)arg;
		break;
	case OPT_FILE_OUT:
		cfg.out_path = arg;
		break;
	case OPT_MODE:
		cfg.mode = str_to_mode(arg);
		break;
	case OPT_DEBUG:
		break;
	case OPT_MAX:
		cfg.max = atoi(arg);
		break;
	case OPT_MIN:
		cfg.min = atoi(arg);
		break;
	case OPT_SEED:
		cfg.seed = atoll(arg);
		break;
	case OPT_TRACES:
		cfg.n_traces = atoi(arg);
		break;
	case OPT_CONGESTION:
		cfg.con_bound = atoi(arg);
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1)
			/* Too many arguments. */
			argp_usage (state);

		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

void read_config(int argc, char**argv) {
	argp_parse (&argp, argc, argv, 0, 0, nullptr);
}
