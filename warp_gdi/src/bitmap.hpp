#include "framework.h"

class img {
public:
  unsigned char *img;
  int w, h, channels;
  unsigned char* dibdata; // that's what we need to bitblt!!
  HDC hdcMem;
  int load(std::string filename);
};

class sprite
{
public:
  unsigned char *img; // stbi
  int w, h, channels;
  unsigned char* dibdata; // that's what we need to bitblt!!
  HDC hdcMem; // 
  HDC hdcMemMask; // 
  int iAnim = 0;
  int anim_phases; // number of anim phases, mandatory to set this!
  int load(std::string filename);
};

int img::load(std::string filename)
{
  transform(filename.begin(), filename.end(), filename.begin(), std::tolower);
  if (STR_ENDS_WITH(filename.c_str(), ".jpg") ||
    STR_ENDS_WITH(filename.c_str(), ".jpeg") ||
    STR_ENDS_WITH(filename.c_str(), ".png")) {
    if (!stbi_info(filename.c_str(), &w, &h, &channels)) {
      printf("Couldn't read header %s\n", filename.c_str());
      return 0;
    }
    else {
      img = stbi_load(filename.c_str(), &w, &h, &channels, 0);
      if (img == NULL)
      {
        printf("Error reading %s\n", filename.c_str());
      }
      else
      {
        // the below conversions work great with jpg, partially with png and not good with gif (vertical green stripes)
        // --> check stbi_image
#ifdef USE_BITBLT
        // (1) RGB (stbimage) to BGR (DIBSection)
//          https://stackoverflow.com/questions/34033876/how-to-change-rgb-to-bgr
        unsigned char tmp;
        int raw_image_size = w * h * channels;
        for (int i = 0; i < raw_image_size; i += 3)
        {
          // swap R and B; raw_image[i + 1] is G, so it stays where it is.
          tmp = img[i + 0];
          img[i + 0] = img[i + 2];
          img[i + 2] = tmp;
        }


        // (2) copy image to DIBsection, in order to bitblt it later
        //     seems CreateCompatibleBitmap() could be even faster, but what the heck
        unsigned char* p, *q;
        int IMGRowBytes, DIBRowBytes;

        //        https://stackoverflow.com/questions/7502588/createcompatiblebitmap-and-createdibsection-memory-dcs
        //        https://libredd.it/pqiq24

        BITMAPINFO bi;
        memset(&bi, 0, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h; // upside down
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = channels * 8; // assumed to be 24 bits
        bi.bmiHeader.biCompression = BI_RGB;
#define DIB_HSECTION NULL // system will alloc memory
        HBITMAP m_hBmp = CreateDIBSection(hdcMem, &bi, DIB_RGB_COLORS, (void**)&dibdata, DIB_HSECTION, NULL);
        printf("DIB Handle=%pn", m_hBmp);

        DIBRowBytes = IMGRowBytes = w * channels;
        while (DIBRowBytes & 3) ++DIBRowBytes;  // DIB needs 4n bytes/row

        p = dibdata;
        q = img;

        for (int y = 0; y < h; ++y) {
          memcpy(p, q, IMGRowBytes);
          p += DIBRowBytes;
          q += IMGRowBytes;
        }

        HBITMAP m_hOldBmp = (HBITMAP)SelectObject(hdcMem, m_hBmp);
        DeleteObject(m_hBmp);
        DeleteObject(m_hOldBmp);
#endif

        // ...don't forget to free all mem!
      }
    }
  }
  return 1;
}

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent); // s.below

