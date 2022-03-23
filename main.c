#include "curl.h"
#include <concord/discord.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void nhentai_command_callback(struct curl_write_cb *data) {
	printf("The callback was called!\n");
	discord_create_message(data->client, data->channel, &(struct discord_create_message){.content = data->buf}, NULL);
}

static void nhentaiCommand(struct discord *client, const struct discord_message *msg) {
	struct curl_write_cb *data = curl_write_cb_init(client, nhentai_command_callback, msg->channel_id);
	CURL *easy = create_curl_easy_handle(client, msg->content, data);
}

int main(int argc, char *argv[]) {
	ccord_global_init();

	struct discord *client = discord_config_init(argc > 1 ? argv[1] : "config.json");
	struct discord_data dd = {
			.multi = curl_multi_init(),
	};
	discord_set_data(client, &dd);
	io_poller_curlm_add(discord_get_io_poller(client), dd.multi, multi_cb, client);

	// add the command and start the client
	discord_set_prefix(client, "!@#$");
	discord_set_on_command(client, "nhentai", nhentaiCommand);
	discord_run(client);

	// cleanup of everything
	io_poller_curlm_del(discord_get_io_poller(client), dd.multi);
	curl_multi_cleanup(dd.multi);
	discord_cleanup(client);
	ccord_global_cleanup();
}
