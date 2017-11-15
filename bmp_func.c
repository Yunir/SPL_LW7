#include "bmp_struct.h"
#include "bmp_func.h"

void load_bmp(FILE* file, struct bmp_header* header, struct picture* image) {
    //fetch header
    fread(header, sizeof(struct bmp_header), 1, file);
    if (header->biSizeImage == 0) header->biSizeImage = header->bfileSize;
    image->width = header->biWidth;
    image->height = header->biHeight;
    int padding = (4 - (header->biWidth * sizeof(struct pixel)) % 4) % 4;
    image->data = (struct pixel*)malloc((header->biWidth* sizeof(struct pixel) + padding) * header->biHeight);
    //fetch body
    for (int i = 0; i < image->height; ++i) {
        fread(image->data + i*image->width, sizeof(struct pixel), image->width, file);
		fseek(file, padding, SEEK_CUR);
    }
}

struct picture* rotate_left(struct picture const * source) {
	struct picture* new_img = (struct picture*)malloc(sizeof(struct picture));
	new_img->width = source->height;
	new_img->height = source->width;
	new_img->data = (struct pixel*)malloc(sizeof(struct pixel)*new_img->width*new_img->height);
	int i, j;
	for (i = 0; i<new_img->height; i++) {
		for (j = 0; j<new_img->width; j++) {
			*(new_img->data + i*new_img->width + j) = *(source->data + (new_img->width - j - 1)*new_img->height + i);
		}
	}
	return new_img;
}
struct picture* rotate_right(struct picture const * source) {
	struct picture* new_img = (struct picture*)malloc(sizeof(struct picture));
	new_img->width = source->height;
	new_img->height = source->width;
	new_img->data = (struct pixel*)malloc(sizeof(struct pixel)*new_img->width*new_img->height);
	int i, j;
	for (i = 0; i<new_img->height; i++) {
		for (j = 0; j<new_img->width; j++) {
			*(new_img->data + i*new_img->width + j) = *(source->data + j*source->width + (new_img->width-1-i));
		}
	}
	return new_img;
}

struct bmp_header* rotate_header(struct bmp_header const * old_header) {
	uint32_t new_width = old_header->biHeight;
	uint32_t new_height = old_header->biWidth;
	const int padding = (4 - (new_width * sizeof(struct pixel)) % 4) % 4;
	uint32_t new_fileSize = sizeof(struct bmp_header) + (new_width + padding)*new_height;
	uint32_t new_sizeImage = (new_width + padding)*new_height;
	struct bmp_header* new_header = (struct bmp_header*)malloc(sizeof(struct bmp_header));
	new_header->biHeight = new_height;
	new_header->biWidth = new_width;
	new_header->bfileSize = new_fileSize;
	new_header->biSizeImage = new_sizeImage;
	return new_header;
}

void save_bmp(FILE* file, struct bmp_header* header, struct picture* image) {
    int padding = (4 - (header->biWidth * sizeof(struct pixel)) % 4) % 4;
    struct bmp_header new_header = { 0x4D42, sizeof(struct bmp_header) + (image->width* sizeof(struct pixel) + padding)*image->height, 0, 54, 40, image->width, image->height, 1, 24, 0, (image->width* sizeof(struct pixel)  + padding)*image->height, 0, 0, 0, 0 };
    fwrite(&new_header, sizeof(struct bmp_header), 1, file);
    for (int i = 0; i < image->height; ++i) {
        fwrite(image->data + i*image->width, sizeof(struct pixel), image->width, file);
		fseek(file, padding, SEEK_CUR);
    }
}
