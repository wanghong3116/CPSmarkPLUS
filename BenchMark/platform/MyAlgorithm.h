
#include "stdafx.h"

#include "PlatformDefine.h"
#ifndef _ALGORITHM_H_1FB9AA7D_02DC_4D0E_9E85_F05FEA83C648
#define _ALGORITHM_H_1FB9AA7D_02DC_4D0E_9E85_F05FEA83C648
namespace MyPlatform
{

template<class tType>
double	crSet3(tType* ptData, int nWidth, int nHeight, int nDepth, int nDstZ, tType tValue,
					 int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	nXBeg	= nXBeg	* nDepth + nDstZ;
	nXEnd	= nXEnd * nDepth;
	int		nYArea	= nWidth * nDepth;
	int		nOffset	= nYBeg * nYArea;

	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX+=nDepth)
		{
			ptData[nOffset+nX]	= tValue;
		}
		nOffset	+= nYArea;
	}

	return	1;
}


template<class tType>
tType	crMin(tType* ptData, int nWidth, int nHeight, int nDepth, int nDstZ,
					 int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	nXBeg	= nXBeg	* nDepth + nDstZ;
	nXEnd	= nXEnd * nDepth;
	int		nYArea	= nWidth * nDepth;
	int		nOffset	= nYBeg * nYArea;
	tType	tMin	= ptData[nOffset];

	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX+=nDepth)
		{
			if (tMin > ptData[nOffset+nX])
				tMin	= ptData[nOffset+nX];
		}
		nOffset	+= nYArea;
	}

	return	tMin;
}

template<class tType>
tType	crMin(tType* ptData, int nWidth, int nHeight)
{
	return	crMin(ptData, nWidth, nHeight, 1, 0, 0, 0, nWidth, nHeight);
}


template<class tType>
tType	crMax(tType* ptData, int nWidth, int nHeight, int nDepth, int nDstZ,
			  int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	nXBeg	= nXBeg	* nDepth + nDstZ;
	nXEnd	= nXEnd * nDepth;
	int		nYArea	= nWidth * nDepth;
	int		nOffset	= nYBeg * nYArea;
	tType	tMax	= ptData[nOffset];

	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX+=nDepth)
		{
			if (tMax < ptData[nOffset+nX])
				tMax	= ptData[nOffset+nX];
		}
		nOffset	+= nYArea;
	}

	return	tMax;
}

template<class tType>
tType	crMax(tType* ptData, int nWidth, int nHeight)
{
	return	crMax(ptData, nWidth, nHeight, 1, 0, 0, 0, nWidth, nHeight);
}

template<class tType>
double	crSummation3(tType* ptData, int nWidth, int nHeight, int nDepth, int nDstZ,
						int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dSum	= 0;
	nXBeg	= nXBeg	* nDepth + nDstZ;
	nXEnd	= nXEnd * nDepth;
	int		nYArea	= nWidth * nDepth;
	int		nOffset	= nYBeg * nYArea;

	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX+=nDepth)
		{
			dSum	+= (double)ptData[nOffset+nX];
		}
		nOffset	+= nYArea;
	}

	return	dSum;
}

/// 矩阵求均值
template<class tType>
inline double	crAverage3(tType* ptData, int nWidth, int nHeight, int nDepth, int nDstZ,
							  int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dSum	= crSummation3(ptData, nWidth, nHeight, nDepth, nDstZ, nXBeg, nYBeg, nXEnd, nYEnd);

	if (dSum == 0)
		return	0;
	else 
		return	dSum / ((nYEnd - nYBeg) * (nXEnd - nXBeg));
}

/// 把矩阵中的模块区域设置成值 tValue
template<class tType>
double SetMatValue(tType* ptMat, int nMatW, int nMatH, tType tValue)
{
	return SetMatValue(ptMat, nMatW, nMatH, tValue, 0, 0, nMatW, nMatH);
}

/// 把矩阵中的模块区域设置成值 tValue
template<class tType>
double		SetMatValue(tType* ptMat, int nMatWidth, int nMatHeight, tType tValue,
				  int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	int		nOffset	= nYBeg * nMatWidth;
	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX++)
		{
			ptMat[nOffset+nX] = tValue;
		}
		nOffset	+= nMatWidth;
	}

	return	1;
}

/// 计算矩阵转置
template<class tType>
int	crMatT(tType* pDst, tType* pSrc, int nSrcW, int nSrcH)
{
	for (int nY = 0; nY < nSrcH; nY++)
	{
		for (int nX = 0; nX < nSrcW; nX++)
		{
			pDst[nY + nX * nSrcH]	= pSrc[nX + nY * nSrcW];
		}
	}

	return	1;
}

