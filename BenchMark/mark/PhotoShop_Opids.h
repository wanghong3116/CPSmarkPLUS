#pragma once
#include "Opids.h"
class CPhotoShop_Opids :
	public COpids
{
public:
	CPhotoShop_Opids();
	~CPhotoShop_Opids();
	 
	static int OPID_TYPE_MARKER;
	
	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;
	static int OPID_DUPLICATE;

	static int OPID_MOVE_POSITION;
	static int OPID_MOVE_LAYER;
	static int OPID_CREATE_A_LARGE_PICTURE;
	static int OPID_ORDER_FILES;
	static int OPID_MERGE_INTO_A_LARGE_PICTURE;
	static int OPID_OPEN_PICTURE;
	static int OPID_VIRTUALIZE_BACKGROUND;

	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;
	static tstring DESC_DUPLICATE;

	static tstring DESC_MOVE_POSITION;
	static tstring DESC_MOVE_LAYER;
	static tstring DESC_CREATE_A_LARGE_PICTURE;
	static tstring DESC_ORDER_FILES;
	static tstring DESC_MERGE_INTO_A_LARGE_PICTURE;
	static tstring DESC_OPEN_PICTURE;
	static tstring DESC_VIRTUALIZE_BACKGROUND;
};

/*Photoshop::移动位置
Photoshop::移动布局
Photoshop::创建大图文件
Photoshop::排列文件
Photoshop::合并为一张大图
Photoshop::打开图片
Photoshop::虚化背景...
*/