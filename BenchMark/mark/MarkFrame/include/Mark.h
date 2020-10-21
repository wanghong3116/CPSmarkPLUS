#pragma once
#include "MarkBase.h"

// MAllocator
namespace Mark{
	enum ValueType{
		ValueUnknown,
		ValueBoolean,
		ValueNumI32,
		ValueNumI64,
		ValueNumF32,
		ValueNumF64,
		ValuePointer,
		ValueString,
		ValueWString,
		ValueArrayNodes,
		ValueArrayItems,
		ValueTypeLength,
	};
	enum ValueCompare{
		ValueCompareStrict = 0,
		ValueCompareNumber = 1,
		ValueCompareStringCaseless = 2,
		ValueCompareStringType = 4,
		ValueCompareString = (ValueCompareStringCaseless | ValueCompareStringType),
		ValueCompareFuzzy = (ValueCompareNumber | ValueCompareString),
	};
#if defined(UNICODE) || defined(_UNICODE)
#define ValueTString ValueWString
#else
#define ValueTString ValueString
#endif
	struct ValueHead{
		Long refs;
		struct{
			Long all;
			Long use;
		}size;
		Long type;
		Long comp;
	};
	union ValueData{
		Char valChar;
		WChar valWChar;
		Short valShort;
		Int valInt;
		Long valLong;
		LLong valLLong;
		UChar valUChar;
		UShort valUShort;
		UInt valUInt;
		ULong valULong;
		ULLong valULLong;
		Float valFloat;
		Double valDouble;
		Boolean valBool;
	};
	struct ValueNode{
		Long count;
		ValueNode *parent;
		ValueNode *prev, *next;
		ValueNode *begin, *end;
	};
	struct ValueItems{
		Long count;
	};
	class MCLS MAllocator{
	public:
		enum{
			MiniPageSize  = 128,
		};
	public:
		MAllocator();
		~MAllocator();
	public:
		static LPointer Alloc(ValueType type, Long nsize = 0);
		static LPointer Reloc(LPointer ptr, Long nsize);
		static LPointer Reloc(LPointer ptr, ValueType type, Long nsize);
		static LPointer Cploc(LPointer ptr);
		static Boolean Deloc(LPointer ptr);
		static LPointer Share(LPointer ptr);
	};
};
/*
// ILocker
namespace Mark{
	class MCLS ILocker{
	public:
		ILocker();
		~ILocker();
	public:
		virtual Void Lock();
		virtual Boolean TryLock();
		virtual Void Unlock();
		virtual Boolean Locked();
	};
	class MCLS IAutoLock{
	private:
		ILocker *m_ILocker;
		Boolean m_GotLock;
	public:
		IAutoLock(ILocker *lpILocker, Boolean bForceLock = true);
		~IAutoLock();
	public:
		operator Boolean();
	};
};

// MLocker
namespace Mark{
	class MCLS MLockerAtomic : public ILocker{
		//
	};
	class MCLS MLocker : public ILocker{
		//
	};
	class MCLS MLockerMutex : public ILocker{
		//
	};
};
*/

// MStr, MWcs
namespace Mark{
	/*class MCLS MStrRegExp{
	public:
	};
	class MCLS MWcsRegExp{
	public:
	};
	class MCLS MStrMatches : MNode<LPString>{
	public:
	};
	class MCLS MWcsMatches : MNode<LPWString>{
	public:
	};*/
	class MCLS MStr{
	public:
		typedef Char ValType;
		typedef Const ValType ValConstType;

		typedef ValType *ValPtr;
		typedef Const ValType *ValConstPtr;

		typedef ValType *Iterator;
		typedef Const ValType *IteratorConst;
		typedef ValType *IteratorReverse;
		typedef Const ValType *IteratorReverseConst;

		typedef MStr StrType;
		typedef Const StrType StrTypeConst;

	private:
		ValPtr m_String;
		friend class MStr;
		friend class MWcs;

	public:
		MStr();
		MStr(Const MStr &);
		MStr(ValConstPtr lpString);
		MStr(ValConstPtr lpString, Int sublen);
		MStr(ValConstPtr lpBegin, ValConstPtr lpEnd);
		~MStr();
	public:
		StrType &operator=(Const StrType& str);
		StrType &operator=(ValConstPtr s);
		StrType &operator=(ValType c);
	public: // Iterators:
		Iterator Begin();
		Iterator End();
		IteratorReverse BeginRight();
		IteratorReverse EndRight();
		IteratorConst BeginConst() Const;
		IteratorConst EndConst() Const;
		IteratorReverseConst BeginRightConst() Const;
		IteratorReverseConst EndRightConst() Const;
	public: // Capacity:
		Int Size() Const;
		Int Length() Const; // String Length
		Int BLength() Const; // String Byte Length
		Void Resize(Int nSize);
		Int Capacity() Const;
		Void Reserve(Int n = 0);
		Void Clear();
		Boolean Empty() Const;
		Void ShrinkToFit();
	public: // Element access:
		ValType &operator[](Int);
		Const ValType &operator[](Int) Const;
		ValType &At(Int);
		Const ValType &At(Int) Const;
		ValType &Back();
		Const ValType &Back() Const;
		ValType &Front();
		Const ValType &Front() Const;
	public: // Modifiers:
		Int Format(ValConstPtr fmt, ...);
		Int FormatAppend(ValConstPtr fmt, ...);

