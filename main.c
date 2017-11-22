#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdint.h>
#include "bmp_struct.h"
#include "bmp_func.h"
#include "sepia_func.h"

void main() {
    FILE* pic_file = fopen("./images/p.bmp", "rb");
    struct bmp_header* pic_header = malloc(sizeof(struct bmp_header));
    struct picture* pic_config = malloc(sizeof(struct picture));
    load_bmp(pic_file, pic_header, pic_config);
    fclose(pic_file);
    pic_file = fopen("./images/rp.bmp", "wb");
    pic_header = rotate_header(pic_header);
    pic_config = rotate_right(pic_config);
    save_bmp(pic_file, pic_header, pic_config);
    fclose(pic_file);

    pic_file = fopen("./images/p.bmp", "rb");
    pic_header = malloc(sizeof(struct bmp_header));
    struct picture* image = malloc(sizeof(struct picture));
    load_bmp(pic_file, pic_header, image);

    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;

    //sepia_c_inplace(image);

    image_sepia_sse(image);

    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
    printf( "Time elapsed in microseconds: %ld\n", res );

    fclose(pic_file);
    pic_file = fopen("./images/sp.bmp", "wb");
    save_bmp(pic_file, pic_header, image);
    fclose(pic_file);
}
