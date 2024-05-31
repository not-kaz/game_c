#include <stdio.h>
#include <string.h>
#include "config.h"

void config_add_entry(struct config *config, const char *key, int value)
{
	struct config_entry *entry;
	size_t len;

	if (!config || config->size >= CONFIG_MAX_NUM_ENTRIES || !key) {
		return;
	}
	entry = &config->entries[config->size];
	len = strlen(key);
	printf("%llu", len);
	if (len > CONFIG_ENTRY_KEY_MAXLEN - 1) {
		fprintf(stderr, "Failed to add entry (%s - %d) to config (%s). "
			"Key added is over length limit %d.\n", key, value,
			config->name, CONFIG_ENTRY_KEY_MAXLEN);
		return;
	}
	// TODO: Check for duplicates. If key is already present, quit out.
	strncpy(entry->key, key, len);
	config->entries[config->size].value = value;
	config->size++;
}

struct config_entry *config_find_entry(struct config *config, const char *key)
{
	size_t len;

	if (!config || !key) {
		return NULL;
	}
	// REVIEW: Consider strnlen() even though it's not offically portable.
	len = strlen(key);
	// TODO: Hash keys to improve performance.
	for (size_t i = 0; i < CONFIG_MAX_NUM_ENTRIES; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			return &config->entries[i];
		}
	}
	return NULL;
}

void config_remove_entry(struct config *config, char *key)
{
	if (!config || !key) {
		return;
	}
	for
}
