#include "sepia_func.h"

static uint8_t sat(uint64_t x) { return x > 255 ? 255 : x; }
static void sepia_one(struct pixel* pix) {
    static float c[3][3] = { { .393f, .769f, .189f },
                             { .349f, .686f, .168f },
                             { .272f, .543f, .131f }
    };
    struct pixel old = *pix;
    pix->r = sat(old.r*c[0][0] + old.g*c[0][1] + old.b*c[0][2]);
    pix->g = sat(old.r*c[1][0] + old.g*c[1][1] + old.b*c[1][2]);
    pix->b = sat(old.r*c[2][0] + old.g*c[2][1] + old.b*c[2][2]);
}

void sepia_c_inplace(struct picture* pic) {
    size_t i, j;
    size_t w = pic->width;
    size_t h = pic->height;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            sepia_one(pic->data + i*w + j);
        }
    }
}


void sepia_sse_block(float* b_block, float* g_block, float* r_block, uint8_t* dst);
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
