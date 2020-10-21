#include "stdafx.h"

#include "PlatformHeader.h"
#include "PlatromPaint.h"
#include "MyAlgorithm.h"
#include <stdio.h>

#include "PlatformDefine.h"
#include <stdlib.h>

namespace MyPlatform
{

	int ChangeBitmap24Data(char* pcDstData, const char* pcSrcData, int nImgW, int nImgH, bool bAlignment)
	{
		int nLineByte = ((nImgW * 3 + 3) / 4) * 4;

		if (bAlignment)
		{
			memcpy(pcDstData, pcSrcData, nLineByte * nImgH);
		}
		{
			crCopy(pcDstData, nLineByte, nImgH, 0, 0, pcSrcData, nImgW * 3, nImgH, 0, 0, nImgW * 3, nImgH);
		}

		return 1;
	}

	HBITMAP CreateBitmap24(HDC hDC, char* pcImgData, int nWidth, int nHeight, char** ppcBmpData, bool bAlignment)
	{
		BITMAPINFO bi = { 0 };
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = nWidth;
		bi.bmiHeader.biHeight = nHeight;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 24;
		bi.bmiHeader.biCompression = BI_RGB;

		HBITMAP hBmp = CreateDIBSection(hDC, &bi, DIB_RGB_COLORS, (void**)ppcBmpData, NULL, 0);

		if (NULL != pcImgData)
		{
			ChangeBitmap24Data(*ppcBmpData, (const char*)pcImgData, nWidth, nHeight, bAlignment);
		}

		return hBmp;
	}

	HBITMAP CreateBitmap32(HDC hDC, int nWidth, int nHeight, unsigned char* pucImgIn, unsigned char** ppucOutBmpBuf)
	{
		BITMAPINFO bi = { 0 };
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = nWidth;
		bi.bmiHeader.biHeight = nHeight;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biCompression = BI_RGB;

		unsigned char *pucBmpImgData = NULL;

		HBITMAP hBmp = CreateDIBSection(hDC, &bi, DIB_RGB_COLORS, (void**)&pucBmpImgData, NULL, 0);

		if (NULL != pucImgIn)
		{
			memcpy(pucBmpImgData, pucImgIn, nWidth * nHeight * 4);
		}
		if (NULL != ppucOutBmpBuf)
		{
			*ppucOutBmpBuf = pucBmpImgData;
		}
		return hBmp;
	}

	int GetRGBImgValidPart(const unsigned char* pcucSrcImgData, int nSrcImgW, int nSrcImgH, unsigned char** ppucDstImgData, int* pnDstImgW, int* pnDstImgH, COLORREF dwInvalidColor, bool bAlignment)
	{
		if (NULL == pcucSrcImgData)
		{
			return -1;
		}

		int nInvalidR = GetRValue(dwInvalidColor);
		int nInvalidG = GetGValue(dwInvalidColor);
		int nInvalidB = GetBValue(dwInvalidColor);
		RECT rectValid = { nSrcImgW, nSrcImgH, 0, 0 };
		int nLineByte = ((nSrcImgW * 3 + 3) / 4) * 4;
		const unsigned char* pcucCurPixel = pcucSrcImgData;

		for (int nY = 0; nY < nSrcImgH; nY++)
		{
			pcucCurPixel = bAlignment ? pcucSrcImgData + nY * nLineByte : pcucCurPixel;
			for (int nX = 0; nX < nSrcImgW; nX++, pcucCurPixel += 3)
			{
				if (pcucCurPixel[0] >= nInvalidR && pcucCurPixel[1] >= nInvalidG && pcucCurPixel[2] >= nInvalidB)
				{
					continue;
				}

				rectValid.left = __min(rectValid.left, nX);
				rectValid.top = __min(rectValid.top, nY);
				rectValid.right = __max(rectValid.right, nX);
				rectValid.bottom = __max(rectValid.bottom, nY);
			}
		}
		if (rectValid.right - rectValid.left <= 0 || rectValid.bottom - rectValid.top <= 0)
		{
			return -2;
		}

		*pnDstImgW = rectValid.right - rectValid.left;
		*pnDstImgH = rectValid.bottom - rectValid.top;
		*ppucDstImgData = (unsigned char*)malloc((*pnDstImgW) * (*pnDstImgH) * sizeof(unsigned char) * 3);
		if (NULL == *ppucDstImgData)
		{
			return -3;
		}

		return crCopy(*ppucDstImgData, *pnDstImgW * 3, *pnDstImgH, 0, 0, pcucSrcImgData, bAlignment ? nLineByte : nSrcImgW * 3, nSrcImgH, rectValid.left * 3, rectValid.top, *pnDstImgW * 3, *pnDstImgH);
	}

