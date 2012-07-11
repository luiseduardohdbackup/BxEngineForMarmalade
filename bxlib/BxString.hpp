#pragma once
#include "BxVar.hpp"
#include "BxUtil.hpp"

//! \brief 스트링 관리
class BxString
{
public:
	/*!
	\brief 기본생성자
	*/
	BxString() {Chars.Insert(0, Pool.Make()) = '\0';}

	/*!
	\brief 복사생성자(char)
	\param RHS : 복사할 인스턴스
	*/
	BxString(char RHS) {operator=(RHS);}

	/*!
	\brief 복사생성자(string)
	\param RHS : 복사할 인스턴스
	*/
	BxString(string RHS) {operator=(RHS);}

	/*!
	\brief 복사생성자(BxString)
	\param RHS : 복사할 인스턴스
	*/
	BxString(const BxString& RHS) {operator=(RHS);}

	/*!
	\brief 복사생성자(sprintf)
	\param src : printf양식의 맵(%d/%s/...)
	\param nouse : 생성자 식별용 값
	\param ... : 가변인자들
	*/
	BxString(string src, void* nouse, ...)
	{
		va_list List;
		va_start(List, nouse);
		FormatV(src, List);
		va_end(List);
	}

	/*!
	\brief 소멸자
	*/
	virtual ~BxString() {Empty();}

public:
	/*!
	\brief 내용 비우기
	*/
	void Empty()
	{
		for(int i = 0; i < Chars.Length(); ++i)
			Pool.Free(Chars.Access(i));
		Chars.Reset(false);
	}

	/*!
	\brief 길이 구하기
	\return 스트링의 글자길이
	*/
	inline int GetLength()
	{
		return BxUtilGlobal::Max(0, Chars.Length() - 1);
	}

	/*!
	\brief 배열접근
	\param index : 배열번호
	\return char형 해당 데이터
	*/
	inline char operator[](int index)
	{
		if(0 <= index && index < Chars.Length())
			return Chars[index];
		return '\0';
	}

	/*!
	\brief 형변환 중복함수(string)
	\return string형 전체 스트링
	*/
	inline operator string()
	{
		return (string) Chars.GetBytes();
	}

	/*!
	\brief 연산자 중복함수+(char)
	\param RHS : 우측 피연산자
	\return 새로운 인스턴스
	*/
	inline BxString operator+(char RHS)
	{
		BxString NewString = *this;
		NewString.Chars.Insert(END, Pool.Make()) = RHS;
		return NewString;
	}

	/*!
	\brief 연산자 중복함수+(string)
	\param RHS : 우측 피연산자
	\return 새로운 인스턴스
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
	\brief 연산자 중복함수+(BxString)
	\param RHS : 우측 피연산자
	\return 새로운 인스턴스
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
	\brief 연산자 중복함수=(char)
	\param RHS : 우측 피연산자
	\return 자신을 리턴
	*/
	inline BxString& operator=(char RHS)
	{
		Empty();
		Chars.Insert(0, Pool.Make()) = RHS;
		Chars.Insert(1, Pool.Make()) = '\0';
		return *this;
	}

	/*!
	\brief 연산자 중복함수=(string)
	\param RHS : 우측 피연산자
	\return 자신을 리턴
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
	\brief 연산자 중복함수=(BxString)
	\param RHS : 우측 피연산자
	\return 자신을 리턴
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
	\brief 연산자 중복함수+=(char)
	\param RHS : 우측 피연산자
	*/
	inline void operator+=(char RHS)
	{
		Chars.Insert(END, Pool.Make()) = RHS;
	}

	/*!
	\brief 연산자 중복함수+=(string)
	\param RHS : 우측 피연산자
	*/
	void operator+=(string RHS)
	{
		const int RHSLen = BxUtilGlobal::StrLen(RHS);
		for(int i = 0; i < RHSLen; ++i)
			Chars.Insert(END, Pool.Make()) = RHS[i];
	}

