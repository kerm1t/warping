#pragma once
#include "bitmap.hpp"

// the pokcte handbook ... (c) 1993
void warp(img* in, img* out, int sx, int sy, int ex, int ey, int *wx, int *wy) {
  float a, b, c, d, e, f, i, j, x, y, dstx, dsty;
  int dy;  

  a = (float)(-(*wx) + *(wx + 1)) / (ey - sy);
  b = (float)(-(*wx) + *(wx + 3)) / (ex - sx);
  c = (float)((*wx) - *(wx + 1) + *(wx+2) - *(wx+3)) / ((ey - sy) * (ex - sx));
  d = (float)*wx;
  e = (float)(-(*wy) + *(wy + 1)) / (ey - sy);
  f = (float)(-(*wy) + *(wy + 3)) / (ex - sx);
  i = (float)((*wy) - *(wy + 1) + *(wy + 2) - *(wy + 3)) / ((ey - sy) * (ex - sx));
  j = (float)*wy;
  
  int IMGRowBytes = in->w * in->channels; // needed for img access
  unsigned char *pti, *pto;

  for (y = sy; y < ey; y+=0.5) {
    dy = (int)y + 0.5;
    for (x = sx; x < ex;x+=0.5) {
      dstx = a * x + b * y + c * x*y + d;
      dsty = e * x + f * y + i * x*y + j;
//      *(out.data + out.cols*(int)dsty + 0.5) + (int)dstx) = (img.data + (long)(y*img.cols + (long)(x + 0.05)));
      int ix, iy;
      ix = x;
      iy = y;
      int ox, oy;
      ox = dstx;
      oy = dsty;
      pti = in->dibdata + ix * 3 + iy * IMGRowBytes;
      pto = out->dibdata + ox * 3 + oy * IMGRowBytes;
//      memset(pt, 0x11, 3);
      memcpy((void*)pto, (void*)pti, 3);
    }
  }
}
