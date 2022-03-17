#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CommandHandler.h"
#include "strings.h"

#define DEFAULT_PREFIX "!@#$"
struct discord *client;

void TestCommand(struct discord *client, const struct discord_message *msg) {
		printf("Someone sent this message: %s\n", msg->content);
}

void PingCommand(struct discord *client, const struct discord_message *msg) {
		uint64_t currentTime = discord_timestamp(client);
		uint64_t diff = currentTime - msg->timestamp;

		char *response;
		int stringLength = snprintf(response, 0, "The ping is: %lums", diff) + 1;
		response = malloc(stringLength);
		snprintf(response, stringLength, "The ping is: %lums", diff);

		struct discord_embed embeds[] = {
						{
										.color = 177013,
										.description = response,
										.timestamp = currentTime,
						}};

		struct discord_create_message params = {
						.embeds = &(struct discord_embeds){
										.size = sizeof(embeds) / sizeof *embeds,
										.array = embeds,
						}};

		discord_create_message(client, msg->channel_id, &params, NULL);
		free(response);
}

void on_message_create(struct discord *client, const struct discord_message *msg) {
		char **tokens = str_split(msg->content, " ");
		char *firstWord = *(tokens);

		for (int idx = 0; idx < commands->length; idx++) {
				CommandHandler *cmdHandler = GetArray(commands, idx);
				char *cmdName = cmdHandler->name;
				char *fullThing = malloc(strlen(DEFAULT_PREFIX) + strlen(cmdName) + 2);
				strcpy(fullThing, DEFAULT_PREFIX);
				strcat(fullThing, cmdName);

				if (!strcmp(firstWord, fullThing)) {
						(*cmdHandler->function)(client, msg);
						break;
				}

				free(fullThing);
		}
}

int main(int argc, char *argv[]) {
		const char *config_file = "./config.json";

		ccord_global_init();
		client = discord_config_init(config_file);
		assert(NULL != client && "Couldn't initialize client");

		discord_set_on_message_create(client, &on_message_create);

		commands = CreateGenericArray(10, sizeof(CommandHandler));
		AddCommandToCommandsList("testCommand", &TestCommand);
		AddCommandToCommandsList("ping", &PingCommand);

		discord_run(client);

		discord_cleanup(client);
		ccord_global_cleanup();
}