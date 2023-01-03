#include <iostream>
#include "text.h"
#include "framework.h"

int clip_x0,clip_x1,clip_y0,clip_y1;	// momentary clipping-area, is usually a gui_Window

/*
	Here is some source code that i made for writing some text with tinyPTC,
	you can modify it to plot alfa-blended lights instead of just white dots
	for each character. u should call loadText() for initialization, and doText()
	on the main drawing loop.

	Andr� Barata (pr0b3r@users.sourceforge.net)
	https://sourceforge.net/forum/forum.php?thread_id=980269&forum_id=17619
*/

// Text-Bitmap (155 x 12)
const unsigned char bmp[240] = {
0x67, 0x19, 0xCF, 0x79, 0xD2, 0x70, 0xA5, 0x09, 0x49, 0x9C, 0x67, 0x1C, 0xE9, 0x4A, 0x52, 0x97, 0x94, 0x30, 0x00, 0x00,
0x94, 0xA5, 0x28, 0x42, 0x12, 0x20, 0xA9, 0x0F, 0x4A, 0x52, 0x94, 0xA0, 0x49, 0x4A, 0x52, 0x90, 0x94, 0x48, 0x00, 0x00,
0xF7, 0x21, 0x2E, 0x72, 0xDE, 0x20, 0xB1, 0x0F, 0x6A, 0x5C, 0x97, 0x18, 0x49, 0x4A, 0x4C, 0x61, 0x00, 0xB4, 0x00, 0x00,
0x94, 0xA1, 0x28, 0x42, 0x52, 0x24, 0xB1, 0x09, 0x5A, 0x50, 0xD6, 0x04, 0x49, 0x4B, 0xCC, 0x22, 0x00, 0xBC, 0x00, 0x00,
0x94, 0xA5, 0x28, 0x42, 0x52, 0x24, 0xA9, 0x09, 0x4A, 0x50, 0xB5, 0x04, 0x49, 0x33, 0xD2, 0x24, 0x00, 0x40, 0x00, 0x00,
0x97, 0x19, 0xCF, 0x41, 0x92, 0x73, 0x25, 0xE9, 0x49, 0x90, 0x74, 0xB8, 0x46, 0x32, 0x52, 0x27, 0x80, 0x30, 0x00, 0x00,
0x61, 0x39, 0xE2, 0x79, 0x9E, 0x63, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x40, 0x01, 0x86, 0x21, 0x18, 0x00, 0x11, 0x80,
0x93, 0x04, 0x46, 0x42, 0x02, 0x94, 0x80, 0x00, 0x0A, 0x00, 0x22, 0x00, 0x48, 0x05, 0x02, 0x52, 0x9A, 0x07, 0x3A, 0x40,
0xB1, 0x04, 0xCA, 0x73, 0x84, 0x64, 0x80, 0x06, 0x11, 0x00, 0x02, 0x01, 0xF4, 0x09, 0x02, 0x89, 0x04, 0x00, 0x30, 0x40,
0xD1, 0x18, 0x2F, 0x0A, 0x48, 0x93, 0x80, 0x00, 0x11, 0x1E, 0x02, 0x00, 0x42, 0x11, 0x02, 0x02, 0xC8, 0x07, 0x19, 0x80,
0x91, 0x21, 0x22, 0x0A, 0x48, 0x90, 0x80, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x41, 0x21, 0x02, 0x02, 0x96, 0x40, 0x38, 0x00,
0x61, 0x3C, 0xC2, 0x71, 0x88, 0x63, 0x2B, 0x80, 0x00, 0x00, 0x02, 0x3E, 0x00, 0xC1, 0x86, 0x01, 0x46, 0x40, 0x11, 0x80
};
const int bmpW = 155, bmpH = 12, rowByteAlign = 20;
// Zeichenbreite 5, Zeichenh�he 6
const int tCharW = 5, tCharH = 6;
// Aufteilung der Zeichen
//const char *tString = "abcdefghijklmnopqrstuvwxyz\"@* 0123456789;.:()-'!_+\\/[]^&%,=$?";
// die Zeichen \ und " sind nicht escaped
const char *tString = "abcdefghijklmnopqrstuvwxyz\\\"@* 0123456789;.:()-'!_+\\/[]^&%,=$?";

// ------------------
//
//
// ein "*" schreiben funktioniert nicht
//
//
// ------------------
const int tCharXnr = bmpW/tCharW, tCharYnr = bmpH/tCharH, tCharNr = tCharXnr*tCharYnr;
int *screen;
int screenW, screenH;
typedef struct {
	int plot[tCharW*tCharH][2];
	int plotNr;
} tChar_type;
tChar_type tChar[tCharNr];

