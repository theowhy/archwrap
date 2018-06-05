#include "archwrap.h"

#include <archive.h>

#define BLOCKSIZE 16384

static ssize_t archwrap_cookie_read(void *archive, char *buf, size_t size) {
	la_ssize_t result = 0;

	do {
		result = archive_read_data((struct archive *)archive, buf, size);
	} while(result == ARCHIVE_RETRY);

	if(result == ARCHIVE_FATAL || result == ARCHIVE_WARN) {
		return 0;
	} else {
		return result;
	}
}

static int archwrap_cookie_close(void *archive) {
	if(archive_read_free((struct archive *)archive) != ARCHIVE_OK) {
		return EOF;
	}

	return 0;
}

FILE * archwrap_open(const char *path) {
	FILE *fstr = NULL;
	struct archive *archive = NULL;
	struct archive_entry *archive_entry = NULL;

	cookie_io_functions_t archive_functions = {
		.read = archwrap_cookie_read,
		.write = NULL,
		.seek = NULL,
		.close = archwrap_cookie_close
	};

	archive = archive_read_new();
	archive_read_support_filter_all(archive);
	archive_read_support_format_raw(archive);

	if (archive_read_open_filename(archive, path, BLOCKSIZE) != ARCHIVE_OK) {
		return NULL;
	}

	if (archive_read_next_header(archive, &archive_entry) != ARCHIVE_OK) {
		return NULL;
	}

	return fopencookie(archive,"r", archive_functions);
}
