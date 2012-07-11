#pragma once
#include "BxCore.hpp"

/*!
\defgroup BxNew
\brief #define BxNew(TYPE)
\brief �޸� �⺻�Ҵ�
\param TYPE : Ŭ����Ÿ��
\return Ŭ����Ÿ�� ������
*/
#define BxNew(TYPE)                        new(BxMemory::Alloc(sizeof(TYPE), __FILE__, __LINE__)) TYPE

/*!
\defgroup BxNew_Param
\brief #define BxNew_Param(TYPE, ...)
\brief �޸� �μ��Ҵ�
\param TYPE : Ŭ����Ÿ��
\param ... : ������ �μ�
\return Ŭ����Ÿ�� ������
*/
#define BxNew_Param(TYPE, ...)             new(BxMemory::Alloc(sizeof(TYPE), __FILE__, __LINE__)) TYPE(__VA_ARGS__)

/*!
\defgroup BxNew_Array
\brief #define BxNew_Array(TYPE, COUNT)
\brief �޸� �迭�Ҵ�
\param TYPE : Ŭ����Ÿ��
\param COUNT : ����
\return Ŭ����Ÿ�� �迭������
*/
#define BxNew_Array(TYPE, COUNT)           BxMemory::NewArray<TYPE>(COUNT, __FILE__, __LINE__)

/*!
\defgroup BxNew_ArrayParam
\brief #define BxNew_ArrayParam(TYPE, COUNT, ...)
\brief �޸� �迭�μ��Ҵ�
\param TYPE : Ŭ����Ÿ��
\param COUNT : ����
\param ... : ������ �μ�
\return Ŭ����Ÿ�� �迭������
*/
#define BxNew_ArrayParam(TYPE, COUNT, ...) BxMemory::NewArrayParam<TYPE>(COUNT, __VA_ARGS__, __FILE__, __LINE__)

/*!
\defgroup BxDelete
\brief #define BxDelete(PTR)
\brief �޸� �⺻�Ҹ�
\param PTR : �ش� Ŭ����Ÿ�� ������
*/
#define BxDelete(PTR)                      do{BxMemory::Delete(PTR, __FILE__, __LINE__); PTR = null;} while(false)

/*!
\defgroup BxDelete_ByType
\brief #define BxDelete_ByType(TYPE, PTR)
\brief �޸� ����ȯ�� �Ҹ�
\param TYPE : ����ȯ��ų Ŭ����Ÿ��
\param PTR : �ش� ������
*/
#define BxDelete_ByType(TYPE, PTR)         do{BxMemory::Delete((TYPE*) PTR, __FILE__, __LINE__); PTR = null;} while(false)

/*!
\defgroup BxDelete_Array
\brief #define BxDelete_Array(PTR)
\brief �޸� �迭�Ҹ�
\param PTR : �ش� Ŭ����Ÿ�� �迭������
*/
#define BxDelete_Array(PTR)                do{BxMemory::DeleteArray(PTR, __FILE__, __LINE__); PTR = null;} while(false)

/*!
\defgroup BxDelete_ArrayByType
\brief #define BxDelete_ArrayByType(TYPE, PTR)
\brief �޸� ����ȯ�� �迭�Ҹ�
\param TYPE : ����ȯ��ų Ŭ����Ÿ��
\param PTR : �ش� �迭������
*/
#define BxDelete_ArrayByType(TYPE, PTR)    do{BxMemory::DeleteArray((TYPE*) PTR, __FILE__, __LINE__); PTR = null;} while(false)

/*!
\defgroup BxAlloc
\brief #define BxAlloc(LENGTH)
\brief �޸𸮴��� �Ҵ�
\param LENGTH : ����Ʈ����
\return �޸𸮴��� ������
*/
#define BxAlloc(LENGTH)                    BxMemory::Alloc(LENGTH, __FILE__, __LINE__)

/*!
\defgroup BxFree
\brief #define BxFree(PTR)
\brief �޸𸮴��� �Ҹ�
\param PTR : �޸𸮴��� ������
*/
#define BxFree(PTR)                        BxMemory::Free(PTR, __FILE__, __LINE__)

