#pragma once


// enum EMarkType
// {
// 	E_MARK_TYPE_BEG,
// 
// 	E_MARK_TYPE_HAND_BRAKE,
// 	E_MARK_TYPE_AE_MARK,
// 	E_MARK_TYPE_
// 
// 	E_MARK_TYPE_END
// };


enum E_USER_DEFINE_MESSAGE
{
	WM_USER_DEFINE_MESSAGE_BEGIN = WM_USER,

	WM_STARTWORK,				/// 
	WS_CHANGE_RUNNING_INFOR,	/// wParam , lParam - (const TCHAR*)������Ϣ
	WS_MARK_WORK_OVER,			/// wParma ���н�������������� BENCH_MARK_OK����LPARMAΪMyplatform::tstringָ�뱣�������Ϣ����Ҫ�����ͷ�
	WS_MARK_WORK_PROGRESS,		/// wParam(0-��ʾ��ʼ,1-��ʾ������,2-��ʾ���Խ���); lParam ��wParamΪ1ʱ��ʾ��ǰ�Ľ��� ��wParamΪ0ʱ��ʾ������ܸ���
	WS_MARK_TYPE_NAME,


	WM_BATTERY_CONTINUE_WORK,
	WM_BATTERY_OVER,
};


enum EMarkType
{
	E_MARK_TYPE_BEG,

	E_MARK_NET = E_MARK_TYPE_BEG,
	E_MARK_OFFICE_EXCEL,
	E_MARK_OFFICE_POWERPOINT,
	E_MARK_OFFICE_WORD,
	E_MARK_PDF,
	E_MARK_OUTLOOK,
	E_MARK_WINRAR,

	E_MARK_3DSMAX_WHALE,
	E_MARK_AE,
	E_MARK_HANDBRAKE,
	E_MARK_CAD,
	E_MARK_PHOTOSHOP,
	E_MARK_PREMIERE,

	E_MARK_TYPE_END
};

enum EMarkClassify
{
	E_MARK_CLASSIFY_BEG,

	E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	E_MARK_CLASSIFY_Comprehensive_Application,
	

	E_MARK_CLASSIFY_END

};


enum ERunMarkStatus
{

	E_RUNMARKSTATUS_BEG,

	E_RMS_RUN = E_RUNMARKSTATUS_BEG,
	E_RMS_STOP,
	E_RMS_COUNT_DOWN,

	E_RUNMARKSTATUS_END

};


