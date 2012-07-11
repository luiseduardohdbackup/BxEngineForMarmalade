#pragma once
#include "BxCore.hpp"

//! \brief map�� �����迭 ����
template<typename TYPE>
class BxVarMap
{
public:
	// ����Key�� �������Ҵ�
	TYPE& operator[](uint key)
	{
		BxVarMap* CurNode = this;
		while(key)
		{
			const uint index = (key & 0xF);
			CurNode->ValidChild();
			CurNode = (CurNode->Child[index])? CurNode->Child[index]
				: (CurNode->Child[index] = new BxVarMap);
			key >>= 4;
		}
		return (CurNode->Data)? *CurNode->Data : *(CurNode->Data = new TYPE);
	}

	// ����Key�� ����������
	TYPE* Access(uint key) const
	{
		const BxVarMap* CurNode = this;
		while(key)
		{
			const uint index = (key & 0xF);
			if(!CurNode->Child || !CurNode->Child[index]) return null;
			CurNode = CurNode->Child[index];
			key >>= 4;
		}
		return CurNode->Data;
	}

	// ���ڿ�Key�� �������Ҵ�
	TYPE& operator[](string key)
	{
		BxVarMap* CurNode = this;
		while(*key)
		{
			const uint indexHi = ((*key & 0xF0) >> 4);
			CurNode->ValidChild();
			CurNode = (CurNode->Child[indexHi])? CurNode->Child[indexHi]
				: (CurNode->Child[indexHi] = new BxVarMap);
			const uint indexLo = (*(key++) & 0xF);
			if(!*key && !indexLo) break;
			CurNode->ValidChild();
			CurNode = (CurNode->Child[indexLo])? CurNode->Child[indexLo]
				: (CurNode->Child[indexLo] = new BxVarMap);
		}
		return (CurNode->Data)? *CurNode->Data : *(CurNode->Data = new TYPE);
	}

	// ���ڿ�Key�� ����������
	TYPE* Access(string key) const
	{
		const BxVarMap* CurNode = this;
		while(*key)
		{
			const uint indexHi = ((*key & 0xF0) >> 4);
			if(!CurNode->Child || !CurNode->Child[indexHi]) return null;
			CurNode = CurNode->Child[indexHi];
			const uint indexLo = (*(key++) & 0xF);
			if(!*key && !indexLo) break;
			if(!CurNode->Child || !CurNode->Child[indexLo]) return null;
			CurNode = CurNode->Child[indexLo];
		}
		return CurNode->Data;
	}

	// ��ü�ʱ�ȭ
	void Reset()
	{
		delete Data;
		if(Child)
		{
			for(uint i = 0; i < 16; ++i)
				delete Child[i];
			delete[] Child;
		}
		Data = null;
		Child = null;
	}

	// ������/�Ҹ���
	BxVarMap()
	{
		Data = null;
		Child = null;
	}
	~BxVarMap()
	{
		Reset();
	}

private:
	TYPE* Data;
	BxVarMap** Child;

	inline void ValidChild()
	{
		if(Child) return;
		Child = new BxVarMap*[16];
		BxCore::Util::MemSet(Child, null, sizeof(BxVarMap*) * 16);
	}
};
