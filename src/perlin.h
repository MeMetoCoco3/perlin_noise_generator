#include "vstd/vtypes.h"

void GeneratePerlinNoise(void* buff, int w, int h);
static f32 Perlin(f32 x, f32 y);
static f32 Interpolate(f32 a0, f32 a1, f32 w);
static f32 DotGridGradient(int ix, int iy, f32 x, f32 y);
static vec2 RandomGradient(int ix, int iy);
