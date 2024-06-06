#include <stdio.h>
#include <string.h>
#include "config.h"

// NOTE: We use a strlen() here instead of the safer alternative strnlen(),
//	 because it is not in the C99 standard. Consider workarounds.

// TODO: Implement hashing to improve lookup performance, currently we loop
//	 through the occupied array space to find key match.

int config_add_entry(struct config *config, const char *key, int val)
{
	size_t len = 0;

	if (!config || config->size >= CONFIG_MAX_NUM_ENTRIES || !key) {
		return 0;
	}
	len = strlen(key);
	if (len > CONFIG_ENTRY_KEY_MAXLEN - 1) {
		fprintf(stderr, "Failed to add entry (%s - %d) to config (%s). "
			"Key added is over length limit %d.\n", key, val,
			config->name, CONFIG_ENTRY_KEY_MAXLEN);
		return 0;
	}
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			config->entries[i].val = val;
			return 1;
		}
	}
	strncpy(config->entries[config->size].key, key, len);
	config->entries[config->size].val = val;
	config->size++;
	return 1;
}

int config_remove_entry(struct config *config, const char *key)
{
	size_t len = 0;

	if (!config || !key) {
		return 0;
	}
	len = strlen(key);
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			memset(&config->entries[i], 0,
				sizeof(struct config_entry));
			return 1;
		}
	}
	return 0;
}

int config_get_entry_val(struct config *config, const char *key)
{
	size_t len = 0;

	if (!config || !key) {
		return 0;
	}
	len = strlen(key);
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			return config->entries[i].val;
		}
	}
	// HACK: We return zero on failure which can also be a valid value.
	return 0;
}

int config_read_from_file(struct config *config, const char *filename)
{
	if (!config || !filename) {
		return 0;
	}
	// TODO: Implement this function.
	return 0;
}
