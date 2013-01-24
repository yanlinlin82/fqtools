#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "filter.h"
#include "fqtools.h"

const char *in_file = "-";
const char *out_file = "-";
int max_N = 0;
int max_qual = 0;
float max_low_base = 0;
bool invert = false;
int qual_base = 33;

void print_usage(const char *cmd)
{
	fprintf(stderr,
			"\n"
			"Usage: " APP_NAME " %s [<options> ...]\n"
			"\n"
			"Options:\n"
			"   -h               Show this help\n"
			"   -i <in.fq>       Input (merged) FASTQ file, default: %s\n"
			"   -o <out.fq>      Output (merged) FASTQ file, default: %s\n"
			"   -N <int>         Filter if any read contains more than <int> N bases, default: %d\n"
			"   -n <number>      Filter if any read contains more than <number> low quality bases, default: %d\n"
			"                    Decimal fraction can be used here. E.g. 0.5 means half of the read.\n"
			"   -q <int>         A base is low quality if it is not higher than <int>, default: %d\n"
			"   -v, --invert     Show filtered read pairs, default: no\n"
			"   --q33 / --q64    Quality base, default: %d\n"
			"\n", cmd, in_file, out_file, max_N, static_cast<int>(max_low_base), max_qual, qual_base);
}

int filter_main(int argc, const char **argv)
{
	for (int i = 1; i < argc; ++i) {
		if (0 == strcmp(argv[i], "-h")) {
			print_usage(argv[0]);
			return 1;
		} else if (0 == strcmp(argv[i], "-i")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: Missing option for '%s'!\n", argv[i]);
				return 1;
			}
			in_file = argv[++i];
		} else if (0 == strcmp(argv[i], "-o")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: Missing option for '%s'!\n", argv[i]);
				return 1;
			}
			out_file = argv[++i];
		} else if (0 == strcmp(argv[i], "-N")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: Missing option for '%s'!\n", argv[i]);
				return 1;
			}
			max_N = atoi(argv[++i]);
			if (max_N < 0) {
				fprintf(stderr, "Error: Bad option for '%s'! Only non-negative integer is allowed.\n", argv[i]);
				return 1;
			}
		} else if (0 == strcmp(argv[i], "-n")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: Missing option for '%s'!\n", argv[i]);
				return 1;
			}
			max_low_base = atoi(argv[++i]);
			if (max_low_base < 0) {
				fprintf(stderr, "Error: Bad option for '%s'! Only non-negative integer is allowed.\n", argv[i]);
				return 1;
			}
		} else if (0 == strcmp(argv[i], "-q")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Error: Missing option for '%s'!\n", argv[i]);
				return 1;
			}
			max_qual = atoi(argv[++i]);
			if (max_qual < 0) {
				fprintf(stderr, "Error: Bad option for '%s'! Only non-negative integer is allowed.\n", argv[i]);
				return 1;
			}
		} else if (0 == strcmp(argv[i], "-v") || 0 == strcmp(argv[i], "--invert")) {
			invert = true;
		} else if (0 == strcmp(argv[i], "--q64")) {
			qual_base = 64;
		} else if (0 == strcmp(argv[i], "--q33")) {
			qual_base = 33;
		} else {
			fprintf(stderr, "Error: Unknown or unexpected parameter '%s'!\n", argv[i]);
			return 1;
		}
	}
	if (max_N <= 0 && max_low_base <= 0) {
		print_usage(argv[0]);
		return 1;
	}

	FILE *fp_in = (0 == strcmp(in_file, "-") ? stdin : fopen(in_file, "r"));
	if (!fp_in) {
		fprintf(stderr, "Error: Can not open file '%s' for reading!\n", in_file);
		return 1;
	}

	FILE *fp_out = (0 == strcmp(out_file, "-") ? stdout : fopen(out_file, "r"));
	if (!fp_out) {
		fprintf(stderr, "Error: Can not open file '%s' for writing!\n", out_file);
		fclose(fp_in);
		return 1;
	}

	char buf[8][MAX_LINE_SIZE];
	while (!feof(fp_in)) {
		for (int i = 0; i < 8; ++i) {
			if (!fgets(buf[i], sizeof(buf[i]), fp_in)) goto out;
		}

		bool filter = false;
		for (int i = 0; i < 2; ++i) {
			int N_count = 0;
			int low_base_count = 0;
			for (int j = 0; buf[4 * i + 1][j] > ' '; ++j) {
				N_count += (buf[4 * i + 1][j] == 'N');

				int qual = buf[4 * i + 3][j] - qual_base;
				if (qual < 0) {
					fprintf(stderr, "Error: Quality is lower than zero, quality base may not be %d!\n", qual_base);
					fclose(fp_out);
					fclose(fp_in);
					return 1;
				}
				low_base_count += (qual <= max_qual);
			}
			if (max_N > 0) {
				if (N_count > max_N) {
					filter = true;
					break;
				}
			}
			if (max_low_base > 0) {
				if (max_low_base < 1) {
					if (low_base_count > (strlen(buf[4 * i + 3]) - 1) * max_low_base) {
						filter = true;
						break;
					}
				} else {
					if (low_base_count > max_low_base) {
						filter = true;
						break;
					}
				}
			}
		}

		if (filter == invert) {
			for (int i = 0; i < 8; ++i) {
				fprintf(fp_out, "%s", buf[i]);
			}
		}
	}
out:
	fclose(fp_out);
	fclose(fp_in);
	return 0;
}
