#include <stdio.h>
#include <string.h>
#include "config.h"

// NOTE: We use a strlen() here instead of the safer alternative strnlen(),
//	 because it is not in the C99 standard. Consider workarounds.

// TODO: Implement hashing to improve lookup performance, currently we loop
//	 through the occupied array space to find key match.

void config_add_entry(struct config *config, const char *key, int value)
{
	size_t len;

	if (!config || config->size >= CONFIG_MAX_NUM_ENTRIES || !key) {
		return;
	}
	len = strlen(key);
	if (len > CONFIG_ENTRY_KEY_MAXLEN - 1) {
		fprintf(stderr, "Failed to add entry (%s - %d) to config (%s). "
			"Key added is over length limit %d.\n", key, value,
			config->name, CONFIG_ENTRY_KEY_MAXLEN);
		return;
	}
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			config->entries[i].value = value;
			return;
		}
	}
	strncpy(config->entries[config->size].key, key, len);
	config->entries[config->size].value = value;
	config->size++;
}

struct config_entry *config_find_entry(struct config *config, const char *key)
{
	size_t len;

	if (!config || !key) {
		return NULL;
	}
	len = strlen(key);
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			return &config->entries[i];
		}
	}
	return NULL;
}

void config_remove_entry(struct config *config, char *key)
{
	size_t len;

	if (!config || !key) {
		return;
	}
	len = strlen(key);
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			memset(&config->entries[i], 0,
				sizeof(struct config_entry));
		}
	}
}
