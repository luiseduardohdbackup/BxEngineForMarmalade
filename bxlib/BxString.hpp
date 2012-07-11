#pragma once
#include "BxVar.hpp"
#include "BxUtil.hpp"

//! \brief ��Ʈ�� ����
class BxString
{
public:
	/*!
	\brief �⺻������
	*/
	BxString() {Chars.Insert(0, Pool.Make()) = '\0';}

	/*!
	\brief ���������(char)
	\param RHS : ������ �ν��Ͻ�
	*/
	BxString(char RHS) {operator=(RHS);}

	/*!
	\brief ���������(string)
	\param RHS : ������ �ν��Ͻ�
	*/
	BxString(string RHS) {operator=(RHS);}

	/*!
	\brief ���������(BxString)
	\param RHS : ������ �ν��Ͻ�
	*/
	BxString(const BxString& RHS) {operator=(RHS);}

	/*!
	\brief ���������(sprintf)
	\param src : printf����� ��(%d/%s/...)
	\param nouse : ������ �ĺ��� ��
	\param ... : �������ڵ�
	*/
	BxString(string src, void* nouse, ...)
	{
		va_list List;
		va_start(List, nouse);
		FormatV(src, List);
		va_end(List);
	}

	/*!
	\brief �Ҹ���
	*/
	virtual ~BxString() {Empty();}

public:
	/*!
	\brief ���� ����
	*/
	void Empty()
	{
		for(int i = 0; i < Chars.Length(); ++i)
			Pool.Free(Chars.Access(i));
		Chars.Reset(false);
	}

	/*!
	\brief ���� ���ϱ�
	\return ��Ʈ���� ���ڱ���
	*/
	inline int GetLength()
	{
		return BxUtilGlobal::Max(0, Chars.Length() - 1);
	}

	/*!
	\brief �迭����
	\param index : �迭��ȣ
	\return char�� �ش� ������
	*/
	inline char operator[](int index)
	{
		if(0 <= index && index < Chars.Length())
			return Chars[index];
		return '\0';
	}

	/*!
	\brief ����ȯ �ߺ��Լ�(string)
	\return string�� ��ü ��Ʈ��
	*/
	inline operator string()
	{
		return (string) Chars.GetBytes();
	}

	/*!
	\brief ������ �ߺ��Լ�+(char)
	\param RHS : ���� �ǿ�����
	\return ���ο� �ν��Ͻ�
	*/
	inline BxString operator+(char RHS)
	{
		BxString NewString = *this;
		NewString.Chars.Insert(END, Pool.Make()) = RHS;
		return NewString;
	}

	/*!
	\brief ������ �ߺ��Լ�+(string)
	\param RHS : ���� �ǿ�����
	\return ���ο� �ν��Ͻ�
	*/
	BxString operator+(string RHS)
	{
		BxString NewString = *this;
		const int RHSLen = BxUtilGlobal::StrLen(RHS);
		for(int i = 0; i < RHSLen; ++i)
			NewString.Chars.Insert(END, Pool.Make()) = RHS[i];
		return NewString;
	}

	/*!
	\brief ������ �ߺ��Լ�+(BxString)
	\param RHS : ���� �ǿ�����
	\return ���ο� �ν��Ͻ�
	*/
	BxString operator+(BxString& RHS)
	{
		BxString NewString = *this;
		const int RHSLen = RHS.GetLength();
		for(int i = 0; i < RHSLen; ++i)
			NewString.Chars.Insert(END, Pool.Make()) = RHS[i];
		return NewString;
	}

	/*!
	\brief ������ �ߺ��Լ�=(char)
	\param RHS : ���� �ǿ�����
	\return �ڽ��� ����
	*/
	inline BxString& operator=(char RHS)
	{
		Empty();
		Chars.Insert(0, Pool.Make()) = RHS;
		Chars.Insert(1, Pool.Make()) = '\0';
		return *this;
	}

	/*!
	\brief ������ �ߺ��Լ�=(string)
	\param RHS : ���� �ǿ�����
	\return �ڽ��� ����
	*/
	BxString& operator=(string RHS)
	{
		Empty();
		const int RHSLen = BxUtilGlobal::StrLen(RHS);
		for(int i = 0; i <= RHSLen; ++i)
			Chars.Insert(i, Pool.Make()) = RHS[i];
		return *this;
	}

	/*!
	\brief ������ �ߺ��Լ�=(BxString)
	\param RHS : ���� �ǿ�����
	\return �ڽ��� ����
	*/
	BxString& operator=(const BxString& RHS)
	{
		Empty();
		BxString* RHSPtr = (BxString*) &RHS;
		const int RHSLen = RHSPtr->GetLength();
		for(int i = 0; i <= RHSLen; ++i)
			Chars.Insert(i, Pool.Make()) = (*RHSPtr)[i];
		return *this;
	}

