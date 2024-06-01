#ifndef CONFIG_H
#define CONFIG_H
#include <stddef.h>

#define CONFIG_ENTRY_KEY_MAXLEN 32
#define CONFIG_NAME_MAXLEN 32
#define CONFIG_MAX_NUM_ENTRIES 512

struct config_entry {
	char key[CONFIG_ENTRY_KEY_MAXLEN];
	int val;
};

struct config {
	char name[CONFIG_NAME_MAXLEN];
	struct config_entry entries[CONFIG_MAX_NUM_ENTRIES];
	size_t size;
};

int config_add_entry(struct config *config, const char *key, int val);
int config_remove_entry(struct config *config, const char *key);
int config_get_entry_val(struct config *config, const char *key);
int config_read_from_file(struct config *config, const char *filename);

#endif
