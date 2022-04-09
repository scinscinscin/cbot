#include "cJSON.h"
#include "curl.h"
#include "nhentai.h"
#include "strings.h"
#include <concord/discord.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct nhentai_tags_types {
	char *artist;
	char *category;
	char *character;
	char *group;
	char *language;
	char *parody;
	char *tag;
};

char *nhentai_command_tags_processor(char *dest, ArrayPtr tags) {
	struct nhentai_tags_types *cache = calloc(1, sizeof(struct nhentai_tags_types));

	for (int idx = 0; idx < tags->length; idx++) {
		struct nhentai_tag *tag = GetArray(tags, idx);

		char **ptr_to_char_array;
		if (!strcmp(tag->type, "artist")) ptr_to_char_array = &(cache->artist);
		else if (!strcmp(tag->type, "category")) ptr_to_char_array = &(cache->category);
		else if (!strcmp(tag->type, "character")) ptr_to_char_array = &(cache->character);
		else if (!strcmp(tag->type, "group")) ptr_to_char_array = &(cache->group);
		else if (!strcmp(tag->type, "language")) ptr_to_char_array = &(cache->language);
		else if (!strcmp(tag->type, "parody")) ptr_to_char_array = &(cache->parody);
		else if (!strcmp(tag->type, "tag")) ptr_to_char_array = &(cache->tag);

		if (*ptr_to_char_array == NULL) {
			*ptr_to_char_array = malloc(strlen(tag->name) + 1);
			strcpy(*ptr_to_char_array, tag->name);
		} else {
			*ptr_to_char_array = scin_string_append(*ptr_to_char_array, ", ");
			*ptr_to_char_array = scin_string_append(*ptr_to_char_array, tag->name);
		}
	}

	if (cache->artist) {
		dest = scin_string_append(dest, "\n**Artist:** ");
		dest = scin_string_append(dest, cache->artist);
	}

	if (cache->category) {
		dest = scin_string_append(dest, "\n**Category:** ");
		dest = scin_string_append(dest, cache->category);
	}

	if (cache->character) {
		dest = scin_string_append(dest, "\n**Character:** ");
		dest = scin_string_append(dest, cache->character);
	}

	if (cache->group) {
		dest = scin_string_append(dest, "\n**Group:** ");
		dest = scin_string_append(dest, cache->group);
	}

	if (cache->language) {
		dest = scin_string_append(dest, "\n**Languages:** ");
		dest = scin_string_append(dest, cache->language);
	}

	if (cache->parody) {
		dest = scin_string_append(dest, "\n**Parodies:** ");
		dest = scin_string_append(dest, cache->parody);
	}

	if (cache->tag) {
		dest = scin_string_append(dest, "\n**Tags:** ");
		dest = scin_string_append(dest, cache->tag);
	}

	free(cache);
	return dest;
}

char *convert_int_to_string(int number) {
	int length = snprintf(NULL, 0, "%d", number) + 1; // get the length of the string
	char *str = malloc(length);
	snprintf(str, length, "%d", number);
	return str;
}

void nhentai_command_callback(struct curl_write_cb *data) {
	struct nhentai_doujin *bruh = nhentai_process_doujin(data->buf);

	char formatted_date[30];
	strftime(formatted_date, 30, "%D %r", gmtime((time_t *)&(bruh->upload_date)));
	char *string_favorites = convert_int_to_string(bruh->num_favorites);
	char *string_pages = convert_int_to_string(bruh->num_pages);

	char *description = malloc(1000);
	description = scin_string_append(description, "**Upload Date:** ");
	description = scin_string_append(description, formatted_date);
	description = scin_string_append(description, "**\nFavorites:** ");
	description = scin_string_append(description, string_favorites);
	description = scin_string_append(description, "**\nPages:** ");
	description = scin_string_append(description, string_pages);
	description = nhentai_command_tags_processor(description, bruh->tags);

	char *thumbnail_url = nhentai_get_thumbnail(bruh->media_id);

	struct discord_embed embeds[] = {
			{.title = bruh->title.english,
			 .color = 3447003,
			 .timestamp = discord_timestamp(data->client),
			 .description = description,
			 .thumbnail = &(struct discord_embed_thumbnail){
					 .url = thumbnail_url}},
	};

	struct discord_create_message response = {
			.embeds =
					&(struct discord_embeds){
							.size = sizeof(embeds) / sizeof *embeds,
							.array = embeds,
					},
	};
	discord_create_message(data->client, data->channel, &response, NULL);
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
