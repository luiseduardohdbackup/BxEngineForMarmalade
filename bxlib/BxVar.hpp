#pragma once
#include "BxMemory.hpp"

#ifndef END
#define END   ((int) 0xD0000000)
#endif
#ifndef LAST
#define LAST  ((int) 0xD0000000 + 1)
#endif
#ifndef FINAL
#define FINAL ((int) 0xDFFFFFFF)
#endif

//! \brief 가변배열 제공
template<typename TYPE>
class BxVar
{
public:
	enum Parameter {AutoCreate = null, NoNeed = null};
	enum Type {LengthOfRange, LengthOfCount};

	// 데이터할당
	inline TYPE& operator[](int index)
	{
		CalcIndex(index);
		VarLeaf* Leaf = (VarLeaf*) Root.GetLeaf(index);
		if(!Leaf)
		{
			BytesUpdated = false;
			return AddFirstData(index, new TYPE);
		}
		VarList* List = Leaf->GetList(index);
		if(!List)
		{
			BytesUpdated = false;
			return Leaf->AddList(index, new TYPE, VarLeaf::NoRanging);
		}
		return *List->Data;
	}

	// 데이터접근
	inline TYPE* Access(int index)
	{
		CalcIndex(index);
		VarLeaf* Leaf = (VarLeaf*) Root.GetLeaf(index);
		if(!Leaf) return null;
		VarList* List = Leaf->GetList(index);
		if(!List) return null;
		return List->Data;
	}

	// 데이터길이
	inline int Length(Type type = LengthOfRange)
	{
		if(type == LengthOfRange)
			return Root.LeftRange;
		return GetCount(&Root);
	}

	// 전체초기화
	inline void Reset(const bool doRemove = true)
	{
		Root.LeftRange = 0;
		VarList::DoRemoveData() = doRemove;
		VarNode::DeleteNode(Root.Child[0]);
		Root.Child[0] = null;
		BytesUpdated = false;
	}

	// 리스트삽입
	inline TYPE& Insert(int index, TYPE* newData = (TYPE*) AutoCreate)
	{
		CalcIndex(index);
		VarLeaf* Leaf = (VarLeaf*) Root.GetLeaf(index);
		BytesUpdated = false;
		if(!Leaf) return AddFirstData(index, (newData)? newData : new TYPE);
		return Leaf->AddList(index, (newData)? newData : new TYPE, VarLeaf::DoRanging);
	}

	// 리스트삭제
	void Delete(int index, TYPE** oldData = (TYPE**) NoNeed)
	{
		CalcIndex(index);
		if(Root.LeftRange <= index) return;
		VarLeaf* Leaf = (VarLeaf*) Root.GetLeaf(index);
		if(!Leaf)
		{
			--Root.LeftRange;
			return;
		}
		VarList* List = Leaf->GetList(index);
		if(List)
		{
			if(oldData) *oldData = List->Data;
			else delete List->Data;
			List->Data = null;
			if(Leaf->SubList(List, VarLeaf::DoRanging) == VarLeaf::DeleteMe)
				VarLeafPool().Free(Leaf->Quit());
		}
		else Leaf->SubBlankRange(index);
		BytesUpdated = false;
	}

	// 데이터수정
	bool ModifyData(int index, TYPE* newData = (TYPE*) AutoCreate, TYPE** oldData = (TYPE**) NoNeed)
	{
		CalcIndex(index);
		if(Root.LeftRange <= index) return false;
		VarLeaf* Leaf = (VarLeaf*) Root.GetLeaf(index);
		if(!Leaf) return false;
		VarList* List = Leaf->GetList(index);
		if(List)
		{
			if(oldData) *oldData = List->Data;
			else delete List->Data;
			List->Data = (newData)? newData : new TYPE;
		}
		else Leaf->AddList(index, (newData)? newData : new TYPE, VarLeaf::NoRanging);
		BytesUpdated = false;
		return true;
	}

