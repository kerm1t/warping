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


// texture mapping ...
// [...]


const float OUT_W = 1600.0f;
const float OUT_H = 500.0f;

// https://www.flipcode.com/archives/The_Art_of_Demomaking-Issue_10_Roto-Zooming.shtml
// it's not roto zoom, rather a kind of texture mapper
// roto + zoom has to happen outside
void roto_zoom(img* in, img* out, float Ax, float Ay, float Bx, float By, float Cx, float Cy) {
  int IMGRowBytes = in->w * in->channels; // needed for img access
  unsigned char *pti, *pto;

  // compute deltas
  float dxdx = (Bx - Ax) / OUT_W,
    dydx = (By - Ay) / OUT_W,
    dxdy = (Cx - Ax) / OUT_H,
    dydy = (Cy - Ay) / OUT_H;
  long offs = 0;
  // loop for all lines
  for (int j = 0; j < (int)OUT_H; j++)
  {
    Cx = Ax;
    Cy = Ay;
    // for each pixel
    for (int i = 0; i < (int)OUT_W; i++)
    {
      // get texel and store pixel
//      page_draw[offs] = texture[((Cy >> 8) & 0xff00) + ((Cx >> 16) & 0xff)];
      int ix = Cx;
      int iy = Cy;
      int ox = offs % (int)OUT_W;
      int oy = offs / (int)OUT_W;
      if ((ix > 0) && (iy > 0) && (ix < in->w) && (iy < in->h)) { // in + out same size here
        pti = in->dibdata + ix * 3 + iy * IMGRowBytes;
        pto = out->dibdata + ox * 3 + oy * IMGRowBytes;
        memcpy((void*)pto, (void*)pti, 3);
      }
      // interpolate to get next texel in texture space
      Cx += dxdx;
      Cy += dydx;
      offs++;
    }
    // interpolate to get start of next line in texture space
    Ax += dxdy;
    Ay += dydy;
  }
}