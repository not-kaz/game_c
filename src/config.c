#include <stdio.h>
#include <string.h>
#include "config.h"
#include "result_code.h"

// NOTE: We use a strlen() here instead of the safer alternative strnlen(),
//	 because it is not in the C99 standard. Consider workarounds.

// TODO: Implement hashing to improve lookup performance, currently we loop
//	 through the occupied array space to find key match.

int config_init(struct config *config, const char *name)
{
	size_t len;

	if (name == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	len = strlen(name);
	memset(config, 0, sizeof(struct config));
	if (len <= 0 || len >= CONFIG_NAME_MAXLEN) {
		return RESULT_CODE_PARAM_INVALID;
	}
	strncpy(config->name, name, len);
	return RESULT_CODE_SUCCESS;
}

int config_finish(struct config *config)
{
	if (config == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	memset(config, 0, sizeof(struct config));
	return RESULT_CODE_SUCCESS;
}

int config_add_entry(struct config *config, const char *key, int val)
{
	size_t len;

	if (config == NULL || key == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	if (config->size >= CONFIG_MAX_NUM_ENTRIES) {
		return RESULT_CODE_LIMIT_EXCEEDED;
	}
	len = strlen(key);
	if (len > CONFIG_ENTRY_KEY_MAXLEN - 1) {
		fprintf(stderr, "Failed to add entry (%s - %d) to config (%s). "
			"Key added is over length limit %d.\n", key, val,
			config->name, CONFIG_ENTRY_KEY_MAXLEN);
		return RESULT_CODE_PARAM_INVALID;
	}
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			config->entries[i].val = val;
			return RESULT_CODE_SUCCESS;
		}
	}
	strncpy(config->entries[config->size].key, key, len);
	config->entries[config->size].val = val;
	config->size++;
	return RESULT_CODE_SUCCESS;
}

int config_remove_entry(struct config *config, const char *key)
{
	size_t len;

	if (config == NULL || key == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	len = strlen(key);
	for (size_t i = 0; i < config->size; i++) {
		if (strncmp(config->entries[i].key, key, len) == 0) {
			memset(&config->entries[i], 0,
				sizeof(struct config_entry));
			return RESULT_CODE_SUCCESS;
		}
	}
	return RESULT_CODE_PARAM_INVALID;
}

int config_get_entry_val(const struct config *config, const char *key)
{
	size_t len;

	if (config == NULL || key == NULL) {
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
	if (config == NULL || filename == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	// TODO: Implement this function.
	return 0;
}