	// 데이터지움
	bool RemoveData(int index, TYPE** oldData = (TYPE**) NoNeed)
	{
		CalcIndex(index);
		if(Root.LeftRange <= index) return false;
		VarLeaf* Leaf = (VarLeaf*) Root.GetLeaf(index);
		if(!Leaf) return false;
		VarList* List = Leaf->GetList(index);
		if(List)
		{
			if(oldData) *oldData = List->Data;
			else delete List->Data;
			List->Data = null;
			if(Leaf->SubList(List, VarLeaf::NoRanging) == VarLeaf::DeleteMe)
				VarLeafPool().Free(Leaf->Quit());
			BytesUpdated = false;
			return true;
		}
		return false;
	}

	// 데이터교체
	inline void SwapData(int index1, int index2)
	{
		TYPE* Data1 = Access(index1);
		TYPE* Data2 = Access(index2);
		TYPE* NoDelete = null;
		if(!Data1 && !Data2) return;
		if(Data1) ModifyData(index2, Data1, &NoDelete);
		else RemoveData(index2, &NoDelete);
		if(Data2) ModifyData(index1, Data2, &NoDelete);
		else RemoveData(index1, &NoDelete);
		BytesUpdated = false;
	}

	// 데이터직렬화
	const byte* GetBytes()
	{
		if(!BytesUpdated)
		{
			ReleaseBytes();
			if(Root.LeftRange)
			{
				BytesData = new TYPE[Root.LeftRange];
				for(int i = 0; i < Root.LeftRange; ++i)
				{
					TYPE* Data = Access(i);
					if(Data) BxCore::Util::MemMove(&BytesData[i], Data, sizeof(TYPE));
				}
			}
			BytesUpdated = true;
		}
		return (const byte*) BytesData;
	}

	// 데이터직렬화 해제
	inline void ReleaseBytes()
	{
		delete[] BytesData;
		BytesData = null;
		BytesUpdated = false;
	}

	// 데이터복제
	BxVar& operator=(BxVar& RHS)
	{
		Reset();
		for(int i = 0; i < RHS.Length(); ++i)
		{
			TYPE* Data = RHS.Access(i);
			if(Data) operator[](i) = *Data;
		}
		Root.LeftRange = RHS.Length();
		return *this;
	}

	// 생성자
	BxVar() : Root(null), BytesData(null), BytesUpdated(true) {}
	// 생성자
	BxVar(BxVar& RHS) : Root(null), BytesData(null), BytesUpdated(true) {operator=(RHS);}
	// 소멸자
	virtual ~BxVar() {ReleaseBytes(); VarList::DoRemoveData() = true;}

private:
	class VarList;
	class VarNode;
	class VarPath;
	class VarLeaf;
	global_func inline BxMemoryPool<VarList>& VarListPool() {global_data BxMemoryPool<VarList> Pool; return Pool;}
	global_func inline BxMemoryPool<VarPath>& VarPathPool() {global_data BxMemoryPool<VarPath> Pool; return Pool;}
	global_func inline BxMemoryPool<VarLeaf>& VarLeafPool() {global_data BxMemoryPool<VarLeaf> Pool; return Pool;}
	TYPE* BytesData;
	bool BytesUpdated;

	inline void CalcIndex(int& index)
	{
		if((index & 0xE0000000) == 0xC0000000)
		{
			if(index == FINAL) index = (0 < Root.LeftRange)? Root.LeftRange - 1 : 0;
			else index = index - END + Root.LeftRange - 1;
		}
		BxAssert("BxVar", 0 <= index);
	}

	TYPE& AddFirstData(const int index, TYPE* newData)
	{
		VarList* NewList = VarListPool().Make()->Init(index, newData);
		VarLeaf* NewLeaf = VarLeafPool().Make()->Init(&Root, 1, NewList);
		if(Root.LeftRange < index + 1)
			Root.LeftRange = index + 1;
		Root.Child[0] = NewLeaf;
		return *newData;
	}

	int GetCount(VarNode* node)
	{
		if(node->Type == VarNode::PATH)
		{
			VarPath* Path = (VarPath*) node;
			return ((Path->Child[0])? GetCount(Path->Child[0]) : 0) + ((Path->Child[1])? GetCount(Path->Child[1]) : 0);
		}
		VarLeaf* Leaf = (VarLeaf*) node;
		return Leaf->Count;
	}

