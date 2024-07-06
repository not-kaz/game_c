#include <stdio.h>
#include "result_code.h"

const char *result_code_to_str(int code)
{
	const char *str = NULL;

	switch (code) {
	case RESULT_CODE_SUCCESS:
		str = "Function executed successfully.";
		break;
	case RESULT_CODE_UNSPECIFIED_ERROR:
		str = "An unspecified error occured.";
		break;
	case RESULT_CODE_INIT_FAIL:
		str = "Initialization failed.";
		break;
	case RESULT_CODE_OUT_OF_MEM:
		str = "Out of memory.";
		break;
	case RESULT_CODE_NULL_PTR:
		str = "Unexpected null pointer.";
		break;
	default:
		str = "Result code invalid.";
		break;
	}
	return str;
}
