#ifndef __OPTION_H
#define __OPTION_H
#include <getopt.h>

// Constants
static const struct option long_options[];
static const char opt_string[];

// Flags
bool enable_debug ;

// Functions
int process_options(int argc, char* const argv[]);
#endif
