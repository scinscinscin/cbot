#include "Vector.h"
#include <concord/discord.h>

ArrayPtr commands;
typedef void (*commandPtr)(struct discord *, const struct discord_message *);
typedef struct
{
	char *name;
	commandPtr function;
} CommandHandler;

void AddCommandToCommandsList(char *name, commandPtr function);