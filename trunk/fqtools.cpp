#include <cstdio>
#include <cstring>
#include "merge.h"
#include "split.h"
#include "filter.h"
#include "fqtools.h"

int main(int argc, const char **argv)
{
	if (argc < 2) {
		fprintf(stderr,
				"\n"
				"Program: " APP_NAME " (ver: 0.0.1-r" SVN_VER ")\n"
				"Author : Linlin Yan (yanll<at>mail.cbi.pku.edu.cn)\n"
				"Copyright: 2013, Centre for Bioinformatics, Peking University, China\n"
				"\n"
				"Usage: " APP_NAME " <subcommand> [<options> ...]\n"
				"\n"
				"Available subcommands:\n"
				"   merge\n"
				"   split\n"
				"   filter\n"
				"\n"
				"For more information, see http://fqtools.googlecode.com/\n"
				"\n");
		return 1;
	}

	if (0 == strcmp(argv[1], "merge")) {
		return merge_main(argc - 1, argv + 1);
	}
	if (0 == strcmp(argv[1], "split")) {
		return split_main(argc - 1, argv + 1);
	}
	if (0 == strcmp(argv[1], "filter")) {
		return filter_main(argc - 1, argv + 1);
	}

	fprintf(stderr, "Error: Unknown subcommand '%s'!\n", argv[1]);
	return 0;
}