	/*!
	\brief 연산자 중복함수+=(BxString)
	\param RHS : 우측 피연산자
	*/
	void operator+=(BxString& RHS)
	{
		const int RHSLen = RHS.GetLength();
		for(int i = 0; i < RHSLen; ++i)
			Chars.Insert(END, Pool.Make()) = RHS[i];
	}

	/*!
	\brief 좌측 복사하기
	\param Count : 복사할 글자수
	\return 새로운 인스턴스
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
	\brief 우측 복사하기
	\param Count : 복사할 글자수
	\return 새로운 인스턴스
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
	\brief 글자 찾기
	\param Key : 찾을 글자
	\return 배열위치, -1은 실패
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
	\brief 꺼꾸로 글자 찾기
	\param Key : 찾을 글자
	\return 배열위치, -1은 실패
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
	\brief 스트링 나누기
	\param Other : 작업할 스트링
	\param Result : 나눔기호(첫바이트)로 분리된 결과(강제 Reset은 안함)
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
	\brief 스트링간 비교(string)
	\param Other : 비교할 스트링
	\return same-같음, param_is_more-인수가 큼, param_is_less-인수가 작음
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
	\brief 스트링간 비교(BxString)
	\param Other : 비교할 스트링
	\return same-같음, param_is_more-인수가 큼, param_is_less-인수가 작음
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
	\brief 스트링간 대소문자 구분없는 비교(string)
	\param Other : 비교할 스트링
	\return same-같음, param_is_more-인수가 큼, param_is_less-인수가 작음
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
	\brief 스트링간 대소문자 구분없는 비교(BxString)
	\param Other : 비교할 스트링
	\return same-같음, param_is_more-인수가 큼, param_is_less-인수가 작음
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
	\brief sprintf형태로 스트링 생성
	\param format : 생성할 포맷스트링
	\param ... : 데이터 인수들
	*/
	void Format(string format, ...)
	{
		va_list List;
		va_start(List, format);
		FormatV(format, List);
		va_end(List);
	}

	/*!
	\brief vsprintf형태로 스트링 생성
	\param format : 생성할 포맷스트링
	\param list : 데이터 인수들
	*/
	void FormatV(string format, va_list list)
	{
		const int TempLen = BxCore::Util::VsnPrintf(null, 0, format, list);
		BxAssert("BxString<Temp의 예상길이를 얻는데 실패하였습니다>", 0 <= TempLen);
		string_rw Temp = (string_rw) BxAlloc(TempLen + 1);
		BxCore::Util::VsnPrintf(Temp, TempLen + 1, format, list);
		Empty();
		for(int i = 0; i <= TempLen; ++i)
			Chars.Insert(i, Pool.Make()) = Temp[i];
		BxFree(Temp);
	}

	/*!
	\brief 경로에서 파일명 추출
	\return 추출된 스트링
	*/
	inline BxString GetFileName()
	{
		return (Find('/') < 0)? *this : Right(GetLength() - ReverseFind('/') - 1);
	}

	/*!
	\brief 경로에서 파일명을 제외한 패스명(/문자 포함) 추출
	\return 추출된 스트링
	*/
	inline BxString GetFilePath()
	{
		return (Find('/') < 0)? "" : Left(ReverseFind('/') + 1);
	}

	/*!
	\brief 경로에서 파일타이틀(확장자 제외) 추출
	\return 추출된 스트링
	*/
	inline BxString GetFileTitle()
	{
		BxString FileName = GetFileName();
		return (FileName.Find('.') < 0)? FileName : FileName.Left(FileName.ReverseFind('.'));
	}

	/*!
	\brief 경로에서 파일확장자 추출
	\return 추출된 스트링
	*/
	inline BxString GetFileExt()
	{
		BxString FileExt = (Find('.') < 0)? "" : Right(GetLength() - ReverseFind('.') - 1);
		return (0 <= FileExt.Find('/'))? "" : FileExt;
	}

	/*!
	\brief 데이터에서 로드
	\param Resource : uint16으로 스트링사이즈를 얻은 다음, 해당 스트링을 로드
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
