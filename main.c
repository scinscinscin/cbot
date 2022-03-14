#include <concord/discord.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Vector.h"
#include "strings.h"

#define DEFAULT_PREFIX "!@#$"

struct discord *client;

ArrayPtr commands;
typedef void (*commandPtr)(struct discord*, const struct discord_message*);
typedef struct {
	char *name;
	commandPtr function;
} CommandHandler;

void AddCommandToCommandsList(char *name, commandPtr function){
	CommandHandler newCmd = {name, function};
	ArrPush(commands, &newCmd);
}

void testCommand(struct discord *client, const struct discord_message *msg){
		printf("Someone sent this message: %s\n", msg->content);
}

void on_message_create(struct discord *client, const struct discord_message *msg){
		char** tokens = str_split(msg->content, " ");
		char* firstWord = *(tokens);

		for(int idx = 0; idx < commands->length; idx++){
			CommandHandler *cmdHandler = GetArray(commands, idx);
			char *cmdName = cmdHandler->name;
			char *fullThing = malloc(strlen(DEFAULT_PREFIX) + strlen(cmdName) + 2);
			strcpy(fullThing, DEFAULT_PREFIX);
			strcat(fullThing, cmdName);

			if(!strcmp(firstWord, fullThing)){
				(*cmdHandler->function)(client, msg);
				break;
			}
		}
}

int main(int argc, char *argv[]){
		const char *config_file = "./config.json";

		ccord_global_init();
		client = discord_config_init(config_file);
		assert(NULL != client && "Couldn't initialize client");

		discord_set_on_message_create(client, &on_message_create);

		commands = createGenericArray(10, sizeof(CommandHandler));
		AddCommandToCommandsList("testCommand", &testCommand);

		discord_run(client);

		discord_cleanup(client);
		ccord_global_cleanup();
}