// this fct() is more generalized as load_img, as it returns the img, thus can load more sprites/imgs
int sprite::load(std::string filename)
{
  transform(filename.begin(), filename.end(), filename.begin(), std::tolower);
  if (STR_ENDS_WITH(filename.c_str(), ".jpg") ||
    STR_ENDS_WITH(filename.c_str(), ".jpeg") ||
    STR_ENDS_WITH(filename.c_str(), ".png")) {
    if (!stbi_info(filename.c_str(), &w, &h, &channels)) {
      printf("Couldn't read header %s\n", filename.c_str());
      return 0;
    }
    else {
      img = stbi_load(filename.c_str(), &w, &h, &channels, 0);
      if (img == NULL)
      {
        printf("Error reading %s\n", filename.c_str());
      }
      else
      {
        // the below conversions work great with jpg, partially with png and not good with gif (vertical green stripes)
        // --> check stbi_image
#ifdef USE_BITBLT
        // (1) RGB (stbimage) to BGR (DIBSection)
//          https://stackoverflow.com/questions/34033876/how-to-change-rgb-to-bgr
        unsigned char tmp;
        int raw_image_size = w * h * channels;
        for (int i = 0; i < raw_image_size; i += 3)
        {
          // swap R and B; raw_image[i + 1] is G, so it stays where it is.
          tmp = img[i + 0];
          img[i + 0] = img[i + 2];
          img[i + 2] = tmp;
        }


        // (2) copy image to DIBsection, in order to bitblt it later
        //     seems CreateCompatibleBitmap() could be even faster, but what the heck
        unsigned char* p, *q;
        int IMGRowBytes, DIBRowBytes;

        //        https://stackoverflow.com/questions/7502588/createcompatiblebitmap-and-createdibsection-memory-dcs
        //        https://libredd.it/pqiq24

        BITMAPINFO bi;
        memset(&bi, 0, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h; // upside down
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = channels * 8; // assumed to be 24 bits
        bi.bmiHeader.biCompression = BI_RGB;
        HBITMAP m_hBmp = CreateDIBSection(hdcMem, &bi, DIB_RGB_COLORS, (void**)&dibdata, NULL, NULL);
        printf("DIB Handle=%pn", m_hBmp);

        DIBRowBytes = IMGRowBytes = w * channels;
        while (DIBRowBytes & 3) ++DIBRowBytes;  // DIB needs 4n bytes/row

        p = dibdata;
        q = img;

        for (int y = 0; y < h; ++y) {
          memcpy(p, q, IMGRowBytes);
          p += DIBRowBytes;
          q += IMGRowBytes;
        }
        //        HBITMAP m_hOldBmp = (HBITMAP)SelectObject(spr.hdcMem, m_hBmp);

                // create mask
        HBITMAP m_hBmpMask = CreateBitmapMask(m_hBmp, RGB(0, 0, 0)); // doesn't work, when object already selected as above 
        SelectObject(hdcMemMask, m_hBmpMask);

        //        DeleteObject(m_hBmp);
        //        DeleteObject(m_hOldBmp);
        DeleteObject(m_hBmpMask);
#endif
        // cannot have dual SelectObject, i.e. bmp assigned to two dc's
        // https://stackoverflow.com/questions/27422871/does-deletedc-automatically-unselect-objects
        HBITMAP m_hOldBmp = (HBITMAP)SelectObject(hdcMem, m_hBmp);
        DeleteObject(m_hOldBmp);
        DeleteObject(m_hBmp);

        // ...don't forget to free all mem!
      }
    }
  }
  return 1;
}



HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
  HDC hdcTmp, hdcTmp2;
  HBITMAP hbmMask;
  BITMAP bm;

  // Create monochrome (1 bit) mask bitmap.  

  GetObject(hbmColour, sizeof(BITMAP), &bm);
  hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

  // Get some HDCs that are compatible with the display driver

  hdcTmp = CreateCompatibleDC(0);
  hdcTmp2 = CreateCompatibleDC(0);

  SelectBitmap(hdcTmp, hbmColour);
  SelectBitmap(hdcTmp2, hbmMask);

  // Set the background colour of the colour image to the colour
  // you want to be transparent.
  SetBkColor(hdcTmp, crTransparent);

  // Copy the bits from the colour image to the B+W mask... everything
  // with the background colour ends up white while everythig else ends up
  // black...Just what we wanted.

  BitBlt(hdcTmp2, 0, 0, bm.bmWidth, bm.bmHeight, hdcTmp, 0, 0, SRCCOPY);

  // Take our new mask and use it to turn the transparent colour in our
  // original colour image to black so the transparency effect will
  // work right.
  BitBlt(hdcTmp, 0, 0, bm.bmWidth, bm.bmHeight, hdcTmp2, 0, 0, SRCINVERT);

  // Clean up.

  DeleteDC(hdcTmp);
  DeleteDC(hdcTmp2);

  return hbmMask;
}
