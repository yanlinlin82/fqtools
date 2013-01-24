#include <cstdio>
#include <cstring>
#include "split.h"
#include "fqtools.h"

int split_main(int argc, const char **argv)
{
	const char *in_file = "-";
	const char *files[2] = { NULL, NULL };
	for (int i = 1; i < argc; ++i) {
		if (0 == strcmp(argv[i], "-i")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: Missiong option for '%s'!\n", argv[i]);
				return 1;
			}
			in_file = argv[++i];
		} else {
			if (!files[0]) {
				files[0] = argv[i];
			} else if (!files[1]) {
				files[1] = argv[i];
			} else {
				fprintf(stderr, "Error: Unexpected parameter '%s'!\n", argv[i]);
				return 1;
			}
		}
	}
	if (!files[0] || !files[1]) {
		fprintf(stderr,
				"\n"
				"Usage: " APP_NAME " %s <out_1.fq> <out_2.fq> [<options> ...]\n"
				"\n"
				"Options:\n"
				"   -i <in_merged.fq>      Input merged FASTQ file (merged by '" APP_NAME " merge'), default is from stdin\n"
				"\n", argv[0]);
		return 1;
	}

	FILE *fp_in = (0 == strcmp(in_file, "-") ? stdin : fopen(in_file, "r"));
	if (!fp_in) {
		fprintf(stderr, "Error: Can not open file '%s' for reading!\n", in_file);
		return 1;
	}

	FILE *fp1 = (0 == strcmp(files[0], "-") ? stdout : fopen(files[0], "w"));
	if (!fp1) {
		fprintf(stderr, "Error: Can not open file '%s' for writing!\n", files[0]);
		fclose(fp_in);
		return 1;
	}

	FILE *fp2 = (0 == strcmp(files[1], "-") ? stdout : fopen(files[1], "w"));
	if (!fp2) {
		fprintf(stderr, "Error: Can not open file '%s' for writing!\n", files[1]);
		fclose(fp1);
		fclose(fp_in);
		return 1;
	}

	char buf[MAX_LINE_SIZE];
	while (!feof(fp_in)) {
		for (int i = 0; i < 4; ++i) {
			if (!fgets(buf, sizeof(buf), fp_in)) goto out;
			fprintf(fp1, "%s", buf);
		}
		for (int i = 0; i < 4; ++i) {
			if (!fgets(buf, sizeof(buf), fp_in)) goto out;
			fprintf(fp2, "%s", buf);
		}
	}
out:
	fclose(fp2);
	fclose(fp1);
	fclose(fp_in);
	return 0;
}
