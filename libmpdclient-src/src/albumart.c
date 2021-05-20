#include <mpd/send.h>
#include <mpd/recv.h>
#include <mpd/pair.h>
#include <mpd/response.h>
#include <mpd/albumart.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

long long
mpd_run_albumart(struct mpd_connection *c, const char *uri, unsigned char **buf)
{
	unsigned long long offset = 0;
	unsigned long long total_size;
	*buf = NULL;

	do {
		char offset_s[32];
		snprintf(offset_s, sizeof(offset_s), "%lu", (unsigned long)offset);

		if (!mpd_send_command(c, "albumart", uri, offset_s, NULL))
			return -1;

		struct mpd_pair *pair = mpd_recv_pair_named(c, "size");
		if (pair == NULL) {
			return -1;
		}

		total_size = strtoull(pair->value, NULL, 10);
		if (*buf == NULL) {
			*buf = malloc(sizeof(unsigned char) * total_size);
		}

		mpd_return_pair(c, pair);

		pair = mpd_recv_pair_named(c, "binary");
		if (pair == NULL) {
			return -1;
		}

		const unsigned long long chunk_size = strtoull(pair->value, NULL, 10);
		mpd_return_pair(c, pair);

		if (chunk_size == 0)
			break;

		char *p = malloc(chunk_size);
		if (p == NULL) {
			return -1;
		}

		if (!mpd_recv_binary(c, p, chunk_size)) {
			free(p);
			return -1;
		}

		if (!mpd_response_finish(c)) {
			free(p);
			return -1;
		}

		memcpy((*buf) + offset, p, chunk_size);

		free(p);
		offset += chunk_size;
	} while (offset < total_size);

	return total_size;
}
