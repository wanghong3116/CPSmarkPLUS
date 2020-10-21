#include "stdafx.h"
#include "PhotoShop_Opids.h"


CPhotoShop_Opids::CPhotoShop_Opids()
{
 	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 10, DESC_OPEN_DOCUMENT + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻyi.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 20, DESC_OPEN_DOCUMENT + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻer.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 30, DESC_OPEN_DOCUMENT + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻsan.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 40, DESC_OPEN_DOCUMENT + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻsi.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 50, DESC_OPEN_DOCUMENT + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻwu.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_DUPLICATE + 10, DESC_DUPLICATE + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻyi.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_DUPLICATE + 20, DESC_DUPLICATE + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻer.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_DUPLICATE + 30, DESC_DUPLICATE + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻsan.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_DUPLICATE + 40, DESC_DUPLICATE + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻsi.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_DUPLICATE + 50, DESC_DUPLICATE + _T("Resources\\Photoshop\\С��Ϻ\\С��Ϻwu.psd")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_MOVE_POSITION + 10, DESC_MOVE_POSITION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_MOVE_LAYER + 10, DESC_MOVE_LAYER + _T("A")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_MOVE_LAYER + 50, DESC_MOVE_LAYER + _T("B")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_CREATE_A_LARGE_PICTURE + 10, DESC_CREATE_A_LARGE_PICTURE));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ORDER_FILES + 10, DESC_ORDER_FILES));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_MERGE_INTO_A_LARGE_PICTURE + 10, DESC_MERGE_INTO_A_LARGE_PICTURE));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_PICTURE + 10, DESC_OPEN_PICTURE + _T("Photoshop::�򿪱���ͼƬ")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_PICTURE + 50, DESC_OPEN_PICTURE + _T("Photoshop::����ŮͼƬ")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_VIRTUALIZE_BACKGROUND + 10, DESC_VIRTUALIZE_BACKGROUND));
}

CPhotoShop_Opids::~CPhotoShop_Opids()
{
}
tstring CPhotoShop_Opids::DESC_TYPE_MARKER = _T("PhotoShop");
tstring CPhotoShop_Opids::DESC_OPEN_APPLICATION = _T("PhotoShop::�򿪳���(Open Application)");
tstring CPhotoShop_Opids::DESC_OPEN_DOCUMENT = _T("PhotoShop::���ĵ�(Open Document)");
tstring CPhotoShop_Opids::DESC_DUPLICATE = _T("PhotoShop::��������(Duplicate)");
tstring CPhotoShop_Opids::DESC_MOVE_POSITION = _T("PhotoShop::�ƶ�λ��(Move Position)");;
tstring CPhotoShop_Opids::DESC_MOVE_LAYER = _T("PhotoShop::�ƶ�����(Move Layer)");;
tstring CPhotoShop_Opids::DESC_CREATE_A_LARGE_PICTURE = _T("PhotoShop::������ͼ�ļ�(Create A Large Picture)");;
tstring CPhotoShop_Opids::DESC_ORDER_FILES = _T("PhotoShop::�����ļ�(Order Files)");;
tstring CPhotoShop_Opids::DESC_MERGE_INTO_A_LARGE_PICTURE = _T("PhotoShop::�ϲ�Ϊһ�Ŵ�ͼ(Merge Into A Large Picture)");;
tstring CPhotoShop_Opids::DESC_OPEN_PICTURE = _T("PhotoShop::��ͼƬ(Open Picture)");;
tstring CPhotoShop_Opids::DESC_VIRTUALIZE_BACKGROUND = _T("PhotoShop::�黯����(Vitualize Background)");;

int CPhotoShop_Opids::OPID_TYPE_MARKER = 70000;

int CPhotoShop_Opids::OPID_OPEN_APPLICATION = 70100;
int CPhotoShop_Opids::OPID_OPEN_DOCUMENT = 70200;
int CPhotoShop_Opids::OPID_DUPLICATE = 70300;

int CPhotoShop_Opids::OPID_MOVE_POSITION = 70400;
int CPhotoShop_Opids::OPID_MOVE_LAYER = 70500;
int CPhotoShop_Opids::OPID_CREATE_A_LARGE_PICTURE = 70600;
int CPhotoShop_Opids::OPID_ORDER_FILES = 70700;
int CPhotoShop_Opids::OPID_MERGE_INTO_A_LARGE_PICTURE = 70800;
int CPhotoShop_Opids::OPID_OPEN_PICTURE = 70900;
int CPhotoShop_Opids::OPID_VIRTUALIZE_BACKGROUND = 71100;
