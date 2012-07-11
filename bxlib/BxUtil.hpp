#pragma once
#include "BxMemory.hpp"

//! \brief BxUtil°ü·Ã ±Û·Î¹úÇÔ¼ö
namespace BxUtilGlobal
{
	/*!
	\brief ÆÄÀÌ°ª ±¸ÇÏ±â
	\return ÆÄÀÌ°ª(3.141592...)
	*/
	static inline fint Pi()
	{
		return (fint)(3.14159265358979323846264338327950288419716939937510 * 0x10000 + 0.5);
	}

	/*!
	\brief ¹Ð¸®ÃÊ¿¡ µû¸¥ ÇÁ·¹ÀÓ¼ö ±¸ÇÏ±â
	\return ÇÁ·¹ÀÓ¼ö
	*/
	static inline int Fps(const int ms)
	{
		return ms / MILLISECOND_PER_FRAME;
	}

	/*!
	\brief ÃÖ¼Ò°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\param B : ÀÎ¼öB
	\return ÃÖ¼Ò°ª
	\see Max
	*/
	static inline const int Min(const int A, const int B)
	{
		return (A < B)? A : B;
	}

	/*!
	\brief ÃÖ´ë°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\param B : ÀÎ¼öB
	\return ÃÖ´ë°ª
	\see Min
	*/
	static inline const int Max(const int A, const int B)
	{
		return (A > B)? A : B;
	}

	/*!
	\brief ÃÖ¼ÒÃÖ´ë ÇÑ°è¼³Á¤
	\param MinValue : ÃÖ¼Ò°ª
	\param Value : ÀÔ·Â°ª
	\param MaxValue : ÃÖ´ë°ª
	\return ÃÖ¼ÒÃÖ´ë ÇÑ°è¼³Á¤À» Àû¿ëÇÑ ÀÔ·Â°ª
	\see Min
	*/
	static inline const int MinMax(const int MinValue, int Value, const int MaxValue)
	{
		Value = (Value < MaxValue)? Value : MaxValue;
		return (MinValue < Value)? Value : MinValue;
	}

	/*!
	\brief Àý´ë°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\return Àý´ë°ª
	*/
	static inline int Abs(const int A)
	{
		return (A < 0)? -A : A;
	}

	/*!
	\brief Á¦°ö°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\return Á¦°ö°ª
	*/
	static inline int Pow(const int A)
	{
		return A * A;
	}

	/*!
	\brief 64ºñÆ® Á¦°ö°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\return 64ºñÆ® Á¦°ö°ª
	*/
	static inline huge Pow64(const int A)
	{
		return ((huge) A) * A;
	}

	/*!
	\brief Á¦°ö±Ù°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\return Á¦°ö±Ù°ª
	*/
	static int Sqrt(const int A)
	{
		uint CheckFlag = 0x8000, BitResult = 0, Result = 0;
		for(int i = 0; i < 16; i++)
			if(Pow(BitResult = Result | (CheckFlag >> i)) <= A)
				Result = BitResult;
		return (int) Result;
	}

	/*!
	\brief 64ºñÆ® Á¦°ö±Ù°ª ±¸ÇÏ±â
	\param A : ÀÎ¼öA
	\return 64ºñÆ® Á¦°ö±Ù°ª
	*/
	static int Sqrt64(const huge A)
	{
		uint CheckFlag = 0x80000000, BitResult = 0, Result = 0;
		for(int i = 0; i < 32; i++)
			if(Pow64(BitResult = Result | (CheckFlag >> i)) <= A)
				Result = BitResult;
		return (int) Result;
	}

	/*!
	\brief ½ºÆ®¸µ±æÀÌ ±¸ÇÏ±â
	\param String : ½ºÆ®¸µ
	\return ½ºÆ®¸µÀÇ ¹ÙÀÌÆ®±æÀÌ
	*/
	static int StrLen(string String)
	{
		BxAssert("BxUtil", String);
		int i = -1;
		while(String[++i]);
		return i;
	}

	/*!
	\brief È®ÀåµÈ ½ºÆ®¸µ±æÀÌ ±¸ÇÏ±â(\r°ú \nµµ ÀÎ½Ä)
	\param String : ½ºÆ®¸µ
	\return ½ºÆ®¸µÀÇ ¹ÙÀÌÆ®±æÀÌ
	*/
	static int StrLenEx(string String)
	{
		BxAssert("BxUtil", String);
		int i = -1;
		while(String[++i] && String[i] != '\r' && String[i] != '\n');
		return i;
	}

	/*!
	\brief ½ºÆ®¸µ º¹»çÇÏ±â
	\param StringDst : º¹»çÇÒ ½ºÆ®¸µ
	\param StringSrc : °¡Á®¿Ã ½ºÆ®¸µ
	\param SrcLength : º¹»çµÉ »çÀÌÁî(StringSrcÀÇ ±æÀÌ·Î ÀÚµ¿¼ÂÆÃ : -1)
	*/
	static void StrCpy(string_rw StringDst, string StringSrc, int SrcLength = -1)
	{
		const int SrcLen = (SrcLength < 0)? StrLen(StringSrc) : SrcLength;
		BxCore::Util::MemMove(StringDst, StringSrc, SrcLen);
		StringDst[SrcLen] = '\0';
	}

	/*!
	\brief ½ºÆ®¸µ ÇÒ´ç ¹× º¹»çÇÏ±â
	\param StringSrc : °¡Á®¿Ã ½ºÆ®¸µ
	\param SrcLength : º¹»çµÉ »çÀÌÁî(StringSrcÀÇ ±æÀÌ·Î ÀÚµ¿¼ÂÆÃ : -1)
	\return ÇÒ´ç ¹× º¹»çµÈ ½ºÆ®¸µ
	*/
	static string StrCpyWithAlloc(string StringSrc, int SrcLength = -1)
	{
		const int SrcLen = (SrcLength < 0)? StrLen(StringSrc) : SrcLength;
		string_rw ResultDst = (string_rw) BxAlloc(SrcLen + 1);
		StrCpy(ResultDst, StringSrc, SrcLen);
		return ResultDst;
	}

	/*!
	\brief ½ºÆ®¸µ ºñ±³ÇÏ±â
	\param StringA : ½ºÆ®¸µA
	\param StringB : ½ºÆ®¸µB
	\param FixedLength : °íÁ¤½Ä ºñ±³±¸°£(»ç¿ë¾ÈÇÔ : -1)
	\return same-°°À½, b_is_more-½ºÆ®¸µB°¡ Å­, b_is_less-½ºÆ®¸µB°¡ ÀÛÀ½
	*/
	static compare StrCmp(string StringA, string StringB, int FixedLength = -1)
	{
		const int MinLen = (FixedLength < 0)? Min(StrLen(StringA), StrLen(StringB)) + 1 : FixedLength;
		for(int i = 0; i < MinLen; ++i)
		{
			if(StringA[i] == StringB[i]) continue;
			return (StringA[i] < StringB[i])? b_is_more : b_is_less;
		}
		return same;
	}

	/*!
	\brief ´ë¼Ò¹®ÀÚ ±¸ºÐ¾øÀÌ ½ºÆ®¸µ ºñ±³ÇÏ±â
	\param StringA : ½ºÆ®¸µA
	\param StringB : ½ºÆ®¸µB
	\param FixedLength : °íÁ¤½Ä ºñ±³±¸°£(»ç¿ë¾ÈÇÔ : -1)
	\return same-°°À½, b_is_more-½ºÆ®¸µB°¡ Å­, b_is_less-½ºÆ®¸µB°¡ ÀÛÀ½
	*/
	static compare StrICmp(string StringA, string StringB, int FixedLength = -1)
	{
		const int MinLen = (FixedLength < 0)? Min(StrLen(StringA), StrLen(StringB)) + 1 : FixedLength;
		for(int i = 0; i < MinLen; ++i)
		{
			char CharA = StringA[i];
			char CharB = StringB[i];
			if('A' <= CharA && CharA <= 'Z') CharA += 'a' - 'A';
			if('A' <= CharB && CharB <= 'Z') CharB += 'a' - 'A';
			if(CharA == CharB) continue;
			return (CharA < CharB)? b_is_more : b_is_less;
		}
		return same;
	}

	/*!
	\brief ½ºÆ®¸µ°£ µ¿ÀÏ±¸¿ª ±æÀÌ±¸ÇÏ±â
	\param StringA : ½ºÆ®¸µA
	\param StringB : ½ºÆ®¸µB
	\return ¹ÙÀÌÆ®±æÀÌ
	*/
	static int StrSameCount(string StringA, string StringB)
	{
		int Focus = 0;
		while(StringA[Focus] && StringA[Focus] == StringB[Focus])
			++Focus;
		return Focus;
	}

	/*!
	\brief ½ºÆ®¸µÀ» Á¤¼ö·Î º¯È¯
	\param String : ½ºÆ®¸µ
	\param StringLength : Ã³¸®µÉ »çÀÌÁî(StringÀÇ ±æÀÌ·Î ÀÚµ¿¼ÂÆÃ : -1)
	\return º¯È¯µÈ Á¤¼ö
	*/
	static int AtoI(string String, int StringLength = -1)
	{
		int IntValue = 0, IsMinus = -1;
		const int StringLen = (StringLength < 0)? StrLenEx(String) : StringLength;
		for(int i = 0; i < StringLen; ++i)
		{
			if('0' <= String[i] && String[i] <= '9')
			{
				IntValue *= 10;
				IntValue += String[i] - '0';
				if(IsMinus == -1) IsMinus = 0;
			}
			else if(IsMinus == -1 && String[i] == '-')
				IsMinus = 1;
			else if(IsMinus == -1 && String[i] == '+')
				IsMinus = 0;
			else if(String[i] != ' ' && String[i] != '\t')
				break;
		}
		return (IsMinus == 1)? -IntValue : IntValue;
	}