		StrType &operator += (Const StrType &str);
		StrType &operator += (ValConstPtr str);
		StrType &operator += (ValType c);

		StrType &Append(Const StrType &str);
		StrType &Append(Const StrType &str, Int subpos, Int sublen);
		StrType &Append(ValConstPtr s);
		StrType &Append(ValConstPtr s, Int n);
		StrType &Append(Int n, ValType c);

		Void PushBack(ValType c);

		StrType &Assign(Const StrType &str);
		StrType &Assign(Const StrType &str, Int subpos, Int sublen);
		StrType &Assign(ValConstPtr s);
		StrType &Assign(ValConstPtr s, Int n);
		StrType &Assign(Int n, ValType c);
		StrType &Assign(ValConstPtr lpBegin, ValConstPtr lpEnd);

		StrType &Insert(Int pos, Const StrType &str);
		StrType &Insert(Int pos, Const StrType &str, Int subpos, Int sublen);
		StrType &Insert(Int pos, ValConstPtr s);
		StrType &Insert(Int pos, ValConstPtr s, Int n);
		StrType &Insert(Int pos, Int n, ValType c);

		StrType &Erase (Int pos = 0, Int len = npos);

		StrType &Replace(Int pos, Int len, Const StrType &str);
		StrType &Replace(Int pos, Int len, Const StrType &str, Int subpos, Int sublen);
		StrType &Replace(Int pos, Int len, ValConstPtr s);
		StrType &Replace(Int pos, Int len, ValConstPtr s, Int n);
		StrType &Replace(Int pos, Int len, Int n, ValType c);

		Void Swap(StrType &str);
		Void PopBack();
	public: // String operations:
		ValConstPtr C_Str() Const;
		ValConstPtr Data() Const;
		Int Copy(ValPtr s, Int len, Int pos = 0) Const;

		Int Find(Const StrType &str, Int pos = 0) Const;
		Int Find(ValConstPtr s, Int pos = 0) Const;
		Int Find(ValConstPtr s, Int pos, Int n) Const;
		Int Find(ValType c, Int pos = 0) Const;

		Int FindRight(Const StrType &str, Int pos = npos) Const;
		Int FindRight(ValConstPtr s, Int pos = npos) Const;
		Int FindRight(ValConstPtr s, Int pos, Int n) Const;
		Int FindRight(ValType c, Int pos = npos) Const;

		Int FindFirstOf(Const StrType &str, Int pos = 0) Const;
		Int FindFirstOf(ValConstPtr s, Int pos = 0) Const;
		Int FindFirstOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindFirstOf(ValType c, Int pos = 0) Const;

		Int FindLastOf(Const StrType &str, Int pos = npos) Const;
		Int FindLastOf(ValConstPtr s, Int pos = npos) Const;
		Int FindLastOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindLastOf(ValType c, Int pos = npos) Const;

		Int FindFirstNotOf(Const StrType &str, Int pos = 0) Const;
		Int FindFirstNotOf(ValConstPtr s, Int pos = 0) Const;
		Int FindFirstNotOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindFirstNotOf(ValType c, Int pos = 0) Const;

		Int FindLastNotOf(Const StrType &str, Int pos = npos) Const;
		Int FindLastNotOf(ValConstPtr s, Int pos = npos) Const;
		Int FindLastNotOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindLastNotOf(ValType c, Int pos = npos) Const;

		StrType SubStr(Int pos = 0, Int len = npos) Const;

		Int Compare(Const StrType &str) Const;
		Int Compare(Int pos, Int len, StrType &str) Const;
		Int Compare(Int pos, Int len, StrType &str, Int subpos, Int sublen) Const;
		Int Compare(ValConstPtr s) Const;
		Int Compare(Int pos, Int len, ValConstPtr s) Const;
		Int Compare(Int pos, Int len, ValConstPtr s, Int n) Const;