	class VarList
	{
	public:
		int Range; // 범위
		TYPE* Data; // 데이터
		VarList* Next; // 다음
		VarList(const int range, TYPE* data) {Init(range, data);}
		~VarList() {Quit();}
		inline VarList* Init(const int range, TYPE* data) {Range = range; Data = data; Next = null; return this;}
		inline VarList* Quit() {if(DoRemoveData()) delete Data; if(Next) VarListPool().Free(Next->Quit()); return this;}
		global_func inline bool& DoRemoveData() {global_data bool _DoRemove = true; return _DoRemove;}
	};

	class VarNode
	{
	public:
		enum NodeType {PATH, LEAF};
		NodeType Type; // 타입
		VarPath* Parent; // 부모

		global_func void DeleteNode(VarNode* node)
		{
			if(!node) return;
			if(node->Type == PATH)
				VarPathPool().Free(((VarPath*) node)->Quit());
			else VarLeafPool().Free(((VarLeaf*) node)->Quit());
		}
	};

	class VarPath : public VarNode
	{
	public:
		int LeftRange; // 좌선범위
		VarNode* Child[2]; // 좌선/우선
		VarPath(VarPath* parent) {Init(parent);}
		~VarPath() {Quit();}
		inline VarPath* Init(VarPath* parent) {VarNode::Type = VarNode::PATH; VarNode::Parent = parent; LeftRange = 0; Child[0] = null; Child[1] = null; return this;}
		inline VarPath* Quit() {VarNode::DeleteNode(Child[0]); VarNode::DeleteNode(Child[1]); return this;}

		VarNode* GetLeaf(int& index)
		{
			VarPath* Path = (VarPath*) Child[0];
			if(Path)
			while(Path->Type == VarNode::PATH)
			{
				if(Path->LeftRange <= index)
				{
					index -= Path->LeftRange;
					Path = (VarPath*) Path->Child[1];
				}
				else Path = (VarPath*) Path->Child[0];
			}
			return Path;
		}

		VarNode* GetLastLeafByRight(const int adjust)
		{
			VarPath* Path = (VarPath*) Child[1];
			if(Path->Type == VarNode::PATH)
			{
				Path->LeftRange += adjust;
				while((Path = (VarPath*) Path->Child[0])->Type == VarNode::PATH);
			}
			return Path;
		}
	} Root;

	class VarLeaf : public VarNode
	{
	public:
		enum Parameter {DoRanging, NoRanging};
		enum Action {NoAction, DeleteMe};
		int Count; // 수량
		VarList* List; // 데이터리스트
		VarLeaf(VarPath* parent, int count, VarList* list) {Init(parent, count, list);}
		~VarLeaf() {Quit();}
		inline VarLeaf* Init(VarPath* parent, int count, VarList* list) {VarNode::Type = VarNode::LEAF; VarNode::Parent = parent; Count = count; List = list; return this;}
		inline VarLeaf* Quit() {if(List) VarListPool().Free(List->Quit()); return this;}

		VarList* GetList(int index)
		{
			VarList* NextList = List; // 항상 존재
			do
			{
				index -= NextList->Range;
				if(index == 0) return NextList;
			}
			while(NextList = NextList->Next);
			return null;
		}

		void SubBlankRange(int index)
		{
			VarList* NextList = List; // 항상 존재
			do
			{
				if(index < NextList->Range)
				{
					--NextList->Range;
					Ranging(-1);
					return;
				}
				index -= NextList->Range;
			}
			while(NextList = NextList->Next);
			Ranging(-1);
		}

