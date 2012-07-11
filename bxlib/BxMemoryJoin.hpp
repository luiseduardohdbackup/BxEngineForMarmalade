#pragma once
#include "BxCore.hpp"
#include "BxMemory.hpp"

//! \brief 정적인스턴스들의 일괄소멸을 위한 도구
template<memoryjoin MEMORYJOIN_ID>
class BxMemoryJoin
{
public:
	template<typename TYPE>
	global_func inline TYPE* Make(const uint count)
	{
		return List::Add(BxNew_Array(TYPE, count));
	}

	template<typename TYPE>
	global_func inline void Free(TYPE*& ptr)
	{
		List::Sub(ptr);
		ptr = null;
	}

	global_func inline void Release()
	{
		List::Release();
	}

	template<typename TYPE, typename PARAM1>
	global_func inline TYPE* Make(const uint count, PARAM1 value1)
	{return List::Add(BxNew_ArrayParam(TYPE, count, value1));}
	template<typename TYPE, typename PARAM1, typename PARAM2>
	global_func inline TYPE* Make(const uint count, PARAM1 value1, PARAM2 value2)
	{return List::Add(BxNew_ArrayParam(TYPE, count, value1, value2));}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3>
	global_func inline TYPE* Make(const uint count, PARAM1 value1, PARAM2 value2, PARAM3 value3)
	{return List::Add(BxNew_ArrayParam(TYPE, count, value1, value2, value3));}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	global_func inline TYPE* Make(const uint count, PARAM1 value1, PARAM2 value2, PARAM3 value3, PARAM4 value4)
	{return List::Add(BxNew_ArrayParam(TYPE, count, value1, value2, value3, value4));}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4, typename PARAM5>
	global_func inline TYPE* Make(const uint count, PARAM1 value1, PARAM2 value2, PARAM3 value3, PARAM4 value4, PARAM4 value5)
	{return List::Add(BxNew_ArrayParam(TYPE, count, value1, value2, value3, value4, value5));}

private:
	class Node
	{
	public:
		Node() {}
		virtual ~Node() {}
	};
	template<typename TYPE>
	class NodeData : public Node
	{
		void* Datas;
	public:
		NodeData(TYPE* datas)
		{
			Datas = datas;
		}
		virtual ~NodeData()
		{
			BxDelete_ArrayByType(TYPE, Datas);
		}
	};
	class List
	{
		Node* Unit;
		List* Next;
		global_func List* Head() {global_data List Begin; return &Begin;}
	private:
		List(Node* unit = null)
		{
			Unit = unit;
			Next = null;
		}
		~List()
		{
			delete Unit;
		}
		void __Release__()
		{
			List* CurNode = Next;
			while(CurNode)
			{
				List* DelList = CurNode;
				CurNode = CurNode->Next;
				DelList->Next = null;
				delete DelList;
			}
		}
	public:
		global_func void Release()
		{
			Head()->__Release__();
		}
		template<typename TYPE>
		global_func TYPE* Add(TYPE* datas)
		{
			List* NewList = new List(new NodeData<TYPE>(datas));
			NewList->Next = Head()->Next;
			Head()->Next = NewList;
			return datas;
		}
		template<typename TYPE>
		global_func void Sub(TYPE* datas)
		{
			List* CurNode = Head();
			while(CurNode->Next)
			{
				if(((NodeData<TYPE>*) CurNode->Next->Unit)->Datas == datas)
				{
					List* DelList = CurNode->Next;
					CurNode->Next = DelList->Next;
					DelList->Next = null;
					delete DelList;
					break;
				}
				CurNode = CurNode->Next;
			}
		}
	};
};
