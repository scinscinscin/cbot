#include <concord/discord.h>

#ifndef CURL_H_IS_DEFINED
#define CURL_H_IS_DEFINED

struct curl_write_cb; // forward declaration
typedef void (*curl_pretty_callback)(struct curl_write_cb *);
struct discord_data {
	CURLM *multi;
};

struct curl_write_cb {
	struct discord *client;
	u64snowflake channel;		 // the channel where the response should be sent to
	curl_pretty_callback cb; // the callback to where the struct should send itself to
	char *buf;
	void *data;
	size_t len;
};

static size_t curl_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata);
int multi_cb(struct io_poller *io, CURLM *multi, void *client);
CURL *create_curl_easy_handle(struct discord *client, const char *url, struct curl_write_cb *data);
struct curl_write_cb *curl_write_cb_init(struct discord *client, curl_pretty_callback cb, u64snowflake channel);
#endif