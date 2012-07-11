#pragma once
#include "BxCore.hpp"
#include "BxMemory.hpp"

//! \brief ����ڸ���� ���� Ǯ������
template<typename TYPE>
class BxMemoryPool
{
public:
	/*!
	\brief ���ø����� �ϳ��� �޸𸮰��� �Ҵ�
	\return �Ҵ�� �޸�������, �ʱ�ȭ ����
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
					BxAssert("BxMemoryPool<�޸𸮰� �����մϴ�>", List->Next != null); // �޸𸮺���
				}
				List = List->Next;
			}
			PoolFocus() = List;
		}
		return Ptr;
	}

	/*!
	\brief ���ø����� �ϳ��� �޸𸮰��� �Ҵ� �� ������ȣ��
	\return �Ҵ�� �޸�������
	\see FreeClass
	*/
	TYPE* MakeClass()
	{
		return new(Make()) TYPE;
	}

	/*!
	\brief Make()�� ������ �޸𸮰��� ��ȯ
	\param Ptr : ��ȯ��ų �޸�������
	\return ��ȯ��������
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
	\brief MakeClass()�� ������ �޸𸮰��� ��ȯ �� �Ҹ���ȣ��
	\param Ptr : ��ȯ��ų �޸�������
	\return ��ȯ��������
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
			for(int i = 0; i < MAX - 1; ++i) // �������� ��ũ��� ���ʿ�
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
			BxAssert("BxMemoryPool", Ptr != null); // null������
			if(Ptr < ((TYPE*) &Data[0][0]) || ((TYPE*) &Data[0][0]) + MAX <= Ptr)
				return false;
			return true;
		}
		inline void Free(TYPE* Ptr)
		{
			BxAssert("BxMemoryPool", Focus != Ptr); // ���ӵ� �ߺ�����
			BxAssert("BxMemoryPool", 0 < Count); // �Ҵ�� �޸𸮰� ����
			BxAssert("BxMemoryPool", ((((byte*) Ptr) - &Data[0][0]) % sizeof(TYPE)) == 0); // ����Ȯ�� �޸���ġ
			if(Count-- < MAX)
			{
				BxAssert("BxMemoryPool", Focus != null); // ��Ŀ������ ��ȿ������-1
				BxAssert("BxMemoryPool", ((TYPE*) &Data[0][0]) <= Focus && Focus < ((TYPE*) &Data[0][0]) + MAX); // ��Ŀ������ ��ȿ������-2
				BxAssert("BxMemoryPool", ((((byte*) Focus) - &Data[0][0]) % sizeof(TYPE)) == 0); // ��Ŀ������ ��ȿ������-3
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