// >> private
void pset(int x, int y, short r, short g, short b) {
	if (x<0 || x > screenW || y < 0 || y > screenH) return;	// ws
	screen[y*screenW+x] = (r<<16)|(g<<8)|b;
}

// >> private und dann im Konstruktor
void loadText(int sW, int sH) {
	int x, y, t, r, cnr;
//	screen = scr;
  screenW = sW; screenH = sH;

	for (y=0; y<bmpH; y++)
		for (x=0; x<bmpW; x++) {
			t = (y*rowByteAlign) + (x/8);
			r = 1 << (7-(x%8));
			if (bmp[t] & r ) {
				cnr = (x/tCharW)+(y/tCharH*tCharXnr);
				tChar[cnr].plot[tChar[cnr].plotNr][0] = x % tCharW;
				tChar[cnr].plot[tChar[cnr].plotNr][1] = y % tCharH;
				tChar[cnr].plotNr++;
			}
		}
}

// >> public
// Parameter sollten noch durch c++ string ersetzt werden
//void doText(int x, int y, char *str, int strLen);
void doTextCol(unsigned char* dibdata, int IMGRowBytes, int x, int y, std::string stri, short r, short g, short b) {
  int i, j, p;
  int pixel_x, pixel_y;

  //	char *str = "made by andre barata, 2003";
  //	int strLen = 26;
  unsigned char* pt;

  char *str = (char*)stri.c_str();
  int strLen = stri.length();

  // clear first
  for (i = 0; i < 20; i++) { // 20 = hack
    for (int k = 0; k < 6; k++) {
      for (int l = 0; l < 10; l++) {
        pixel_x = x + k + (i * 6);
        pixel_y = y + l;
        pt = dibdata + pixel_x * 3 + pixel_y * IMGRowBytes;
        memset(pt, 0x11, 3);
      }
    }
  }
  for (i=0; i<strLen; i++)
		for (j=0; j<tCharNr; j++)
			if (str[i] == tString[j])
				for (p=0; p<tChar[j].plotNr; p++)
				{
					// gui_Window - Fenster!!
					pixel_x = x + tChar[j].plot[p][0]+(i*6);
					pixel_y = y + tChar[j].plot[p][1];
					// den Text hier in grau ausdrucken, damit er sich von der Grafik abhebt
//					if (pixel_x>clip_x0 && pixel_x<clip_x1 && pixel_y>clip_y0 && pixel_y<clip_y1)
//						pset(pixel_x, pixel_y , r,g,b);
///            SDL_RenderDrawPoint(rndr, pixel_x, pixel_y);
            pt = dibdata + pixel_x * 3 + pixel_y * IMGRowBytes;
            memset(pt, 0xff, 3);
				}
}

// >> public
// Parameter sollten noch durch c++ string ersetzt werden
//void doText(int x, int y, char *str, int strLen);
void doTextCol_2X(unsigned char* dibdata, int IMGRowBytes, int x, int y, std::string stri, short r, short g, short b) {
  int i, j, p;
  int pixel_x, pixel_y;

  //	char *str = "made by andre barata, 2003";
  //	int strLen = 26;

  char *str = (char*)stri.c_str();
  int strLen = stri.length();

  for (i = 0; i < strLen; i++)
    for (j = 0; j < tCharNr; j++)
      if (str[i] == tString[j])
        for (p = 0; p < tChar[j].plotNr; p++)
        {
          // gui_Window - Fenster!!
          pixel_x = x + tChar[j].plot[p][0] + (i * 12); // 12
          pixel_y = y + tChar[j].plot[p][1]*2; // stretch y
          // den Text hier in grau ausdrucken, damit er sich von der Grafik abhebt
//					if (pixel_x>clip_x0 && pixel_x<clip_x1 && pixel_y>clip_y0 && pixel_y<clip_y1)
//						pset(pixel_x, pixel_y , r,g,b);
///          SDL_RenderDrawPoint(rndr, pixel_x, pixel_y);
///          SDL_RenderDrawPoint(rndr, pixel_x+1, pixel_y);
        }
}

void doText(unsigned char* dibdata, int IMGRowBytes, int x, int y, std::string stri) {
	short r,g,b;
	r = g = b = 100;
	doTextCol(dibdata, IMGRowBytes, x, y, stri, r, g, b);
//	doTextCol(x, y, stri, 155, 155, 155);
}