template<class tType0, class tType1>
int		crCopy(tType0* ptDst, int nDstW, int nDstH, int nDstX, int nDstY, 
	const tType1* ptSrc, int nSrcW, int nSrcH, int nSrcX, int nSrcY, int nCpyW, int nCpyH)
{
	ptDst	+= nDstX + nDstY * nDstW;
	ptSrc	+= nSrcX + nSrcY * nSrcW;

	for (int nY = 0; nY < nCpyH; nY++)
	{
		for (int nX = 0; nX < nCpyW; nX++)
		{
			ptDst[nX]	= (tType0)ptSrc[nX];
		}
		ptDst	+= nDstW;
		ptSrc	+= nSrcW;
	}

	return	PLATFORM_FUNC_NOERROR;
}

template<class tType0, class tType1>
int		crCopy(tType0* ptDst, const tType1* ptSrc, int nCpyW, int nCpyH)
{
	crCopy(ptDst, nCpyW, nCpyH, 0, 0, ptSrc, nCpyW, nCpyH, 0, 0, nCpyW, nCpyH);
	return	1;
}

template<class tType0, class tType1>
int crCopy4(tType0* ptDst, int nDstA0, int nDstA1, int nDstA2, int nDstA3, int nDstP2, int nDstP3, 
			tType1* ptSrc, int nSrcA0, int nSrcA1)
{
	for (int nY = 0; nY < nSrcA1; nY++)
	{
		for (int nX = 0; nX < nSrcA0; nX++)
		{
			ptDst[ (nX * nDstA2 + + nDstP2) + (nY * nDstA3 + nDstP3) * nDstA0]
			= (tType0)ptSrc[nX + nY * nSrcA0];
		}
	}
	return	1;
};


template<class tType>
int		crSub2(tType* ptDst, tType* ptLeft, tType* ptRight, int nWidth, int nHeigh, tType tMin)
{
	int		nArea	= nWidth * nHeigh;

	for (int nIdx = 0; nIdx < nArea; nIdx++)
	{
		if ( ((double)ptLeft[nIdx] - (double)ptRight[nIdx]) > (double)tMin)
			ptDst[nIdx]	= ptLeft[nIdx] - ptRight[nIdx];
		else 
			ptDst[nIdx]	= tMin;
	}

	return	1;
}

// 合并led图片
template<class tType>
int		crMergeXYZ(tType *ptDst, int nDstW, int nDstH, 
				   tType* ptSrc, int nSrcW, int nSrcH, int nSrcIdx)
{
	int nBlockXNum = nDstW / nSrcW;
	int nBlockYNum = nDstH / nSrcH;
	int nDstXNum	= nDstW * 3;
	int nXBeg = nSrcIdx % nBlockXNum;

	int	nSrcOffset = 0;
	for (int nY = nSrcIdx/nBlockXNum; nY < nDstH; nY+=nBlockYNum)
	{
		for (int nX = nXBeg; nX < nDstW; nX+=nBlockXNum)
		{
			int nDstOffset = nX * 3 + nY * nDstXNum;

			ptDst[nDstOffset]	= ptSrc[nSrcOffset++];
			ptDst[nDstOffset+1]	= ptSrc[nSrcOffset++];
			ptDst[nDstOffset+2]	= ptSrc[nSrcOffset++];
		}
	}

	return	1;
}

/// 矩阵除以一个数
template<class tType>
int		crDivide(tType* ptMatDst, const tType* ptMatSrc, int nMatWidth, int nMatHeight, double dRatio,
				 int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dSum	= 0;
	int		nOffset	= nYBeg * nMatWidth;
	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX++)
		{
			ptMatDst[nOffset+nX]	= (tType)(ptMatSrc[nOffset+nX] / dRatio);
		}
		nOffset	+= nMatWidth;
	}

	return	1;
}


/// 矩阵除以一个数
template<class tType>
int		crDivide(tType* ptMat, int nMatWidth, int nMatHeight, double dRatio,
				 int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	return	crDivide(ptMat, ptMat, nMatWidth, nMatHeight, dRatio, nXBeg, nYBeg, nXEnd, nYEnd);
}

/// 矩阵除以一个数
template<class tType>
int		crDivide(tType* ptMat, int nMatW, int nMatH, double dRatio)
{
	return	crDivide(ptMat, ptMat, nMatW, nMatH, dRatio, 0, 0, nMatW, nMatH);
}

/// 矩阵求和
template<class tType>
double		crSummation(tType* ptMat, int nMatWidth, int nMatHeight,
	int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dSum	= 0;
	int		nOffset	= nYBeg * nMatWidth;
	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX++)
		{
			dSum	+= (double)ptMat[nOffset+nX];
		}
		nOffset	+= nMatWidth;
	}

	return	dSum;
}