		//Boolean RegExpTest(ValConstPtr pattern, Int offset = 0);
		//Boolean RegExpMatch(ValConstPtr pattern, MStrMatches &results, Int offset = 0) Const;
		//Boolean RegExpReplace(ValConstPtr pattern, Int offset = 0);
	public: // Member constants
		enum{
			npos = -1,
			valuetype = ValueString,
		};
	public: // Non-member function overloads
		StrType operator+(Const StrType &str);
		StrType operator+(ValConstPtr str);
		StrType operator+(ValType str);
		Boolean operator==(Const StrType &str);
		Boolean operator==(ValConstPtr str);
		Boolean operator!=(Const StrType &str);
		Boolean operator!=(ValConstPtr str);
		Boolean operator< (Const StrType &str);
		Boolean operator< (ValConstPtr str);
		Boolean operator<=(Const StrType &str);
		Boolean operator<=(ValConstPtr str);
		Boolean operator> (Const StrType &str);
		Boolean operator> (ValConstPtr str);
		Boolean operator>=(Const StrType &str);
		Boolean operator>=(ValConstPtr str);
	public:
		MWcs ToWcs();
		operator MWcs();
	private:
		MStr(LPointer lpAttach, Boolean bReqShare = true);
		Int Find(ValConstPtr src, Int clen, ValConstPtr sub, Int blen, Boolean bFromRight = false, Boolean bCaseless = false) Const;
		Int FindNot(ValConstPtr src, Int clen, ValConstPtr sub, Int blen, Boolean bFromRight = false, Boolean bCaseless = false) Const;
		Int Compare(ValConstPtr src, Int clen, ValConstPtr dst, Int dlen, Boolean bCaseless = false) Const;
	};
	class MCLS MWcs{
	public:
		typedef WChar ValType;
		typedef Const ValType ValConstType;

		typedef ValType *ValPtr;
		typedef Const ValType *ValConstPtr;

		typedef ValType *Iterator;
		typedef Const ValType *IteratorConst;
		typedef ValType *IteratorReverse;
		typedef Const ValType *IteratorReverseConst;

		typedef MWcs StrType;

	private:
		ValPtr m_String;
		friend class MStr;
		friend class MWcs;

	public:
		MWcs();
		MWcs(Const MWcs &);
		MWcs(ValConstPtr lpString);
		MWcs(ValConstPtr lpString, Int sublen);
		MWcs(ValConstPtr lpBegin, ValConstPtr lpEnd);
		~MWcs();
	public:
		StrType &operator=(Const StrType& str);
		StrType &operator=(ValConstPtr s);
		StrType &operator=(ValType c);
	public: // Iterators:
		Iterator Begin();
		Iterator End();
		IteratorReverse BeginRight();
		IteratorReverse EndRight();
		IteratorConst BeginConst() Const;
		IteratorConst EndConst() Const;
		IteratorReverseConst BeginRightConst() Const;
		 IteratorReverseConst EndRightConst() Const;
	public: // Capacity:
		Int Size() Const;
		Int Length() Const; // String Length
		Int BLength() Const; // String Byte Length
		Void Resize(Int nSize);
		Int Capacity() Const;
		Void Reserve(Int n = 0);
		Void Clear();
		Boolean Empty() Const;
		Void ShrinkToFit();
	public: // Element access:
		ValType &operator[](Int);
		Const ValType &operator[](Int) Const;
		ValType &At(Int);
		Const ValType &At(Int) Const;
		ValType &Back();
		Const ValType &Back() Const;
		ValType &Front();
		Const ValType &Front() Const;
	public: // Modifiers:
		Int Format(ValConstPtr fmt, ...);
		Int FormatAppend(ValConstPtr fmt, ...);

		StrType &operator += (Const StrType &str);
		StrType &operator += (ValConstPtr str);
		StrType &operator += (ValType c);

		StrType &Append(Const StrType &str);
		StrType &Append(Const StrType &str, Int subpos, Int sublen);
		StrType &Append(ValConstPtr s);
		StrType &Append(ValConstPtr s, Int n);
		StrType &Append(Int n, ValType c);

		Void PushBack(ValType c);

		StrType &Assign(Const StrType &str);
		StrType &Assign(Const StrType &str, Int subpos, Int sublen);
		StrType &Assign(ValConstPtr s);
		StrType &Assign(ValConstPtr s, Int n);
		StrType &Assign(Int n, ValType c);
		StrType &Assign(ValConstPtr lpBegin, ValConstPtr lpEnd);

		StrType &Insert(Int pos, Const StrType &str);
		StrType &Insert(Int pos, Const StrType &str, Int subpos, Int sublen);
		StrType &Insert(Int pos, ValConstPtr s);
		StrType &Insert(Int pos, ValConstPtr s, Int n);
		StrType &Insert(Int pos, Int n, ValType c);

		StrType &Erase (Int pos = 0, Int len = npos);

		StrType &Replace(Int pos, Int len, Const StrType &str);
		StrType &Replace(Int pos, Int len, Const StrType &str, Int subpos, Int sublen);
		StrType &Replace(Int pos, Int len, ValConstPtr s);
		StrType &Replace(Int pos, Int len, ValConstPtr s, Int n);
		StrType &Replace(Int pos, Int len, Int n, ValType c);

		Void Swap(StrType &str);
		Void PopBack();
	public: // String operations:
		ValConstPtr C_Str() Const;
		ValConstPtr Data() Const;
		Int Copy(ValPtr s, Int len, Int pos = 0) Const;

		Int Find(Const StrType &str, Int pos = 0) Const;
		Int Find(ValConstPtr s, Int pos = 0) Const;
		Int Find(ValConstPtr s, Int pos, Int n) Const;
		Int Find(ValType c, Int pos = 0) Const;

		Int FindRight(Const StrType &str, Int pos = npos) Const;
		Int FindRight(ValConstPtr s, Int pos = npos) Const;
		Int FindRight(ValConstPtr s, Int pos, Int n) Const;
		Int FindRight(ValType c, Int pos = npos) Const;

