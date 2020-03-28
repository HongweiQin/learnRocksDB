#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "rocksdb/c.h"

#include <unistd.h>

const char DBPath[] = "/tmp/rocksdb_cli_example";

void printMenu()
{
	printf("-------------------\n");
	printf("get(key): g [key]\n");
	printf("put(key, value): p [key] [value]\n");
	printf("exit: e\n");
	printf("-------------------\n");
}

int main(int argc, char **argv) {
	rocksdb_t *db;
	char *err = NULL;
	rocksdb_options_t *options = rocksdb_options_create();
	long cpus = sysconf(_SC_NPROCESSORS_ONLN);  // get # of online cores
	char cmd;
	char s_key[256], s_value[256];
	size_t len;
	char *get_ret;
	rocksdb_readoptions_t *readoptions;
	rocksdb_writeoptions_t *writeoptions;

	rocksdb_options_increase_parallelism(options, (int)(cpus));
	rocksdb_options_optimize_level_style_compaction(options, 0);
	rocksdb_options_set_create_if_missing(options, 1);

	db = rocksdb_open(options, DBPath, &err);
	assert(!err);

	readoptions = rocksdb_readoptions_create();
	writeoptions = rocksdb_writeoptions_create();

	printMenu();

	for (;;) {
		cmd = getchar();
		switch(cmd)
		{
		case 'g':
			getchar();
			scanf("%s", s_key);
			getchar();

			get_ret =
     			rocksdb_get(db, readoptions, s_key, strlen(s_key), &len, &err);

			assert(!err);
			printf("key[%s] value[%s]\n", s_key, get_ret);
			free(get_ret);
			break;
		case 'p':
			getchar();
			scanf("%s %s", s_key, s_value);
			getchar();

			rocksdb_put(db, writeoptions,
							s_key, strlen(s_key),
							s_value, strlen(s_value) + 1,
              				&err);
			assert(!err);
			printf("put key[%s] value[%s] finished\n", s_key, s_value);
			break;
		case 'e':
			goto out;
		default :
			printMenu();
			break;
		}
	}
out:
	rocksdb_close(db);
	return 0;
}