template<class tType>
double		crSum2(tType* ptMat, int nMatWidth, int nMatHeight)
{
	return	crSummation(ptMat, nMatWidth, nMatHeight, 0, 0, nMatWidth, nMatHeight);
}

/// 矩阵求和
template<class tType>
double		crSum2(const tType* ptMat, int nMatWidth, int nMatHeight,
				   int nXBeg, int nYBeg, int nXEnd, int nYEnd,  int nSumNum, tType* ptDst = NULL)
{
	int		nWidth	= nXEnd - nXBeg;
	int		nHeight = nYEnd - nYBeg;
	int		nArea	= nWidth * nHeight;
	tType*	ptSrc	= ptDst;
	if (NULL == ptSrc)
		ptDst = (tType*)malloc(nArea * sizeof(tType));

	crCopy(ptDst, nWidth, nHeight, 0, 0, ptMat, nMatWidth, nMatHeight, nXBeg, nYBeg, nWidth, nHeight);

	for (int nY = 0; nY < nSumNum; nY++)
	{
		int nDstIdx = nY;
		for (int nX = nY+1; nX < nArea; nX++)
		{
			if (ptDst[nX] > ptDst[nDstIdx])
			{
				nDstIdx	= nX;
			}
		}

		if (nDstIdx	!= nY)
		{
			tType	bTmp	= ptDst[nDstIdx];
			ptDst[nDstIdx]	= ptDst[nY];
			ptDst[nY]		= bTmp;
		}
	}

	double	dSum	= crSum2(ptDst, nSumNum, 1);
	if (NULL == ptSrc)
		free (ptDst);
	return	dSum;
}

 template<class tType0, class tType1>
 inline int crAveCol(tType0* ptCol, tType1* ptMat, int nMatW, int nMatH)
 {
	double* pdSumCol = (double*)malloc(nMatW * sizeof(double));
	crSumCol(pdSumCol, ptMat, nMatW, nMatH);
	
	for (int nX = 0; nX < nMatW; nX++)
	{
		ptCol[nX] = (tType0)(pdSumCol[nX] / nMatH);
	}

	free(pdSumCol);

	 return 1;
 }

 template<class tType0, class tType1>
 inline int crAveRow(tType0* ptRow, tType1* ptMat, int nMatW, int nMatH)
 {
	 double* pdSumRow = (double*)malloc(nMatH * sizeof(double));
	 crSumRow(pdSumRow, ptMat, nMatW, nMatH);

	 for (int nY = 0; nY < nMatH; nY++)
	 {
		 ptRow[nY] = (tType0)(pdSumRow[nY] / nMatW);
	 }

	 free(pdSumRow);

	 return 1;
 }

/// 上下加
template<class tType0, class tType1>
inline int crSumCol(tType0* ptCol, tType1* ptMat, int nMatW, int nMatH )
{
	memset(ptCol, 0, nMatW * sizeof(tType0));
	for (int nY = 0; nY < nMatH; nY++)
	{
		for (int nX = 0; nX < nMatW; nX++)
		{
			ptCol[nX] += ptMat[nX];
		}
		ptMat	+= nMatW;
	}

	return 1;
};

/// 左右加
/// 上下加
template<class tType0, class tType1>
inline int crSumRow(tType0* ptRow, tType1* ptMat, int nMatW, int nMatH )
{
	memset(ptRow, 0, nMatH * sizeof(tType0));
	for (int nY = 0; nY < nMatH; nY++)
	{
		for (int nX = 0; nX < nMatW; nX++)
		{
			ptRow[nY] += (tType0)ptMat[nX];
		}
		ptMat	+= nMatW;
	}

	return 1;
};


/// 矩阵求均值
template<class tType>
inline double	crAverage(tType* ptMat, int nMatWidth, int nMatHeight,
						  int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dSum	= crSummation(ptMat, nMatWidth, nMatHeight, nXBeg, nYBeg, nXEnd, nYEnd);
	
	if (dSum == 0)
		return	0;
	else 
		return	dSum / ((nYEnd - nYBeg) * (nXEnd - nXBeg));
}

template<class tType>
inline double	crAverage(tType* ptMat, int nMatWidth, int nMatHeight)
{
	return	crAverage(ptMat, nMatWidth, nMatHeight, 0, 0, nMatWidth, nMatHeight);
}

