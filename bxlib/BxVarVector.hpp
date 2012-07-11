#pragma once
#include "BxCore.hpp"

#ifndef END
#define END   ((int) 0xD0000000)
#endif
#ifndef LAST
#define LAST  ((int) 0xD0000000 + 1)
#endif
#ifndef FINAL
#define FINAL ((int) 0xDFFFFFFF)
#endif

//! \brief vector식 가변배열 제공(BxVar와 동일인터페이스)
template<typename TYPE, int UNITSIZE>
class BxVarVector
{
public:
	enum Parameter {AutoCreate = null, NoNeed = null};
	enum Type {LengthOfRange, LengthOfCount};

	// 데이터할당
	inline TYPE& operator[](int index)
	{
		CalcIndex(index);
		if(!Unit[index])
		{
			Unit[index] = new TYPE;
			if(Size < index + 1) Size = index + 1;
			++Count;
		}
		return *Unit[index];
	}

	// 데이터접근
	inline TYPE* Access(int index)
	{
		CalcIndex(index);
		return Unit[index];
	}

	// 데이터길이
	inline int Length(Type type = LengthOfRange)
	{
		if(type == LengthOfRange)
			return Size;
		return Count;
	}

	// 전체초기화
	void Reset(const bool doRemove = true)
	{
		for(int i = 0; i < Size; ++i)
		{
			if(doRemove) delete Unit[i];
			Unit[i] = null;
		}
		Size = 0;
		Count = 0;
	}

	// 리스트삽입
	inline TYPE& Insert(int index, TYPE* newData = (TYPE*) AutoCreate)
	{
		CalcIndex(index);
		if(index < Size)
		{
			ValidIndex(Size);
			BxCore::Util::MemMove(&Unit[index + 1], &Unit[index], sizeof(TYPE*) * (Size - index));
		}
		Unit[index] = (newData)? newData : new TYPE;
		if(Size < index + 1) Size = index + 1;
		else ++Size;
		++Count;
		return *Unit[index];
	}

	// 리스트삭제
	void Delete(int index, TYPE** oldData = (TYPE**) NoNeed)
	{
		CalcIndex(index);
		if(index < Size)
		{
			--Size;
			if(Unit[index]) --Count;
		}
		if(oldData) *oldData = Unit[index];
		else delete Unit[index];
		if(0 < Size - index)
			BxCore::Util::MemMove(&Unit[index], &Unit[index + 1], sizeof(TYPE*) * (Size - index));
	}

	// 데이터수정
	bool ModifyData(int index, TYPE* newData = (TYPE*) AutoCreate, TYPE** oldData = (TYPE**) NoNeed)
	{
		BxAssert("BxVarVector", false);
		return true;
	}

	// 데이터지움
	bool RemoveData(int index, TYPE** oldData = (TYPE**) NoNeed)
	{
		BxAssert("BxVarVector", false);
		return false;
	}

	// 데이터교체
	inline void SwapData(int index1, int index2)
	{
		BxAssert("BxVarVector", false);
	}

	// 데이터직렬화
	const byte* GetBytes()
	{
		BxAssert("BxVarVector", false);
		return (const byte*) null;
	}

	// 데이터직렬화 해제
	inline void ReleaseBytes()
	{
	}

	// 데이터복제
	BxVarVector& operator=(BxVarVector& RHS)
	{
		Reset();
		for(int i = 0; i < RHS.Length(); ++i)
		{
			TYPE* Data = RHS.Access(i);
			if(Data) operator[](i) = *Data;
		}
		Size = RHS.Length();
		return *this;
	}

	// 생성자
	BxVarVector()
	{
		UnitSize = UNITSIZE;
		Unit = (TYPE**) BxAlloc(sizeof(TYPE*) * UNITSIZE);
		BxCore::Util::MemSet(Unit, 0, sizeof(TYPE*) * UNITSIZE);
		Size = 0;
		Count = 0;
	}
	// 생성자
	BxVarVector(BxVarVector& RHS)
	{
		UnitSize = UNITSIZE;
		Unit = (TYPE**) BxAlloc(sizeof(TYPE*) * UNITSIZE);
		BxCore::Util::MemSet(Unit, 0, sizeof(TYPE*) * UNITSIZE);
		Size = 0;
		Count = 0;
		operator=(RHS);
	}
	// 소멸자
	virtual ~BxVarVector()
	{
		ReleaseBytes();
		Reset();
		BxFree(Unit);
	}

private:
	int UnitSize;
	TYPE** Unit;
	int Size;
	int Count;

	inline void CalcIndex(int& index)
	{
		if((index & 0xE0000000) == 0xC0000000)
		{
			if(index == FINAL) index = (0 < Size)? Size - 1 : 0;
			else index = index - END + Size - 1;
		}
		BxAssert("BxVarVector", 0 <= index);
		ValidIndex(index);
	}
	void ValidIndex(const int index)
	{
		if(index < UnitSize) return;
		const int NewUnitSize = BxUtilGlobal::Max(UnitSize + UNITSIZE, (index + 1 + UNITSIZE - 1) / UNITSIZE * UNITSIZE);
		TYPE** NewUnit = (TYPE**) BxAlloc(sizeof(TYPE*) * NewUnitSize);
		BxCore::Util::MemMove(NewUnit, Unit, sizeof(TYPE*) * UnitSize);
		BxCore::Util::MemSet(NewUnit + UnitSize, 0, sizeof(TYPE*) * (NewUnitSize - UnitSize));
		UnitSize = NewUnitSize;
		BxFree(Unit);
		Unit = NewUnit;
	}
};