	int SaveBmp24(const char* szBmpFP, const unsigned char* pcucImgData, int nImgW, int nImgH, bool bAlignment /*= true*/)
	{
		unsigned char* pucDstImgData = NULL;
		int nRecordH = nImgH;
		int nLineByte = 0;
		BITMAPFILEHEADER bfh = { 0 };
		BITMAPINFOHEADER bih = { 0 };
		FILE* fp = NULL;

		nImgH = ABS(nImgH);
		nLineByte = (nImgW * 3 + 3) / 4 * 4;
		bfh.bfType = 0x4d42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nLineByte * nImgH;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biBitCount = 24;
		bih.biWidth = nImgW;
		bih.biHeight = nRecordH;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		bih.biCompression = BI_RGB;

		try
		{
			pucDstImgData = (unsigned char*)malloc(nImgH * nLineByte * sizeof(unsigned char));
			if (NULL == pucDstImgData)
			{
				throw - 1;
			}
			for (int nY = 0; nY < nImgH; nY++)
			{
				unsigned char* pucDst = pucDstImgData + nY * nLineByte;
				const unsigned char* pucSrc = pcucImgData + nY * (bAlignment ? nLineByte : nImgW * 3);
				memcpy(pucDst, pucSrc, nImgW * 3);
			}

			//ChangeBitmap24Data((char*)pucDstImgDat, (const char*)pcucImgData, nImgW, nImgH, bAlignment);

			fopen_s(&fp, szBmpFP, "wb");
			if (NULL == fp)
			{
				throw - 2;
			}
			fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
			fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
			fwrite(pucDstImgData, nImgH * nLineByte * sizeof(unsigned char), 1, fp);

			throw PLATFORM_FUNC_NOERROR;

		}
		catch (int nRet)
		{
			FREE(pucDstImgData);
			FCLOSE(fp);
			return nRet;
		}
	}


	int SaveBmp8(const char *szBmpFP, const unsigned char *pucImgData, int nImgW, int nImgH, int nSrcLineBytes)
	{
		unsigned char* pucDstImgData = NULL;
		int nRecordH = nImgH;
		int nDstLineByte = 0;
		BITMAPFILEHEADER bfh = { 0 };
		BITMAPINFOHEADER bih = { 0 };
		FILE* fp = NULL;

		nImgH = ABS(nImgH);
		nDstLineByte = BITMAPLINEBYTES(nImgW, 8);
		bfh.bfType = 0x4d42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nDstLineByte * nImgH;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biBitCount = 8;
		bih.biWidth = nImgW;
		bih.biHeight = nRecordH;
		bih.biPlanes = 1;
		bih.biBitCount = 8;
		bih.biCompression = BI_RGB;

		try
		{
			pucDstImgData = (unsigned char*)malloc(nImgH * nDstLineByte * sizeof(unsigned char));
			if (NULL == pucDstImgData)
			{
				throw - 1;
			}
			for (int nY = 0; nY < nImgH; nY++)
			{
				unsigned char* pucDst = pucDstImgData + nY * nDstLineByte;
				const unsigned char* pucSrc = pucImgData + nY * nSrcLineBytes;
				memcpy(pucDst, pucSrc, nSrcLineBytes);
			}

			//ChangeBitmap24Data((char*)pucDstImgDat, (const char*)pcucImgData, nImgW, nImgH, bAlignment);

			fopen_s(&fp, szBmpFP, "wb");
			if (NULL == fp)
			{
				throw - 2;
			}
			fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
			fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
			fwrite(pucDstImgData, nImgH * nDstLineByte * sizeof(unsigned char), 1, fp);

			throw PLATFORM_FUNC_NOERROR;

		}
		catch (int nRet)
		{
			FREE(pucDstImgData);
			FCLOSE(fp);
			return nRet;
		}
	}

