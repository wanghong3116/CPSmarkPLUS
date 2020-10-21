#ifndef PLATFORM_PAINT_H_86D47E1E_B48E_457C_893F_5E1FE865650E
#define PLATFORM_PAINT_H_86D47E1E_B48E_457C_893F_5E1FE865650E

#include "PlatformHeader.h"

namespace MyPlatform
{

int ChangeBitmap24Data( char* pcDstData, const char* pcSrcData, int nImgW, int nImgH,bool bAlignment );

HBITMAP CreateBitmap24(HDC hDC, char* pcImgData, int nWidth, int nHeight, char** ppcBmpData, bool bAlignment );
HBITMAP CreateBitmap32(HDC hDC,  int nWidth, int nHeight, unsigned char* pucImgIn = 0, unsigned char** ppucOutBmpBuf = 0);

int GetRGBImgValidPart( const unsigned char* pcucSrcImgData, int nSrcImgW, int nSrcImgH, unsigned char** ppucDstImgData, int* pnDstImgW, int* pnDstImgH, COLORREF dwInvalidColor, bool bAlignment );

int SaveBmp24( const char* szBmpFP, const unsigned char* pcucImgData, int nImgW, int nImgH, bool bAlignment /*= true*/ );
int SaveBmp8(const char *szBmpFP, const unsigned char *pucImgData, int nImgW, int nImgH, int nBufLineBytes);

int UYVYImageDrawToUYVYBackground(unsigned char *pucDst, int nDstW, int nDstH, int nDrawX, int nDrawY,
	const unsigned char *pucImage, int nImW, int nImgH, const unsigned char *pucAlpha, int nAlphaLineBytes);

int UYVYToBGR32_Native(unsigned char* pucBGR32, int nWidth, int nHeight, unsigned char ucAlpha, const unsigned char* pucYUV);
int rgb322uyvy(unsigned char* pBGRAData, int nBGRAPitch, unsigned char* pucYUV, int nWidth, int nHeight);

int getRGB32Alpha(const unsigned char *pucRGBA, int nBGRAPitch, int nImgW, int nImgH, unsigned char *pucAlpha, int nAlphaLineBytes);
};

#endif
