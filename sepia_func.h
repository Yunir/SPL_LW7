#ifndef _SEPIA_FUNC_H_
#define _SEPIA_FUNC_H_
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp_struct.h"

void image_sepia(struct picture* image);
void image_sepia_sse(struct picture* image);

#endif
