#include <concord/discord.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void on_ready(struct discord *client){
		const struct discord_user *bot = discord_get_self(client);
		log_info("Copycat-Bot succesfully connected to Discord as %s#%s!", bot->username, bot->discriminator);
}

void on_message_create(struct discord *client, const struct discord_message *msg){
		if (msg->author->bot) return;

		struct discord_create_message params = { .content = msg->content };
		discord_create_message(client, msg->channel_id, &params, NULL);
}

int main(int argc, char *argv[]){
		const char *config_file;
		if (argc > 1)
				config_file = argv[1];
		else
				config_file = "./config.json";

		ccord_global_init();
		struct discord *client = discord_config_init(config_file);
		assert(NULL != client && "Couldn't initialize client");

		discord_set_on_ready(client, &on_ready);
		discord_set_on_message_create(client, &on_message_create);

		discord_run(client);

		discord_cleanup(client);
		ccord_global_cleanup();
}