#include "sepia_func.h"

static uint8_t cut_to_255(uint64_t x) { return x > 255 ? 255 : x; }
static void pixel_sepia(struct pixel* pix) {
    static float c[3][3] = { { .393f, .769f, .189f },
                             { .349f, .686f, .168f },
                             { .272f, .543f, .131f }
    };
    struct pixel temp = *pix;
    pix->r = cut_to_255(temp.r*c[0][0] + temp.g*c[0][1] + temp.b*c[0][2]);
    pix->g = cut_to_255(temp.r*c[1][0] + temp.g*c[1][1] + temp.b*c[1][2]);
    pix->b = cut_to_255(temp.r*c[2][0] + temp.g*c[2][1] + temp.b*c[2][2]);
}

void image_sepia(struct picture* pic) {
    size_t i, j;
    size_t w = pic->width;
    size_t h = pic->height;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixel_sepia(pic->data + i*w + j);
        }
    }
}


void sepia_sse_block(float* b_chan, float* g_chan, float* r_chan, uint8_t* dst);
void image_sepia_sse(struct picture* image) {
    size_t i, j;
    size_t w = image->width;
    size_t h = image->height;

    float b_block[4];
    float g_block[4];
    float r_block[4];
    uint8_t blockff[12];

    struct pixel* pix_tmp;
    for (i = 0; i < w*h; i += 4) {
        for (j = 0; j < 4; j++) {
            pix_tmp  = (image->data + i + j);
            b_block[j] = pix_tmp->b;
            g_block[j] = pix_tmp->g;
            r_block[j] = pix_tmp->r;
        }
        sepia_sse_block(b_block, g_block, r_block, blockff);
        for (j = 0; j < 4; j++) {
            pix_tmp = (image->data + i + j);
            pix_tmp->b = *(blockff + 3*j);
            pix_tmp->g = *(blockff + 3*j + 1);
            pix_tmp->r = *(blockff + 3*j + 2);
        }
    }
}