		Int FindFirstOf(Const StrType &str, Int pos = 0) Const;
		Int FindFirstOf(ValConstPtr s, Int pos = 0) Const;
		Int FindFirstOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindFirstOf(ValType c, Int pos = 0) Const;

		Int FindLastOf(Const StrType &str, Int pos = npos) Const;
		Int FindLastOf(ValConstPtr s, Int pos = npos) Const;
		Int FindLastOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindLastOf(ValType c, Int pos = npos) Const;

		Int FindFirstNotOf(Const StrType &str, Int pos = 0) Const;
		Int FindFirstNotOf(ValConstPtr s, Int pos = 0) Const;
		Int FindFirstNotOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindFirstNotOf(ValType c, Int pos = 0) Const;

		Int FindLastNotOf(Const StrType &str, Int pos = npos) Const;
		Int FindLastNotOf(ValConstPtr s, Int pos = npos) Const;
		Int FindLastNotOf(ValConstPtr s, Int pos, Int n) Const;
		Int FindLastNotOf(ValType c, Int pos = npos) Const;

		StrType SubStr(Int pos = 0, Int len = npos) Const;

		Int Compare(Const StrType &str) Const;
		Int Compare(Int pos, Int len, StrType &str) Const;
		Int Compare(Int pos, Int len, StrType &str, Int subpos, Int sublen) Const;
		Int Compare(ValConstPtr s) Const;
		Int Compare(Int pos, Int len, ValConstPtr s) Const;
		Int Compare(Int pos, Int len, ValConstPtr s, Int n) Const;
	public: // Member constants
		enum{
			npos = -1,
			valuetype = ValueString,
		};
	public: // Non-member function overloads
		StrType operator+(Const StrType &str);
		StrType operator+(ValConstPtr str);
		StrType operator+(ValType str);
		Boolean operator==(Const StrType &str);
		Boolean operator==(ValConstPtr str);
		Boolean operator!=(Const StrType &str);
		Boolean operator!=(ValConstPtr str);
		Boolean operator< (Const StrType &str);
		Boolean operator< (ValConstPtr str);
		Boolean operator<=(Const StrType &str);
		Boolean operator<=(ValConstPtr str);
		Boolean operator> (Const StrType &str);
		Boolean operator> (ValConstPtr str);
		Boolean operator>=(Const StrType &str);
		Boolean operator>=(ValConstPtr str);
	public:
		MStr ToStr();
		operator MStr();
	private:
		MWcs(LPointer lpAttach, Boolean bReqShare = true);
		Int Find(ValConstPtr src, Int clen, ValConstPtr sub, Int blen, Boolean bFromRight = false, Boolean bCaseless = false) Const;
		Int FindNot(ValConstPtr src, Int clen, ValConstPtr sub, Int blen, Boolean bFromRight = false, Boolean bCaseless = false) Const;
		Int Compare(ValConstPtr src, Int clen, ValConstPtr dst, Int dlen, Boolean bCaseless = false) Const;
	};

#if defined(UNICODE) || defined(_UNICODE)
	typedef MWcs MTStr;
#else
	typedef MStr MTStr;
#endif
};

// IMessenger, ILog
namespace Mark{
	class MCLS IMessenger{
	public:
		enum MessengerType{
			MessengerTip,
			MessengerLog,
		};
		typedef MessengerType Msgt;
	public:
		IMessenger(){};
		~IMessenger(){};
	public:
		virtual Void OnMessage(IMessenger *lpSender, Int arg0, Int arg1, LPCTString param0, LPCTString param1, LPointer lpUserData) = 0;
		virtual Msgt GetMessengerType() = 0;
	};
	class MCLS ILog : public IMessenger{
	public:
		enum LogType{
			LogNull,
			LogStd,
			LogTrace,
			LogPipline,
			LogTcp,
			LogUdp,
			LogCache,
			LogUnknown,
		};
		enum LogLevel{
			LogTips,
			LogInfo,
			LogWarn,
			LogError,
			LogAssert,
		};
		typedef LogType Logt;
	public:
		ILog();
		~ILog();
	public:
		virtual Void OnMessage(IMessenger *lpSender, Int level, Int code, LPCTString lpTag, LPCTString lpDesc, LPointer lpUserData);
		virtual Msgt GetMessengerType();
		virtual Logt GetLogType() = 0;
	public:
		Int T(LPCString lpTag, LPCString lpFmt, ...);
		Int I(LPCString lpTag, LPCString lpFmt, ...);
		Int W(LPCString lpTag, LPCString lpFmt, ...);
		Int E(LPCString lpTag, LPCString lpFmt, ...);
		Int A(LPCString lpTag, LPCString lpFmt, ...);
		Int P(Int level, LPCString lpTag, LPCString lpFmt, ...);
	public:
		Int T(LPCWString lpTag, LPCWString lpFmt, ...);
		Int I(LPCWString lpTag, LPCWString lpFmt, ...);
		Int W(LPCWString lpTag, LPCWString lpFmt, ...);
		Int E(LPCWString lpTag, LPCWString lpFmt, ...);
		Int A(LPCWString lpTag, LPCWString lpFmt, ...);
		Int P(Int level, LPCWString lpTag, LPCWString lpFmt, ...);
	protected:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs) = 0;
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs) = 0;
	};
};

