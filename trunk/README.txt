FQtools: Tools for manipulating FastQ files

Example:

$ fqtools merge <(zcat 1.fq.gz) <(zcat 2.fq.gz) | fqtools filter -N 10 | fqtools split >(gzip -9c > 3.fq.gz) >(gzip -9c > 4.fq.gz)
