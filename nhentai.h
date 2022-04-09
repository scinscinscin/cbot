#include "Vector.h"
#include "cJSON.h"

struct nhentai_titles {
	char *english;
	char *japanese;
	char *pretty;
};

struct nhentai_image {
	char *t;
	int w;
	int h;
};

struct nhentai_images {
	ArrayPtr pages;
	struct nhentai_image cover;
	struct nhentai_image thumbnail;
};

struct nhentai_tag {
	int id;
	char *type;
	char *name;
	char *url;
	int count;
};

struct nhentai_doujin {
	int id;
	char *media_id;
	struct nhentai_titles title;
	struct nhentai_images images;
	char *scanlator;
	int upload_date;
	ArrayPtr tags;
	int num_pages;
	int num_favorites;
};

struct nhentai_doujin *nhentai_process_doujin(char *buf);
struct nhentai_images *nhentai_process_images(cJSON *images);
struct nhentai_image *nhentai_process_image(cJSON *image);
ArrayPtr nhentai_process_tags(cJSON *tags);

char *nhentai_get_thumbnail(char *media_id);