// MLog
namespace Mark{
	class MCLS MLogNull : public ILog{
	public:
		virtual Logt GetLogType();
	public:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};
	class MCLS MLogStd : public ILog{
	public:
		virtual Logt GetLogType();
	public:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};
	class MCLS MLogTrace : public ILog{
	public:
		virtual Logt GetLogType();
	public:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};
	class MCLS MLogCache : public ILog{
	public:
		MStr m_LogStr;
		MWcs m_LogWcs;
	public:
		virtual Logt GetLogType();
	public:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};
/*	class MCLS MLogPipe : public ILog{
	public:
		virtual Logt GetLogType();
	protected:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};
	class MCLS MLogTcp : public ILog{
	public:
		virtual Logt GetLogType();
	protected:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};
	class MCLS MLogUdp : public ILog{
	public:
		virtual Logt GetLogType();
	protected:
		virtual Int Print(Int level, LPCString  lpTag, LPCString  lpFmt, LPointer lpArgs);
		virtual Int Print(Int level, LPCWString lpTag, LPCWString lpFmt, LPointer lpArgs);
	};*/
};

// MThread
namespace Mark{
#ifdef _WIN32
	unsigned _stdcall win32ThreadFunc(void* args);
#endif
	class MCLS MThread{
	private:
#ifdef _WIN32
		friend unsigned _stdcall win32ThreadFunc(void* args);
#endif
		LPointer m_TID, m_Data;
	public:
		MThread();
		~MThread();
	public:
		LPointer GetThreadID();
		Boolean ThreadBegin(LPointer lpArg);
		Boolean ThreadExists();
		Boolean ThreadJoint();
		Boolean ThreadJoint(Long ms);
	protected:
		virtual Void ThreadProc(LPointer lpArg) = 0;
	};
};

// MTimer, MSysTimer
namespace Mark{

	class MCLS MTimer{
	private:
		friend class MTimer;
		Double m_TimeFloat, m_TimeFreq, m_TimeCount;
	public:
		MTimer();
		~MTimer();
	public:
		Double Diff(MTimer &t);
		Double AbsDiff(MTimer &t);
	public:
		operator Double();
	};

	class MCLS MSysTimer{
	private:
		enum { Year, Month, DayOfWeek, Day, Hour, Minute, Second, MilliSeconds, Length, };
		UInt m_TimeDatas[Length];
	public:
		MSysTimer();
		~MSysTimer();
	public:
		UInt GetYear();
		UInt GetMonth();
		UInt GetDay();
		//UInt GetYDay();
		UInt GetWDay();
		UInt GetHour();
		UInt GetMinute();
		UInt GetSecond();
		UInt GetMilliSeconds();
	public:
		Void GetFormat(MStr &str);
		Void GetFormat(MWcs &str);
	};

};

// MValue
namespace Mark{
	class MCLS MValue{
	private:
		MAllocator m_Allocator;
		LPointer m_Pointer;
		friend class MValue;
	private:
		MValue(LPointer lpPointer, Boolean bAttach);
	public:
		MValue();
		MValue(Boolean);
		MValue(Int);
		MValue(LLong);
		MValue(Float);
		MValue(Double);
		MValue(LPCString);
		MValue(LPCWString);
		MValue(LPCString,LPCString);
		MValue(LPCWString,LPCWString);
		MValue(Const MValue&);
		~MValue();
	public:
		Void SetBoolean(Boolean  bVal);
		Void SetInt    (Int      iVal);
		Void SetLLong  (LLong    llVal);
		Void SetFloat  (Float    fVal);
		Void SetDouble (Double   dbVal);
		Void SetNumber (Int     numVal);
		Void SetNumber (LLong   numVal);
		Void SetNumber (Float   numVal);
		Void SetNumber (Double  numVal);
		Void SetString (LPCString   cstrVal);
		Void SetWString(LPCWString cwstrVal);
		Void SetTString(LPCTString ctstrVal);
		Void SetString (LPCString   cstrVal, Int iLength);
		Void SetWString(LPCWString cwstrVal, Int iLength);
		Void SetTString(LPCTString ctstrVal, Int iLength);
		Void SetPointer(LPointer     ptrVal, Int iLength);
		Void SetValue  (Const MValue&);
		Void SetUnKnown();
	public:
		ValueType GetType() Const;
	public:
		Boolean IsUnKnown() Const;
		Boolean IsBoolean() Const;
		Boolean IsNumber() Const;
		Boolean IsCache() Const;
	public:
		Boolean IsInt() Const;
		Boolean IsLLong() Const;
		Boolean IsFloat() Const;
		Boolean IsDouble() Const;
	public:
		Boolean IsString() Const;
		Boolean IsWString() Const;
		Boolean IsTString() Const;
		Boolean IsPointer() Const;
	public:
		operator Boolean();
		operator Int();
		operator LLong();
		operator Float();
		operator Double();
		operator LPCString();
		operator LPCWString();
		operator LPointer();
	public:
		Boolean GetBoolean() Const;
		Int GetInt() Const;
		LLong GetLLong() Const;
		Float GetFloat() Const;
		Double GetDouble() Const;
		Double GetNumber() Const;
		LPCString GetString(Long *lpLength = 0) Const;
		LPCWString GetWString(Long *lpLength = 0) Const;
		LPCTString GetTString(Long *lpLength = 0) Const;
		LPointer GetPointer(Long *lpSize) Const;
	public:
		Boolean Share(MValue &);
		Boolean Copy(MValue &);
	public:
		Boolean operator==(MValue &mVal);
		Boolean operator==(Boolean);
		Boolean operator==(Int);
		Boolean operator==(LLong);
		Boolean operator==(Float);
		Boolean operator==(Double);
		Boolean operator==(LPCString);
		Boolean operator==(LPCWString);
	public:
		Boolean operator!=(MValue &mVal);
		Boolean operator!=(Boolean);
		Boolean operator!=(Int);
		Boolean operator!=(LLong);
		Boolean operator!=(Float);
		Boolean operator!=(Double);
		Boolean operator!=(LPCString);
		Boolean operator!=(LPCWString);
	public:
		MValue &operator=(Const MValue &mVal);
		MValue &operator=(Boolean);
		MValue &operator=(Int);
		MValue &operator=(LLong);
		MValue &operator=(Float);
		MValue &operator=(Double);
		MValue &operator=(LPCString);
		MValue &operator=(LPCWString);
	};
};