	/*!
	\brief ������ �ߺ��Լ�+=(char)
	\param RHS : ���� �ǿ�����
	*/
	inline void operator+=(char RHS)
	{
		Chars.Insert(END, Pool.Make()) = RHS;
	}

	/*!
	\brief ������ �ߺ��Լ�+=(string)
	\param RHS : ���� �ǿ�����
	*/
	void operator+=(string RHS)
	{
		const int RHSLen = BxUtilGlobal::StrLen(RHS);
		for(int i = 0; i < RHSLen; ++i)
			Chars.Insert(END, Pool.Make()) = RHS[i];
	}

	/*!
	\brief ������ �ߺ��Լ�+=(BxString)
	\param RHS : ���� �ǿ�����
	*/
	void operator+=(BxString& RHS)
	{
		const int RHSLen = RHS.GetLength();
		for(int i = 0; i < RHSLen; ++i)
			Chars.Insert(END, Pool.Make()) = RHS[i];
	}

	/*!
	\brief ���� �����ϱ�
	\param Count : ������ ���ڼ�
	\return ���ο� �ν��Ͻ�
	\see Right
	*/
	BxString Left(int Count)
	{
		BxString NewString;
		Count = BxUtilGlobal::Min(Count, Chars.Length() - 1);
		for(int i = 0; i < Count; ++i)
			NewString.Chars.Insert(i, Pool.Make()) = Chars[i];
		NewString.Chars.Insert(LAST, Pool.Make()) = '\0';
		return NewString;
	}

	/*!
	\brief ���� �����ϱ�
	\param Count : ������ ���ڼ�
	\return ���ο� �ν��Ͻ�
	\see Left
	*/
	BxString Right(int Count)
	{
		BxString NewString;
		Count = BxUtilGlobal::Min(Count, Chars.Length() - 1);
		const int Begin = Chars.Length() - 1 - Count;
		for(int i = 0; i < Count; ++i)
			NewString.Chars.Insert(i, Pool.Make()) = Chars[Begin + i];
		NewString.Chars.Insert(LAST, Pool.Make()) = '\0';
		return NewString;
	}

	/*!
	\brief ���� ã��
	\param Key : ã�� ����
	\return �迭��ġ, -1�� ����
	*/
	int Find(char Key)
	{
		const int Count = Chars.Length() - 1;
		for(int i = 0; i < Count; ++i)
			if(Chars[i] == Key)
				return i;
		return -1;
	}

	/*!
	\brief ���ٷ� ���� ã��
	\param Key : ã�� ����
	\return �迭��ġ, -1�� ����
	*/
	int ReverseFind(char Key)
	{
		const int Count = Chars.Length() - 1;
		for(int i = Count - 1; 0 <= i; --i)
			if(Chars[i] == Key)
				return i;
		return -1;
	}

	/*!
	\brief ��Ʈ�� ������
	\param Other : �۾��� ��Ʈ��
	\param Result : ������ȣ(ù����Ʈ)�� �и��� ���(���� Reset�� ����)
	*/
	global_func void ParseDivision(string Other, BxVar<BxString>& Result)
	{
		const int OtherLength = BxUtilGlobal::StrLen(Other);
		if(OtherLength && Other[0] == Other[OtherLength - 1])
		for(int i = 0; i < OtherLength - 1; ++i)
		{
			if(Other[i] == Other[0])
				Result.Insert(LAST);
			else Result[END] += Other[i];
		}
	}

	/*!
	\brief ��Ʈ���� ��(string)
	\param Other : ���� ��Ʈ��
	\return same-����, param_is_more-�μ��� ŭ, param_is_less-�μ��� ����
	*/
	compare Compare(string Other)
	{
		const int MinLen = BxUtilGlobal::Min(Chars.Length(), BxUtilGlobal::StrLen(Other) + 1);
		for(int i = 0; i < MinLen; ++i)
		{
			if(Chars[i] == Other[i]) continue;
			return (Chars[i] < Other[i])? param_is_more : param_is_less;
		}
		return same;
	}

	/*!
	\brief ��Ʈ���� ��(BxString)
	\param Other : ���� ��Ʈ��
	\return same-����, param_is_more-�μ��� ŭ, param_is_less-�μ��� ����
	*/
	compare Compare(BxString& Other)
	{
		const int MinLen = BxUtilGlobal::Min(Chars.Length(), Other.Chars.Length());
		for(int i = 0; i < MinLen; ++i)
		{
			if(Chars[i] == Other.Chars[i]) continue;
			return (Chars[i] < Other.Chars[i])? param_is_more : param_is_less;
		}
		return same;
	}