/// 矩阵求方差
template<class tType>
double crVariance(tType* ptMat, int nMatWidth, int nMatHeight, 
				  int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dAverage	= crAverage(ptMat, nMatWidth, nMatHeight, nXBeg, nYBeg, nXEnd, nYEnd);
	double	dSum	= 0;
	int		nOffset	= nYBeg * nMatWidth;
	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX++)
		{
			dSum	+= pow(dAverage - ptMat[nOffset+nX], 2);
		}
		nOffset	+= nMatWidth;
	}

	return	dSum / ((nYEnd - nYBeg) * (nXEnd - nXBeg));
}

template<class tType>
double crVariance(tType* ptMat, int nMatWidth, int nMatHeight)
{
	return	crVariance(ptMat, nMatWidth, nMatHeight, 0, 0, nMatWidth, nMatHeight);
}

/// 矩阵求标准差
template<class tType>
double crStdDeviation(tType* ptMat, int nMatWidth, int nMatHeight, int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	double	dSum	= crVariance(ptMat, nMatWidth, nMatHeight, nXBeg, nYBeg, nXEnd, nYEnd);
	
	return	sqrt(dSum);
}

template<class tType>
double crStdDeviation(tType* ptMat, int nMatWidth, int nMatHeight)
{
	return	crStdDeviation(ptMat, nMatWidth, nMatHeight, 0, 0, nMatWidth, nMatHeight);
}

/// 矩阵中值
template<class tType>
double		crMedian(tType* ptMat, int nMatWidth, int nMatHeight,
					 int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	int		nWidth	= nXEnd - nXBeg;
	int		nHeight = nYEnd - nYBeg;
	int		nArea	= nWidth * nHeight;

	tType*	ptData	= (tType*)malloc(nArea * sizeof(tType));

	ptMat			= ptMat + nXBeg + nYBeg * nMatWidth;
	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		memcpy (ptData, ptMat, nWidth * sizeof(tType));
		ptData		+= nWidth;
		ptMat		+= nMatWidth;
	}
	ptData			-= nArea;

	csSort(ptData, nArea, csIsLess<tType>, (nArea+1)/2);

	tType	tMedian	= ptData[(nArea-1)/2];
	free (ptData);

	return	(double)tMedian;
}

template<class tType>
double		crMedian(tType* ptMat, int nMatWidth, int nMatHeight)
{
	return	crMedian(ptMat, nMatWidth, nMatHeight, 0, 0, nMatWidth, nMatHeight);
}

/// 矩阵相加
template<class tType0, class tType1, class tType2>
int crAdd(tType0* ptSum, int nSumW, int nSumH, int nSumX, int nSumY, 
		  const tType1* ptAdd0, int nAdd0W, int nAdd0H, int nAdd0X, int nAdd0Y, 
		  const tType2* ptAdd1, int nAdd1W, int nAdd1H, int nAdd1X, int nAdd1Y,
		  int nMatW, int nMatH)
{
	ptSum	+= nSumY * nSumW + nSumX;
	ptAdd0	+= nAdd0Y * nAdd0W + nAdd0X;
	ptAdd1	+= nAdd1Y * nAdd1W + nAdd1X;

	for (int nY = 0; nY < nMatH; nY++)
	{
		for (int nX = 0; nX < nMatW; nX++)
		{
			ptSum[nX] = ptAdd0[nX] + ptAdd1[nX];
		}
		ptSum	+= nSumW;
		ptAdd0	+= nAdd0W;
		ptAdd1   += nAdd1W;
	}

	return 1;
}

 template<class tType0, class tType1>
 int		crAdd(tType0* ptSum, int nSumW, int nSumH, 
 			  const tType1* ptAdd, int nAddW, int nAddH, int nAddX, int nAdd1Y)
 {
 	return	crAdd(	ptSum, nSumW, nSumH, 0, 0, 
 					ptSum, nSumW, nSumH, 0, 0, 
 					ptAdd, nAddW, nAddH, nAddX, nAdd1Y, 
 					nSumW, nSumH);
 }

/// 矩阵乘以一个数
template<class tType1, class tType2>
int crMul(tType1* pTMat, int nMatW, int nMatH, tType2 tFactor)
{
	return crMul(pTMat, nMatW, tFactor, 0, 0, nMatW, nMatH);
}

template<class tType1, class tType2>
int crMul(tType1* pTMat, int nMatW, tType2 tFactor, int nXBeg, int nYBeg, int nXEnd, int nYEnd)
{
	tType1* ptPos = pTMat + nYBeg * nMatW + nXBeg;
	for (int nY = nYBeg; nY < nYEnd; nY++)
	{
		for (int nX = nXBeg; nX < nXEnd; nX++)
		{
			 ptPos[nX - nXBeg] = (tType1)(ptPos[nX - nXBeg] * tFactor);
		}
		ptPos += nMatW;
	}

	return 1;
}



};
#endif

