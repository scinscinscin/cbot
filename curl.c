#include "curl.h"
#include <concord/discord.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t curl_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	struct curl_write_cb *data = userdata;
	data->buf = realloc(data->buf, data->len + nmemb + 1);
	memcpy(data->buf + data->len, ptr, nmemb);
	data->buf[data->len += nmemb] = 0;
	return nmemb;
}

int multi_cb(struct io_poller *io, CURLM *multi, void *client) {
	int running;
	curl_multi_socket_all(multi, &running);
	while (1) {
		int msgs;
		CURLMsg *info = curl_multi_info_read(multi, &msgs);
		if (!info) break;
		if (info->msg == CURLMSG_DONE) {
			struct curl_write_cb *data;
			curl_easy_getinfo(info->easy_handle, CURLINFO_PRIVATE, &data);

			data->cb(data);

			free(data->buf);
			free(data);
			curl_multi_remove_handle(multi, info->easy_handle);
			curl_easy_cleanup(info->easy_handle);
		}
	}
}

CURL *create_curl_easy_handle(struct discord *client, const char *url, struct curl_write_cb *data) {
	CURL *easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, url);
	curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, curl_write_cb);
	curl_easy_setopt(easy, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(easy, CURLOPT_PRIVATE, data);

	struct discord_data *dd = discord_get_data(client);
	curl_multi_add_handle(dd->multi, easy);
	io_poller_curlm_enable_perform(discord_get_io_poller(client), dd->multi);
	return easy;
}

struct curl_write_cb *curl_write_cb_init(struct discord *client, curl_pretty_callback cb, u64snowflake channel) {
	struct curl_write_cb *ret = calloc(1, sizeof(struct curl_write_cb));
	ret->channel = channel;
	ret->client = client;
	ret->cb = cb;
	ret->buf = calloc(1, 1);
	return ret;
}