	/*!
	\brief ��Ʈ���� ��ҹ��� ���о��� ��(string)
	\param Other : ���� ��Ʈ��
	\return same-����, param_is_more-�μ��� ŭ, param_is_less-�μ��� ����
	*/
	compare CompareNoCase(string Other)
	{
		const int MinLen = BxUtilGlobal::Min(Chars.Length(), BxUtilGlobal::StrLen(Other) + 1);
		for(int i = 0; i < MinLen; ++i)
		{
			char CharC = Chars[i];
			char CharO = Other[i];
			if('A' <= CharC && CharC <= 'Z') CharC += 'a' - 'A';
			if('A' <= CharO && CharO <= 'Z') CharO += 'a' - 'A';
			if(CharC == CharO) continue;
			return (CharC < CharO)? param_is_more : param_is_less;
		}
		return same;
	}

	/*!
	\brief ��Ʈ���� ��ҹ��� ���о��� ��(BxString)
	\param Other : ���� ��Ʈ��
	\return same-����, param_is_more-�μ��� ŭ, param_is_less-�μ��� ����
	*/
	compare CompareNoCase(BxString& Other)
	{
		const int MinLen = BxUtilGlobal::Min(Chars.Length(), Other.Chars.Length());
		for(int i = 0; i < MinLen; ++i)
		{
			char CharC = Chars[i];
			char CharO = Other.Chars[i];
			if('A' <= CharC && CharC <= 'Z') CharC += 'a' - 'A';
			if('A' <= CharO && CharO <= 'Z') CharO += 'a' - 'A';
			if(CharC == CharO) continue;
			return (CharC < CharO)? param_is_more : param_is_less;
		}
		return same;
	}

	/*!
	\brief sprintf���·� ��Ʈ�� ����
	\param format : ������ ���˽�Ʈ��
	\param ... : ������ �μ���
	*/
	void Format(string format, ...)
	{
		va_list List;
		va_start(List, format);
		FormatV(format, List);
		va_end(List);
	}

	/*!
	\brief vsprintf���·� ��Ʈ�� ����
	\param format : ������ ���˽�Ʈ��
	\param list : ������ �μ���
	*/
	void FormatV(string format, va_list list)
	{
		const int TempLen = BxCore::Util::VsnPrintf(null, 0, format, list);
		BxAssert("BxString<Temp�� ������̸� ��µ� �����Ͽ����ϴ�>", 0 <= TempLen);
		string_rw Temp = (string_rw) BxAlloc(TempLen + 1);
		BxCore::Util::VsnPrintf(Temp, TempLen + 1, format, list);
		Empty();
		for(int i = 0; i <= TempLen; ++i)
			Chars.Insert(i, Pool.Make()) = Temp[i];
		BxFree(Temp);
	}

	/*!
	\brief ��ο��� ���ϸ� ����
	\return ����� ��Ʈ��
	*/
	inline BxString GetFileName()
	{
		return (Find('/') < 0)? *this : Right(GetLength() - ReverseFind('/') - 1);
	}

	/*!
	\brief ��ο��� ���ϸ��� ������ �н���(/���� ����) ����
	\return ����� ��Ʈ��
	*/
	inline BxString GetFilePath()
	{
		return (Find('/') < 0)? "" : Left(ReverseFind('/') + 1);
	}

	/*!
	\brief ��ο��� ����Ÿ��Ʋ(Ȯ���� ����) ����
	\return ����� ��Ʈ��
	*/
	inline BxString GetFileTitle()
	{
		BxString FileName = GetFileName();
		return (FileName.Find('.') < 0)? FileName : FileName.Left(FileName.ReverseFind('.'));
	}

	/*!
	\brief ��ο��� ����Ȯ���� ����
	\return ����� ��Ʈ��
	*/
	inline BxString GetFileExt()
	{
		BxString FileExt = (Find('.') < 0)? "" : Right(GetLength() - ReverseFind('.') - 1);
		return (0 <= FileExt.Find('/'))? "" : FileExt;
	}

	/*!
	\brief �����Ϳ��� �ε�
	\param Resource : uint16���� ��Ʈ������� ���� ����, �ش� ��Ʈ���� �ε�
	*/
	void Load(byte*& Resource)
	{
		Empty();
		const int Length = BxUtilGlobal::LoadUint16(Resource);
		for(int i = 0; i <= Length; ++i)
			Chars.Insert(END, Pool.Make()) = BxUtilGlobal::LoadInt8(Resource);
	}

protected:
	/// @cond SECTION_NAME
	BxMemoryPool<char> Pool;
	BxVar<char> Chars;
	/// @endcond
};