/// @cond SECTION_NAME
#ifndef IW_STD_NEW_H
#define IW_STD_NEW_H
inline void* operator new(uint, void* ptr) { return ptr;}
inline void* operator new[](uint, void* ptr) { return ptr;}
inline void operator delete(void*, void*) {}
inline void operator delete[](void*, void*) {}
	#if defined _MSC_VER && _MSC_VER <= 1200
	void* operator new[](size_t);
	void operator delete[](void*);
	#endif
#endif
/// @endcond

/// @cond SECTION_NAME
typedef class BxMemory3
{
public:
	template<typename TYPE>
	global_func TYPE* NewArray(const uint count, string file, const int line)
	{
		uint* DataArray = (uint*) Alloc(sizeof(int) + sizeof(TYPE) * count, file, line);
		DataArray[0] = count;
		TYPE* ClassArray = (TYPE*) &DataArray[1];
		for(uint i = 0; i < count; ++i)
			new(&ClassArray[i]) TYPE;
		return ClassArray;
	}

	template<typename TYPE>
	global_func void Delete(const TYPE* ptr, string file, const int line)
	{
		if(ptr == null) return;
		ptr->~TYPE();
		Free(ptr, file, line);
	}

	template<typename TYPE>
	global_func void DeleteArray(const TYPE* ptr, string file, const int line)
	{
		if(ptr == null) return;
		const uint* DataArray = &((const uint*) ptr)[-1];
		for(uint i = 0, count = DataArray[0]; i < count; ++i)
			ptr[i].~TYPE();
		Free(DataArray, file, line);
	}

	global_func inline void* Alloc(const uint length, string file, const int line)
	{
		void* Ptr = MakeMethod()(length);
		LogMethod()(0, file, line);
		return Ptr;
	}

	global_func inline void Free(const void* ptr, string file, const int line)
	{
		if(ptr == null) return;
		LogMethod()(1, file, line);
		FreeMethod()(ptr);
	}

	#define BXMEMORY_ARRAY_CORE(...) \
		uint* DataArray = (uint*) Alloc(sizeof(int) + sizeof(TYPE) * count, file, line); \
		DataArray[0] = count; \
		TYPE* ClassArray = (TYPE*) &DataArray[1]; \
		for(uint i = 0; i < count; ++i) \
			new(&ClassArray[i]) TYPE(__VA_ARGS__); \
		return ClassArray
	template<typename TYPE, typename PARAM1>
	global_func TYPE* NewArrayParam(const uint count, PARAM1 value1, string file, const int line)
	{BXMEMORY_ARRAY_CORE(value1);}
	template<typename TYPE, typename PARAM1, typename PARAM2>
	global_func TYPE* NewArrayParam(const uint count, PARAM1 value1, PARAM2 value2, string file, const int line)
	{BXMEMORY_ARRAY_CORE(value1, value2);}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3>
	global_func TYPE* NewArrayParam(const uint count, PARAM1 value1, PARAM2 value2, PARAM3 value3, string file, const int line)
	{BXMEMORY_ARRAY_CORE(value1, value2, value3);}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	global_func TYPE* NewArrayParam(const uint count, PARAM1 value1, PARAM2 value2, PARAM3 value3, PARAM4 value4, string file, const int line)
	{BXMEMORY_ARRAY_CORE(value1, value2, value3, value4);}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4, typename PARAM5>
	global_func TYPE* NewArrayParam(const uint count, PARAM1 value1, PARAM2 value2, PARAM3 value3, PARAM4 value4, PARAM4 value5, string file, const int line)
	{BXMEMORY_ARRAY_CORE(value1, value2, value3, value4, value5);}
	global_func inline bool DoMemLogger() {return (DLLHandle() != null);}

private:
	template<typename TYPE> global_func inline TYPE* GetModel() {global_data TYPE Type; return &Type;}
	global_func inline id_library& DLLHandle() {global_data id_library Handle = null; return Handle;}
	typedef int (*LogType)(int type, string file, int line);
	typedef void* (*MakeType)(uint size);
	typedef void (*FreeType)(const void* ptr);
	global_func inline LogType& LogMethod() {global_data LogType Log = BeginLog; return Log;}
	global_func inline MakeType& MakeMethod() {global_data MakeType Make = BeginMake; return Make;}
	global_func inline FreeType& FreeMethod() {global_data FreeType Free = BeginFree; return Free;}
	global_func int BeginLog(int type, string file, int line)
	{
		InitMemory();
		return LogMethod()(type, file, line);
	}
	global_func int NullLog(int type, string file, int line)
	{
		return 0;
	}
	global_func void* BeginMake(uint size)
	{
		InitMemory();
		return MakeMethod()(size);
	}
	global_func void BeginFree(const void* ptr)
	{
		BxAssert("BxMemory", false);
	}
	global_func void InitMemory()
	{
		global_data bool IsInit = false;
		if(IsInit) return;
		IsInit = true;
		#if defined(__BX_DEBUG) && !defined(__GNUC__) && !defined(__ARMCC_VERSION)
			DLLHandle() = BxCore::Library::Open("../bxdll/CodeLogger.dll");
			if(DLLHandle())
			{
				int MemSize = 0;
				BxCore::System::GetConfigInt("s3e", "MemSizeBx", &MemSize);
				id_memory Pool = BxCore::Util::Malloc(MemSize); // ������ ����
				typedef bool (*LoggerOnType)(string sysname, uint memorysizebykb, string projname, string filename);
				LoggerOnType LoggerOnMethod = (LoggerOnType) BxCore::Library::Link(DLLHandle(), "BxMemLoggerOn");
				if(LoggerOnMethod("BxMemoryFast", MemSize / 1024, "BxMemory", __FILE__))
				{
					LogMethod() = (LogType) BxCore::Library::Link(DLLHandle(), "BxLog");
					MakeMethod() = (MakeType) BxCore::Library::Link(DLLHandle(), "BxMake");
					FreeMethod() = (FreeType) BxCore::Library::Link(DLLHandle(), "BxFree");
					return;
				}
				else BxCore::Util::Free(Pool);
			}
		#endif
		if(DLLHandle() != null)
		{
			BxCore::Library::Close(DLLHandle());
			DLLHandle() = null;
		}
		LogMethod() = NullLog;
		MakeMethod() = MakePack;
		FreeMethod() = FreePack;
	}

// BxMemoryPack
private:
	#define BXMEMORY_MEMSET_ON					(0)
	// FLAG
	#define BXMEMORY_CHUNK_HEADER_USEDFLAG		(0x80000000)
	#define BXMEMORY_CHUNK_HEADER_PACKFLAG		(0x40000000)
	#define BXMEMORY_CHUNK_HEADER_SIZEAREA		(0x3FFFFFFF)
	#define BXMEMORY_PACK_HEADER_IDAREA			(0xFF000000)
	#define BXMEMORY_PACK_HEADER_COUNTAREA		(0x00FFF000)
	#define BXMEMORY_PACK_HEADER_FREEAREA		(0x00000FFF)
	#define BXMEMORY_UNITSIZE					(sizeof(MemUint))
	// CHUNK HEADER
	#define BXMEMORY_IS_USED(CHUNK_PTR)			(*(CHUNK_PTR) & BXMEMORY_CHUNK_HEADER_USEDFLAG)
	#define BXMEMORY_IS_PACK(CHUNK_PTR)			(*(CHUNK_PTR) & BXMEMORY_CHUNK_HEADER_PACKFLAG)
	#define BXMEMORY_GET_SIZE(CHUNK_PTR)		(*(CHUNK_PTR) & BXMEMORY_CHUNK_HEADER_SIZEAREA)
	#define BXMEMORY_SET_USED(CHUNK_PTR)		(*(CHUNK_PTR) |= BXMEMORY_CHUNK_HEADER_USEDFLAG)
	#define BXMEMORY_SET_PACK(CHUNK_PTR)		(*(CHUNK_PTR) |= BXMEMORY_CHUNK_HEADER_PACKFLAG)
	#define BXMEMORY_SET_FREE(CHUNK_PTR)		(*(CHUNK_PTR) &= ~BXMEMORY_CHUNK_HEADER_USEDFLAG)
	// PACK HEADER
	#define BXMEMORY_GET_PACK_ID(PACK_PTR)		((*(PACK_PTR) & BXMEMORY_PACK_HEADER_IDAREA) >> 24)
	#define BXMEMORY_GET_PACK_COUNT(PACK_PTR)	((*(PACK_PTR) & BXMEMORY_PACK_HEADER_COUNTAREA) >> 12)
	#define BXMEMORY_GET_PACK_FREE(PACK_PTR)	(*(PACK_PTR) & BXMEMORY_PACK_HEADER_FREEAREA)
	// ETC
	#define BXMEMORY_NEXT_CHUNK(CHUNK_PTR)		((CHUNK_PTR) + BXMEMORY_GET_SIZE(CHUNK_PTR))
	#define BXMEMORY_CHUNK_HEADER_SIZE			(1)
	#define BXMEMORY_PACK_HEADER_SIZE			(1)
	#define BXMEMORY_PACK_BITMASK_SIZE			(1)
	#define BXMEMORY_RECENT_COUNT				(4)

	typedef unsigned MemUint;
	/// @cond SECTION_NAME
	typedef struct
	{
		MemUint* Recent[BXMEMORY_RECENT_COUNT];
		unsigned NumPack;
		unsigned TotalCountUnit;
		unsigned TotalFreeUnit;
	} PackState;
	/// @endcond
	enum {PackMaximum = 256};
	enum {UnitSizeMinimum = 32};
	enum {UnitCountMaximum = 4095};
	// PACK
	global_func inline PackState& Pack(const int index) {global_data PackState _Pack[PackMaximum]; return _Pack[index];}
	global_func inline unsigned& NumChunk() {global_data unsigned _NumChunk = 0; return _NumChunk;}
	// POOL
	global_func inline MemUint*& Pool() {global_data MemUint* _Pool = null; return _Pool;}
	global_func inline unsigned& PoolLength() {global_data unsigned _PoolLength = 0; return _PoolLength;}
	// MIN/MAX
	template <class TYPE> global_func inline const TYPE& Min(const TYPE& A, const TYPE& B) {return (A < B)? A : B;}
	template <class TYPE> global_func inline const TYPE& Max(const TYPE& A, const TYPE& B) {return (A > B)? A : B;}

	// Ptr�� �����ϴ� ûũ �˻�
	global_func MemUint* FindChunk(MemUint* Ptr)
	{
		if(Ptr < Pool() || Pool() + PoolLength() <= Ptr)
			return null;
		MemUint* ChunkFocus = Pool();
		while(BXMEMORY_NEXT_CHUNK(ChunkFocus) <= Ptr)
			ChunkFocus = BXMEMORY_NEXT_CHUNK(ChunkFocus);
		return ChunkFocus;
	}

	// �ű�ûũ Ȯ��
	global_func MemUint* LockChunk(unsigned Size, unsigned AddCount)
	{
		const unsigned PackID = Size - 1;
		const unsigned PackCount = 1 + AddCount;
		const unsigned BitUnitSize = 8 * BXMEMORY_UNITSIZE;
		const unsigned BitMaskSize = BXMEMORY_PACK_BITMASK_SIZE + AddCount / (8 * BXMEMORY_UNITSIZE);
		const bool IsPack = (Size <= PackMaximum);
		unsigned NeedSize = BXMEMORY_CHUNK_HEADER_SIZE + ((!IsPack)? Size : Size * (1 + AddCount) + BXMEMORY_PACK_HEADER_SIZE + BitMaskSize);
		// �����˻�
		MemUint* ChunkFocus = Pool();
		const MemUint* PoolEnd = Pool() + PoolLength();
		unsigned SumFreeChunkSize = 0;
		unsigned SumFreeChunkCount = 0;
		while(BXMEMORY_IS_USED(ChunkFocus) || BXMEMORY_GET_SIZE(ChunkFocus) < NeedSize)
		{
			if(!BXMEMORY_IS_USED(ChunkFocus))
			{
				if(!SumFreeChunkSize || SumFreeChunkSize + BXMEMORY_GET_SIZE(ChunkFocus) < NeedSize)
				{
					SumFreeChunkSize += BXMEMORY_GET_SIZE(ChunkFocus);
					++SumFreeChunkCount;
				}
				// �����Ҵ��� ���� ����
				else
				{
					*(ChunkFocus - SumFreeChunkSize) = SumFreeChunkSize + BXMEMORY_GET_SIZE(ChunkFocus);
					ChunkFocus -= SumFreeChunkSize;
					break;
				}
			}
			else
			{
				// �˻��ӵ��� ���� ����
				if(1 < SumFreeChunkCount)
					*(ChunkFocus - SumFreeChunkSize) = SumFreeChunkSize;
				SumFreeChunkSize = 0;
				SumFreeChunkCount = 0;
			}
			ChunkFocus = BXMEMORY_NEXT_CHUNK(ChunkFocus);
			if(ChunkFocus == PoolEnd)
			{
				BxAssert("BxMemory<�޸𸮰� �����մϴ�>", false); // �޸𸮺���
				return null;
			}
		}
		// ��������
		if(BXMEMORY_CHUNK_HEADER_SIZE < BXMEMORY_GET_SIZE(ChunkFocus) - NeedSize)
			*(ChunkFocus + NeedSize) = BXMEMORY_GET_SIZE(ChunkFocus) - NeedSize;
		else NeedSize = BXMEMORY_GET_SIZE(ChunkFocus);
		*(ChunkFocus) = NeedSize;
		BXMEMORY_SET_USED(ChunkFocus);
		#if(BXMEMORY_MEMSET_ON == 1)
			BxCore::Util::MemSet(ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE, 0, BXMEMORY_BXMEMORY_UNITSIZE * (NeedSize - BXMEMORY_CHUNK_HEADER_SIZE));
		#endif
		// �ѱ���
		if(IsPack)
		{
			BXMEMORY_SET_PACK(ChunkFocus);
			MemUint* BitMaskPtr = ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE + BXMEMORY_PACK_HEADER_SIZE;
			*(ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE) = ((PackID << 24) & BXMEMORY_PACK_HEADER_IDAREA)
				| ((PackCount << 12) & BXMEMORY_PACK_HEADER_COUNTAREA)
				| (PackCount & BXMEMORY_PACK_HEADER_FREEAREA);
			BxCore::Util::MemSet(BitMaskPtr, 0xFF, BXMEMORY_UNITSIZE * BitMaskSize);
			for(unsigned Focus = 0; Focus < PackCount; ++Focus)
			{
				const unsigned FocusBit = 1 << (Focus % BitUnitSize);
				BitMaskPtr[Focus / BitUnitSize] &= ~FocusBit;
			}
		}
		// ûũ���� ó��
		++NumChunk();
		return ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE;
	}

	// �ش�ûũ ��ȯ
	global_func void UnlockChunk(MemUint* ChunkPtr)
	{
		// ��������
		if(BXMEMORY_IS_USED(ChunkPtr))
		{
			BXMEMORY_SET_FREE(ChunkPtr);
			// ûũ���� ó��
			--NumChunk();
		}
	}

	// �ش��� �˻�
	global_func MemUint* FindPack(unsigned PackID)
	{
		MemUint* ChunkFocus = Pool();
		const MemUint* PoolEnd = Pool() + PoolLength();
		while(ChunkFocus != PoolEnd)
		{
			if(BXMEMORY_IS_USED(ChunkFocus) && BXMEMORY_IS_PACK(ChunkFocus)
				&& BXMEMORY_GET_PACK_ID(ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE) == PackID
				&& 0 < BXMEMORY_GET_PACK_FREE(ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE))
				return ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE;
			ChunkFocus = BXMEMORY_NEXT_CHUNK(ChunkFocus);
		}
		return null;
	}

	// �ѳ� �ű�����Ʈ Ȯ��
	global_func MemUint* LockUnit(MemUint* PackPtr)
	{
		const unsigned PackID = BXMEMORY_GET_PACK_ID(PackPtr);
		const unsigned PackCount = BXMEMORY_GET_PACK_COUNT(PackPtr);
		const unsigned PackFree = BXMEMORY_GET_PACK_FREE(PackPtr);
		const unsigned BitUnitSize = 8 * BXMEMORY_UNITSIZE;
		const unsigned BitMaskSize = (PackCount + BitUnitSize - 1) / BitUnitSize;
		MemUint* BitMaskPtr = PackPtr + BXMEMORY_PACK_HEADER_SIZE;
		for(unsigned Focus32 = 0; Focus32 < BitMaskSize; ++Focus32)
		{
			if(BitMaskPtr[Focus32] == 0xFFFFFFFF) continue;
			for(unsigned Focus = Focus32 * BitUnitSize; Focus < PackCount; ++Focus)
			{
				const unsigned FocusBit = 1 << (Focus % BitUnitSize);
				if(BitMaskPtr[Focus / BitUnitSize] & FocusBit) continue;
				// ����ũ ó��
				BitMaskPtr[Focus / BitUnitSize] |= FocusBit;
				// ������ ó��
				*(PackPtr) = (*(PackPtr) & (BXMEMORY_PACK_HEADER_IDAREA | BXMEMORY_PACK_HEADER_COUNTAREA))
					| ((PackFree - 1) & BXMEMORY_PACK_HEADER_FREEAREA);
				// ����Ȳ ó��
				--Pack(PackID).TotalFreeUnit;
				return BitMaskPtr + BitMaskSize + (PackID + 1) * Focus;
			}
		}
		return null;
	}

	// �ѳ� �ش�����Ʈ ��ȯ
	global_func bool UnlockUnit(MemUint* PackPtr, MemUint* Ptr)
	{
		const unsigned PackID = BXMEMORY_GET_PACK_ID(PackPtr);
		const unsigned PackCount = BXMEMORY_GET_PACK_COUNT(PackPtr);
		const unsigned PackFree = BXMEMORY_GET_PACK_FREE(PackPtr);
		const unsigned BitUnitSize = 8 * BXMEMORY_UNITSIZE;
		const unsigned BitMaskSize = (PackCount + BitUnitSize - 1) / BitUnitSize;
		MemUint* BitMaskPtr = PackPtr + BXMEMORY_PACK_HEADER_SIZE;
		const unsigned Focus = (Ptr - (BitMaskPtr + BitMaskSize)) / (PackID + 1);
		const unsigned FocusBit = 1 << (Focus % BitUnitSize);
		// ��������
		if(BitMaskPtr[Focus / BitUnitSize] & FocusBit)
		{
			// ����ũ ó��
			BitMaskPtr[Focus / BitUnitSize] &= ~FocusBit;
			// ������ ó��
			*(PackPtr) = (*(PackPtr) & (BXMEMORY_PACK_HEADER_IDAREA | BXMEMORY_PACK_HEADER_COUNTAREA))
				| ((PackFree + 1) & BXMEMORY_PACK_HEADER_FREEAREA);
			// ����Ȳ ó��
			++Pack(PackID).TotalFreeUnit;
			// ��� ����Ʈ ��ȯ
			if(PackFree + 1 == PackCount)
				return true;
		}
		return false;
	}

	global_func void* MakePack(unsigned size)
	{
		// �ʱ�ȭ
		global_data bool IsInitial = false;
		if(!IsInitial)
		{
			IsInitial = true;
			int MemSize = 0;
			BxCore::System::GetConfigInt("s3e", "MemSizeBx", &MemSize);
			PoolLength() = MemSize / BXMEMORY_UNITSIZE;
			Pool() = (MemUint*) BxCore::Util::Malloc(BXMEMORY_UNITSIZE * PoolLength());
			BxCore::Util::MemSet(&Pack(0), 0, sizeof(PackState) * PackMaximum);
			BxCore::Util::MemSet(Pool(), 0, BXMEMORY_UNITSIZE * PoolLength());
			Pool()[0] = PoolLength();
		}
		const unsigned Size = (size + BXMEMORY_UNITSIZE - 1) / BXMEMORY_UNITSIZE;
		// ��뷮������ ó��(non packed)
		if(PackMaximum < Size)
			return LockChunk(Size, 0);
		// �ҿ뷮������ ó��(packed)
		if(0 < Size)
		{
			MemUint* NewPack = null;
			const unsigned PackID = Size - 1;
			PackState* PackInfo = &Pack(PackID);
			int RecentFocus = BXMEMORY_RECENT_COUNT;
			// �ű�ûũ Ȯ��
			if(PackInfo->TotalFreeUnit == 0)
			{
				unsigned AddCount = Min(Max((unsigned)(UnitSizeMinimum * 2 / Size),
					(unsigned)(PackInfo->TotalCountUnit - PackInfo->TotalFreeUnit)), (unsigned)(UnitCountMaximum * 2));
				while(!(NewPack = LockChunk(Size, AddCount /= 2)))
					if(AddCount == 0)
					{
						BxAssert("BxMemory<�޸𸮰� �����մϴ�>", false); // �޸𸮺���
						return null;
					}
				// ������ ó��
				++PackInfo->NumPack;
				// ����Ȳ ó��
				PackInfo->TotalCountUnit += BXMEMORY_GET_PACK_COUNT(NewPack);
				PackInfo->TotalFreeUnit += BXMEMORY_GET_PACK_FREE(NewPack);
			}
			// ����ûũ ����
			else
			{
				// ����� ����
				for(RecentFocus = 0; RecentFocus < BXMEMORY_RECENT_COUNT && !NewPack; ++RecentFocus)
					if(PackInfo->Recent[RecentFocus] && BXMEMORY_GET_PACK_FREE(PackInfo->Recent[RecentFocus]))
						NewPack = PackInfo->Recent[RecentFocus];
				// ����� �˻�
				if(!NewPack) NewPack = FindPack(PackID);
				BxAssert("BxMemory", NewPack != null); // TotalFreeUnit���� ����ġ
			}
			// �켱���� ó��
			for(int i = RecentFocus - 1; 0 < i; --i)
				PackInfo->Recent[i] = PackInfo->Recent[i - 1];
			PackInfo->Recent[0] = NewPack;
			// ����� ��ȯ
			return LockUnit(NewPack);
		}
		return null;
	}

	global_func void FreePack(const void* ptr)
	{
		// ûũ �˻�
		MemUint* DelChunk = FindChunk((MemUint*) ptr);
		if(DelChunk && BXMEMORY_IS_USED(DelChunk))
		{
			// �ش�����Ʈ ��ȯ
			if(BXMEMORY_IS_PACK(DelChunk))
			{
				MemUint* DelPack = DelChunk + BXMEMORY_CHUNK_HEADER_SIZE;
				const unsigned PackID = BXMEMORY_GET_PACK_ID(DelPack);
				PackState* PackInfo = &Pack(PackID);
				if(UnlockUnit(DelPack, (MemUint*) ptr))
				{
					// �Ѹ���Ʈ ����
					int RecentFocus = -1;
					while(++RecentFocus < BXMEMORY_RECENT_COUNT)
						if(PackInfo->Recent[RecentFocus] == DelPack)
						{
							PackInfo->Recent[RecentFocus] = null;
							break;
						}
					// �켱���� ó��
					for(int i = RecentFocus; i < BXMEMORY_RECENT_COUNT - 1; ++i)
						PackInfo->Recent[i] = PackInfo->Recent[i + 1];
					// ������ ó��
					--PackInfo->NumPack;
					// ����Ȳ ó��
					PackInfo->TotalCountUnit -= BXMEMORY_GET_PACK_COUNT(DelPack);
					PackInfo->TotalFreeUnit -= BXMEMORY_GET_PACK_FREE(DelPack);
				}
				else return;
			}
			// ûũ ��ȯ
			UnlockChunk(DelChunk);
		}
	}
} BxMemory;
/// @endcond