	int UYVYToBGR24_Native(unsigned char* pucBGR24, int nWidth, int nHeight, int nRGBLineBytes, const unsigned char* pucYUV)
	{
		if (nWidth < 1 || nHeight < 1 || pucYUV == NULL || pucBGR24 == NULL)
		{
			return -1;
		}

		const unsigned char *pucYUVLine = pucYUV;
		RGBTRIPLE *prgbTriple = NULL;
		unsigned char ucU = 0, ucV = 0, ucY = 0;

		for (int nY = 0; nY < nHeight; nY++)
		{
			pucYUVLine = pucYUV + nY * nWidth * 2;
			prgbTriple = (RGBTRIPLE *)(pucBGR24 + nY * nRGBLineBytes);
			for (int nX = 0; nX < nWidth; nX += 2)
			{
				ucU = pucYUVLine[0];
				ucY = pucYUVLine[1];
				ucV = pucYUVLine[2];

				prgbTriple->rgbtBlue = (unsigned char)(ucY + 1.7790 * (ucU - 128) + 0.5);
				prgbTriple->rgbtBlue = (unsigned char)(ucY - 0.3455 * (ucU - 128) - (0.7169 * (ucV - 128)));
				prgbTriple->rgbtRed = (unsigned char)(ucY + 1.4075 * (ucV - 128));
				prgbTriple++;

				ucY = pucYUVLine[3];
				prgbTriple->rgbtBlue = (unsigned char)(ucY + 1.7790 * (ucU - 128) + 0.5);
				prgbTriple->rgbtBlue = (unsigned char)(ucY - 0.3455 * (ucU - 128) - (0.7169 * (ucV - 128)));
				prgbTriple->rgbtRed = (unsigned char)(ucY + 1.4075 * (ucV - 128));

				pucYUVLine += 4;
			}
		}

		return 0;
	}


