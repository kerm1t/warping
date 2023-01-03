#if !defined(GLOBAL__TINYPTC_TEXT_)
#define GLOBAL__TINYPTC_TEXT_
#include <string>
//#include "SDL.h"

void pset(int x, int y, short r, short g, short b);
void loadText(int sW, int sH);
//void doText(int x, int y, char *str, int strLen);
void doText(unsigned char* dibdata, int IMGRowBytes, int x, int y, std::string stri);
void doTextCol(unsigned char* dibdata, int IMGRowBytes, int x, int y, std::string stri, short r, short g, short b);

#endif // !defined(GLOBAL__TINYPTC_TEXT_)
