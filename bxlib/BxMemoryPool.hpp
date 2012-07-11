#pragma once
#include "BxCore.hpp"
#include "BxMemory.hpp"

//! \brief 사용자모듈을 위한 풀링도구
template<typename TYPE>
class BxMemoryPool
{
public:
	/*!
	\brief 템플릿단위 하나의 메모리공간 할당
	\return 할당된 메모리포인터, 초기화 없음
	\see Free
	*/
	TYPE* Make()
	{
		TYPE* Ptr = PoolFocus()->Make();
		if(!Ptr)
		{
			Unit* List = &PoolBegin();
			while(!(Ptr = List->Make()))
			{
				if(!List->Next)
				{
					List->Next = BxNew(Unit);
					BxAssert("BxMemoryPool<메모리가 부족합니다>", List->Next != null); // 메모리부족
				}
				List = List->Next;
			}
			PoolFocus() = List;
		}
		return Ptr;
	}

	/*!
	\brief 템플릿단위 하나의 메모리공간 할당 및 생성자호출
	\return 할당된 메모리포인터
	\see FreeClass
	*/
	TYPE* MakeClass()
	{
		return new(Make()) TYPE;
	}

	/*!
	\brief Make()로 생성된 메모리공간 반환
	\param Ptr : 반환시킬 메모리포인터
	\return 반환성공여부
	\see Make
	*/
	bool Free(TYPE* Ptr)
	{
		if(!Ptr) return false;
		if(!PoolFocus()->PreFree(Ptr))
		{
			Unit* List = &PoolBegin();
			while(!List->PreFree(Ptr))
			{
				if(!List->Next)
					return false;
				List = List->Next;
			}
			List->Free(Ptr);
			PoolFocus() = List;
		}
		else
		{
			PoolFocus()->Free(Ptr);
		}
		return true;
	}

	/*!
	\brief MakeClass()로 생성된 메모리공간 반환 및 소멸자호출
	\param Ptr : 반환시킬 메모리포인터
	\return 반환성공여부
	\see MakeClass
	*/
	bool FreeClass(TYPE* Ptr)
	{
		if(!Ptr) return false;
		if(!PoolFocus()->PreFree(Ptr))
		{
			Unit* List = &PoolBegin();
			while(!List->PreFree(Ptr))
			{
				if(!List->Next)
					return false;
				List = List->Next;
			}
			Ptr->~TYPE();
			List->Free(Ptr);
			PoolFocus() = List;
		}
		else
		{
			Ptr->~TYPE();
			PoolFocus()->Free(Ptr);
		}
		return true;
	}

private:
	class Unit
	{
	public:
		enum {MAX = 256};
		byte Data[MAX][sizeof(TYPE)];
		TYPE* Focus;
		int Count;
		Unit* Next;
		Unit()
		{
			for(int i = 0; i < MAX - 1; ++i) // 마지막은 링크기록 불필요
				Data[i][0] = i + 1;
			Focus = (TYPE*) &Data[0][0];
			Count = 0;
			Next = null;
		}
		virtual ~Unit()
		{
			Unit* CurNode = null;
			Unit* NextNode = Next;
			while(CurNode = NextNode)
			{
				NextNode = CurNode->Next;
				CurNode->Next = null;
				BxDelete(CurNode);
			}
		}
		inline TYPE* Make()
		{
			TYPE* Ptr = Focus;
			if(Count < MAX)
				Focus = (++Count == MAX)? null : ((TYPE*) &Data[0][0]) + *((byte*) Focus);
			return Ptr;
		}
		inline bool PreFree(TYPE* Ptr)
		{
			BxAssert("BxMemoryPool", Ptr != null); // null포인터
			if(Ptr < ((TYPE*) &Data[0][0]) || ((TYPE*) &Data[0][0]) + MAX <= Ptr)
				return false;
			return true;
		}
		inline void Free(TYPE* Ptr)
		{
			BxAssert("BxMemoryPool", Focus != Ptr); // 연속된 중복삭제
			BxAssert("BxMemoryPool", 0 < Count); // 할당된 메모리가 없음
			BxAssert("BxMemoryPool", ((((byte*) Ptr) - &Data[0][0]) % sizeof(TYPE)) == 0); // 부정확한 메모리위치
			if(Count-- < MAX)
			{
				BxAssert("BxMemoryPool", Focus != null); // 포커스정보 유효성보장-1
				BxAssert("BxMemoryPool", ((TYPE*) &Data[0][0]) <= Focus && Focus < ((TYPE*) &Data[0][0]) + MAX); // 포커스정보 유효성보장-2
				BxAssert("BxMemoryPool", ((((byte*) Focus) - &Data[0][0]) % sizeof(TYPE)) == 0); // 포커스정보 유효성보장-3
				*((byte*) Ptr) = (((byte*) Focus) - &Data[0][0]) / sizeof(TYPE);
			}
			Focus = Ptr;
		}
	};

	inline Unit& PoolBegin()
	{
		global_data Unit Begin;
		return Begin;
	}

	inline Unit*& PoolFocus()
	{
		global_data Unit* Focus = &PoolBegin();
		return Focus;
	}
};
