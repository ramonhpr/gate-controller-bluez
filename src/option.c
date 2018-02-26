#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "option.h"
// Constants
static const struct option long_options[] = {
	{ "enable-debug",	no_argument, NULL, 'd' },
	{ "verbose", 		no_argument, NULL, 'v' },
	{ "help",		no_argument, NULL, 'h' },
	{ }
};

static const char opt_string[] = "dvh";

// Flags
bool enable_debug = false;
bool verbose = false;

static void usage() {
	printf("gatectl - Gate control daemon\n"
		"Usage:\n");
	printf("\tgatectl[options]:\n");
	printf("Options:\n"
		"\t-d, --enable-debug	 	Enable debug messages\n"
		"\t-v, --verbose		Enable verbose\n"
		"\t-h, --help		 	Show this usage message\n");
}

int process_options(int argc, char* const argv[]) {
	int opt;

	while(1) {
		opt = getopt_long(argc, argv, opt_string, long_options, NULL);
		if (opt < 0)
			break;
		switch(opt) {
			case 'd':
				enable_debug = true;
				break;
			case 'v':
				verbose = true;
				break;
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			default:
				fprintf(stderr, "Invalid parameters\n");
				fprintf(stderr, "Try '%s -h' for more info\n", argv[0]);
				return -1;
				
		}
	}
	if (argc - optind > 0)
		exit(EXIT_FAILURE);
	
	return 0;
}