// MNodeMap
namespace Mark{
	template<class _IndexType, class _DataType>
	class MNodeMap{
	public:
		typedef _DataType DataType;
		typedef _IndexType IndexType;
		typedef MNodeMap<IndexType, DataType> NodeType;
	private:
		ValueNode *m_Node;
		IndexType *m_Index;
		DataType *m_Data;
		Boolean m_Empty;
	public:
		MNodeMap() : m_Node(0), m_Index(0), m_Data(0), m_Empty(true){
			m_Node = _CreateNode(0, 0);
			if(m_Node){
				m_Index = (IndexType *)(m_Node + 1);
				m_Data = (DataType *)(m_Index + 1);
			};
		};
		MNodeMap(ValueNode *lpNode) : m_Node(0), m_Index(0), m_Data(0), m_Empty(false){
			m_Node = (ValueNode *)MAllocator::Share(lpNode);
			if(m_Node){
				m_Index = (IndexType *)(m_Node + 1);
				m_Data = (DataType *)(m_Index + 1);
			};
		};
		MNodeMap(ValueNode *lpNode, Boolean bShare) : m_Node(0), m_Index(0), m_Data(0), m_Empty(false){
			if(bShare){
				m_Node = (ValueNode *)MAllocator::Share(lpNode);
			}else{
				m_Node = (ValueNode *)lpNode;
			};
			if(m_Node){
				m_Index = (IndexType *)(m_Node + 1);
				m_Data = (DataType *)(m_Index + 1);
			};
		};
		MNodeMap(NodeType &refNodeBase) : m_Node(0), m_Index(0), m_Data(0), m_Empty(false){
			m_Node = (ValueNode *)MAllocator::Share(refNodeBase.m_Node);
			if(m_Node){
				m_Index = (IndexType *)(m_Node + 1);
				m_Data = (DataType *)(m_Index + 1);
				m_Empty = refNodeBase.m_Empty;
			};
		};
		~MNodeMap(){
			_DestroyNode(m_Node, m_Index, m_Data);
		};
	public:
		NodeType &operator=(NodeType &refNodeBase){
			if(m_Node) _DestroyNode(m_Node, m_Index, m_Data);
			m_Node = (ValueNode *)MAllocator::Share(refNodeBase.m_Node);
			if(m_Node){
				m_Index = (IndexType *)(m_Node + 1);
				m_Data = (DataType *)(m_Index + 1);
				m_Empty = refNodeBase.m_Empty;
			};
			return *this;
		};
	public:
		Boolean Empty(){
			return m_Node == 0;
		};
		NodeType operator[](Const IndexType &index){
			ValueNode *lpNode = m_Node->begin, *lpEnd = m_Node->end;
			IndexType *lpIndex = 0;
			if(lpNode){
				for (int i = 0; i < GetLength(); i++){
					lpIndex = (IndexType *)(lpNode + 1);
					if(*lpIndex == index) return NodeType(lpNode);
					lpNode = lpNode->next;
				};
			};
			return PushBack(&index, (DataType *)0);
		};
		ValueNode *GetValueNode(){
			return m_Node;
		};
		NodeType GetParent(){
			return NodeType(m_Node->parent);
		};
		NodeType GetPrev(){
			return NodeType(m_Node->prev);
		};
		NodeType GetNext(){
			return NodeType(m_Node->next);
		};
		NodeType GetBegin(){
			return NodeType(m_Node->begin);
		};
		NodeType GetEnd(){
			return NodeType(m_Node->end);
		};
		IndexType *GetIndexPtr(){
			return m_Index;
		};
		IndexType &GetIndex(){
			return *m_Index;
		};
		DataType *GetDataPtr(){
			return m_Data;
		};
		DataType &GetData(){
			return *m_Data;
		};
		Void SetData(Const DataType &refData){
			*m_Data = refData;
		};
		Int GetLength(){
			return m_Node->count;
		};
	public:
		NodeType PushBack(Const IndexType &refIndexType, Const DataType &refDataType){
			return PushBack(&refIndexType, &refDataType);
		};
		NodeType PushBack(Const IndexType &refIndexType, Const DataType *lpDataType){
			return PushBack(&refIndexType, lpDataType);
		};
		NodeType PushBack(Const IndexType *lpIndexType, Const DataType &refDataType){
			return PushBack(lpIndexType, &refDataType);
		};
		NodeType PushBack(Const IndexType *lpIndexType, Const DataType *lpDataType){
			ValueNode *lpNnew = (ValueNode *)_CreateNode(lpIndexType, lpDataType), *lpNold;
			if(lpNnew){
				ValueNode *lpNode = (ValueNode *)m_Node;
				lpNnew->parent = lpNode;
				lpNold = lpNode->end;
				if(lpNold){
					lpNold->next = lpNnew;
				};

				lpNnew->prev = lpNold;
				lpNnew->next = 0;
				lpNnew->begin = 0;
				lpNnew->end = 0;

				lpNode->end = lpNnew;
				if(lpNode->count++ == 0){
					lpNode->begin = lpNnew;
				};
			};
			return NodeType(lpNnew);
		};
		NodeType PushFront(Const IndexType &refIndexType,Const DataType &refDataType){
			return PushFront(&refIndexType, &refDataType);
		};
		NodeType PushFront(Const IndexType *lpIndexType,Const DataType &refDataType){
			return PushFront(lpIndexType, &refDataType);
		};
		NodeType PushFront(Const IndexType &refIndexType,Const DataType *lpDataType){
			return PushFront(&refIndexType, lpDataType);
		};
		NodeType PushFront(Const IndexType *lpIndexType, Const DataType *lpDataType){
			ValueNode *lpNnew = (ValueNode *)_CreateNode(lpIndexType, lpDataType), *lpNold;
			if(lpNnew){
				ValueNode *lpNode = (ValueNode *)m_Node;
				lpNnew->parent = lpNode;
				lpNold = lpNode->begin;
				if(lpNold){
					lpNold->next = lpNnew;
				};

				lpNnew->prev = 0;
				lpNnew->next = lpNold;
				lpNnew->begin = 0;
				lpNnew->end = 0;

				lpNode->begin = lpNnew;
				if(lpNode->count++ == 0){
					lpNode->end = lpNnew;
				};
			};
			return NodeType(lpNnew);
		};
	public:
		Void Clear(){
			ValueNode *lpNode = m_Node->begin, *lpNext = 0, *lpEnd = m_Node->end;
			if(lpNode){
				for(lpNode = m_Node->begin;lpNode != 0;lpNode = lpNext){
					NodeType nb(lpNode, false);
					lpNext = lpNode->next;
					nb.Clear();
					if(lpNode == lpEnd) break;
				};
			};
			m_Node->count = 0;
			//m_Node->prev = 0;
			//m_Node->next = 0;
			m_Node->begin = 0;
			m_Node->end = 0;
		};
	private:
		Void _DestroyNode(ValueNode *lpNode, IndexType *lpIndex, DataType *lpData){
			if(lpNode){
				ValueHead *lpHead = ((ValueHead *)lpNode) - 1;
				if(lpHead->refs == 1){
					((IndexType *)lpIndex)->~IndexType();
					((DataType *)lpData)->~DataType();
				};
				MAllocator::Deloc(lpNode);
			};
		};
		ValueNode *_CreateNode(Const IndexType *lpIndexIn, Const DataType *lpDataIn){
;			ValueNode *lpNode = (ValueNode *)MAllocator::Alloc(ValueType::ValueArrayNodes, sizeof(ValueNode) + sizeof(IndexType) + sizeof(DataType));
			IndexType *lpIndex = (IndexType *)(lpNode + 1);
			DataType  *lpData = (DataType *)(lpIndex + 1);
			if(lpNode){
				memset(lpNode, 0, sizeof(ValueNode) + sizeof(IndexType) + sizeof(DataType));
				::new ((void*)lpIndex) IndexType;
				if(lpIndexIn){
					*lpIndex = *lpIndexIn;
				};
				::new ((void*)lpData) DataType;
				if(lpDataIn){
					*lpData = *lpDataIn;
				};
			};
			return lpNode;
		};
	};
};

