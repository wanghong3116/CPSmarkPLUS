
#ifndef _DEFINE_H__8EB00854_B0D1_46FB_8429_BC9EF59B1879
#define _DEFINE_H__8EB00854_B0D1_46FB_8429_BC9EF59B1879

namespace MyPlatform
{
#define PLATFORM_FUNC_NOERROR								0
#define F_OUT_OF_MEMORY							-0xfffff
#define FOLDER_DMP								"dmp"
#define UNICODE_MAX_PATH_SIZE					(MAX_PATH*sizeof(wchar_t))

#define DIR_FANGZHENGBACKUP						_T("FangZhengBackup")
#define DIR_FANGZHENG11							_T("FangZhengBackup\\FangZheng11")
#define DIR_FZ_NOPRO							_T("FangZhengBackup\\NoPro")
#define DIR_FZ_EXEFAILED						_T("FangZhengBackup\\ExeFailed")

#define DIR_FEITENG								_T("FeiTengBackup")

#define DIR_P65									_T("P65Backup")

#define DIR_INDD								_T("InddBackup")

#define INI_HOMEPAGE							"homepage"
#define INI_HOMEPAGE_CHANNELINFO				"channelinfo"
#define INI_HOMEPAGE_SCHOOLINFO					"schoolinfo"
#define INI_HOMEPAGE_ADIMGLIST					"adimglist"

#define INI_BOOKCATEVERSION						"bookcateversion"

#define INI_DLDATA								"downloaddata"
#define INI_DLDATA_RESOURSE_URL					"resourseurl"
#define INI_DLDATA_SAVE_PATH					"savepath"
#define INI_DLDATA_START_PAGE					"startpage"
#define INI_DLDATA_END_PAGE						"endpage"
#define INI_DLDATA_COOKIES						"cookies"
#define INI_DLDATA_HAS_DOWN_PAGE				"downloadpage"
#define INI_DLDATA_STAID						"staid"
#define INI_PAGE								"page"

#define INI_BASICSOFT							_T("基础软件")
#define INI_BASICSOFT_WINRAR					_T("压缩软件winrar")

#define INI_FANGZHENG							_T("方正数据")
#define INI_FANGZHENG_NEWBD_9					_T("方正9")
#define INI_FANGZHENG_NEWBD_10					_T("方正10")
#define INI_FANGZHENG_NEWBD_11					_T("方正11")
#define INI_FANGZHENG_APABI						_T("Apabi Maker")
#define INI_FANGZHENG_NVWA						_T("女娲补字")

#define INI_FEITENG								_T("飞腾数据")
#define INI_FEITENG_FEITENG						_T("飞腾")
#define INI_FEITENG_APABI						_T("Apabi Maker")

#define INI_P65									_T("P65数据")
#define INI_P65_PAGE_MAKER						_T("PM65C")

#define INI_INDD								_T("indd数据")
#define INI_INDD_IDDESIGN						_T("AdobeInDesignCS6")

#define INI_DEPENDS								_T("DEPENDS")
#define INI_DEPENDS_NEWBD9_FP					_T("NewBD9FP")
#define INI_DEPENDS_NEWBD10_FP					_T("NewBD10FP")
#define INI_DEPENDS_NVWA_FP						_T("nvwafp")
#define INI_DEPENDS_APABI_MAKER_FP				_T("ApabiMakerFP")
#define INI_DEPENDS_FEI_TENG_FP					_T("FeiTeng")

#define INI_HANDLE								_T("HANDLETASK")	
#define INI_HANDLE_PREDIR						_T("HandlePreDir")
#define INI_HANDLE_PROGRESS						_T("HandleProgress")

#define INI_FZSETTING							_T("FangZhengSetting")
#define INI_FZSETTING_USE10TO11					_T("Use10To11")
#define INI_FZSETTING_U9TONODAYANG				_T("Use9ToNoDaYang")

#define FILE_CHANNELINFO						"channelinfo.info"
#define FILE_SCHOOLINFO						"schoolinfo.info"
#define FILE_ADIMGLIST							"adimglist.info"

#define WORD_WATERMARK						_T("ChaoXing")

#define EXT_PDG								".pdg"
#define EXT_PDZ								".pdz"
#define EXT_PDZX								".pdzx"
#define EXT_PDF								_T(".pdf")
#define EXT_EPUB								".epub"
#define EXT_TXT								".txt"
#define EXT_INDD							_T(".indd")
#define EXT_XML								_T(".xml")

#define EXT_TIFA							".tif"
#define EXT_TIFW							L".tif"
#define EXT_CSSA							".css"
#define EXT_CSSW							L".css"
#ifdef _UNICODE
#define EXT_TIF								EXT_TIFW
#define EXT_CSS								EXT_CSSW
#else
#define EXT_TIF								EXT_TIFA
#define EXT_CSS								EXT_CSSA
#endif

#define EXT_JPG								".jpg"
#define EXT_JPEG								".jpeg"
#define EXT_PNG								".png"
#define EXT_TEMP							_T(".temp")
#define EXT_PS								_T(".ps")
#define EXT_CEB								_T(".ceb")
#define EXT_PRO								_T(".PRO")
#define EXT_P65								_T(".p65")
#define EXT_FBD								_T(".fbd")
#define EXT_FIT								_T(".FIT")
#define EXT_DBF								_T(".dbf")


#define HTML_NODE_IMG							"img"
#define IMG_ATTRIBUTE_SRC						"src"

#define CLS_NAME_DLG						_T("#32770")
#define CLS_NAME_TAB_CTRL					_T("SysTabControl32")
#define CLS_NAME_BUTTON						_T("Button")
#define CLS_NAME_COMBOX						_T("ComboBox")
#define CLS_NAME_EDIT						_T("Edit")
#define CLS_NAME_STATIC						_T("Static")
#define CLS_NAME_LISTBOX					_T("ListBox")

#define L0 0			/// 错误
#define L1 1			/// 警告
#define L2 2			//  信息

#define UCHAR_NUM								(UCHAR_MAX + 1)
#define BOOKNUM_LEN								20

/// wchar_t
#define FILE_UNICODE_HEADER(c)					{(c) = 0xfeff;}
// char[3]
#define FILE_UTF8_HEADER(p)						{(p)[0] = 0xef, (p)[1] = 0xbb, (p)[2] = 0xbf;}

#define ABS(n)									((n) > 0 ? (n) : -(n))
#define FCLOSE(fp)								{if (NULL != (fp)) fclose(fp); (fp) = NULL;}
#define FREE(p)									{if (NULL != (p)) free(p); (p) = NULL;}
#define SDELETE(p)								{if (NULL != (p)) delete(p); (p) = NULL;}
#define DELETEALL(p)							{if (NULL != (p)) delete[] (p); (p) = NULL; }
#define CLOSEHANDLE(p)							{if (NULL != (p)) CloseHandle(p); (p) = NULL;}
#define DELETEOBJECT(p)							{if (NULL != (p)) DeleteObject(p); (p) = NULL;}
#define ISODD(n)								(1 == (n) % 2)
#define ISPOINTINRECT(p, r)						(((p).x >= (r).left && (p).x <= (r).right) && ((p).y >= (r).top && (p).y <= (r).bottom))
#define BETWEEN(value, vmin, vmax)				((value) > (vmax) ? (vmax) : ((value) < (vmin) ? (vmin) : (value)))
#define STRETCHVALUE(v, smin, smax, dmin, dmax)	( (double)((dmax) - (dmin)) / ((smax) - (smin)) * ((v) - (smin)) + (dmin))
#define THROWINT_STD(v, std, out)				{int nValue = (v), nStd = (std), nOut = (out); if (nValue != nStd) throw nOut;}
#define THROWINT_ERR(v, err, out)				{int nValue = (v), nerr = (err), nOut = (out); if (nValue == nerr) throw nOut;}
#define RETURNINT_STD(v, std, out)				{int nValue = (v), nStd = (std), nOut = (out); if (nValue != nStd) return nOut;}
#define RETURNINT_ERR(v, err, out)				{int nValue = (v), nErr = (err), nOut = (out); if (nValue == nErr) return nOut;}
#define FREELIBRARY(h)							{if (NULL != (h)) ::FreeLibrary(h); (h) = NULL;}
#define ARRARY_COUNT(p)							(sizeof(p) / sizeof((p)[0]))
#define SRELEASECOMPTR(p)						{if (NULL != (p)){(p).Release();(p) = NULL;}};

#define RGB_TO_Y(r, g, b)						((unsigned char)(0.299 * (r)+ 0.587 * (g)+ 0.114 * (b)))
#define RGB_TO_YUV(dstu, dsty, dstv, r, g, b)	\
{\
	dsty = RGB_TO_Y((r), (g), (b)); \
	dstu = (unsigned char)(128 - 0.168736 * r - 0.331264 * g + 0.5 * b); \
	dstv=(unsigned char)(128 + 0.5 * r - 0.418688 * g - 0.081312 * b);}
#define BITMAPLINEBYTES(width, pixelBit)		(((width) * (pixelBit) + 31) / 32 * 4)

#define NULL_RETURN(p,nRet,szTrace)				\
if (NULL == p)\
{\
	crTrace(L0, szTrace);\
	return nRet;\
}
#ifndef MAPVK_VK_TO_VSC
#define MAPVK_VK_TO_VSC 0
#endif

#define doKeyEvent(key, bUp, bExt) \
	do{ \
	BYTE vsc = MapVirtualKey(key, MAPVK_VK_TO_VSC); \
	DWORD flag = ((bExt)? KEYEVENTF_EXTENDEDKEY:0)|((bUp)? KEYEVENTF_KEYUP:0); \
	keybd_event(key, vsc, flag, 0); \
	Sleep(10); \
	}while(0);
#define doKeyClick(key, bExt) \
	do \
	{ \
	doKeyEvent(key, 0, bExt); \
	doKeyEvent(key, 1, bExt); \
}while(0);

#define doLButton(x,y)	\
	SetCursorPos(x,y);\
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);\
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);

};

#endif