	/*!
	\brief Á¤¼ö¸¦ ½ºÆ®¸µÀ¸·Î º¯È¯
	\param String : ½ºÆ®¸µ
	\return º¯È¯µÈ Á¤¼ö
	*/
	static string ItoA(int Value)
	{
		const int Length = 255;
		global_data char Result[Length + 1];
		Result[Length] = '\0';
		const bool IsMinus = (Value < 0);
		uint UValue = (uint)((IsMinus)? -Value : Value);
		for(int i = Length - 1; 0 <= i; --i)
		{
			Result[i] = '0' + (UValue % 10);
			UValue /= 10;
			if(UValue == 0)
			{
				if(IsMinus)
				{
					Result[i - 1] = '-';
					return &Result[i - 1];
				}
				else return &Result[i];
			}
		}
		return &Result[Length];
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ·Îµå(char)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\return ·ÎµåÇÑ °ª
	*/
	static inline char LoadInt8(byte*& Resource)
	{
		char Result = (Resource[0] & 0xFF);
		Resource += 1;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ·Îµå(short)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\return ·ÎµåÇÑ °ª
	*/
	static inline short LoadInt16(byte*& Resource)
	{
		short Result = (Resource[0] & 0xFF) | ((Resource[1] & 0xFF) << 8);
		Resource += 2;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ·Îµå(int)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\return ·ÎµåÇÑ °ª
	*/
	static inline int LoadInt32(byte*& Resource)
	{
		int Result = (Resource[0] & 0xFF) | ((Resource[1] & 0xFF) << 8) | ((Resource[2] & 0xFF) << 16) | ((Resource[3] & 0xFF) << 24);
		Resource += 4;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ·Îµå(byte)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\return ·ÎµåÇÑ °ª
	*/
	static inline byte LoadUint8(byte*& Resource)
	{
		byte Result = (Resource[0] & 0xFF);
		Resource += 1;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ·Îµå(ushort)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\return ·ÎµåÇÑ °ª
	*/
	static inline ushort LoadUint16(byte*& Resource)
	{
		ushort Result = (Resource[0] & 0xFF) | ((Resource[1] & 0xFF) << 8);
		Resource += 2;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ·Îµå(uint)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\return ·ÎµåÇÑ °ª
	*/
	static inline uint LoadUint32(byte*& Resource)
	{
		uint Result = (Resource[0] & 0xFF) | ((Resource[1] & 0xFF) << 8) | ((Resource[2] & 0xFF) << 16) | ((Resource[3] & 0xFF) << 24);
		Resource += 4;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ¹ÙÀÌÆ®¹è¿­ ·Îµå(uint)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\param Dest : ¾µ µ¥ÀÌÅÍ
	\param Length : ¾µ ¹ÙÀÌÆ®±æÀÌ
	*/
	static inline void LoadBytes(byte*& Resource, void* Dest, uint Length)
	{
		BxCore::Util::MemMove(Dest, Resource, Length);
		Resource += Length;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ºñÆ®¹è¿­ ·Îµå(uint)
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\param CurrentBit : ÇöÀç ºñÆ®
	\param BitCount : ÀÐÀ» ºñÆ®±æÀÌ
	\return ·ÎµåÇÑ °ª
	*/
	static uint LoadBits(byte*& Resource, int& CurrentBit, const int BitCount)
	{
		uint Result = 0;
		for(int i = 0; i < BitCount; ++i)
		{
			Result |= ((*Resource >> CurrentBit) & 0x1) << i;
			if(!(++CurrentBit & 0x7))
			{
				CurrentBit = 0;
				++Resource;
			}
		}
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ÀÓÀÇ¹è¿­ ·Îµå
	\param Resource : ÀÐÀ» µ¥ÀÌÅÍ
	\param TypeLen : ÀÐÀ» ¹ÙÀÌÆ®±æÀÌ
	\return ·ÎµåÇÑ µ¥ÀÌÅÍ
	*/
	static inline byte* LoadType(byte*& Resource, uint TypeLen)
	{
		byte* Result = Resource;
		Resource += TypeLen;
		return Result;
	}

	/*!
	\brief µ¥ÀÌÅÍ¿¡¼­ ½ºÆ®¸µ ·Îµå
	\param Resource : uint16À¸·Î ½ºÆ®¸µ»çÀÌÁî¸¦ ¾òÀº ´ÙÀ½, ÇØ´ç ½ºÆ®¸µÀ» ·Îµå
	\return ·ÎµåÇÑ ½ºÆ®¸µ
	*/
	static inline string LoadString(byte*& Resource)
	{
		uint Length = LoadUint16(Resource);
		char* Chars = BxNew_Array(char, Length + 1);
		LoadBytes(Resource, Chars, Length);
		Chars[Length] = '\0';
		return Chars;
	}

	/*!
	\brief »ó¼ö³ª¿­·Î ¹è¿­À» ÃÊ±âÈ­
	\param Dst : ÃÊ±âÈ­ÇÒ ¹è¿­
	\param Count : »ó¼ö³ª¿­±æÀÌ
	\param Value : Ã¹¹øÂ° »ó¼ö°ª
	\param ... : »ó¼ö°ªµé
	*/
	template<typename TYPE1, typename TYPE2>
	static inline void SetArray(TYPE1* Dst, int Count, TYPE2 Value, ...)
	{
		BxAssert("BxUtil", 0 < Count);
		Dst[0] = (TYPE1) Value;
		va_list List;
		va_start(List, Value);
		for(int i = 1; i < Count; ++i)
			Dst[i] = (TYPE1) va_arg(List, TYPE2);
		va_end(List);
	}
}

//! \brief °¢Á¾ ¼öÇÐ/¹°¸®¸ðµâ Á¦°ø
class BxUtil
{
public:
	/*!
	\brief ·£´ýÇÔ¼ö ÃÊ±âÈ­
	\param Seed : ÃÊ±âÈ­°ª
	\see BxCore::System::GetTimerMs
	*/
	global_func void SetRandom(const uint Seed)
	{
		_SetRand(Seed);
		for(int i = 0; i < 256; ++i)
			RandomTable(i) = (_MakeRand() << 30) | (_MakeRand() << 15) | _MakeRand();
	}

	/*!
	\brief ·£´ý°ª ±¸ÇÏ±â
	\param Key : ·£´ýÅ×ÀÌºí ÀÎµ¦½º°ª, -1Àº ½Ç ·£´ý°ª
	\return °á°ú°ª
	*/
	global_func inline uint GetRandom(const int Key = -1)
	{
		if(Key < 0) return (_MakeRand() << 30) | (_MakeRand() << 15) | _MakeRand();
		return RandomTable(Key & 0xFF);
	}

	/*!
	\brief ±¸°£³» ·£´ý°ª ±¸ÇÏ±â
	\param MinValue : ÃÖ¼Ò°ª
	\param MaxValue : ÃÖ´ë°ª
	\param Key : ·£´ýÅ×ÀÌºí ÀÎµ¦½º°ª, -1Àº ½Ç ·£´ý°ª
	\return ÃÖ¼Ò°ª(Æ÷ÇÔ)¿¡¼­ ÃÖ´ë°ª(Æ÷ÇÔ)»çÀÌÀÇ °á°ú°ª
	*/
	global_func inline int GetValue(const int MinValue, const int MaxValue, int Key = -1)
	{
		return MinValue + (GetRandom(Key) % (MaxValue - MinValue + 1));
	}

	/*!
	\brief °¢µµ³» ·£´ý°ª ±¸ÇÏ±â
	\param MinValue : ÃÖ¼Ò°¢µµ°ª
	\param MaxValue : ÃÖ´ë°¢µµ°ª
	\param Key : ·£´ýÅ×ÀÌºí ÀÎµ¦½º°ª, -1Àº ½Ç ·£´ý°ª
	\return ÃÖ¼Ò°ª(Æ÷ÇÔ)¿¡¼­ ÃÖ´ë°ª(Æ÷ÇÔ)»çÀÌÀÇ °á°ú°ª
	*/
	global_func inline int GetValueAngle(const int MinValue, const int MaxValue, int Key = -1)
	{
		if(180 < MaxValue - MinValue)
			return (MaxValue + (GetRandom(Key) % (360 - MaxValue + MinValue + 1))) % 360;
		return GetValue(MinValue, MaxValue, Key) % 360;
	}

	/*!
	\brief Cos°ª ±¸ÇÏ±â
	\param Angle1024 : °¢µµ°ª
	\return Cos°ª
	*/
	global_func inline fint Cos(int Angle1024)
	{
		Angle1024 = Angle1024 & 0x3FF;
		return Cos1024Table(Angle90 - BxUtilGlobal::Abs((Angle1024 & 0x1FF) - Angle90))
			* ((((Angle1024 + Angle270 - 1) & 0x3FF) < Angle180)? -1 : 1);
	}

	/*!
	\brief Sin°ª ±¸ÇÏ±â
	\param Angle1024 : °¢µµ°ª
	\return Sin°ª
	*/
	global_func inline fint Sin(int Angle1024)
	{
		return Cos(Angle1024 + Angle270);
	}

	/*!
	\brief ¹æÇâÀ¸·Î °¢µµ ±¸ÇÏ±â
	\param VectorX : ¹æÇâX
	\param VectorY : ¹æÇâY
	\return °¢µµ°ª
	*/
	global_func int GetAngle1024(const int VectorX, const int VectorY)
	{
		if(VectorX == 0) return (VectorY < 0)? Angle270 : Angle90;
		int Angle1024 = -1, Tan1024 = BxUtilGlobal::Abs((int)((((huge) VectorY) << 16) / ((huge) VectorX)));
		while(++Angle1024 < Angle90 && Tan1024Table(Angle1024) <= Tan1024);
		return 0x3FF & (((VectorY < 0)? Angle180 : 0)
			+ (((VectorX < 0) == (VectorY < 0))? Angle1024 : Angle180 - Angle1024));
	}

	/*!
	\brief Á¡ 3°³°¡ ½Ã°è¹æÇâÀÎÁö¸¦ È®ÀÎ
	\param Start : Ã¹Á¡
	\param End : µÑÂ°Á¡
	\param Src : ºñ±³ÇÒ ¼ÂÂ°Á¡
	\return true-½Ã°è¹æÇâ, false-¹Ý½Ã°è¹æÇâ
	*/
	global_func inline bool IsClockwise(const point Start, const point End, const point Src)
	{
		return ((Start.x - End.x) * (Src.y - End.y) - (Start.y - End.y) * (Src.x - End.x)) < 0;
	}

	/*!
	\brief CP949¸¦ UTF16À¸·Î ÀüÈ¯
	\param cp949 : º¯È¯ÇÒ CP949½ºÆ®¸µ
	\param cp949length : º¯È¯ÇÒ CP949½ºÆ®¸µ ¹è¿­¼ö·®(-1Àº ±æÀÌ ÀÚµ¿ÃøÁ¤)
	\param utf16 : ¹ÞÀ» UTF16½ºÆ®¸µ
	\param utf16length : ¹ÞÀ» UTF16½ºÆ®¸µ ¹è¿­¼ö·®
	\return º¯È¯µÈ ¹è¿­¼ö·®
	*/
	global_func int CP949ToUTF16(string cp949, int cp949length, wstring_rw utf16, const int utf16length)
	{
		if(cp949length < 0) cp949length = 0x7FFFFFFF;
		int utf16focus = 0;
		for(int i = 0; cp949[i] && i < cp949length && utf16focus < utf16length; ++i)
		{
			if(cp949[i] < 0x80)
				utf16[utf16focus++] = CP949ToUTF16_Search(cp949[i]);
			else if(cp949[++i] && i < cp949length)
				utf16[utf16focus++] = CP949ToUTF16_Search(cp949[i - 1], cp949[i]);
		}
		utf16[utf16focus] = 0;
		return utf16focus;
	}

	/*!
	\brief UTF16¸¦ CP949·Î ÀüÈ¯
	\param utf16 : º¯È¯ÇÒ UTF16½ºÆ®¸µ
	\param utf16length : º¯È¯ÇÒ UTF16½ºÆ®¸µ ¹è¿­¼ö·®(-1Àº ±æÀÌ ÀÚµ¿ÃøÁ¤)
	\param cp949 : ¹ÞÀ» CP949½ºÆ®¸µ
	\param cp949length : ¹ÞÀ» CP949½ºÆ®¸µ ¹è¿­¼ö·®
	\return º¯È¯µÈ ¹è¿­¼ö·®
	*/
	global_func int UTF16ToCP949(wstring utf16, int utf16length, string_rw cp949, const int cp949length)
	{
		if(utf16length < 0) utf16length = 0x7FFFFFFF;
		int cp949focus = 0;
		for(int i = 0; utf16[i] && i < utf16length && cp949focus < cp949length; ++i)
		{
			string Result = UTF16ToCP949_Search(utf16[i]);
			cp949[cp949focus++] = Result[0];
			if(0x80 <= Result[0] && cp949focus < cp949length)
				cp949[cp949focus++] = Result[1];
		}
		cp949[cp949focus] = 0;
		return cp949focus;
	}

	/*!
	\brief UTF16¸¦ UTF8·Î ÀüÈ¯
	\param utf16 : º¯È¯ÇÒ UTF16½ºÆ®¸µ
	\param utf16length : º¯È¯ÇÒ UTF16½ºÆ®¸µ ¹è¿­¼ö·®(-1Àº ±æÀÌ ÀÚµ¿ÃøÁ¤)
	\param utf8 : ¹ÞÀ» UTF8½ºÆ®¸µ
	\see BxUtil::GetLengthByUTF8
	*/
	global_func void UTF16ToUTF8(wstring utf16, int utf16length, string_rw utf8)
	{
		if(utf16length < 0) utf16length = 0x7FFFFFFF;
		for(int i = 0; i < utf16length && utf16[i]; ++i)
		{
			if((utf16[i] & 0xFF80) == 0x0000) // Ascii
				*(utf8++) = utf16[i] & 0x00FF;
			else if((utf16[i] & 0xF800) == 0x0000) // 2Bytes-Less
			{
				*(utf8++) = 0xC0 | ((utf16[i] & 0x07C0) >> 6);
				*(utf8++) = 0x80 | (utf16[i] & 0x003F);
			}
			else if((utf16[i] & 0xFC00) == 0xD800 && (utf16[i + 1] & 0xFC00) == 0xDC00) // 4Bytes
			{
				const wchar U_Value = ((utf16[i] & 0x03C0) >> 6) + 1;
				*(utf8++) = 0xF0 | ((U_Value & 0x001C) >> 2);
				*(utf8++) = 0x80 | ((U_Value & 0x0003) << 4) | ((utf16[i] & 0x003C) >> 2);
				*(utf8++) = 0x80 | ((utf16[i] & 0x0003) << 4) | ((utf16[i + 1] & 0x03C0) >> 6);
				*(utf8++) = 0x80 | (utf16[i + 1] & 0x003F);
				i += 1;
			}
			else // 2Bytes-Full
			{
				*(utf8++) = 0xE0 | ((utf16[i] & 0xF000) >> 12);
				*(utf8++) = 0x80 | ((utf16[i] & 0x0FC0) >> 6);
				*(utf8++) = 0x80 | (utf16[i] & 0x003F);
			}
		}
	}

	/*!
	\brief UTF16À» UTF8·Î ÀüÈ¯½Ã ÇÊ¿äÇÑ UTF8ÀÇ ¹è¿­¼ö·®
	\param utf16 : º¯È¯ÇÒ UTF16½ºÆ®¸µ
	\param utf16length : º¯È¯ÇÒ UTF16½ºÆ®¸µ ¹è¿­¼ö·®(-1Àº ±æÀÌ ÀÚµ¿ÃøÁ¤)
	\return ÇÊ¿äÇÑ ¹è¿­¼ö·®
	*/
	global_func int GetLengthByUTF8(wstring utf16, int utf16length)
	{
		if(utf16length < 0) utf16length = 0x7FFFFFFF;
		int Result = 0;
		for(int i = 0; i < utf16length && utf16[i]; ++i)
		{
			if((utf16[i] & 0xFF80) == 0x0000) // Ascii
				Result += 1;
			else if((utf16[i] & 0xF800) == 0x0000) // 2Bytes-Less
				Result += 2;
			else if((utf16[i] & 0xFC00) == 0xD800 && (utf16[i + 1] & 0xFC00) == 0xDC00) // 4Bytes
			{
				Result += 4;
				i += 1;
			}
			else // 2Bytes-Full
				Result += 3;
		}
		return Result;
	}

	/*!
	\brief UTF8À» UTF16À¸·Î ÀüÈ¯
	\param utf8 : º¯È¯ÇÒ UTF8½ºÆ®¸µ
	\param utf8length : º¯È¯ÇÒ UTF8½ºÆ®¸µ ¹è¿­¼ö·®(-1Àº ±æÀÌ ÀÚµ¿ÃøÁ¤)
	\param utf16 : ¹ÞÀ» UTF16½ºÆ®¸µ
	\see BxUtil::GetLengthByUTF16
	*/
	global_func void UTF8ToUTF16(string utf8, int utf8length, wstring_rw utf16)
	{
		if(utf8length < 0) utf8length = 0x7FFFFFFF;
		for(int i = 0; i < utf8length && utf8[i]; ++i)
		{
			if((utf8[i] & 0xF8) == 0xF0) // 4Bytes
			{
				const char W_Value = (((utf8[i] & 0x07) << 2) | ((utf8[i + 1] & 0x30) >> 4)) - 1;
				*(utf16++) = 0xD800 | (W_Value << 6) | ((utf8[i + 1] & 0x0F) << 2) | ((utf8[i + 2] & 0x30) >> 4);
				*(utf16++) = 0xDC00 | ((utf8[i + 2] & 0x0F) << 6) | (utf8[i + 3] & 0x3F);
				i += 3;
			}
			else if((utf8[i] & 0xF0) == 0xE0) // 3Bytes
			{
				*(utf16++) = ((utf8[i] & 0x0F) << 12) | ((utf8[i + 1] & 0x3F) << 6) | (utf8[i + 2] & 0x3F);
				i += 2;
			}
			else if((utf8[i] & 0xE0) == 0xC0) // 2Bytes
			{
				*(utf16++) = ((utf8[i] & 0x1F) << 6) | (utf8[i + 1] & 0x3F);
				i += 1;
			}
			else // 1Bytes
			{
				*(utf16++) = utf8[i] & 0xFF;
			}
		}
	}

	/*!
	\brief UTF8À» UTF16À¸·Î ÀüÈ¯½Ã ÇÊ¿äÇÑ UTF16ÀÇ ¹è¿­¼ö·®
	\param utf8 : º¯È¯ÇÒ UTF8½ºÆ®¸µ
	\param utf8length : º¯È¯ÇÒ UTF8½ºÆ®¸µ ¹è¿­¼ö·®(-1Àº ±æÀÌ ÀÚµ¿ÃøÁ¤)
	\return ÇÊ¿äÇÑ ¹è¿­¼ö·®
	*/
	global_func int GetLengthByUTF16(string utf8, int utf8length)
	{
		if(utf8length < 0) utf8length = 0x7FFFFFFF;
		int Result = 0;
		for(int i = 0; i < utf8length && utf8[i]; ++i)
		{
			if((utf8[i] & 0xF8) == 0xF0) // 4Bytes
			{
				Result += 2;
				i += 3;
			}
			else if((utf8[i] & 0xF0) == 0xE0) // 3Bytes
			{
				Result += 1;
				i += 2;
			}
			else if((utf8[i] & 0xE0) == 0xC0) // 2Bytes
			{
				Result += 1;
				i += 1;
			}
			else // 1Bytes
				Result += 1;
		}
		return Result;
	}

private:
	enum {Angle90 = 256, Angle180 = 512, Angle270 = 768};

	global_func inline uint& RandomTable(int Index)
	{
		global_data uint _RandomTable[256] = {0};
		return _RandomTable[Index];
	}

	global_func inline const int& Cos1024Table(int Index)
	{
		global_data const int _Cos1024Table[] = {
			0x10000,
			0xFFFF, 0xFFFB, 0xFFF5, 0xFFEC, 0xFFE1, 0xFFD4, 0xFFC4, 0xFFB1, 0xFF9C, 0xFF85, 0xFF6B, 0xFF4E, 0xFF30, 0xFF0E, 0xFEEB, 0xFEC4,
			0xFE9C, 0xFE71, 0xFE43, 0xFE13, 0xFDE1, 0xFDAC, 0xFD74, 0xFD3B, 0xFCFE, 0xFCC0, 0xFC7F, 0xFC3B, 0xFBF5, 0xFBAD, 0xFB62, 0xFB15,
			0xFAC5, 0xFA73, 0xFA1F, 0xF9C8, 0xF96E, 0xF913, 0xF8B4, 0xF854, 0xF7F1, 0xF78C, 0xF724, 0xF6BA, 0xF64E, 0xF5DF, 0xF56E, 0xF4FA,
			0xF484, 0xF40C, 0xF391, 0xF314, 0xF295, 0xF213, 0xF18F, 0xF109, 0xF080, 0xEFF5, 0xEF68, 0xEED9, 0xEE47, 0xEDB3, 0xED1C, 0xEC83,
			0xEBE8, 0xEB4B, 0xEAAB, 0xEA0A, 0xE966, 0xE8BF, 0xE817, 0xE76C, 0xE6BF, 0xE610, 0xE55E, 0xE4AA, 0xE3F4, 0xE33C, 0xE282, 0xE1C6,
			0xE107, 0xE046, 0xDF83, 0xDEBE, 0xDDF7, 0xDD2D, 0xDC62, 0xDB94, 0xDAC4, 0xD9F2, 0xD91E, 0xD848, 0xD770, 0xD696, 0xD5BA, 0xD4DB,
			0xD3FB, 0xD318, 0xD234, 0xD14D, 0xD065, 0xCF7A, 0xCE8E, 0xCD9F, 0xCCAE, 0xCBBC, 0xCAC7, 0xC9D1, 0xC8D9, 0xC7DE, 0xC6E2, 0xC5E4,
			0xC4E4, 0xC3E2, 0xC2DE, 0xC1D8, 0xC0D1, 0xBFC7, 0xBEBC, 0xBDAF, 0xBCA0, 0xBB8F, 0xBA7D, 0xB968, 0xB852, 0xB73A, 0xB620, 0xB505,
			0xB3E8, 0xB2C9, 0xB1A8, 0xB086, 0xAF62, 0xAE3C, 0xAD14, 0xABEB, 0xAAC1, 0xA994, 0xA866, 0xA736, 0xA605, 0xA4D2, 0xA39E, 0xA268,
			0xA130, 0x9FF7, 0x9EBC, 0x9D80, 0x9C42, 0x9B03, 0x99C2, 0x9880, 0x973C, 0x95F7, 0x94B0, 0x9368, 0x921F, 0x90D4, 0x8F88, 0x8E3A,
			0x8CEB, 0x8B9A, 0x8A49, 0x88F6, 0x87A1, 0x864C, 0x84F5, 0x839C, 0x8243, 0x80E8, 0x7F8C, 0x7E2F, 0x7CD0, 0x7B70, 0x7A10, 0x78AD,
			0x774A, 0x75E6, 0x7480, 0x731A, 0x71B2, 0x7049, 0x6EDF, 0x6D74, 0x6C08, 0x6A9B, 0x692D, 0x67BE, 0x664E, 0x64DD, 0x636B, 0x61F8,
			0x6084, 0x5F0F, 0x5D99, 0x5C22, 0x5AAA, 0x5932, 0x57B9, 0x563E, 0x54C3, 0x5348, 0x51CB, 0x504D, 0x4ECF, 0x4D50, 0x4BD1, 0x4A50,
			0x48CF, 0x474D, 0x45CB, 0x4447, 0x42C3, 0x413F, 0x3FBA, 0x3E34, 0x3CAE, 0x3B27, 0x399F, 0x3817, 0x368E, 0x3505, 0x337C, 0x31F1,
			0x3067, 0x2EDC, 0x2D50, 0x2BC4, 0x2A38, 0x28AB, 0x271E, 0x2590, 0x2402, 0x2274, 0x20E5, 0x1F56, 0x1DC7, 0x1C38, 0x1AA8, 0x1918,
			0x1787, 0x15F7, 0x1466, 0x12D5, 0x1144, 0x0FB3, 0x0E21, 0x0C90, 0x0AFE, 0x096C, 0x07DA, 0x0648, 0x04B6, 0x0324, 0x0192, 0x0000
		};
		return _Cos1024Table[Index];
	}

	global_func inline const int& Tan1024Table(int Index)
	{
		global_data const int _Tan1024Table[] = {
			0x000000C9, 0x0000025B, 0x000003ED, 0x0000057F, 0x00000712, 0x000008A4, 0x00000A37, 0x00000BCA, 0x00000D5D, 0x00000EF0, 0x00001084, 0x00001218, 0x000013AC, 0x00001541, 0x000016D6, 0x0000186B,
			0x00001A01, 0x00001B98, 0x00001D2F, 0x00001EC7, 0x0000205F, 0x000021F8, 0x00002391, 0x0000252B, 0x000026C6, 0x00002862, 0x000029FF, 0x00002B9C, 0x00002D3A, 0x00002ED9, 0x00003079, 0x0000321B,
			0x000033BD, 0x00003560, 0x00003704, 0x000038A9, 0x00003A4F, 0x00003BF7, 0x00003DA0, 0x00003F4A, 0x000040F5, 0x000042A2, 0x00004450, 0x000045FF, 0x000047B0, 0x00004963, 0x00004B17, 0x00004CCC,
			0x00004E83, 0x0000503C, 0x000051F7, 0x000053B3, 0x00005571, 0x00005731, 0x000058F2, 0x00005AB6, 0x00005C7C, 0x00005E43, 0x0000600D, 0x000061D9, 0x000063A7, 0x00006577, 0x00006749, 0x0000691E,
			0x00006AF5, 0x00006CCF, 0x00006EAB, 0x00007089, 0x0000726B, 0x0000744E, 0x00007635, 0x0000781E, 0x00007A0A, 0x00007BF9, 0x00007DEB, 0x00007FE0, 0x000081D8, 0x000083D3, 0x000085D1, 0x000087D3,
			0x000089D8, 0x00008BE1, 0x00008DED, 0x00008FFC, 0x0000920F, 0x00009426, 0x00009641, 0x0000985F, 0x00009A82, 0x00009CA9, 0x00009ED4, 0x0000A103, 0x0000A336, 0x0000A56E, 0x0000A7AA, 0x0000A9EB,
			0x0000AC31, 0x0000AE7B, 0x0000B0CB, 0x0000B31F, 0x0000B579, 0x0000B7D7, 0x0000BA3C, 0x0000BCA5, 0x0000BF15, 0x0000C18A, 0x0000C405, 0x0000C686, 0x0000C90D, 0x0000CB9A, 0x0000CE2E, 0x0000D0C8,
			0x0000D369, 0x0000D611, 0x0000D8BF, 0x0000DB75, 0x0000DE33, 0x0000E0F8, 0x0000E3C4, 0x0000E699, 0x0000E975, 0x0000EC5A, 0x0000EF47, 0x0000F23D, 0x0000F53B, 0x0000F843, 0x0000FB54, 0x0000FE6F,
			0x00010193, 0x000104C1, 0x000107FA, 0x00010B3D, 0x00010E8A, 0x000111E3, 0x00011547, 0x000118B7, 0x00011C33, 0x00011FBB, 0x0001234F, 0x000126F0, 0x00012A9F, 0x00012E5B, 0x00013225, 0x000135FE,
			0x000139E5, 0x00013DDB, 0x000141E1, 0x000145F7, 0x00014A1D, 0x00014E55, 0x0001529E, 0x000156F8, 0x00015B66, 0x00015FE6, 0x0001647A, 0x00016922, 0x00016DDF, 0x000172B1, 0x00017799, 0x00017C99,
			0x000181AF, 0x000186DF, 0x00018C27, 0x00019189, 0x00019706, 0x00019C9F, 0x0001A254, 0x0001A827, 0x0001AE18, 0x0001B42A, 0x0001BA5C, 0x0001C0B0, 0x0001C727, 0x0001CDC3, 0x0001D484, 0x0001DB6D,
			0x0001E27F, 0x0001E9BB, 0x0001F123, 0x0001F8B9, 0x0002007E, 0x00020875, 0x0002109F, 0x000218FF, 0x00022197, 0x00022A69, 0x00023378, 0x00023CC6, 0x00024657, 0x0002502D, 0x00025A4C, 0x000264B7,
			0x00026F71, 0x00027A7E, 0x000285E3, 0x000291A3, 0x00029DC4, 0x0002AA4A, 0x0002B73B, 0x0002C49C, 0x0002D273, 0x0002E0C7, 0x0002EFA0, 0x0002FF03, 0x00030EFA, 0x00031F8E, 0x000330C7, 0x000342B1,
			0x00035555, 0x000368C1, 0x00037D02, 0x00039225, 0x0003A83A, 0x0003BF54, 0x0003D783, 0x0003F0DE, 0x00040B7A, 0x00042771, 0x000444DE, 0x000463E1, 0x0004849B, 0x0004A733, 0x0004CBD4, 0x0004F2AD,
			0x00051BF5, 0x000547E9, 0x000576CE, 0x0005A8F5, 0x0005DEB8, 0x0006187F, 0x000656C3, 0x00069A12, 0x0006E30F, 0x0007327E, 0x00078943, 0x0007E873, 0x00085158, 0x0008C585, 0x000946EA, 0x0009D7EE,
			0x000A7B97, 0x000B35BF, 0x000C0B69, 0x000D032C, 0x000E25EF, 0x000F7FFA, 0x001122C3, 0x001327F6, 0x0015B6F0, 0x00190F4A, 0x001D9ED9, 0x00243517, 0x002E8E99, 0x00412F4B, 0x006CA58E, 0x0145F2C3
		};
		return _Tan1024Table[Index];
	}

	global_func const wchar CP949ToUTF16_Search(const char cp949a, const char cp949b = ' ')
	{
		global_data const int Length = 98 + 2350;
		global_data wstring CP949 = (wstring)
			"\t \n \r   ! \" # $ % & \' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I J K L "
			"M N O P Q R S T U V W X Y Z [ \\ ] ^ _ ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~ "
			"‚Rˆš°¡±¡²¡³¡´¡µ¡¶¡·¡¸¡¹¡º¡»¡¼¡½¡¾¡¿¡À¡Á¡Â¡Ã¡Ä¡Å¡Æ¡Ç¡È¡°¢±¢²¢³¢´¢µ¢¶¢·¢¸¢¹¢º¢»¢¼¢½¢¾¢¿¢À¢Á¢Â¢Ã¢Ä¢Å¢Æ¢Ç¢È¢°£±£"
			"²£³£´£µ£¶£·£¸£¹£º£»£¼£½£¾£¿£À£Á£Â£Ã£Ä£Å£Æ£Ç£È£°¤±¤²¤³¤´¤µ¤¶¤·¤¸¤¹¤º¤»¤¼¤½¤¾¤¿¤À¤Á¤Â¤Ã¤Ä¤Å¤Æ¤Ç¤È¤°¥±¥²¥³¥´¥µ¥"
			"¶¥·¥¸¥¹¥º¥»¥¼¥½¥¾¥¿¥À¥Á¥Â¥Ã¥Ä¥Å¥Æ¥Ç¥È¥°¦±¦²¦³¦´¦µ¦¶¦·¦¸¦¹¦º¦»¦¼¦½¦¾¦¿¦À¦Á¦Â¦Ã¦Ä¦Å¦Æ¦Ç¦È¦°§±§²§³§´§µ§¶§·§¸§¹§"
			"º§»§¼§½§¾§¿§À§Á§Â§Ã§Ä§Å§Æ§Ç§È§°¨²¨³¨´¨µ¨¶¨·¨¸¨¹¨º¨»¨¼¨½¨¾¨¿¨À¨Á¨Â¨Ã¨Ä¨Å¨Æ¨Ç¨È¨°©±©²©³©´©µ©¶©·©¸©¹©º©»©¼©½©¾©"
			"¿©À©Á©Â©Ã©Ä©Å©Æ©Ç©È©°ª±ª²ª³ª´ªµª¶ª·ª¸ª¹ªºª»ª¼ª½ª¾ª¿ªÀªÁªÂªÃªÄªÅªÆªÇªÈª°«±«²«³«´«µ«¶«·«¸«¹«º«»«¼«½«¾«¿«À«Á«Â«"
			"Ã«Ä«Å«Æ«Ç«È«°¬±¬²¬³¬´¬µ¬¶¬·¬¸¬¹¬º¬»¬¼¬½¬¾¬¿¬À¬Á¬Â¬Ã¬Ä¬Å¬Æ¬Ç¬È¬°­±­²­³­´­µ­¶­·­¸­¹­º­»­¼­½­¾­¿­À­Á­Â­Ã­Ä­Å­Æ­"
			"Ç­È­°®±®²®³®´®µ®¶®·®¸®¹®º®»®¼®½®¾®¿®À®Á®Â®Ã®Ä®Å®Æ®Ç®È®°¯±¯²¯³¯´¯µ¯¶¯·¯¸¯¹¯º¯»¯¼¯½¯¾¯¿¯À¯Á¯Â¯Ã¯Ä¯Å¯Æ¯Ç¯È¯°°±°"
			"²°³°´°µ°¶°·°¸°¹°º°»°¼°½°¾°¿°À°Á°Â°Ã°Ä°Å°Æ°Ç°È°°±±±²±³±´±µ±¶±·±¸±¹±º±»±¼±½±¾±¿±À±Á±Â±Ã±Ä±Å±Æ±Ç±È±°²±²²²³²´²µ²"
			"¶²·²¸²¹²º²»²¼²½²¾²¿²À²Á²Â²Ã²Ä²Å²Æ²Ç²È²°³±³²³³³´³µ³¶³·³¸³¹³º³»³¼³½³¾³¿³À³Á³Â³Ã³Ä³Å³Æ³Ç³È³°´±´²´³´´´µ´¶´·´¸´¹´"
			"º´»´¼´½´¾´¿´À´Á´Â´Ã´Ä´Å´Æ´Ç´È´°µ±µ²µ³µ´µµµ¶µ·µ¸µ¹µºµ»µ¼µ½µ¾µ¿µÀµÁµÂµÃµÄµÅµÆµÇµÈµ°¶±¶²¶³¶´¶µ¶¶¶·¶¸¶¹¶º¶»¶¼¶½¶"
			"¾¶¿¶À¶Á¶Â¶Ã¶Ä¶Å¶Æ¶Ç¶È¶°·±·²·³·´·µ·¶···¸·¹·º·»·¼·½·¾·¿·À·Á·Â·Ã·Ä·Å·Æ·Ç·È·°¸±¸²¸³¸´¸µ¸¶¸·¸¸¸¹¸º¸»¸¼¸½¸¾¸¿¸À¸Á¸"
			"Â¸Ã¸Ä¸Å¸Æ¸Ç¸È¸°¹±¹²¹³¹´¹µ¹¶¹·¹¸¹¹¹º¹»¹¼¹½¹¾¹¿¹À¹Á¹Â¹Ã¹Ä¹Å¹Æ¹Ç¹È¹°º±º²º³º´ºµº¶º·º¸º¹ººº»º¼º½º¾º¿ºÀºÁºÂºÃºÄºÅº"
			"ÆºÇºÈº°»±»²»³»´»µ»¶»·»¸»¹»º»»»¼»½»¾»¿»À»Á»Â»Ã»Ä»Å»Æ»Ç»È»°¼±¼²¼³¼´¼µ¼¶¼·¼¸¼¹¼º¼»¼¼¼½¼¾¼¿¼À¼Á¼Â¼Ã¼Ä¼Å¼Æ¼Ç¼È¼°½"
			"±½²½³½´½µ½¶½·½¸½¹½º½»½¼½½½¾½¿½À½Á½Â½Ã½Ä½Å½Æ½Ç½È½°¾±¾²¾³¾´¾µ¾¶¾·¾¸¾¹¾º¾»¾¼¾½¾¾¾¿¾À¾Á¾Â¾Ã¾Ä¾Å¾Æ¾Ç¾È¾°¿±¿²¿³¿´¿"
			"µ¿¶¿·¿¸¿¹¿º¿»¿¼¿½¿¾¿¿¿À¿Á¿Â¿Ã¿Ä¿Å¿Æ¿Ç¿È¿›À°À±À²À³À´ÀµÀ¶À·À¸À¹ÀºÀ»À¼À½À¾À¿ÀÀÀÁÀÂÀÃÀÄÀÅÀÆÀÇÀÈÀ°Á±Á²Á³Á´ÁµÁ¶Á·Á"
			"¸Á¹ÁºÁ»Á¼Á½Á¾Á¿ÁÀÁÁÁÂÁÃÁÄÁÅÁÆÁÇÁÈÁ°Â±Â²Â³Â´ÂµÂ¶Â·Â¸Â¹ÂºÂ»Â¼Â½Â¾Â¿ÂÀÂÁÂÂÂÃÂÄÂÅÂÆÂÇÂÈÂ°Ã±Ã²Ã³Ã´ÃµÃ¶Ã·Ã¸Ã¹ÃºÃ»Ã"
			"¼Ã½Ã¾Ã¿ÃÀÃÁÃÂÃÃÃÄÃÅÃÆÃÇÃÈÃ°Ä±Ä²Ä³Ä´ÄµÄ¶Ä·Ä¸Ä¹ÄºÄ»Ä¼Ä½Ä¾Ä¿ÄÀÄÁÄÂÄÃÄÄÄÅÄÆÄÇÄÈÄ°Å±Å²Å³Å´ÅµÅ¶Å·Å¸Å¹ÅºÅ»Å¼Å½Å¾Å¿Å"
			"ÀÅÁÅÂÅÃÅÄÅÅÅÆÅÇÅÈÅ°Æ±Æ²Æ³Æ´ÆµÆ¶Æ·Æ¸Æ¹ÆºÆ»Æ¼Æ½Æ¾Æ¿ÆÀÆÁÆÂÆÃÆÄÆÅÆÆÆÇÆÈÆ°Ç±Ç²Ç³Ç´ÇµÇ¶Ç·Ç¸Ç¹ÇºÇ»Ç¼Ç½Ç¾Ç¿ÇÀÇÁÇÂÇÃÇ"
			"ÄÇÅÇÆÇÇÇÈÇ°È±È²È³È´ÈµÈ¶È·È¸È¹ÈºÈ»È¼È½È¾È¿ÈÀÈÁÈÂÈÃÈÄÈÅÈÆÈÇÈÈÈ°É±É²É³É´ÉµÉ¶É·É¸É¹ÉºÉ»É¼É½É¾É¿ÉÀÉÁÉÂÉÃÉÄÉÅÉÆÉÇÉ"
			"ÈÉ°Ê±Ê²Ê³Ê´ÊµÊ¶Ê·Ê¸Ê¹ÊºÊ»Ê¼Ê½Ê¾Ê¿ÊÀÊÁÊÂÊÃÊÄÊÅÊÆÊÇÊÈÊ°Ë±Ë²Ë³Ë´ËµË¶Ë·Ë¸Ë¹ËºË»Ë¼Ë½Ë¾Ë¿ËÀËÁËÂËÃËÄËÅËÆËÇËÈË°Ì±Ì²Ì"
			"³Ì´ÌµÌ¶Ì·Ì¸Ì¹ÌºÌ»Ì¼Ì½Ì¾Ì¿ÌÀÌÁÌÂÌÃÌÄÌÅÌÆÌÇÌÈÌ°Í±Í²Í³Í´ÍµÍ¶Í·Í¸Í¹ÍºÍ»Í¼Í½Í¾Í¿ÍÀÍÁÍÂÍÃÍÄÍÅÍÆÍÇÍÈÍ°Î±Î²Î³Î´ÎµÎ¶Î"
			"·Î¸Î¹ÎºÎ»Î¼Î½Î¾Î¿ÎÀÎÁÎÂÎÃÎÄÎÅÎÆÎÇÎÈÎ°Ï±Ï²Ï³Ï´ÏµÏ¶Ï·Ï¸Ï¹ÏºÏ»Ï¼Ï½Ï¾Ï¿ÏÀÏÁÏÂÏÃÏÄÏÅÏÆÏÇÏÈÏ°Ð±Ð²Ð³Ð´ÐµÐ¶Ð·Ð¸Ð¹ÐºÐ"
			"»Ð¼Ð½Ð¾Ð¿ÐÀÐÁÐÂÐÃÐÄÐÅÐÆÐÇÐÈÐ°Ñ±Ñ²Ñ³Ñ´ÑµÑ¶Ñ·Ñ¸Ñ¹ÑºÑ»Ñ¼Ñ½Ñ¾Ñ¿ÑÀÑÁÑÂÑÃÑÄÑÅÑÆÑÇÑÈÑ°Ò±Ò²Ò³Ò´ÒµÒ¶Ò·Ò¸Ò¹ÒºÒ»Ò¼Ò½Ò¾Ò"
			"¿ÒÀÒÁÒÂÒÃÒÄÒÅÒÆÒÇÒÈÒ°Ó±Ó²Ó³ÓµÓ¶Ó·Ó¸Ó¹ÓºÓ»Ó¼Ó½Ó¾Ó¿ÓÀÓÁÓÂÓÃÓÄÓÅÓÆÓÇÓÈÓ°Ô±Ô²Ô³Ô´ÔµÔ¶Ô·Ô¸Ô¹ÔºÔ»Ô¼Ô½Ô¾Ô¿ÔÀÔÁÔÂÔÃÔ"
			"ÄÔÅÔÆÔÇÔÈÔ°Õ±Õ²Õ³Õ´ÕµÕ¶Õ·Õ¸Õ¹ÕºÕ»Õ¼Õ½Õ¾Õ¿ÕÀÕÁÕÂÕÃÕÄÕÅÕÆÕÇÕÈÕ°Ö±Ö²Ö³Ö´ÖµÖ¶Ö·Ö¸Ö¹ÖºÖ»Ö¼Ö½Ö¾Ö¿ÖÀÖÁÖÂÖÃÖÄÖÅÖÆÖÇÖ"
			"ÈÖ°×±×²×³×´×µ×¶×·×¸×¹×º×»×¼×½×¾×¿×À×Á×Â×Ã×Ä×Å×Æ×Ç×È×°Ø±Ø²Ø³Ø´ØµØ¶Ø·Ø¸Ø¹ØºØ»Ø¼Ø½Ø¾Ø¿ØÀØÁØÂØÃØÄØÅØÆØÇØÈØ°Ù±Ù²Ù"
			"³Ù´ÙµÙ¶Ù·Ù¸Ù¹ÙºÙ»Ù¼Ù½Ù¾Ù¿ÙÀÙÁÙÂÙÃÙÄÙÅÙÆÙÇÙÈÙ°Ú±Ú²Ú³Ú´ÚµÚ¶Ú·Ú¸Ú¹ÚºÚ»Ú¼Ú½Ú¾Ú¿ÚÀÚÁÚÂÚÃÚÄÚÅÚÆÚÇÚÈÚ°Û±Û²Û³Û´ÛµÛ¶Û"
			"·Û¸Û¹ÛºÛ»Û¼Û½Û¾Û¿ÛÀÛÁÛÂÛÃÛÄÛÅÛÆÛÇÛÈÛ°Ü±Ü²Ü³Ü´ÜµÜ¶Ü·Ü¸Ü¹ÜºÜ»Ü¼Ü½Ü¾Ü¿ÜÀÜÁÜÂÜÃÜÄÜÅÜÆÜÇÜÈÜ°Ý±Ý²Ý³Ý´ÝµÝ¶Ý·Ý¸Ý¹ÝºÝ"
			"»Ý¼Ý½Ý¾Ý¿ÝÀÝÁÝÂÝÃÝÄÝÅÝÆÝÇÝÈÝ°Þ±Þ²Þ³Þ´ÞµÞ¶Þ·Þ¸Þ¹ÞºÞ»Þ¼Þ½Þ¾Þ¿ÞÀÞÁÞÂÞÃÞÄÞÅÞÆÞÇÞÈÞ°ß±ß²ß³ß´ßµß¶ß·ß¸ß¹ßºß»ß¼ß½ß¾ß"
			"¿ßÀßÁßÂßÃßÄßÅßÆßÇßÈß°à±à²à³à´àµà¶à·à¸à¹àºà»à¼à½à¾à¿àÀàÁàÂàÃàÄàÅàÆàÇàÈà°á±á²á³á´áµá¶á·á¸á¹áºá»á¼á½á¾á¿áÀáÁáÂá"
			"ÃáÄáÅáÆáÇáÈá°â±â²â³â´âµâ¶â·â¸â¹âºâ»â¼â½â¾â¿âÀâÁâÂâÃâÄâÅâÆâÇâÈâ°ã±ã²ã³ã´ãµã¶ã·ã¸ã¹ãºã»ã¼ã½ã¾ã¿ãÀãÁãÂãÃãÄãÅãÆã"
			"ÇãÈã°ä±ä²ä³ä´äµä¶ä·ä¸ä¹äºä»ä¼ä½ä¾ä¿äÀäÁäÂäÃäÄäÅäÆäÇäÈä°å±å²å³å´åµå¶å·å¸å¹åºå»å¼å½å¾å¿åÀåÁåÂåÃåÄåÅåÆåÇåÈå°æ±æ"
			"²æ³æ´æµæ¶æ·æ¸æ¹æºæ»æ¼æ½æ¾æ¿æÀæÁæÂæÃæÄæÅæÆæÇæÈæ°ç±ç²ç³ç´çµç¶ç·ç¸ç¹çºç»ç¼ç½ç¾ç¿çÀçÁçÂçÃçÄçÅçÆçÇçÈç°è±è²è³è´èµè"
			"¶è·è¸è¹èºè»è¼è½è¾è¿èÀèÁèÂèÃèÄèÅèÆèÇèÈè°é±é²é³é´éµé¶é·é¸é¹éºé»é¼é½é¾é¿éÀéÁéÂéÃéÄéÅéÆéÇéÈé°ê±ê²ê³ê´êµê¶ê·ê¸ê¹ê"
			"ºê»ê¼ê½ê¾ê¿êÀêÁêÂêÃêÄêÅêÆêÇêÈê°ë±ë²ë³ë´ëµë¶ë·ë¸ë¹ëºë»ë¼ë½ë¾ë¿ëÀëÁëÂëÃëÄëÅëÆëÇëÈë°ì±ì²ì³ì´ìµì¶ì·ì¸ì¹ìºì»ì¼ì½ì"
			"¾ì¿ìÀìÁìÂìÃìÄìÅìÆìÇìÈì°í±í²í³í´íµí¶í·í¸í¹íºí»í¼í¾í¿íÀíÁíÂíÃíÄíÅíÆíÇíÈí°î±î²î³î´îµî¶î·î¸î¹îºî»î¼î½î¾î¿îÀîÁîÂî"
			"ÃîÄîÅîÆîÇîÈî°ï±ï²ï³ï´ïµï¶ï·ï¸ï¹ïºï»ï¼ï½ï¾ï¿ïÀïÁïÂïÃïÄïÅïÆïÇïÈï°ð±ð²ð³ð´ðµð¶ð·ð¸ð¹ðºð»ð¼ð½ð¾ð¿ðÀðÁðÂðÃðÄðÅðÆð"
			"ÇðÈð°ñ±ñ²ñ³ñ´ñµñ¶ñ·ñ¸ñ¹ñºñ»ñ¼ñ½ñ¾ñ¿ñÀñÁñÂñÃñÄñÅñÆñÇñÈñ°ò±ò²ò³ò´òµò¶ò·ò¸ò¹òºò»ò¼ò½ò¾ò¿òÀòÁòÂòÃòÄòÅòÆòÇòÈò°ó±ó"
			"²ó³ó´óµó¶ó·ó¸ó¹óºó»ó¼ó½ó¾ó¿óÀóÁóÂóÃóÄóÅóÆóÇóÈó°ô±ô²ô³ô´ôµô¶ô·ô¸ô¹ôºô»ô¼ô½ô¾ô¿ôÀôÁôÂôÃôÄôÅôÆôÇôÈô°õ±õ²õ³õ´õµõ"
			"¶õ·õ¸õ¹õºõ»õ¼õ½õ¾õ¿õÀõÁõÂõÃõÄõÅõÆõÇõÈõ°ö±ö²ö³ö´öµö¶ö·ö¸ö¹öºö»ö¼ö½ö¾ö¿öÀöÁöÂöÃöÄöÅöÆöÇöÈö°÷±÷²÷³÷´÷µ÷¶÷·÷¸÷¹÷"
			"º÷»÷¼÷½÷¾÷¿÷À÷Á÷Â÷Ã÷Ä÷Å÷Æ÷Ç÷È÷°ø±ø²ø³ø´øµø¶ø·ø¸ø¹øºø»ø¼ø½ø¾ø¿øÀøÁøÂøÃøÄøÅøÆøÇøÈø°ù±ù²ù³ù´ùµù¶ù·ù¸ù¹ùºù»ù¼ù½ù"
			"¾ù¿ùÀùÁùÂùÃùÄùÅùÆùÇùÈù°ú±ú²ú³ú´úµú¶ú·ú¸ú¹úºú»ú¼ú½ú¾ú¿úÀúÁúÂúÃúÄúÅúÆúÇúÈú°û±û²û³û´ûµû¶û·û¸û¹ûºû»û¼û½û¾û¿ûÀûÁû"
			"ÂûÃûÄûÅûÆûÇûÈû°ü±ü²ü³ü´üµü¶ü·ü¸ü¹üºü»ü¼ü½ü¾ü¿üÀüÁüÂüÃüÄüÅüÆüÇüÈü°ý±ý²ý³ý´ýµý¶ý·ý¸ý¹ýºý»ý¼ý½ý¾ý¿ýÀýÁýÂýÃýÄýÅý"
			"ÆýÇýÈý°þ±þ²þ³þ´þµþ¶þ·þ¸þ¹þºþ»þ¼þ½þ¾þ¿þÀþÁþÂþÃþÄþÅþÆþÇþÈþ";
		global_data wstring UTF16 = (wstring)
			L"\t\n\r !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKL"
			L"MNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"\xAD28\xB2D6\xAC00\xAD0C\xAE79\xB05D\xB1DF\xB367\xB540\xB798\xB96B\xBB00\xBC99\xBE68\xC0E5\xC22F\xC434\xC5D0\xC6E9\xC810\xC9D5\xCC3C\xCE58\xD044\xD264"
			L"\xD408\xD624\xAC01\xAD0D\xAE7B\xB07C\xB1E8\xB369\xB541\xB799\xB96D\xBB04\xBC9A\xBE6A\xC0E8\xC231\xC43C\xC5D1\xC6EC\xC811\xC9D6\xCC3D\xCE59\xD045\xD280"
			L"\xD41D\xD62D\xAC04\xAD0F\xAE7C\xB07D\xB1E9\xB36B\xB543\xB79C\xB974\xBB0D\xBCA0\xBE70\xC0EC\xC232\xC43D\xC5D4\xC6F0\xC813\xC9D9\xCC3E\xCE5C\xD047\xD281"
			L"\xD440\xD638\xAC07\xAD11\xAE7D\xB080\xB1EC\xB36E\xB544\xB7A0\xB975\xBB0F\xBCA1\xBE71\xC0F4\xC234\xC448\xC5D8\xC6F8\xC815\xC9DA\xCC44\xCE5F\xD049\xD284"
			L"\xD444\xD639\xAC08\xAD18\xAE84\xB084\xB1F0\xB370\xB545\xB7A8\xB978\xBB11\xBCA4\xBE73\xC0F5\xC248\xC464\xC5E0\xC6F9\xC816\xC9DC\xCC45\xCE60\xD050\xD288"
			L"\xD45C\xD63C\xAC09\xAD1C\xAE85\xB08C\xB1F9\xB371\xB54B\xB7A9\xB97C\xBB18\xBCA7\xBE74\xC0F7\xC250\xC465\xC5E1\xC6FD\xC81C\xC9DD\xCC48\xCE61\xD054\xD290"
			L"\xD460\xD640\xAC0A\xAD20\xAE8C\xB08D\xB1FB\xB374\xB54C\xB7AB\xB984\xBB1C\xBCA8\xBE75\xC0F9\xC251\xC468\xC5E3\xC704\xC81D\xC9E0\xCC4C\xCE68\xD058\xD291"
			L"\xD464\xD645\xAC10\xAEBC\xB08F\xB1FD\xB378\xB54D\xB7AC\xB985\xBB20\xBCB0\xBE7B\xC100\xC254\xC46C\xC5E5\xC705\xC820\xC9E2\xCC54\xCE69\xD060\xD295\xD46D"
			L"\xD648\xAC11\xAD2C\xAEBD\xB091\xB204\xB380\xB550\xB7AD\xB987\xBB29\xBCB1\xBE7C\xC104\xC258\xC474\xC5EC\xC708\xC824\xC9E4\xCC55\xCE6B\xD06C\xD29C\xD46F"
			L"\xD649\xAC12\xAD2D\xAEBE\xB098\xB205\xB381\xB554\xB7B4\xB989\xBB2B\xBCB3\xBE7D\xC108\xC260\xC475\xC5ED\xC70C\xC82C\xC9E7\xCC57\xCE6D\xD06D\xD2A0\xD478"
			L"\xD64B\xAC13\xAD34\xAEC0\xB099\xB208\xB383\xB55C\xB7B5\xB98A\xBB34\xBCB4\xBE80\xC110\xC265\xC479\xC5EE\xC714\xC82D\xC9EC\xCC58\xCE74\xD070\xD2A4\xD479"
			L"\xD64D\xAC14\xAD35\xAEC4\xB09A\xB20B\xB384\xB55D\xB7B8\xB98D\xBB35\xBCB5\xBE84\xC115\xC26C\xC480\xC5F0\xC715\xC82F\xC9ED\xCC59\xCE75\xD074\xD2AC\xD47C"
			L"\xD651\xAC15\xAD38\xAECC\xB09C\xB20C\xB385\xB55F\xB7C7\xB98E\xBB36\xBCBC\xBE8C\xC11C\xC26D\xC494\xC5F4\xC717\xC831\xC9EF\xCC60\xCE78\xD07C\xD2B1\xD47F"
			L"\xD654\xAC16\xAD3C\xAECD\xB09F\xB214\xB38C\xB560\xB7C9\xB9AC\xBB38\xBCBD\xBE8D\xC11D\xC270\xC49C\xC5F6\xC719\xC838\xC9F0\xCC64\xCE7C\xD07D\xD2B8\xD480"
			L"\xD655\xAC17\xAD44\xAECF\xB0A0\xB215\xB390\xB561\xB7EC\xB9AD\xBB3B\xBCC0\xBE8F\xC11E\xC274\xC4B8\xC5F7\xC720\xC83C\xC9F1\xCC66\xCE84\xD081\xD2B9\xD482"
			L"\xD658\xAC19\xAD45\xAED0\xB0A1\xB217\xB394\xB5A0\xB7ED\xB9B0\xBB3C\xBCC4\xBE90\xC11F\xC27C\xC4BC\xC5FC\xC721\xC840\xC9F8\xCC68\xCE85\xD0A4\xD2BC\xD488"
			L"\xD65C\xAC1A\xAD47\xAED1\xB0A2\xB219\xB3A0\xB5A1\xB7F0\xB9B4\xBB3D\xBCCD\xBE91\xC120\xC27D\xC4E9\xC5FD\xC724\xC848\xC9F9\xCC70\xCE87\xD0A5\xD2BF\xD489"
			L"\xD667\xAC1B\xAD49\xAED8\xB0A8\xB220\xB3A1\xB5A4\xB7F4\xB9BC\xBB3E\xBCCF\xBE98\xC123\xC27F\xC4F0\xC5FE\xC728\xC849\xC9FC\xCC75\xCE89\xD0A8\xD2C0\xD48B"
			L"\xD669\xAC1C\xAD50\xAED9\xB0A9\xB234\xB3A8\xB5A8\xB7FC\xB9BD\xBB44\xBCD0\xBE99\xC124\xC281\xC4F1\xC5FF\xC730\xC84C\xCA00\xCC98\xCE90\xD0AC\xD2C2\xD48D"
			L"\xD670\xAC1D\xAD54\xAEDC\xB0AB\xB23C\xB3AC\xB5AA\xB7FD\xB9BF\xBB45\xBCD1\xBEA8\xC126\xC288\xC4F4\xC600\xC731\xC84D\xCA08\xCC99\xCE91\xD0B4\xD2C8\xD494"
			L"\xD671\xAC20\xAD58\xAEE8\xB0AC\xB258\xB3C4\xB5AB\xB7FF\xB9C1\xBB47\xBCD5\xBED0\xC127\xC289\xC4F8\xC601\xC733\xC854\xCA09\xCC9C\xCE94\xD0B5\xD2C9\xD4A9"
			L"\xD674\xAC24\xAD61\xAEEB\xB0AD\xB25C\xB3C5\xB5B0\xB800\xB9C8\xBB49\xBCD8\xBED1\xC12C\xC290\xC4FA\xC605\xC735\xC870\xCA0B\xCCA0\xCE98\xD0B7\xD2CB\xD4CC"
			L"\xD683\xAC2C\xAD63\xAEED\xB0AE\xB260\xB3C8\xB5B1\xB801\xB9C9\xBB4D\xBCDC\xBED4\xC12D\xC298\xC4FF\xC606\xC737\xC871\xCA0C\xCCA8\xCEA0\xD0B9\xD2D4\xD4D0"
			L"\xD685\xAC2D\xAD6C\xAEF4\xB0AF\xB268\xB3CB\xB5B3\xB807\xB9CC\xBB4F\xBCF4\xBED7\xC12F\xC29B\xC500\xC607\xC73C\xC874\xCA0D\xCCA9\xCEA1\xD0C0\xD2D8\xD4D4"
			L"\xD68C\xAC2F\xAD6D\xAEF8\xB0B1\xB269\xB3CC\xB5B4\xB808\xB9CE\xBB50\xBCF5\xBED8\xC130\xC29D\xC501\xC608\xC73D\xC878\xCA14\xCCAB\xCEA3\xD0C1\xD2DC\xD4DC"
			L"\xD68D\xAC30\xAD70\xAEFC\xB0B3\xB274\xB3CE\xB5B5\xB809\xB9CF\xBB54\xBCF6\xBEE0\xC131\xC2A4\xC50C\xC60C\xC740\xC87A\xCA18\xCCAC\xCEA4\xD0C4\xD2E4\xD4DF"
			L"\xD690\xAC31\xAD73\xAF07\xB0B4\xB275\xB3D0\xB5BB\xB80C\xB9D0\xBB58\xBCF8\xBEE3\xC136\xC2A5\xC510\xC610\xC744\xC880\xCA29\xCCAD\xCEA5\xD0C8\xD2E5\xD4E8"
			L"\xD694\xAC38\xAD74\xAF08\xB0B5\xB27C\xB3D4\xB5BC\xB810\xB9D1\xBB61\xBCFC\xBEE4\xC138\xC2A8\xC514\xC618\xC74A\xC881\xCA4C\xCCB4\xCEAC\xD0C9\xD2F0\xD4EC"
			L"\xD69D\xAC39\xAD75\xAF0D\xB0B8\xB284\xB3D5\xB5BD\xB818\xB9D2\xBB63\xBD04\xBEE5\xC139\xC2AC\xC51C\xC619\xC74C\xC883\xCA4D\xCCB5\xCEAD\xD0D0\xD2F1\xD4F0"
			L"\xD69F\xAC3C\xAD76\xAF10\xB0BC\xB285\xB3D7\xB5C0\xB819\xB9D8\xBB6C\xBD05\xBEEC\xC13C\xC2AD\xC528\xC61B\xC74D\xC885\xCA50\xCCB8\xCEC1\xD0D1\xD2F4\xD4F8"
			L"\xD6A1\xAC40\xAD7B\xAF2C\xB0C4\xB289\xB3D9\xB5C4\xB81B\xB9D9\xBB88\xBD07\xBF01\xC140\xC2B4\xC529\xC61C\xC74F\xC886\xCA54\xCCBC\xCEE4\xD0D3\xD2F8\xD4FB"
			L"\xD6A8\xC399\xAC4B\xAD7C\xAF2D\xB0C5\xB290\xB3DB\xB5CC\xB81D\xB9DB\xBB8C\xBD09\xBF08\xC148\xC2B5\xC52C\xC624\xC751\xC887\xCA5C\xCCC4\xCEE5\xD0D4\xD300"
			L"\xD4FD\xD6AC\xAC4D\xAD7D\xAF30\xB0C7\xB291\xB3DD\xB5CD\xB824\xB9DD\xBB90\xBD10\xBF09\xC149\xC2B7\xC530\xC625\xC752\xC88B\xCA5D\xCCC5\xCEE8\xD0D5\xD301"
			L"\xD504\xD6B0\xAC54\xAD7F\xAF32\xB0C8\xB294\xB3E0\xB5CF\xB825\xB9DE\xBBA4\xBD14\xBF18\xC14B\xC2B9\xC538\xC628\xC753\xC88C\xCA5F\xCCC7\xCEEB\xD0DC\xD303"
			L"\xD508\xD6B9\xAC58\xAD81\xAF34\xB0C9\xB298\xB3E4\xB5D0\xB828\xB9E1\xBBA8\xBD24\xBF19\xC14C\xC2DC\xC539\xC62C\xC754\xC88D\xCA60\xCCC9\xCEEC\xD0DD\xD305"
			L"\xD50C\xD6BB\xAC5C\xAD82\xAF3C\xB0D0\xB299\xB3E8\xB5D1\xB82C\xB9E3\xBBAC\xBD2C\xBF1B\xC14D\xC2DD\xC53B\xC62D\xC755\xC894\xCA61\xCCD0\xCEF4\xD0E0\xD30C"
			L"\xD514\xD6C4\xAC70\xAD88\xAF3D\xB0D1\xB29A\xB3FC\xB5D8\xB834\xB9E4\xBBB4\xBD40\xBF1C\xC154\xC2E0\xC53D\xC62E\xC756\xC89D\xCA68\xCCD4\xCEF5\xD0E4\xD30D"
			L"\xD515\xD6C5\xAC71\xAD89\xAF3F\xB0D4\xB2A0\xB410\xB5EC\xB835\xB9E5\xBBB7\xBD48\xBF1D\xC155\xC2E3\xC544\xC630\xC757\xC89F\xCA7D\xCCE4\xCEF7\xD0EC\xD30E"
			L"\xD517\xD6C8\xAC74\xAD8C\xAF41\xB0D8\xB2A1\xB418\xB610\xB837\xB9E8\xBBC0\xBD49\xBF40\xC158\xC2E4\xC545\xC633\xC758\xC8A1\xCA84\xCCEC\xCEF8\xD0ED\xD310"
			L"\xD53C\xD6CC\xAC77\xAD90\xAF42\xB0E0\xB2A3\xB41C\xB611\xB838\xB9EC\xBBC4\xBD4C\xBF41\xC15C\xC2EB\xC548\xC634\xC75C\xC8A8\xCA98\xCCF0\xCEF9\xD0EF\xD314"
			L"\xD53D\xD6D1\xAC78\xAD9C\xAF43\xB0E5\xB2A5\xB420\xB614\xB839\xB9F4\xBBC8\xBD50\xBF44\xC164\xC2EC\xC549\xC635\xC760\xC8BC\xCABC\xCD01\xCF00\xD0F0\xD316"
			L"\xD540\xD6D4\xAC7A\xAD9D\xAF48\xB108\xB2A6\xB428\xB618\xB840\xB9F5\xBBD0\xBD58\xBF48\xC165\xC2ED\xC54A\xC637\xC768\xC8BD\xCABD\xCD08\xCF01\xD0F1\xD31C"
			L"\xD544\xD6D7\xAC80\xADA4\xAF49\xB109\xB2AA\xB429\xB625\xB844\xB9F7\xBBD3\xBD59\xBF50\xC167\xC2EF\xC54C\xC639\xC76B\xC8C4\xCAC0\xCD09\xCF04\xD0F8\xD31D"
			L"\xD54C\xD6D9\xAC81\xADB7\xAF50\xB10B\xB2AC\xB42B\xB62C\xB851\xB9F8\xBBF8\xBD64\xBF51\xC168\xC2F1\xC54D\xC63B\xC774\xC8C8\xCAC4\xCD0C\xCF08\xD10D\xD31F"
			L"\xD54D\xD6E0\xAC83\xADC0\xAF5C\xB10C\xB2B0\xB434\xB634\xB853\xB9F9\xBBF9\xBD68\xBF55\xC169\xC2F6\xC54E\xC640\xC775\xC8CC\xCACC\xCD10\xCF10\xD130\xD320"
			L"\xD54F\xD6E4\xAC84\xADC1\xAF5D\xB110\xB2B4\xB450\xB648\xB85C\xB9FA\xBBFC\xBD80\xBF94\xC170\xC2F8\xC553\xC641\xC778\xC8D4\xCACD\xCD18\xCF11\xD131\xD321"
			L"\xD551\xD6E8\xAC85\xADC4\xAF64\xB112\xB2C8\xB451\xB664\xB85D\xBA00\xBBFF\xBD81\xBFB0\xC174\xC2F9\xC554\xC644\xC77C\xC8D5\xCACF\xCD19\xCF13\xD134\xD325"
			L"\xD558\xD6F0\xAC86\xADC8\xAF65\xB113\xB2C9\xB454\xB668\xB860\xBA01\xBC00\xBD84\xBFC5\xC178\xC2FB\xC555\xC648\xC77D\xC8D7\xCAD1\xCD1B\xCF15\xD138\xD328"
			L"\xD559\xD6F5\xAC89\xADD0\xAF79\xB118\xB2CC\xB458\xB69C\xB864\xBA08\xBC02\xBD87\xBFCC\xC185\xC2FC\xC557\xC650\xC77E\xC8D9\xCAD3\xCD1D\xCF1C\xD13A\xD329"
			L"\xD55C\xD6FC\xAC8A\xADD1\xAF80\xB119\xB2D0\xB460\xB69D\xB86C\xBA15\xBC08\xBD88\xBFCD\xC18C\xC300\xC558\xC651\xC783\xC8E0\xCAD8\xCD24\xCF20\xD140\xD32C"
			L"\xD560\xD6FD\xAC8B\xADD3\xAF84\xB11B\xB461\xB6A0\xB86D\xBA38\xBC09\xBD89\xBFD0\xC18D\xC308\xC559\xC653\xC784\xC8E1\xCAD9\xCD28\xCF24\xD141\xD330\xD565"
			L"\xD700\xAC8C\xADDC\xAF88\xB11C\xB2D8\xB463\xB6A4\xB86F\xBA39\xBC0B\xBD8A\xBFD4\xC18E\xC309\xC55D\xC654\xC785\xC8E4\xCAE0\xCD2C\xCF2C\xD143\xD338\xD568"
			L"\xD704\xAC90\xADE0\xAF90\xB11D\xB2D9\xB465\xB6AB\xB871\xBA3C\xBC0C\xBD90\xBFDC\xC190\xC30C\xC55E\xC655\xC787\xC8F5\xCAEC\xCD39\xCF2D\xD144\xD339\xD569"
			L"\xD711\xAC94\xADE4\xAF91\xB123\xB2DB\xB46C\xB6AC\xB878\xBA40\xBC0D\xBD91\xBFDF\xC194\xC30D\xC560\xC65C\xC788\xC8FC\xCAF4\xCD5C\xCF2F\xD145\xD33B\xD56B"
			L"\xD718\xAC9C\xADF8\xAF95\xB124\xB2DD\xB480\xB6B1\xB87C\xBA42\xBC0F\xBD93\xBFE1\xC196\xC313\xC561\xC65D\xC789\xC8FD\xCB08\xCD60\xCF30\xD14C\xD33C\xD56D"
			L"\xD719\xAC9D\xADF9\xAF9C\xB125\xB2E2\xB488\xB6D4\xB88D\xBA48\xBC11\xBD95\xC03C\xC19C\xC314\xC564\xC660\xC78A\xC900\xCB10\xCD64\xCF31\xD14D\xD33D\xD574"
			L"\xD71C\xAC9F\xADFC\xAFB8\xB128\xB2E4\xB49D\xB6F0\xB8A8\xBA49\xBC14\xBD99\xC051\xC19D\xC315\xC568\xC66C\xC78E\xC904\xCB14\xCD6C\xCF38\xD150\xD344\xD575"
			L"\xD720\xACA0\xADFF\xAFB9\xB12C\xB2E5\xB4A4\xB6F4\xB8B0\xBA4B\xBC15\xBD9A\xC058\xC19F\xC318\xC570\xC66F\xC790\xC905\xCB18\xCD6D\xCF54\xD154\xD345\xD578"
			L"\xD728\xACA1\xAE00\xAFBC\xB134\xB2E6\xB4A8\xB6F8\xB8B4\xBA4D\xBC16\xBD9C\xC05C\xC1A1\xC31C\xC571\xC671\xC791\xC906\xCB20\xCD6F\xCF55\xD15C\xD37C\xD57C"
			L"\xD729\xACA8\xAE01\xAFC0\xB135\xB2E8\xB4AC\xB700\xB8B8\xBA4E\xBC17\xBDA4\xC060\xC1A5\xC324\xC573\xC678\xC794\xC90C\xCB21\xCD71\xCF58\xD15D\xD37D\xD584"
			L"\xD72B\xACA9\xAE08\xAFC7\xB137\xB2EB\xB4B5\xB701\xB8C0\xBA53\xBC18\xBDB0\xC068\xC1A8\xC325\xC574\xC679\xC796\xC90D\xCB41\xCD78\xCF5C\xD15F\xD380\xD585"
			L"\xD72D\xACAA\xAE09\xAFC8\xB138\xB2EC\xB4B7\xB705\xB8C1\xBA54\xBC1B\xBDB8\xC069\xC1A9\xC328\xC575\xC67C\xC797\xC90F\xCB48\xCD88\xCF64\xD161\xD384\xD587"
			L"\xD734\xACAC\xAE0B\xAFC9\xB139\xB2ED\xB4B9\xB728\xB8C3\xBA55\xBC1C\xBDD4\xC090\xC1AC\xC329\xC57C\xC680\xC798\xC911\xCB49\xCD94\xCF65\xD168\xD38C\xD588"
			L"\xD735\xACAF\xAE0D\xAFCB\xB140\xB2EE\xB4C0\xB729\xB8C5\xBA58\xBC1D\xBDD5\xC091\xC1B0\xC345\xC57D\xC688\xC79A\xC918\xCB4C\xCD95\xCF67\xD16C\xD38D\xD589"
			L"\xD738\xACB0\xAE14\xAFCD\xB141\xB2EF\xB4C4\xB72C\xB8CC\xBA5C\xBC1E\xBDD8\xC094\xC1BD\xC368\xC580\xC689\xC7A0\xC92C\xCB50\xCD98\xCF69\xD17C\xD38F\xD590"
			L"\xD73C\xACB8\xAE30\xAFCE\xB144\xB2F3\xB4C8\xB72F\xB8D0\xBA64\xBC1F\xBDDC\xC098\xC1C4\xC369\xC584\xC68B\xC7A1\xC934\xCB58\xCD9C\xCF70\xD184\xD390\xD5A5"
			L"\xD744\xACB9\xAE31\xAFD4\xB148\xB2F4\xB4D0\xB730\xB8D4\xBA65\xBC24\xBDE9\xC0A0\xC1C8\xC36C\xC587\xC68D\xC7A3\xC950\xCB59\xCDA4\xCF71\xD188\xD391\xD5C8"
			L"\xD747\xACBB\xAE34\xAFDC\xB150\xB2F5\xB4D5\xB738\xB8DD\xBA67\xBC25\xBDF0\xC0A1\xC1CC\xC370\xC58C\xC694\xC7A4\xC951\xCB5D\xCDA5\xCF74\xD1A0\xD398\xD5C9"
			L"\xD749\xACBC\xAE37\xAFE8\xB151\xB2F7\xB4DC\xB739\xB8DF\xBA68\xBC27\xBDF4\xC0A3\xC1D4\xC372\xC58D\xC695\xC7A5\xC954\xCB64\xCDA7\xCF78\xD1A1\xD399\xD5CC"
			L"\xD750\xACBD\xAE38\xAFE9\xB154\xB2F8\xB4DD\xB73B\xB8E1\xBA69\xBC29\xBDF8\xC0A5\xC1D7\xC378\xC58F\xC698\xC7A6\xC958\xCB78\xCDA9\xCF80\xD1A4\xD39C\xD5D0"
			L"\xD751\xACC1\xAE3A\xAFF0\xB155\xB2F9\xB4E0\xB744\xB8E8\xBA70\xBC2D\xBE00\xC0AC\xC1D8\xC379\xC591\xC69C\xC7AC\xC960\xCB79\xCDB0\xCF85\xD1A8\xD3A0\xD5D2"
			L"\xD754\xACC4\xAE40\xAFF1\xB158\xB2FA\xB4E3\xB748\xB8E9\xBA71\xBC30\xBE03\xC0AD\xC1E0\xC37C\xC595\xC6A4\xC7AD\xC961\xCB9C\xCDC4\xCF8C\xD1B0\xD3A8\xD5D8"
			L"\xD756\xACC8\xAE41\xAFF4\xB15C\xB2FB\xB4E4\xB74C\xB8EC\xBA74\xBC31\xBE05\xC0AF\xC1E4\xC37D\xC597\xC6A5\xC7B0\xC963\xCBB8\xCDCC\xCFA1\xD1B1\xD3A9\xD5D9"
			L"\xD757\xACCC\xAE43\xAFF8\xB160\xB2FF\xB4E6\xB754\xB8F0\xBA78\xBC34\xBE0C\xC0B0\xC1E8\xC384\xC598\xC6A7\xC7B4\xC96C\xCBD4\xCDD0\xCFA8\xD1B3\xD3AB\xD5DB"
			L"\xD758\xACD5\xAE45\xB000\xB178\xB300\xB4EC\xB755\xB8F8\xBA83\xBC38\xBE0D\xC0B3\xC1F0\xC388\xC59C\xC6A9\xC7BC\xC970\xCBE4\xCDE8\xCFB0\xD1B5\xD3AD\xD5DD"
			L"\xD759\xACD7\xAE46\xB001\xB179\xB301\xB4ED\xB760\xB8F9\xBA84\xBC40\xBE10\xC0B4\xC1F1\xC38C\xC5A0\xC6B0\xC7BD\xC974\xCBE7\xCDEC\xCFC4\xD1BA\xD3B4\xD5E4"
			L"\xD760\xACE0\xAE4A\xB004\xB17C\xB304\xB4EF\xB764\xB8FB\xBA85\xBC41\xBE14\xC0B5\xC1F3\xC5A9\xC6B1\xC7BF\xC97C\xCBE9\xCDF0\xCFE0\xD1BC\xD3B8\xD5E5\xD761"
			L"\xACE1\xAE4C\xB00C\xB180\xB308\xB4F1\xB768\xB8FD\xBA87\xBC43\xBE1C\xC0B6\xC1FC\xC3D8\xC5B4\xC6B4\xC7C0\xC988\xCC0C\xCDF8\xCFE1\xD1C0\xD3BC\xD5E8\xD763"
			L"\xACE4\xAE4D\xB010\xB182\xB310\xB4F8\xB770\xB904\xBA8C\xBC44\xBE1D\xC0BC\xC1FD\xC3D9\xC5B5\xC6B8\xC7C1\xC989\xCC0D\xCDF9\xCFE4\xD1D8\xD3C4\xD5EC\xD765"
			L"\xACE7\xAE4E\xB014\xB188\xB311\xB514\xB771\xB918\xBAA8\xBC45\xBE1F\xC0BD\xC200\xC3DC\xC5B8\xC6B9\xC7C8\xC98C\xCC10\xCDFB\xCFE8\xD1F4\xD3C5\xD5F4\xD769"
			L"\xACE8\xAE50\xB01C\xB189\xB313\xB515\xB773\xB920\xBAA9\xBC49\xBE44\xC0BF\xC204\xC3DF\xC5B9\xC6BA\xC7C9\xC990\xCC14\xCDFD\xCFF0\xD1F8\xD3C8\xD5F5\xD76C"
			L"\xACEA\xAE54\xB01D\xB18B\xB314\xB518\xB775\xB93C\xBAAB\xBC4C\xBE45\xC0C0\xC20C\xC3E0\xC5BB\xC6C0\xC7CC\xC998\xCC1C\xCE04\xCFF1\xD207\xD3C9\xD5F7\xD770"
			L"\xACEC\xAE56\xB028\xB18D\xB315\xB51B\xB77C\xB93D\xBAAC\xBC4D\xBE48\xC0C1\xC20D\xC3E2\xC5BC\xC6C1\xC7CE\xC999\xCC1D\xCE08\xCFF3\xD209\xD3D0\xD5F9\xD774"
			L"\xACEF\xAE5C\xB044\xB192\xB31C\xB51C\xB77D\xB940\xBAB0\xBC50\xBE4C\xC0C5\xC20F\xC3E8\xC5BD\xC6C3\xC7D0\xC99B\xCC21\xCE0C\xCFF5\xD210\xD3D8\xD600\xD77C"
			L"\xACF0\xAE5D\xB045\xB193\xB354\xB524\xB780\xB944\xBAB2\xBC5D\xBE4E\xC0C8\xC211\xC3E9\xC5BE\xC6C5\xC7D8\xC99D\xCC22\xCE14\xCFFC\xD22C\xD3E1\xD601\xD77D"
			L"\xACF1\xAE5F\xB048\xB194\xB355\xB525\xB784\xB94C\xBAB8\xBC84\xBE54\xC0C9\xC218\xC3ED\xC5C4\xC6CC\xC7DD\xC9C0\xCC27\xCE19\xD000\xD22D\xD3E3\xD604\xD781"
			L"\xACF3\xAE60\xB04A\xB198\xB356\xB527\xB78C\xB94F\xBAB9\xBC85\xBE55\xC0CC\xC219\xC3F4\xC5C5\xC6CD\xC7E4\xC9C1\xCC28\xCE20\xD004\xD230\xD3EC\xD608\xD788"
			L"\xACF5\xAE61\xB04C\xB19C\xB358\xB528\xB78D\xB951\xBABB\xBC88\xBE57\xC0D0\xC21C\xC3F5\xC5C6\xC6D0\xC7E8\xC9C4\xCC29\xCE21\xD011\xD234\xD3ED\xD610\xD789"
			L"\xACF6\xAE65\xB04E\xB1A8\xB35B\xB529\xB78F\xB958\xBABD\xBC8B\xBE59\xC0D8\xC21F\xC3F8\xC5C7\xC6D4\xC7EC\xC9C7\xCC2C\xCE24\xD018\xD23C\xD3F0\xD611\xD78C"
			L"\xACFC\xAE68\xB053\xB1CC\xB35C\xB52A\xB790\xB959\xBAC4\xBC8C\xBE5A\xC0D9\xC220\xC408\xC5C8\xC6DC\xC800\xC9C8\xCC2E\xCE28\xD02D\xD23D\xD3F4\xD613\xD790"
			L"\xACFD\xAE69\xB054\xB1D0\xB35E\xB530\xB791\xB95C\xBAC8\xBC8E\xBE5B\xC0DB\xC228\xC410\xC5C9\xC6DD\xC801\xC9CA\xCC30\xCE30\xD034\xD23F\xD3FC\xD614\xD798"
			L"\xAD00\xAE6C\xB055\xB1D4\xB35F\xB531\xB792\xB960\xBAD8\xBC94\xBE60\xC0DC\xC229\xC424\xC5CA\xC6E0\xC804\xC9D0\xCC38\xCE31\xD035\xD241\xD3FD\xD615\xD799"
			L"\xAD04\xAE70\xB057\xB1DC\xB364\xB534\xB796\xB968\xBAD9\xBC95\xBE61\xC0DD\xC22B\xC42C\xC5CC\xC6E1\xC808\xC9D1\xCC39\xCE33\xD038\xD248\xD3FF\xD61C\xD79B"
			L"\xAD06\xAE78\xB059\xB1DD\xB365\xB538\xB797\xB969\xBAFC\xBC97\xBE64\xC0E4\xC22D\xC430\xC5CE\xC6E8\xC80A\xC9D3\xCC3B\xCE35\xD03C\xD25C\xD401\xD620\xD79D";
		const wchar Key = ((cp949b & 0xFF) << 8) | (cp949a & 0xFF);
		int Lower = 0, Upper = Length - 1;
		while(Lower <= Upper)
		{
			const int Middle = (Lower + Upper) / 2;
			if(CP949[Middle] == Key)
				return (const wchar) UTF16[Middle];
			if(CP949[Middle] < Key)
				Lower = Middle + 1;
			else Upper = Middle - 1;
		}
		return (const wchar) L'?';
	}

	global_func string UTF16ToCP949_Search(const wchar utf16)
	{
		global_data const int Length = 98 + 2350;
		global_data wstring UTF16 = (wstring)
			L"\t\n\r !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKL"
			L"MNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"\xAC00\xAC01\xAC04\xAC07\xAC08\xAC09\xAC0A\xAC10\xAC11\xAC12\xAC13\xAC14\xAC15\xAC16\xAC17\xAC19\xAC1A\xAC1B\xAC1C\xAC1D\xAC20\xAC24\xAC2C\xAC2D\xAC2F"
			L"\xAC30\xAC31\xAC38\xAC39\xAC3C\xAC40\xAC4B\xAC4D\xAC54\xAC58\xAC5C\xAC70\xAC71\xAC74\xAC77\xAC78\xAC7A\xAC80\xAC81\xAC83\xAC84\xAC85\xAC86\xAC89\xAC8A"
			L"\xAC8B\xAC8C\xAC90\xAC94\xAC9C\xAC9D\xAC9F\xACA0\xACA1\xACA8\xACA9\xACAA\xACAC\xACAF\xACB0\xACB8\xACB9\xACBB\xACBC\xACBD\xACC1\xACC4\xACC8\xACCC\xACD5"
			L"\xACD7\xACE0\xACE1\xACE4\xACE7\xACE8\xACEA\xACEC\xACEF\xACF0\xACF1\xACF3\xACF5\xACF6\xACFC\xACFD\xAD00\xAD04\xAD06\xAD0C\xAD0D\xAD0F\xAD11\xAD18\xAD1C"
			L"\xAD20\xAD28\xAD2C\xAD2D\xAD34\xAD35\xAD38\xAD3C\xAD44\xAD45\xAD47\xAD49\xAD50\xAD54\xAD58\xAD61\xAD63\xAD6C\xAD6D\xAD70\xAD73\xAD74\xAD75\xAD76\xAD7B"
			L"\xAD7C\xAD7D\xAD7F\xAD81\xAD82\xAD88\xAD89\xAD8C\xAD90\xAD9C\xAD9D\xADA4\xADB7\xADC0\xADC1\xADC4\xADC8\xADD0\xADD1\xADD3\xADDC\xADE0\xADE4\xADF8\xADF9"
			L"\xADFC\xADFF\xAE00\xAE01\xAE08\xAE09\xAE0B\xAE0D\xAE14\xAE30\xAE31\xAE34\xAE37\xAE38\xAE3A\xAE40\xAE41\xAE43\xAE45\xAE46\xAE4A\xAE4C\xAE4D\xAE4E\xAE50"
			L"\xAE54\xAE56\xAE5C\xAE5D\xAE5F\xAE60\xAE61\xAE65\xAE68\xAE69\xAE6C\xAE70\xAE78\xAE79\xAE7B\xAE7C\xAE7D\xAE84\xAE85\xAE8C\xAEBC\xAEBD\xAEBE\xAEC0\xAEC4"
			L"\xAECC\xAECD\xAECF\xAED0\xAED1\xAED8\xAED9\xAEDC\xAEE8\xAEEB\xAEED\xAEF4\xAEF8\xAEFC\xAF07\xAF08\xAF0D\xAF10\xAF2C\xAF2D\xAF30\xAF32\xAF34\xAF3C\xAF3D"
			L"\xAF3F\xAF41\xAF42\xAF43\xAF48\xAF49\xAF50\xAF5C\xAF5D\xAF64\xAF65\xAF79\xAF80\xAF84\xAF88\xAF90\xAF91\xAF95\xAF9C\xAFB8\xAFB9\xAFBC\xAFC0\xAFC7\xAFC8"
			L"\xAFC9\xAFCB\xAFCD\xAFCE\xAFD4\xAFDC\xAFE8\xAFE9\xAFF0\xAFF1\xAFF4\xAFF8\xB000\xB001\xB004\xB00C\xB010\xB014\xB01C\xB01D\xB028\xB044\xB045\xB048\xB04A"
			L"\xB04C\xB04E\xB053\xB054\xB055\xB057\xB059\xB05D\xB07C\xB07D\xB080\xB084\xB08C\xB08D\xB08F\xB091\xB098\xB099\xB09A\xB09C\xB09F\xB0A0\xB0A1\xB0A2\xB0A8"
			L"\xB0A9\xB0AB\xB0AC\xB0AD\xB0AE\xB0AF\xB0B1\xB0B3\xB0B4\xB0B5\xB0B8\xB0BC\xB0C4\xB0C5\xB0C7\xB0C8\xB0C9\xB0D0\xB0D1\xB0D4\xB0D8\xB0E0\xB0E5\xB108\xB109"
			L"\xB10B\xB10C\xB110\xB112\xB113\xB118\xB119\xB11B\xB11C\xB11D\xB123\xB124\xB125\xB128\xB12C\xB134\xB135\xB137\xB138\xB139\xB140\xB141\xB144\xB148\xB150"
			L"\xB151\xB154\xB155\xB158\xB15C\xB160\xB178\xB179\xB17C\xB180\xB182\xB188\xB189\xB18B\xB18D\xB192\xB193\xB194\xB198\xB19C\xB1A8\xB1CC\xB1D0\xB1D4\xB1DC"
			L"\xB1DD\xB1DF\xB1E8\xB1E9\xB1EC\xB1F0\xB1F9\xB1FB\xB1FD\xB204\xB205\xB208\xB20B\xB20C\xB214\xB215\xB217\xB219\xB220\xB234\xB23C\xB258\xB25C\xB260\xB268"
			L"\xB269\xB274\xB275\xB27C\xB284\xB285\xB289\xB290\xB291\xB294\xB298\xB299\xB29A\xB2A0\xB2A1\xB2A3\xB2A5\xB2A6\xB2AA\xB2AC\xB2B0\xB2B4\xB2C8\xB2C9\xB2CC"
			L"\xB2D0\xB2D6\xB2D8\xB2D9\xB2DB\xB2DD\xB2E2\xB2E4\xB2E5\xB2E6\xB2E8\xB2EB\xB2EC\xB2ED\xB2EE\xB2EF\xB2F3\xB2F4\xB2F5\xB2F7\xB2F8\xB2F9\xB2FA\xB2FB\xB2FF"
			L"\xB300\xB301\xB304\xB308\xB310\xB311\xB313\xB314\xB315\xB31C\xB354\xB355\xB356\xB358\xB35B\xB35C\xB35E\xB35F\xB364\xB365\xB367\xB369\xB36B\xB36E\xB370"
			L"\xB371\xB374\xB378\xB380\xB381\xB383\xB384\xB385\xB38C\xB390\xB394\xB3A0\xB3A1\xB3A8\xB3AC\xB3C4\xB3C5\xB3C8\xB3CB\xB3CC\xB3CE\xB3D0\xB3D4\xB3D5\xB3D7"
			L"\xB3D9\xB3DB\xB3DD\xB3E0\xB3E4\xB3E8\xB3FC\xB410\xB418\xB41C\xB420\xB428\xB429\xB42B\xB434\xB450\xB451\xB454\xB458\xB460\xB461\xB463\xB465\xB46C\xB480"
			L"\xB488\xB49D\xB4A4\xB4A8\xB4AC\xB4B5\xB4B7\xB4B9\xB4C0\xB4C4\xB4C8\xB4D0\xB4D5\xB4DC\xB4DD\xB4E0\xB4E3\xB4E4\xB4E6\xB4EC\xB4ED\xB4EF\xB4F1\xB4F8\xB514"
			L"\xB515\xB518\xB51B\xB51C\xB524\xB525\xB527\xB528\xB529\xB52A\xB530\xB531\xB534\xB538\xB540\xB541\xB543\xB544\xB545\xB54B\xB54C\xB54D\xB550\xB554\xB55C"
			L"\xB55D\xB55F\xB560\xB561\xB5A0\xB5A1\xB5A4\xB5A8\xB5AA\xB5AB\xB5B0\xB5B1\xB5B3\xB5B4\xB5B5\xB5BB\xB5BC\xB5BD\xB5C0\xB5C4\xB5CC\xB5CD\xB5CF\xB5D0\xB5D1"
			L"\xB5D8\xB5EC\xB610\xB611\xB614\xB618\xB625\xB62C\xB634\xB648\xB664\xB668\xB69C\xB69D\xB6A0\xB6A4\xB6AB\xB6AC\xB6B1\xB6D4\xB6F0\xB6F4\xB6F8\xB700\xB701"
			L"\xB705\xB728\xB729\xB72C\xB72F\xB730\xB738\xB739\xB73B\xB744\xB748\xB74C\xB754\xB755\xB760\xB764\xB768\xB770\xB771\xB773\xB775\xB77C\xB77D\xB780\xB784"
			L"\xB78C\xB78D\xB78F\xB790\xB791\xB792\xB796\xB797\xB798\xB799\xB79C\xB7A0\xB7A8\xB7A9\xB7AB\xB7AC\xB7AD\xB7B4\xB7B5\xB7B8\xB7C7\xB7C9\xB7EC\xB7ED\xB7F0"
			L"\xB7F4\xB7FC\xB7FD\xB7FF\xB800\xB801\xB807\xB808\xB809\xB80C\xB810\xB818\xB819\xB81B\xB81D\xB824\xB825\xB828\xB82C\xB834\xB835\xB837\xB838\xB839\xB840"
			L"\xB844\xB851\xB853\xB85C\xB85D\xB860\xB864\xB86C\xB86D\xB86F\xB871\xB878\xB87C\xB88D\xB8A8\xB8B0\xB8B4\xB8B8\xB8C0\xB8C1\xB8C3\xB8C5\xB8CC\xB8D0\xB8D4"
			L"\xB8DD\xB8DF\xB8E1\xB8E8\xB8E9\xB8EC\xB8F0\xB8F8\xB8F9\xB8FB\xB8FD\xB904\xB918\xB920\xB93C\xB93D\xB940\xB944\xB94C\xB94F\xB951\xB958\xB959\xB95C\xB960"
			L"\xB968\xB969\xB96B\xB96D\xB974\xB975\xB978\xB97C\xB984\xB985\xB987\xB989\xB98A\xB98D\xB98E\xB9AC\xB9AD\xB9B0\xB9B4\xB9BC\xB9BD\xB9BF\xB9C1\xB9C8\xB9C9"
			L"\xB9CC\xB9CE\xB9CF\xB9D0\xB9D1\xB9D2\xB9D8\xB9D9\xB9DB\xB9DD\xB9DE\xB9E1\xB9E3\xB9E4\xB9E5\xB9E8\xB9EC\xB9F4\xB9F5\xB9F7\xB9F8\xB9F9\xB9FA\xBA00\xBA01"
			L"\xBA08\xBA15\xBA38\xBA39\xBA3C\xBA40\xBA42\xBA48\xBA49\xBA4B\xBA4D\xBA4E\xBA53\xBA54\xBA55\xBA58\xBA5C\xBA64\xBA65\xBA67\xBA68\xBA69\xBA70\xBA71\xBA74"
			L"\xBA78\xBA83\xBA84\xBA85\xBA87\xBA8C\xBAA8\xBAA9\xBAAB\xBAAC\xBAB0\xBAB2\xBAB8\xBAB9\xBABB\xBABD\xBAC4\xBAC8\xBAD8\xBAD9\xBAFC\xBB00\xBB04\xBB0D\xBB0F"
			L"\xBB11\xBB18\xBB1C\xBB20\xBB29\xBB2B\xBB34\xBB35\xBB36\xBB38\xBB3B\xBB3C\xBB3D\xBB3E\xBB44\xBB45\xBB47\xBB49\xBB4D\xBB4F\xBB50\xBB54\xBB58\xBB61\xBB63"
			L"\xBB6C\xBB88\xBB8C\xBB90\xBBA4\xBBA8\xBBAC\xBBB4\xBBB7\xBBC0\xBBC4\xBBC8\xBBD0\xBBD3\xBBF8\xBBF9\xBBFC\xBBFF\xBC00\xBC02\xBC08\xBC09\xBC0B\xBC0C\xBC0D"
			L"\xBC0F\xBC11\xBC14\xBC15\xBC16\xBC17\xBC18\xBC1B\xBC1C\xBC1D\xBC1E\xBC1F\xBC24\xBC25\xBC27\xBC29\xBC2D\xBC30\xBC31\xBC34\xBC38\xBC40\xBC41\xBC43\xBC44"
			L"\xBC45\xBC49\xBC4C\xBC4D\xBC50\xBC5D\xBC84\xBC85\xBC88\xBC8B\xBC8C\xBC8E\xBC94\xBC95\xBC97\xBC99\xBC9A\xBCA0\xBCA1\xBCA4\xBCA7\xBCA8\xBCB0\xBCB1\xBCB3"
			L"\xBCB4\xBCB5\xBCBC\xBCBD\xBCC0\xBCC4\xBCCD\xBCCF\xBCD0\xBCD1\xBCD5\xBCD8\xBCDC\xBCF4\xBCF5\xBCF6\xBCF8\xBCFC\xBD04\xBD05\xBD07\xBD09\xBD10\xBD14\xBD24"
			L"\xBD2C\xBD40\xBD48\xBD49\xBD4C\xBD50\xBD58\xBD59\xBD64\xBD68\xBD80\xBD81\xBD84\xBD87\xBD88\xBD89\xBD8A\xBD90\xBD91\xBD93\xBD95\xBD99\xBD9A\xBD9C\xBDA4"
			L"\xBDB0\xBDB8\xBDD4\xBDD5\xBDD8\xBDDC\xBDE9\xBDF0\xBDF4\xBDF8\xBE00\xBE03\xBE05\xBE0C\xBE0D\xBE10\xBE14\xBE1C\xBE1D\xBE1F\xBE44\xBE45\xBE48\xBE4C\xBE4E"
			L"\xBE54\xBE55\xBE57\xBE59\xBE5A\xBE5B\xBE60\xBE61\xBE64\xBE68\xBE6A\xBE70\xBE71\xBE73\xBE74\xBE75\xBE7B\xBE7C\xBE7D\xBE80\xBE84\xBE8C\xBE8D\xBE8F\xBE90"
			L"\xBE91\xBE98\xBE99\xBEA8\xBED0\xBED1\xBED4\xBED7\xBED8\xBEE0\xBEE3\xBEE4\xBEE5\xBEEC\xBF01\xBF08\xBF09\xBF18\xBF19\xBF1B\xBF1C\xBF1D\xBF40\xBF41\xBF44"
			L"\xBF48\xBF50\xBF51\xBF55\xBF94\xBFB0\xBFC5\xBFCC\xBFCD\xBFD0\xBFD4\xBFDC\xBFDF\xBFE1\xC03C\xC051\xC058\xC05C\xC060\xC068\xC069\xC090\xC091\xC094\xC098"
			L"\xC0A0\xC0A1\xC0A3\xC0A5\xC0AC\xC0AD\xC0AF\xC0B0\xC0B3\xC0B4\xC0B5\xC0B6\xC0BC\xC0BD\xC0BF\xC0C0\xC0C1\xC0C5\xC0C8\xC0C9\xC0CC\xC0D0\xC0D8\xC0D9\xC0DB"
			L"\xC0DC\xC0DD\xC0E4\xC0E5\xC0E8\xC0EC\xC0F4\xC0F5\xC0F7\xC0F9\xC100\xC104\xC108\xC110\xC115\xC11C\xC11D\xC11E\xC11F\xC120\xC123\xC124\xC126\xC127\xC12C"
			L"\xC12D\xC12F\xC130\xC131\xC136\xC138\xC139\xC13C\xC140\xC148\xC149\xC14B\xC14C\xC14D\xC154\xC155\xC158\xC15C\xC164\xC165\xC167\xC168\xC169\xC170\xC174"
			L"\xC178\xC185\xC18C\xC18D\xC18E\xC190\xC194\xC196\xC19C\xC19D\xC19F\xC1A1\xC1A5\xC1A8\xC1A9\xC1AC\xC1B0\xC1BD\xC1C4\xC1C8\xC1CC\xC1D4\xC1D7\xC1D8\xC1E0"
			L"\xC1E4\xC1E8\xC1F0\xC1F1\xC1F3\xC1FC\xC1FD\xC200\xC204\xC20C\xC20D\xC20F\xC211\xC218\xC219\xC21C\xC21F\xC220\xC228\xC229\xC22B\xC22D\xC22F\xC231\xC232"
			L"\xC234\xC248\xC250\xC251\xC254\xC258\xC260\xC265\xC26C\xC26D\xC270\xC274\xC27C\xC27D\xC27F\xC281\xC288\xC289\xC290\xC298\xC29B\xC29D\xC2A4\xC2A5\xC2A8"
			L"\xC2AC\xC2AD\xC2B4\xC2B5\xC2B7\xC2B9\xC2DC\xC2DD\xC2E0\xC2E3\xC2E4\xC2EB\xC2EC\xC2ED\xC2EF\xC2F1\xC2F6\xC2F8\xC2F9\xC2FB\xC2FC\xC300\xC308\xC309\xC30C"
			L"\xC30D\xC313\xC314\xC315\xC318\xC31C\xC324\xC325\xC328\xC329\xC345\xC368\xC369\xC36C\xC370\xC372\xC378\xC379\xC37C\xC37D\xC384\xC388\xC38C\xC399\xC3D8"
			L"\xC3D9\xC3DC\xC3DF\xC3E0\xC3E2\xC3E8\xC3E9\xC3ED\xC3F4\xC3F5\xC3F8\xC408\xC410\xC424\xC42C\xC430\xC434\xC43C\xC43D\xC448\xC464\xC465\xC468\xC46C\xC474"
			L"\xC475\xC479\xC480\xC494\xC49C\xC4B8\xC4BC\xC4E9\xC4F0\xC4F1\xC4F4\xC4F8\xC4FA\xC4FF\xC500\xC501\xC50C\xC510\xC514\xC51C\xC528\xC529\xC52C\xC530\xC538"
			L"\xC539\xC53B\xC53D\xC544\xC545\xC548\xC549\xC54A\xC54C\xC54D\xC54E\xC553\xC554\xC555\xC557\xC558\xC559\xC55D\xC55E\xC560\xC561\xC564\xC568\xC570\xC571"
			L"\xC573\xC574\xC575\xC57C\xC57D\xC580\xC584\xC587\xC58C\xC58D\xC58F\xC591\xC595\xC597\xC598\xC59C\xC5A0\xC5A9\xC5B4\xC5B5\xC5B8\xC5B9\xC5BB\xC5BC\xC5BD"
			L"\xC5BE\xC5C4\xC5C5\xC5C6\xC5C7\xC5C8\xC5C9\xC5CA\xC5CC\xC5CE\xC5D0\xC5D1\xC5D4\xC5D8\xC5E0\xC5E1\xC5E3\xC5E5\xC5EC\xC5ED\xC5EE\xC5F0\xC5F4\xC5F6\xC5F7"
			L"\xC5FC\xC5FD\xC5FE\xC5FF\xC600\xC601\xC605\xC606\xC607\xC608\xC60C\xC610\xC618\xC619\xC61B\xC61C\xC624\xC625\xC628\xC62C\xC62D\xC62E\xC630\xC633\xC634"
			L"\xC635\xC637\xC639\xC63B\xC640\xC641\xC644\xC648\xC650\xC651\xC653\xC654\xC655\xC65C\xC65D\xC660\xC66C\xC66F\xC671\xC678\xC679\xC67C\xC680\xC688\xC689"
			L"\xC68B\xC68D\xC694\xC695\xC698\xC69C\xC6A4\xC6A5\xC6A7\xC6A9\xC6B0\xC6B1\xC6B4\xC6B8\xC6B9\xC6BA\xC6C0\xC6C1\xC6C3\xC6C5\xC6CC\xC6CD\xC6D0\xC6D4\xC6DC"
			L"\xC6DD\xC6E0\xC6E1\xC6E8\xC6E9\xC6EC\xC6F0\xC6F8\xC6F9\xC6FD\xC704\xC705\xC708\xC70C\xC714\xC715\xC717\xC719\xC720\xC721\xC724\xC728\xC730\xC731\xC733"
			L"\xC735\xC737\xC73C\xC73D\xC740\xC744\xC74A\xC74C\xC74D\xC74F\xC751\xC752\xC753\xC754\xC755\xC756\xC757\xC758\xC75C\xC760\xC768\xC76B\xC774\xC775\xC778"
			L"\xC77C\xC77D\xC77E\xC783\xC784\xC785\xC787\xC788\xC789\xC78A\xC78E\xC790\xC791\xC794\xC796\xC797\xC798\xC79A\xC7A0\xC7A1\xC7A3\xC7A4\xC7A5\xC7A6\xC7AC"
			L"\xC7AD\xC7B0\xC7B4\xC7BC\xC7BD\xC7BF\xC7C0\xC7C1\xC7C8\xC7C9\xC7CC\xC7CE\xC7D0\xC7D8\xC7DD\xC7E4\xC7E8\xC7EC\xC800\xC801\xC804\xC808\xC80A\xC810\xC811"
			L"\xC813\xC815\xC816\xC81C\xC81D\xC820\xC824\xC82C\xC82D\xC82F\xC831\xC838\xC83C\xC840\xC848\xC849\xC84C\xC84D\xC854\xC870\xC871\xC874\xC878\xC87A\xC880"
			L"\xC881\xC883\xC885\xC886\xC887\xC88B\xC88C\xC88D\xC894\xC89D\xC89F\xC8A1\xC8A8\xC8BC\xC8BD\xC8C4\xC8C8\xC8CC\xC8D4\xC8D5\xC8D7\xC8D9\xC8E0\xC8E1\xC8E4"
			L"\xC8F5\xC8FC\xC8FD\xC900\xC904\xC905\xC906\xC90C\xC90D\xC90F\xC911\xC918\xC92C\xC934\xC950\xC951\xC954\xC958\xC960\xC961\xC963\xC96C\xC970\xC974\xC97C"
			L"\xC988\xC989\xC98C\xC990\xC998\xC999\xC99B\xC99D\xC9C0\xC9C1\xC9C4\xC9C7\xC9C8\xC9CA\xC9D0\xC9D1\xC9D3\xC9D5\xC9D6\xC9D9\xC9DA\xC9DC\xC9DD\xC9E0\xC9E2"
			L"\xC9E4\xC9E7\xC9EC\xC9ED\xC9EF\xC9F0\xC9F1\xC9F8\xC9F9\xC9FC\xCA00\xCA08\xCA09\xCA0B\xCA0C\xCA0D\xCA14\xCA18\xCA29\xCA4C\xCA4D\xCA50\xCA54\xCA5C\xCA5D"
			L"\xCA5F\xCA60\xCA61\xCA68\xCA7D\xCA84\xCA98\xCABC\xCABD\xCAC0\xCAC4\xCACC\xCACD\xCACF\xCAD1\xCAD3\xCAD8\xCAD9\xCAE0\xCAEC\xCAF4\xCB08\xCB10\xCB14\xCB18"
			L"\xCB20\xCB21\xCB41\xCB48\xCB49\xCB4C\xCB50\xCB58\xCB59\xCB5D\xCB64\xCB78\xCB79\xCB9C\xCBB8\xCBD4\xCBE4\xCBE7\xCBE9\xCC0C\xCC0D\xCC10\xCC14\xCC1C\xCC1D"
			L"\xCC21\xCC22\xCC27\xCC28\xCC29\xCC2C\xCC2E\xCC30\xCC38\xCC39\xCC3B\xCC3C\xCC3D\xCC3E\xCC44\xCC45\xCC48\xCC4C\xCC54\xCC55\xCC57\xCC58\xCC59\xCC60\xCC64"
			L"\xCC66\xCC68\xCC70\xCC75\xCC98\xCC99\xCC9C\xCCA0\xCCA8\xCCA9\xCCAB\xCCAC\xCCAD\xCCB4\xCCB5\xCCB8\xCCBC\xCCC4\xCCC5\xCCC7\xCCC9\xCCD0\xCCD4\xCCE4\xCCEC"
			L"\xCCF0\xCD01\xCD08\xCD09\xCD0C\xCD10\xCD18\xCD19\xCD1B\xCD1D\xCD24\xCD28\xCD2C\xCD39\xCD5C\xCD60\xCD64\xCD6C\xCD6D\xCD6F\xCD71\xCD78\xCD88\xCD94\xCD95"
			L"\xCD98\xCD9C\xCDA4\xCDA5\xCDA7\xCDA9\xCDB0\xCDC4\xCDCC\xCDD0\xCDE8\xCDEC\xCDF0\xCDF8\xCDF9\xCDFB\xCDFD\xCE04\xCE08\xCE0C\xCE14\xCE19\xCE20\xCE21\xCE24"
			L"\xCE28\xCE30\xCE31\xCE33\xCE35\xCE58\xCE59\xCE5C\xCE5F\xCE60\xCE61\xCE68\xCE69\xCE6B\xCE6D\xCE74\xCE75\xCE78\xCE7C\xCE84\xCE85\xCE87\xCE89\xCE90\xCE91"
			L"\xCE94\xCE98\xCEA0\xCEA1\xCEA3\xCEA4\xCEA5\xCEAC\xCEAD\xCEC1\xCEE4\xCEE5\xCEE8\xCEEB\xCEEC\xCEF4\xCEF5\xCEF7\xCEF8\xCEF9\xCF00\xCF01\xCF04\xCF08\xCF10"
			L"\xCF11\xCF13\xCF15\xCF1C\xCF20\xCF24\xCF2C\xCF2D\xCF2F\xCF30\xCF31\xCF38\xCF54\xCF55\xCF58\xCF5C\xCF64\xCF65\xCF67\xCF69\xCF70\xCF71\xCF74\xCF78\xCF80"
			L"\xCF85\xCF8C\xCFA1\xCFA8\xCFB0\xCFC4\xCFE0\xCFE1\xCFE4\xCFE8\xCFF0\xCFF1\xCFF3\xCFF5\xCFFC\xD000\xD004\xD011\xD018\xD02D\xD034\xD035\xD038\xD03C\xD044"
			L"\xD045\xD047\xD049\xD050\xD054\xD058\xD060\xD06C\xD06D\xD070\xD074\xD07C\xD07D\xD081\xD0A4\xD0A5\xD0A8\xD0AC\xD0B4\xD0B5\xD0B7\xD0B9\xD0C0\xD0C1\xD0C4"
			L"\xD0C8\xD0C9\xD0D0\xD0D1\xD0D3\xD0D4\xD0D5\xD0DC\xD0DD\xD0E0\xD0E4\xD0EC\xD0ED\xD0EF\xD0F0\xD0F1\xD0F8\xD10D\xD130\xD131\xD134\xD138\xD13A\xD140\xD141"
			L"\xD143\xD144\xD145\xD14C\xD14D\xD150\xD154\xD15C\xD15D\xD15F\xD161\xD168\xD16C\xD17C\xD184\xD188\xD1A0\xD1A1\xD1A4\xD1A8\xD1B0\xD1B1\xD1B3\xD1B5\xD1BA"
			L"\xD1BC\xD1C0\xD1D8\xD1F4\xD1F8\xD207\xD209\xD210\xD22C\xD22D\xD230\xD234\xD23C\xD23D\xD23F\xD241\xD248\xD25C\xD264\xD280\xD281\xD284\xD288\xD290\xD291"
			L"\xD295\xD29C\xD2A0\xD2A4\xD2AC\xD2B1\xD2B8\xD2B9\xD2BC\xD2BF\xD2C0\xD2C2\xD2C8\xD2C9\xD2CB\xD2D4\xD2D8\xD2DC\xD2E4\xD2E5\xD2F0\xD2F1\xD2F4\xD2F8\xD300"
			L"\xD301\xD303\xD305\xD30C\xD30D\xD30E\xD310\xD314\xD316\xD31C\xD31D\xD31F\xD320\xD321\xD325\xD328\xD329\xD32C\xD330\xD338\xD339\xD33B\xD33C\xD33D\xD344"
			L"\xD345\xD37C\xD37D\xD380\xD384\xD38C\xD38D\xD38F\xD390\xD391\xD398\xD399\xD39C\xD3A0\xD3A8\xD3A9\xD3AB\xD3AD\xD3B4\xD3B8\xD3BC\xD3C4\xD3C5\xD3C8\xD3C9"
			L"\xD3D0\xD3D8\xD3E1\xD3E3\xD3EC\xD3ED\xD3F0\xD3F4\xD3FC\xD3FD\xD3FF\xD401\xD408\xD41D\xD440\xD444\xD45C\xD460\xD464\xD46D\xD46F\xD478\xD479\xD47C\xD47F"
			L"\xD480\xD482\xD488\xD489\xD48B\xD48D\xD494\xD4A9\xD4CC\xD4D0\xD4D4\xD4DC\xD4DF\xD4E8\xD4EC\xD4F0\xD4F8\xD4FB\xD4FD\xD504\xD508\xD50C\xD514\xD515\xD517"
			L"\xD53C\xD53D\xD540\xD544\xD54C\xD54D\xD54F\xD551\xD558\xD559\xD55C\xD560\xD565\xD568\xD569\xD56B\xD56D\xD574\xD575\xD578\xD57C\xD584\xD585\xD587\xD588"
			L"\xD589\xD590\xD5A5\xD5C8\xD5C9\xD5CC\xD5D0\xD5D2\xD5D8\xD5D9\xD5DB\xD5DD\xD5E4\xD5E5\xD5E8\xD5EC\xD5F4\xD5F5\xD5F7\xD5F9\xD600\xD601\xD604\xD608\xD610"
			L"\xD611\xD613\xD614\xD615\xD61C\xD620\xD624\xD62D\xD638\xD639\xD63C\xD640\xD645\xD648\xD649\xD64B\xD64D\xD651\xD654\xD655\xD658\xD65C\xD667\xD669\xD670"
			L"\xD671\xD674\xD683\xD685\xD68C\xD68D\xD690\xD694\xD69D\xD69F\xD6A1\xD6A8\xD6AC\xD6B0\xD6B9\xD6BB\xD6C4\xD6C5\xD6C8\xD6CC\xD6D1\xD6D4\xD6D7\xD6D9\xD6E0"
			L"\xD6E4\xD6E8\xD6F0\xD6F5\xD6FC\xD6FD\xD700\xD704\xD711\xD718\xD719\xD71C\xD720\xD728\xD729\xD72B\xD72D\xD734\xD735\xD738\xD73C\xD744\xD747\xD749\xD750"
			L"\xD751\xD754\xD756\xD757\xD758\xD759\xD760\xD761\xD763\xD765\xD769\xD76C\xD770\xD774\xD77C\xD77D\xD781\xD788\xD789\xD78C\xD790\xD798\xD799\xD79B\xD79D";
		global_data wstring CP949 = (wstring)
			"\t \n \r   ! \" # $ % & \' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I J K L "
			"M N O P Q R S T U V W X Y Z [ \\ ] ^ _ ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~ "
			"°¡°¢°£°¤°¥°¦°§°¨°©°ª°«°¬°­°®°¯°°°±°²°³°´°µ°¶°·°¸°¹°º°»°¼°½°¾°¿°À°Á°Â°Ã°Ä°Å°Æ°Ç°È°É°Ê°Ë°Ì°Í°Î°Ï°Ð°Ñ°Ò°Ó°Ô°Õ°Ö"
			"°×°Ø°Ù°Ú°Û°Ü°Ý°Þ°ß°à°á°â°ã°ä°å°æ°ç°è°é°ê°ë°ì°í°î°ï°ð°ñ°ò°ó°ô°õ°ö°÷°ø°ù°ú°û°ü°ý°þ±¡±¢±£±¤±¥±¦±§‚R±©±ª±«±¬±­±®"
			"±¯±°±±±²±³±´±µ±¶±·±¸±¹±º±»±¼±½±¾±¿±À±Á±Â±Ã±Ä±Å±Æ±Ç±È±É±Ê±Ë±Ì±Í±Î±Ï±Ð±Ñ±Ò±Ó±Ô±Õ±Ö±×±Ø±Ù±Ú±Û±Ü±Ý±Þ±ß±à±á±â±ã±ä"
			"±å±æ±ç±è±é±ê±ë±ì±í±î±ï±ð±ñ±ò±ó±ô±õ±ö±÷±ø±ù±ú±û±ü±ý±þ²¡²¢²£²¤²¥²¦²§²¨²©²ª²«²¬²­²®²¯²°²±²²²³²´²µ²¶²·²¸²¹²º²»²¼"
			"²½²¾²¿²À²Á²Â²Ã²Ä²Å²Æ²Ç²È²É²Ê²Ë²Ì²Í²Î²Ï²Ð²Ñ²Ò²Ó²Ô²Õ²Ö²×²Ø²Ù²Ú²Û²Ü²Ý²Þ²ß²à²á²â²ã²ä²å²æ²ç²è²é²ê²ë²ì²í²î²ï²ð²ñ²ò"
			"²ó²ô²õ²ö²÷²ø²ù²ú²û²ü²ý²þ³¡³¢³£³¤³¥³¦³§³¨³©³ª³«³¬³­³®³¯³°³±³²³³³´³µ³¶³·³¸³¹³º³»³¼³½³¾³¿³À³Á³Â³Ã³Ä³Å³Æ³Ç³È³É³Ê"
			"³Ë³Ì³Í³Î³Ï³Ð³Ñ³Ò³Ó³Ô³Õ³Ö³×³Ø³Ù³Ú³Û³Ü³Ý³Þ³ß³à³á³â³ã³ä³å³æ³ç³è³é³ê³ë³ì³í³î³ï³ð³ñ³ò³ó³ô³õ³ö³÷³ø³ù³ú³û³ü³ý³þ´¡´¢"
			"´£´¤´¥´¦´§´¨´©´ª´«´¬´­´®´¯´°´±´²´³´´´µ´¶´·´¸´¹´º´»´¼´½´¾´¿´À´Á´Â´Ã´Ä´Å´Æ´Ç´È´É´Ê´Ë´Ì´Í´Î´Ï´Ð´Ñ´Òˆš´Ô´Õ´Ö´×´Ø"
			"´Ù´Ú´Û´Ü´Ý´Þ´ß´à´á´â´ã´ä´å´æ´ç´è´é´ê´ë´ì´í´î´ï´ð´ñ´ò´ó´ô´õ´ö´÷´ø´ù´ú´û´ü´ý´þµ¡µ¢µ£µ¤µ¥µ¦µ§µ¨µ©µªµ«µ¬µ­µ®µ¯µ°"
			"µ±µ²µ³µ´µµµ¶µ·µ¸µ¹µºµ»µ¼µ½µ¾µ¿µÀµÁµÂµÃµÄµÅµÆµÇµÈµÉµÊµËµÌµÍµÎµÏµÐµÑµÒµÓµÔµÕµÖµ×µØµÙµÚµÛµÜµÝµÞµßµàµáµâµãµäµåµæ"
			"µçµèµéµêµëµìµíµîµïµðµñµòµóµôµõµöµ÷µøµùµúµûµüµýµþ¶¡¶¢¶£¶¤¶¥¶¦¶§¶¨¶©¶ª¶«¶¬¶­¶®¶¯¶°¶±¶²¶³¶´¶µ¶¶¶·¶¸¶¹¶º¶»¶¼¶½¶¾"
			"¶¿¶À¶Á¶Â¶Ã¶Ä¶Å¶Æ¶Ç¶È¶É¶Ê¶Ë¶Ì¶Í¶Î¶Ï¶Ð¶Ñ¶Ò¶Ó¶Ô¶Õ¶Ö¶×¶Ø¶Ù¶Ú¶Û¶Ü¶Ý¶Þ¶ß¶à¶á¶â¶ã¶ä¶å¶æ¶ç¶è¶é¶ê¶ë¶ì¶í¶î¶ï¶ð¶ñ¶ò¶ó¶ô"
			"¶õ¶ö¶÷¶ø¶ù¶ú¶û¶ü¶ý¶þ·¡·¢·£·¤·¥·¦·§·¨·©·ª·«·¬·­·®·¯·°·±·²·³·´·µ·¶···¸·¹·º·»·¼·½·¾·¿·À·Á·Â·Ã·Ä·Å·Æ·Ç·È·É·Ê·Ë·Ì"
			"·Í·Î·Ï·Ð·Ñ·Ò·Ó·Ô·Õ·Ö·×·Ø·Ù·Ú·Û·Ü·Ý·Þ·ß·à·á·â·ã·ä·å·æ·ç·è·é·ê·ë·ì·í·î·ï·ð·ñ·ò·ó·ô·õ·ö·÷·ø·ù·ú·û·ü·ý·þ¸¡¸¢¸£¸¤"
			"¸¥¸¦¸§¸¨¸©¸ª¸«¸¬¸­¸®¸¯¸°¸±¸²¸³¸´¸µ¸¶¸·¸¸¸¹¸º¸»¸¼¸½¸¾¸¿¸À¸Á¸Â¸Ã¸Ä¸Å¸Æ¸Ç¸È¸É¸Ê¸Ë¸Ì¸Í¸Î¸Ï¸Ð¸Ñ¸Ò¸Ó¸Ô¸Õ¸Ö¸×¸Ø¸Ù¸Ú"
			"¸Û¸Ü¸Ý¸Þ¸ß¸à¸á¸â¸ã¸ä¸å¸æ¸ç¸è¸é¸ê¸ë¸ì¸í¸î¸ï¸ð¸ñ¸ò¸ó¸ô¸õ¸ö¸÷¸ø¸ù¸ú¸û¸ü¸ý¸þ¹¡¹¢¹£¹¤¹¥¹¦¹§¹¨¹©¹ª¹«¹¬¹­¹®¹¯¹°¹±¹²"
			"¹³¹´¹µ¹¶¹·¹¸¹¹¹º¹»¹¼¹½¹¾¹¿¹À¹Á¹Â¹Ã¹Ä¹Å¹Æ¹Ç¹È¹É¹Ê¹Ë¹Ì¹Í¹Î¹Ï¹Ð¹Ñ¹Ò¹Ó¹Ô¹Õ¹Ö¹×¹Ø¹Ù¹Ú¹Û¹Ü¹Ý¹Þ¹ß¹à¹á¹â¹ã¹ä¹å¹æ¹ç¹è"
			"¹é¹ê¹ë¹ì¹í¹î¹ï¹ð¹ñ¹ò¹ó¹ô¹õ¹ö¹÷¹ø¹ù¹ú¹û¹ü¹ý¹þº¡º¢º£º¤º¥º¦º§º¨º©ºªº«º¬º­º®º¯º°º±º²º³º´ºµº¶º·º¸º¹ººº»º¼º½º¾º¿ºÀ"
			"ºÁºÂºÃºÄºÅºÆºÇºÈºÉºÊºËºÌºÍºÎºÏºÐºÑºÒºÓºÔºÕºÖº×ºØºÙºÚºÛºÜºÝºÞºßºàºáºâºãºäºåºæºçºèºéºêºëºìºíºîºïºðºñºòºóºôºõºö"
			"º÷ºøºùºúºûºüºýºþ»¡»¢»£»¤»¥»¦»§»¨»©»ª»«»¬»­»®»¯»°»±»²»³»´»µ»¶»·»¸»¹»º»»»¼»½»¾»¿»À»Á»Â»Ã»Ä»Å»Æ»Ç»È»É»Ê»Ë»Ì»Í»Î"
			"»Ï»Ð»Ñ»Ò»Ó»Ô»Õ»Ö»×»Ø»Ù»Ú»Û»Ü»Ý»Þ»ß»à»á»â»ã»ä»å»æ»ç»è»é»ê»ë»ì»í»î»ï»ð»ñ»ò»ó»ô»õ»ö»÷»ø»ù»ú»û»ü»ý»þ¼¡¼¢¼£¼¤¼¥¼¦"
			"¼§¼¨¼©¼ª¼«¼¬¼­¼®¼¯¼°¼±¼²¼³¼´¼µ¼¶¼·¼¸¼¹¼º¼»¼¼¼½¼¾¼¿¼À¼Á¼Â¼Ã¼Ä¼Å¼Æ¼Ç¼È¼É¼Ê¼Ë¼Ì¼Í¼Î¼Ï¼Ð¼Ñ¼Ò¼Ó¼Ô¼Õ¼Ö¼×¼Ø¼Ù¼Ú¼Û¼Ü"
			"¼Ý¼Þ¼ß¼à¼á¼â¼ã¼ä¼å¼æ¼ç¼è¼é¼ê¼ë¼ì¼í¼î¼ï¼ð¼ñ¼ò¼ó¼ô¼õ¼ö¼÷¼ø¼ù¼ú¼û¼ü¼ý¼þ½¡½¢½£½¤½¥½¦½§½¨½©½ª½«½¬½­½®½¯½°½±½²½³½´"
			"½µ½¶½·½¸½¹½º½»½¼½½½¾½¿½À½Á½Â½Ã½Ä½Å½Æ½Ç½È½É½Ê½Ë½Ì½Í½Î½Ï½Ð½Ñ½Ò½Ó½Ô½Õ½Ö½×½Ø½Ù½Ú½Û½Ü½Ý½Þ½ß½à½á½â½ã½ä½å½æ½ç½è½é½ê"
			"½ë½ì›À½î½ï½ð½ñ½ò½ó½ô½õ½ö½÷½ø½ù½ú½û½ü½ý½þ¾¡¾¢¾£¾¤¾¥¾¦¾§¾¨¾©¾ª¾«¾¬¾­¾®¾¯¾°¾±¾²¾³¾´¾µ¾¶¾·¾¸¾¹¾º¾»¾¼¾½¾¾¾¿¾À¾Á¾Â"
			"¾Ã¾Ä¾Å¾Æ¾Ç¾È¾É¾Ê¾Ë¾Ì¾Í¾Î¾Ï¾Ð¾Ñ¾Ò¾Ó¾Ô¾Õ¾Ö¾×¾Ø¾Ù¾Ú¾Û¾Ü¾Ý¾Þ¾ß¾à¾á¾â¾ã¾ä¾å¾æ¾ç¾è¾é¾ê¾ë¾ì¾í¾î¾ï¾ð¾ñ¾ò¾ó¾ô¾õ¾ö¾÷¾ø"
			"¾ù¾ú¾û¾ü¾ý¾þ¿¡¿¢¿£¿¤¿¥¿¦¿§¿¨¿©¿ª¿«¿¬¿­¿®¿¯¿°¿±¿²¿³¿´¿µ¿¶¿·¿¸¿¹¿º¿»¿¼¿½¿¾¿¿¿À¿Á¿Â¿Ã¿Ä¿Å¿Æ¿Ç¿È¿É¿Ê¿Ë¿Ì¿Í¿Î¿Ï¿Ð"
			"¿Ñ¿Ò¿Ó¿Ô¿Õ¿Ö¿×¿Ø¿Ù¿Ú¿Û¿Ü¿Ý¿Þ¿ß¿à¿á¿â¿ã¿ä¿å¿æ¿ç¿è¿é¿ê¿ë¿ì¿í¿î¿ï¿ð¿ñ¿ò¿ó¿ô¿õ¿ö¿÷¿ø¿ù¿ú¿û¿ü¿ý¿þÀ¡À¢À£À¤À¥À¦À§À¨"
			"À©ÀªÀ«À¬À­À®À¯À°À±À²À³À´ÀµÀ¶À·À¸À¹ÀºÀ»À¼À½À¾À¿ÀÀÀÁÀÂÀÃÀÄÀÅÀÆÀÇÀÈÀÉÀÊÀËÀÌÀÍÀÎÀÏÀÐÀÑÀÒÀÓÀÔÀÕÀÖÀ×ÀØÀÙÀÚÀÛÀÜÀÝÀÞ"
			"ÀßÀàÀáÀâÀãÀäÀåÀæÀçÀèÀéÀêÀëÀìÀíÀîÀïÀðÀñÀòÀóÀôÀõÀöÀ÷ÀøÀùÀúÀûÀüÀýÀþÁ¡Á¢Á£Á¤Á¥Á¦Á§Á¨Á©ÁªÁ«Á¬Á­Á®Á¯Á°Á±Á²Á³Á´ÁµÁ¶"
			"Á·Á¸Á¹ÁºÁ»Á¼Á½Á¾Á¿ÁÀÁÁÁÂÁÃÁÄÁÅÁÆÁÇÁÈÁÉÁÊÁËÁÌÁÍÁÎÁÏÁÐÁÑÁÒÁÓÁÔÁÕÁÖÁ×ÁØÁÙÁÚÁÛÁÜÁÝÁÞÁßÁàÁáÁâÁãÁäÁåÁæÁçÁèÁéÁêÁëÁì"
			"ÁíÁîÁïÁðÁñÁòÁóÁôÁõÁöÁ÷ÁøÁùÁúÁûÁüÁýÁþÂ¡Â¢Â£Â¤Â¥Â¦Â§Â¨Â©ÂªÂ«Â¬Â­Â®Â¯Â°Â±Â²Â³Â´ÂµÂ¶Â·Â¸Â¹ÂºÂ»Â¼Â½Â¾Â¿ÂÀÂÁÂÂÂÃÂÄ"
			"ÂÅÂÆÂÇÂÈÂÉÂÊÂËÂÌÂÍÂÎÂÏÂÐÂÑÂÒÂÓÂÔÂÕÂÖÂ×ÂØÂÙÂÚÂÛÂÜÂÝÂÞÂßÂàÂáÂâÂãÂäÂåÂæÂçÂèÂéÂêÂëÂìÂíÂîÂïÂðÂñÂòÂóÂôÂõÂöÂ÷ÂøÂùÂú"
			"ÂûÂüÂýÂþÃ¡Ã¢Ã£Ã¤Ã¥Ã¦Ã§Ã¨Ã©ÃªÃ«Ã¬Ã­Ã®Ã¯Ã°Ã±Ã²Ã³Ã´ÃµÃ¶Ã·Ã¸Ã¹ÃºÃ»Ã¼Ã½Ã¾Ã¿ÃÀÃÁÃÂÃÃÃÄÃÅÃÆÃÇÃÈÃÉÃÊÃËÃÌÃÍÃÎÃÏÃÐÃÑÃÒ"
			"ÃÓÃÔÃÕÃÖÃ×ÃØÃÙÃÚÃÛÃÜÃÝÃÞÃßÃàÃáÃâÃãÃäÃåÃæÃçÃèÃéÃêÃëÃìÃíÃîÃïÃðÃñÃòÃóÃôÃõÃöÃ÷ÃøÃùÃúÃûÃüÃýÃþÄ¡Ä¢Ä£Ä¤Ä¥Ä¦Ä§Ä¨Ä©Äª"
			"Ä«Ä¬Ä­Ä®Ä¯Ä°Ä±Ä²Ä³Ä´ÄµÄ¶Ä·Ä¸Ä¹ÄºÄ»Ä¼Ä½Ä¾Ä¿ÄÀÄÁÄÂÄÃÄÄÄÅÄÆÄÇÄÈÄÉÄÊÄËÄÌÄÍÄÎÄÏÄÐÄÑÄÒÄÓÄÔÄÕÄÖÄ×ÄØÄÙÄÚÄÛÄÜÄÝÄÞÄßÄà"
			"ÄáÄâÄãÄäÄåÄæÄçÄèÄéÄêÄëÄìÄíÄîÄïÄðÄñÄòÄóÄôÄõÄöÄ÷ÄøÄùÄúÄûÄüÄýÄþÅ¡Å¢Å£Å¤Å¥Å¦Å§Å¨Å©ÅªÅ«Å¬Å­Å®Å¯Å°Å±Å²Å³Å´ÅµÅ¶Å·Å¸"
			"Å¹ÅºÅ»Å¼Å½Å¾Å¿ÅÀÅÁÅÂÅÃÅÄÅÅÅÆÅÇÅÈÅÉÅÊÅËÅÌÅÍÅÎÅÏÅÐÅÑÅÒÅÓÅÔÅÕÅÖÅ×ÅØÅÙÅÚÅÛÅÜÅÝÅÞÅßÅàÅáÅâÅãÅäÅåÅæÅçÅèÅéÅêÅëÅìÅíÅî"
			"ÅïÅðÅñÅòÅóÅôÅõÅöÅ÷ÅøÅùÅúÅûÅüÅýÅþÆ¡Æ¢Æ£Æ¤Æ¥Æ¦Æ§Æ¨Æ©ÆªÆ«Æ¬Æ­Æ®Æ¯Æ°Æ±Æ²Æ³Æ´ÆµÆ¶Æ·Æ¸Æ¹ÆºÆ»Æ¼Æ½Æ¾Æ¿ÆÀÆÁÆÂÆÃÆÄÆÅÆÆ"
			"ÆÇÆÈÆÉÆÊÆËÆÌÆÍÆÎÆÏÆÐÆÑÆÒÆÓÆÔÆÕÆÖÆ×ÆØÆÙÆÚÆÛÆÜÆÝÆÞÆßÆàÆáÆâÆãÆäÆåÆæÆçÆèÆéÆêÆëÆìÆíÆîÆïÆðÆñÆòÆóÆôÆõÆöÆ÷ÆøÆùÆúÆûÆü"
			"ÆýÆþÇ¡Ç¢Ç£Ç¤Ç¥Ç¦Ç§Ç¨Ç©ÇªÇ«Ç¬Ç­Ç®Ç¯Ç°Ç±Ç²Ç³Ç´ÇµÇ¶Ç·Ç¸Ç¹ÇºÇ»Ç¼Ç½Ç¾Ç¿ÇÀÇÁÇÂÇÃÇÄÇÅÇÆÇÇÇÈÇÉÇÊÇËÇÌÇÍÇÎÇÏÇÐÇÑÇÒÇÓÇÔ"
			"ÇÕÇÖÇ×ÇØÇÙÇÚÇÛÇÜÇÝÇÞÇßÇàÇáÇâÇãÇäÇåÇæÇçÇèÇéÇêÇëÇìÇíÇîÇïÇðÇñÇòÇóÇôÇõÇöÇ÷ÇøÇùÇúÇûÇüÇýÇþÈ¡È¢È£È¤È¥È¦È§È¨È©ÈªÈ«È¬"
			"È­È®È¯È°È±È²È³È´ÈµÈ¶È·È¸È¹ÈºÈ»È¼È½È¾È¿ÈÀÈÁÈÂÈÃÈÄÈÅÈÆÈÇÈÈÈÉÈÊÈËÈÌÈÍÈÎÈÏÈÐÈÑÈÒÈÓÈÔÈÕÈÖÈ×ÈØÈÙÈÚÈÛÈÜÈÝÈÞÈßÈàÈáÈâ"
			"ÈãÈäÈåÈæÈçÈèÈéÈêÈëÈìÈíÈîÈïÈðÈñÈòÈóÈôÈõÈöÈ÷ÈøÈùÈúÈûÈüÈýÈþ";
		const wchar Key = utf16;
		int Lower = 0, Upper = Length - 1;
		while(Lower <= Upper)
		{
			const int Middle = (Lower + Upper) / 2;
			if(UTF16[Middle] == Key)
				return (string) &CP949[Middle];
			if(UTF16[Middle] < Key)
				Lower = Middle + 1;
			else Upper = Middle - 1;
		}
		return (string) "?";
	}

	global_func inline uint& _RandValue()
	{
		global_data uint RandValue = 0;
		return RandValue;
	}

	global_func inline void _SetRand(const uint Key)
	{
		_RandValue() = Key;
	}

	global_func inline uint _MakeRand()
	{
		_RandValue() *= 0x000343fd;
		_RandValue() += 0x00269ec3;
		return (_RandValue() >> 16) & 0x7fff;
	}
};