		TYPE& AddList(const int index, TYPE* data, const Parameter option)
		{
			VarList* NewList = VarListPool().Make()->Init(index, data);
			VarList* PrevList = null;
			VarList* NextList = List; // 항상 존재
			do
			{
				// 리스트삽입-시작/중간
				if(NewList->Range <= NextList->Range)
				{
					// 영역배분
					if(option == DoRanging)
						NextList->Range -= NewList->Range - 1, Ranging(+1);
					else NextList->Range -= NewList->Range;
					// 삽입수순
					((PrevList)? PrevList->Next : List) = NewList;
					NewList->Next = NextList;
					break;
				}
				// 패스
				NewList->Range -= NextList->Range;
			}
			while(NextList = (PrevList = NextList)->Next);
			// 리스트삽입-끝
			if(!NextList)
			{
				const int OldRange = GetRange();
				if(OldRange <= index)
					Ranging(index + 1 - OldRange);
				PrevList->Next = NewList;
			}
			// 리카운트
			if(++Count == 4) AddPath();
			return *data;
		}

		Action SubList(VarList* list, const Parameter option)
		{
			VarList* PrevList = null;
			VarList* NextList = List; // 항상 존재
			while(true)
			{
				if(NextList == list)
				{
					// 영역통합
					if(option == DoRanging)
					{
						if(NextList->Next)
							NextList->Next->Range += NextList->Range - 1;
						Ranging(-1);
					}
					else if(NextList->Next)
						NextList->Next->Range += NextList->Range;
					// 제거수순
					((PrevList)? PrevList->Next : List) = NextList->Next;
					NextList->Next = null;
					VarListPool().Free(NextList->Quit());
					// 리카운트
					if(--Count == 0) return SubPath();
					return NoAction;
				}
				NextList = (PrevList = NextList)->Next;
			}
			return NoAction;
		}

	protected:
		inline void AddPath()
		{
			// 패스생성 및 등록
			VarPath* NewPath = VarPathPool().Make()->Init(VarNode::Parent);
			((VarNode::Parent->Child[0] == this)? VarNode::Parent->Child[0] : VarNode::Parent->Child[1]) = NewPath;
			// 패스구성
			NewPath->LeftRange = 1 + List->Range + List->Next->Range, --List->Next->Next->Range;
			NewPath->Child[0] = this, VarNode::Parent = NewPath, Count = 2;
			NewPath->Child[1] = VarLeafPool().Make()->Init(NewPath, 2, List->Next->Next);
			// 리스트간 관계정리
			List->Next->Next = null;
		}

		inline Action SubPath()
		{
			VarPath* Grand = (VarPath*) VarNode::Parent->Parent;
			// 패스
			if(Grand)
			{
				const int IsRight = (VarNode::Parent->Child[1] == this);
				const int IsGrandRight = (Grand->Child[1] == VarNode::Parent);
				// 좌선이라면 남은 영역길이를 우선의 자식중 제일 좌선의 첫번째 영역길이에 더함
				if(!IsRight)
				{
					VarLeaf* LastLeaf = (VarLeaf*) VarNode::Parent->GetLastLeafByRight(VarNode::Parent->LeftRange);
					LastLeaf->List->Range += VarNode::Parent->LeftRange;
				}
				// 형제를 그랜드의 자식으로 등록
				Grand->Child[IsGrandRight] = VarNode::Parent->Child[!IsRight];
				Grand->Child[IsGrandRight]->Parent = Grand;
				// 부모삭제
				VarNode::Parent->Child[0] = VarNode::Parent->Child[1] = null;
				VarNode::DeleteNode(VarNode::Parent);
			}
			// 루트
			else VarNode::Parent->Child[0] = null;
			return DeleteMe;
		}

		int GetRange()
		{
			int SumLeftRange = 0;
			VarPath* Path = (VarPath*) this;
			VarPath* PathParent = VarNode::Parent; // 항상 존재
			while(true)
			{
				if(PathParent->Child[0] == Path)
					return PathParent->LeftRange - SumLeftRange;
				else SumLeftRange += PathParent->LeftRange;
				PathParent = (Path = PathParent)->Parent;
			}
			return 0;
		}

		void Ranging(const int adjust)
		{
			VarPath* Path = (VarPath*) this;
			VarPath* PathParent = VarNode::Parent; // 항상 존재
			do
			{
				if(PathParent->Child[0] == Path)
					PathParent->LeftRange += adjust;
			}
			while(PathParent = (Path = PathParent)->Parent);
		}
	};
};
