#include <cstdio>
#include <cstring>
#include "merge.h"
#include "fqtools.h"

int merge_main(int argc, const char **argv)
{
	if (argc != 3) {
		fprintf(stderr,
				"\n"
				"Usage: " APP_NAME " %s <in_1.fq> <in_2.fq>\n"
				"\n", argv[0]);
		return 1;
	}

	FILE *fp1 = (0 == strcmp(argv[1], "-") ? stdin : fopen(argv[1], "r"));
	if (!fp1) {
		fprintf(stderr, "Error: Can not open file '%s' for reading!\n", argv[1]);
		return 1;
	}

	FILE *fp2 = (0 == strcmp(argv[2], "-") ? stdin : fopen(argv[2], "r"));
	if (!fp2) {
		fprintf(stderr, "Error: Can not open file '%s' for reading!\n", argv[2]);
		fclose(fp1);
		return 1;
	}

	if (fp1 == stdin && fp2 == stdin) {
		fprintf(stderr, "Error: <1.fq> and <2.fq> can not both be '-'!\n");
		return 1;
	}

	char buf[MAX_LINE_SIZE];
	while (!feof(fp1) && !feof(fp2)) {
		for (int i = 0; i < 4; ++i) {
			if (!fgets(buf, sizeof(buf), fp1)) goto out;
			fprintf(stdout, "%s", buf);
		}
		for (int i = 0; i < 4; ++i) {
			if (!fgets(buf, sizeof(buf), fp2)) goto out;
			fprintf(stdout, "%s", buf);
		}
	}
out:
	fclose(fp2);
	fclose(fp1);
	return 0;
}