// MNodeVec
namespace Mark{
	template<class _DataType>
	class MNodeVec{
	public:
		typedef _DataType DataType;
		typedef MNodeVec<DataType> NodeType;
	private:
		ValueNode *m_Node;
		DataType *m_Data;
	public:
		MNodeVec() : m_Node(0), m_Data(0){
			m_Node = _CreateNode(0);
			m_Data = (DataType *)(m_Node + 1);
		};
		MNodeVec(ValueNode *lpNode) : m_Node(0), m_Data(0){
			m_Node = (ValueNode *)MAllocator::Share(lpNode);
			m_Data = (DataType *)(m_Node + 1);
		};
		MNodeVec(ValueNode *lpNode, Boolean bShare) : m_Node(0), m_Data(0){
			if(bShare){
				m_Node = (ValueNode *)MAllocator::Share(lpNode);
				m_Data = (DataType *)(m_Node + 1);
			}else{
				m_Node = (ValueNode *)lpNode;
				m_Data = (DataType *)(m_Node + 1);
			};
		};
		MNodeVec(NodeType &refNodeBase) : m_Node(0), m_Data(0){
			m_Node = refNodeBase.m_Node;
			m_Data = refNodeBase.m_Data;
		};
		~MNodeVec(){
			_DestroyNode(m_Node, m_Data);
		};
	public:
		NodeType operator[](UInt index){
			ValueNode *lpNode = m_Node->begin;
			for(UInt i = 0;i < index;i++){
				lpNode = lpNode->next;
			};
			return NodeType(lpNode);
		};
		NodeType GetParent(){
			return NodeType(m_Node->parent);
		};
		NodeType GetPrev(){
			return NodeType(m_Node->prev);
		};
		NodeType GetNext(){
			return NodeType(m_Node->next);
		};
		NodeType GetBegin(){
			return NodeType(m_Node->begin);
		};
		NodeType GetEnd(){
			return NodeType(m_Node->end);
		};
		DataType *GetDataPtr(){
			return m_Data;
		};
		DataType &GetData(){
			return *m_Data;
		};
		Int GetLength(){
			return m_Node->count;
		};
	public:
		NodeType PushBack(Const DataType &refDataType){
			return PushBack(&refDataType);
		};
		NodeType PushBack(Const DataType *lpDataType){
			ValueNode *lpNnew = (ValueNode *)_CreateNode(lpDataType), *lpNold;
			if(lpNnew){
				ValueNode *lpNode = (ValueNode *)m_Node;
				lpNnew->parent = lpNode;
				lpNold = lpNode->end;
				if(lpNold){
					lpNold->next = lpNnew;
				};

				lpNnew->prev = lpNold;
				lpNnew->next = 0;
				lpNnew->begin = 0;
				lpNnew->end = 0;

				lpNode->end = lpNnew;
				if(lpNode->count++ == 0){
					lpNode->begin = lpNnew;
				};
			};
			return NodeType(lpNnew);
		};
		NodeType PushFront(Const DataType &refDataType){
			return PushFront(&refDataType);
		};
		NodeType PushFront(Const DataType *lpDataType){
			ValueNode *lpNnew = (ValueNode *)_CreateNode(lpDataType), *lpNold;
			if(lpNnew){
				ValueNode *lpNode = (ValueNode *)m_Node;
				lpNnew->parent = lpNode;
				lpNold = lpNode->begin;
				if(lpNold){
					lpNold->next = lpNnew;
				};

				lpNnew->prev = 0;
				lpNnew->next = lpNold;
				lpNnew->begin = 0;
				lpNnew->end = 0;

				lpNode->begin = lpNnew;
				if(lpNode->count++ == 0){
					lpNode->end = lpNnew;
				};
			};
			return NodeType(lpNnew);
		};
	public:
		Void Clear(){
			ValueNode *lpNode = m_Node->begin, *lpNext = 0, *lpEnd = m_Node->end;
			if(lpNode){
				for(lpNode = m_Node->begin;lpNode != 0;lpNode = lpNext){
					NodeType nb(lpNode, false);
					lpNext = lpNode->next;
					nb.Clear();
					if(lpNode == lpEnd) break;
				};
			};
			m_Node->count = 0;
			m_Node->prev = 0;
			m_Node->next = 0;
			m_Node->begin = 0;
			m_Node->end = 0;
		};
	private:
		Void _DestroyNode(ValueNode *lpNode, DataType *lpData){
			if(lpNode){
				ValueHead *lpHead = ((ValueHead *)lpNode) - 1;
				if(lpHead->refs == 1){
					((DataType *)lpData)->~DataType();
				};
				MAllocator::Deloc(lpNode);
			};
		};
		ValueNode *_CreateNode(Const DataType *lpDataIn){
			ValueNode *lpNode = (ValueNode *)MAllocator::Alloc(ValueType::ValueArrayNodes, sizeof(ValueNode) + sizeof(DataType));
			DataType  *lpData = (DataType *)(lpNode + 1);
			if(lpNode){
				::new ((void*)lpData) DataType;
				if(lpDataIn){
					*lpData = *lpDataIn;
				};
			};
			return lpNode;
		};
	};
};
