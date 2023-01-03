#include "framework.h"

/*
void line(unsigned char* p, int IMGRowBytes, int x0, int y0, int x1, int y1)
{
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2; // error value e_xy

  while (1) {
    ///    setPixel(x0, y0);
    p = dibdata + x0 * 3 + y0 * IMGRowBytes;
    memset(p += 3, 0xff, 3);

    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 > dy) { err += dy; x0 += sx; } // e_xy+e_x > 0
    if (e2 < dx) { err += dx; y0 += sy; } // e_xy+e_y < 0
  }
}


void grid() {
  unsigned char* p;
  int IMGRowBytes = w * channels;

  // pixel1: b,g,r, pixel2: b,g,r
  // some horizontal lines
  p = dibdata + 10 * IMGRowBytes;
  for (int x = 0; x < w * 3; ++x) {
    memset(p++, 0xff, 1);
  }
  p = dibdata + 20 * IMGRowBytes;
  for (int x = 0; x < w * 3; ++x) {
    memset(p++, 0xff, 1);
  }
  // blue line :-)
  p = dibdata + 30 * IMGRowBytes;
  for (int x = 0; x < w; ++x) {
    memset(p += 3, 0xff, 1);
  }
  // green line :-)
  p = dibdata + 40 * IMGRowBytes + 1;
  for (int x = 0; x < w; ++x) {
    memset(p += 3, 0xff, 1);
  }
  // red line :-)
  p = dibdata + 50 * IMGRowBytes + 2;
  for (int x = 0; x < w; ++x) {
    memset(p += 3, 0xff, 1);
  }
  // cyan line :-)
  p = dibdata + 60 * IMGRowBytes;
  for (int x = 0; x < w; ++x) {
    memset(p += 3, 0xff, 2);
  }

  // some vertical lines -> 3 "bitplanes" b,g,r ???
  p = dibdata;
  for (int y = 0; y < h - 5; ++y) {
    memset(p += IMGRowBytes, 0xff, 1);
  }
  p = dibdata + 100;
  for (int y = 0; y < h - 5; ++y) {
    memset(p += IMGRowBytes, 0xff, 1);
  }
  p = dibdata + 200;
  for (int y = 0; y < h - 5; ++y) {
    memset(p += IMGRowBytes, 0xff, 1);
  }
  p = dibdata + 300;
  for (int y = 0; y < h - 5; ++y) {
    memset(p += IMGRowBytes, 0xff, 1);
  }

  // diagonal line
//        p = dibdata;
  line(p, IMGRowBytes, 0, 0, w - 1, h - 1);
}
*/