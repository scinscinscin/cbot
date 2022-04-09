#include "nhentai.h"
#include "cJSON.h"

struct nhentai_image *nhentai_process_image(cJSON *image) {
	struct nhentai_image *ret = malloc(sizeof(struct nhentai_image));
	ret->t = cJSON_GetObjectItemCaseSensitive(image, "t")->valuestring;
	ret->w = cJSON_GetObjectItemCaseSensitive(image, "w")->valueint;
	ret->h = cJSON_GetObjectItemCaseSensitive(image, "h")->valueint;
	return ret;
}

struct nhentai_images *nhentai_process_images(cJSON *images) {
	struct nhentai_images *ret = malloc(sizeof(struct nhentai_images));
	ret->cover = *(nhentai_process_image(cJSON_GetObjectItemCaseSensitive(images, "cover")));
	ret->thumbnail = *(nhentai_process_image(cJSON_GetObjectItemCaseSensitive(images, "thumbnail")));

	ret->pages = CreateGenericArray(10, sizeof(struct nhentai_image));
	cJSON *page = NULL;
	cJSON *pages = cJSON_GetObjectItemCaseSensitive(images, "pages");

	cJSON_ArrayForEach(page, pages) {
		struct nhentai_image *image = nhentai_process_image(page);
		ArrPush(ret->pages, image);
	}
	return ret;
}

ArrayPtr nhentai_process_tags(cJSON *tags) {
	const cJSON *tag = NULL;
	ArrayPtr ret = CreateGenericArray(10, sizeof(struct nhentai_tag));

	cJSON_ArrayForEach(tag, tags) {
		struct nhentai_tag *tag_struct = malloc(sizeof(struct nhentai_tag));
		tag_struct->id = cJSON_GetObjectItemCaseSensitive(tag, "id")->valueint;
		tag_struct->type = cJSON_GetObjectItemCaseSensitive(tag, "type")->valuestring;
		tag_struct->name = cJSON_GetObjectItemCaseSensitive(tag, "name")->valuestring;
		tag_struct->url = cJSON_GetObjectItemCaseSensitive(tag, "url")->valuestring;
		tag_struct->count = cJSON_GetObjectItemCaseSensitive(tag, "count")->valueint;
		ArrPush(ret, tag_struct);
	}
	return ret;
}

struct nhentai_doujin *nhentai_process_doujin(char *buf) {
	cJSON *root_json = cJSON_Parse(buf);
	cJSON *titles_json = cJSON_GetObjectItemCaseSensitive(root_json, "title");

	struct nhentai_doujin *ret = malloc(sizeof(struct nhentai_doujin));
	struct nhentai_titles *titles = malloc(sizeof(struct nhentai_titles));

	ret->id = cJSON_GetObjectItemCaseSensitive(root_json, "id")->valueint;
	ret->media_id = cJSON_GetObjectItemCaseSensitive(root_json, "media_id")->valuestring;
	titles->english = cJSON_GetObjectItemCaseSensitive(titles_json, "english")->valuestring;
	titles->japanese = cJSON_GetObjectItemCaseSensitive(titles_json, "japanese")->valuestring;
	titles->pretty = cJSON_GetObjectItemCaseSensitive(titles_json, "pretty")->valuestring;
	ret->title = *titles;
	ret->scanlator = cJSON_GetObjectItemCaseSensitive(root_json, "scanlator")->valuestring;
	ret->num_pages = cJSON_GetObjectItemCaseSensitive(root_json, "num_pages")->valueint;
	ret->num_favorites = cJSON_GetObjectItemCaseSensitive(root_json, "num_favorites")->valueint;
	ret->images = *nhentai_process_images(cJSON_GetObjectItemCaseSensitive(root_json, "images"));
	ret->tags = nhentai_process_tags(cJSON_GetObjectItemCaseSensitive(root_json, "tags"));

	return ret;
};

char *nhentai_get_thumbnail(char *media_id) {
	char *ret = malloc(60);
	sprintf(ret, "https://t2.nhentai.net/galleries/%s/cover.jpg", media_id);
	return ret;
}