	int UYVYToBGR32_Native(unsigned char* pucBGR32, int nWidth, int nHeight, unsigned char ucAlpha, const unsigned char* pucYUV)
	{
		if (nWidth < 1 || nHeight < 1 || pucYUV == NULL || pucBGR32 == NULL)
		{
			return -1;
		}

		const unsigned char *pucYUVLine = pucYUV;
		RGBQUAD *prgbTriple = (RGBQUAD *)(pucBGR32);
		unsigned char ucU = 0, ucV = 0, ucY = 0;

		for (int nY = 0; nY < nHeight; nY++)
		{
			pucYUVLine = pucYUV + nY * nWidth * 2;
			for (int nX = 0; nX < nWidth; nX += 2)
			{
				ucU = pucYUVLine[0];
				ucY = pucYUVLine[1];
				ucV = pucYUVLine[2];

				prgbTriple->rgbBlue = (unsigned char)(ucY + 1.7790 * (ucU - 128) + 0.5);
				prgbTriple->rgbGreen = (unsigned char)(ucY - 0.3455 * (ucU - 128) - (0.7169 * (ucV - 128)));
				prgbTriple->rgbRed = (unsigned char)(ucY + 1.4075 * (ucV - 128));
				prgbTriple->rgbReserved = ucAlpha;
				prgbTriple++;

				ucY = pucYUVLine[3];
				prgbTriple->rgbBlue = (unsigned char)(ucY + 1.7790 * (ucU - 128) + 0.5);
				prgbTriple->rgbGreen = (unsigned char)(ucY - 0.3455 * (ucU - 128) - (0.7169 * (ucV - 128)));
				prgbTriple->rgbRed = (unsigned char)(ucY + 1.4075 * (ucV - 128));
				prgbTriple->rgbReserved = ucAlpha;
				prgbTriple++;

				pucYUVLine += 4;
			}
		}

		return 0;
	}

int UYVYImageDrawToUYVYBackground(unsigned char *pucDst, int nDstW, int nDstH, int nDrawX, int nDrawY,
	const unsigned char *pucImage, int nImW, int nImgH, const unsigned char *pucAlpha, int nAlphaLineBytes)
{
	if (nDrawX > nDstW
		|| nDrawY > nDstH)
	{
		return -1;
	}

	nDrawX = (0 == nDrawX % 2) ? nDrawX : 0;
	int nEndX = __min(nDrawX + nImW, nDstW);
	int nEndY = __min(nDrawY + nImgH, nDstH);

	unsigned char *pucDstOffset = NULL;
	const unsigned char *pucImgOffset = NULL;
	const unsigned char *pucAlphaOffset = NULL;

	nDrawX *= 2;
	nEndX *= 2;
	for (int nY = nDrawY; nY < nEndY; nY++)
	{
		pucDstOffset = pucDst + nDstW * 2 * nY;
		pucImgOffset = pucImage + (nY - nDrawY) * nImW * 2;
		pucAlphaOffset = (nY - nDrawY) * nAlphaLineBytes + pucAlpha;
		for (int nX = nDrawX, nImgXIdx = 0; nX < nEndX; nX++, nImgXIdx++)
		{
			pucDstOffset[nX] = (pucImgOffset[nImgXIdx] * pucAlphaOffset[nImgXIdx / 2] + pucDstOffset[nX] * (255 - pucAlphaOffset[nImgXIdx / 2])) / 255;
		}
	}

	return 0;
}


//// RGB -> YUV 从网络查询的算法
void rgb24_yuy2(void* rgb, void* yuy2, int width, int height)
{
	int R1, G1, B1, R2, G2, B2, Y1, U1, Y2, V1;
	unsigned char* pRGBData = (unsigned char *)rgb;
	unsigned char* pYUVData = (unsigned char *)yuy2;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width / 2; ++j)
		{
			B1 = *(pRGBData + (height - i - 1)*width * 3 + j * 6);
			G1 = *(pRGBData + (height - i - 1)*width * 3 + j * 6 + 1);
			R1 = *(pRGBData + (height - i - 1)*width * 3 + j * 6 + 2);
			B2 = *(pRGBData + (height - i - 1)*width * 3 + j * 6 + 3);
			G2 = *(pRGBData + (height - i - 1)*width * 3 + j * 6 + 4);
			R2 = *(pRGBData + (height - i - 1)*width * 3 + j * 6 + 5);

			Y1 = ((66 * R1 + 129 * G1 + 25 * B1 + 128) >> 8) + 16;
			U1 = (((-38 * R1 - 74 * G1 + 112 * B1 + 128) >> 8) + ((-38 * R2 - 74 * G2 + 112 * B2 + 128) >> 8)) / 2 + 128;
			Y2 = ((66 * R2 + 129 * G2 + 25 * B2 + 128) >> 8) + 16;
			V1 = (((112 * R1 - 94 * G1 - 18 * B1 + 128) >> 8) + ((112 * R2 - 94 * G2 - 18 * B2 + 128) >> 8)) / 2 + 128;

			*(pYUVData + i*width * 2 + j * 4) = __max(__min(Y1, 255), 0);
			*(pYUVData + i*width * 2 + j * 4 + 1) = __max(__min(U1, 255), 0);
			*(pYUVData + i*width * 2 + j * 4 + 2) = __max(__min(Y2, 255), 0);
			*(pYUVData + i*width * 2 + j * 4 + 3) = __max(__min(V1, 255), 0);
		}
	}
}

