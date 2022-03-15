#include "CommandHandler.h"

void AddCommandToCommandsList(char *name, commandPtr function)
{
	CommandHandler newCmd = {name, function};
	ArrPush(commands, &newCmd);
}