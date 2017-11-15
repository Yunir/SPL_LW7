#include <stdio.h>
#include <stdlib.h>

#ifndef BMP_FUNC_H
    #define BMP_FUNC_H

    void load_bmp(FILE* file, struct bmp_header* header, struct picture* image);
    void save_bmp(FILE* file, struct bmp_header* header, struct picture* image);
    struct picture* rotate_right(struct picture const * source);
    struct picture* rotate_left(struct picture const * source);
    struct bmp_header* rotate_header(struct bmp_header const * old_header);
#endif