int rgb322uyvy(unsigned char* pBGRAData, int nBGRAPitch, unsigned char* pucYUV, int nWidth, int nHeight)
{
	int R1, G1, B1, R2, G2, B2, Y1, U1, Y2, V1;
	//unsigned char* pRGBData = (unsigned char *)pRGBData;
	unsigned char* pucYUVLine = (unsigned char *)pucYUV;
	unsigned char* pucRGBALine = NULL;

	int nEndX = nWidth / 2;
	int nYUVLineBytes = nWidth * 2;
	for (int nY = 0; nY < nHeight; ++nY)
	{
		pucYUVLine = nYUVLineBytes * nY + pucYUV;
		pucRGBALine = pBGRAData + nBGRAPitch * nY;
		for (int nX = 0; nX < nEndX; ++nX)
		{
			B1 = pucRGBALine[0];//*(pBGRData + (nHeight - nY - 1)*nWidth * 3 + nX * 6);
			G1 = pucRGBALine[1];//*(pBGRData + (nHeight - nY - 1)*nWidth * 3 + nX * 6 + 1);
			R1 = pucRGBALine[2];//*(pBGRData + (nHeight - nY - 1)*nWidth * 3 + nX * 6 + 2);
			pucRGBALine += 4;

			B2 = pucRGBALine[0];//*(pBGRData + (nHeight - nY - 1)*nWidth * 3 + nX * 6 + 3);
			G2 = pucRGBALine[1];//*(pBGRData + (nHeight - nY - 1)*nWidth * 3 + nX * 6 + 4);
			R2 = pucRGBALine[2];//*(pBGRData + (nHeight - nY - 1)*nWidth * 3 + nX * 6 + 5);
			pucRGBALine += 4;

			Y1 = ((66 * R1 + 129 * G1 + 25 * B1 + 128) >> 8) + 16;
			U1 = (((-38 * R1 - 74 * G1 + 112 * B1 + 128) >> 8) + ((-38 * R2 - 74 * G2 + 112 * B2 + 128) >> 8)) / 2 + 128;
			Y2 = ((66 * R2 + 129 * G2 + 25 * B2 + 128) >> 8) + 16;
			V1 = (((112 * R1 - 94 * G1 - 18 * B1 + 128) >> 8) + ((112 * R2 - 94 * G2 - 18 * B2 + 128) >> 8)) / 2 + 128;

			pucYUVLine[0] = __max(__min(U1, 255), 0);
			pucYUVLine[1] = __max(__min(Y1, 255), 0);
			pucYUVLine[2] = __max(__min(V1, 255), 0);
			pucYUVLine[2] = __max(__min(Y2, 255), 0);

			pucYUVLine += 4;
			//*(pucLine + nY*nWidth * 2 + nX * 4) = max(min(Y1, 255), 0);
			//*(pucLine + nY*nWidth * 2 + nX * 4 + 1) = max(min(U1, 255), 0);
			//*(pucLine + nY*nWidth * 2 + nX * 4 + 2) = max(min(Y2, 255), 0);
			//*(pucLine + nY*nWidth * 2 + nX * 4 + 3) = max(min(V1, 255), 0);
		}
	}

	return 0;
}

int getRGB32Alpha(const unsigned char *pucRGBA, int nBGRAPitch, int nImgW, int nImgH, unsigned char *pucAlpha, int nAlphaLineBytes)
{

	unsigned char *pucLine = NULL;
	const unsigned char *pucBGRALine = NULL;

	for (int nY = 0; nY < nImgH; nY++)
	{
		pucBGRALine = pucRGBA + nBGRAPitch * nY;
		pucLine = pucAlpha + nAlphaLineBytes * nY;
		for (int nX = 0; nX < nImgW; nX++, pucBGRALine += 4)
		{
			pucLine[nX] = pucBGRALine[3];
		}
	}

	return 0;
}


}