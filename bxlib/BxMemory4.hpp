#pragma once
#include "BxCore.hpp"

/*!
\defgroup BxNew
\brief #define BxNew(TYPE)
\brief 메모리 기본할당
\param TYPE : 클래스타입
\return 클래스타입 포인터
*/
#define BxNew(TYPE)                        new(BxMemory::_Alloc(sizeof(TYPE), 1, __FILE__, __LINE__, __FUNCTION__)) TYPE

/*!
\defgroup BxNew_Param
\brief #define BxNew_Param(TYPE, ...)
\brief 메모리 인수할당
\param TYPE : 클래스타입
\param ... : 전달할 인수
\return 클래스타입 포인터
*/
#define BxNew_Param(TYPE, ...)             new(BxMemory::_Alloc(sizeof(TYPE), 1, __FILE__, __LINE__, __FUNCTION__)) TYPE(__VA_ARGS__)

/*!
\defgroup BxNew_Array
\brief #define BxNew_Array(TYPE, COUNT)
\brief 메모리 배열할당
\param TYPE : 클래스타입
\param COUNT : 수량
\return 클래스타입 배열포인터
*/
#define BxNew_Array(TYPE, COUNT)           BxMemory::_NewArray<TYPE>(COUNT, 2, __FILE__, __LINE__, __FUNCTION__)

/*!
\defgroup BxNew_ArrayParam
\brief #define BxNew_ArrayParam(TYPE, COUNT, ...)
\brief 메모리 배열인수할당
\param TYPE : 클래스타입
\param COUNT : 수량
\param ... : 전달할 인수
\return 클래스타입 배열포인터
*/
#define BxNew_ArrayParam(TYPE, COUNT, ...) BxMemory::_NewArrayParam<TYPE>(COUNT, 2, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

/*!
\defgroup BxDelete
\brief #define BxDelete(PTR)
\brief 메모리 기본소멸
\param PTR : 해당 클래스타입 포인터
*/
#define BxDelete(PTR)                      do{BxMemory::_Delete(PTR, 1, __FILE__, __LINE__, __FUNCTION__); PTR = null;} while(false)

/*!
\defgroup BxDelete_ByType
\brief #define BxDelete_ByType(TYPE, PTR)
\brief 메모리 형변환식 소멸
\param TYPE : 형변환시킬 클래스타입
\param PTR : 해당 포인터
*/
#define BxDelete_ByType(TYPE, PTR)         do{BxMemory::_Delete((TYPE*) PTR, 1, __FILE__, __LINE__, __FUNCTION__); PTR = null;} while(false)

/*!
\defgroup BxDelete_Array
\brief #define BxDelete_Array(PTR)
\brief 메모리 배열소멸
\param PTR : 해당 클래스타입 배열포인터
*/
#define BxDelete_Array(PTR)                do{BxMemory::_DeleteArray(PTR, 2, __FILE__, __LINE__, __FUNCTION__); PTR = null;} while(false)

/*!
\defgroup BxDelete_ArrayByType
\brief #define BxDelete_ArrayByType(TYPE, PTR)
\brief 메모리 형변환식 배열소멸
\param TYPE : 형변환시킬 클래스타입
\param PTR : 해당 배열포인터
*/
#define BxDelete_ArrayByType(TYPE, PTR)    do{BxMemory::_DeleteArray((TYPE*) PTR, 2, __FILE__, __LINE__, __FUNCTION__); PTR = null;} while(false)

/*!
\defgroup BxAlloc
\brief #define BxAlloc(LENGTH)
\brief 메모리덤프 할당
\param LENGTH : 바이트길이
\return 메모리덤프 포인터
*/
#define BxAlloc(LENGTH)                    BxMemory::_Alloc(LENGTH, 0, __FILE__, __LINE__, __FUNCTION__)

/*!
\defgroup BxFree
\brief #define BxFree(PTR)
\brief 메모리덤프 소명
\param PTR : 메모리덤프 포인터
*/
#define BxFree(PTR)                        do{BxMemory::_Free(PTR, 0, __FILE__, __LINE__, __FUNCTION__); PTR = null;} while(false)

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
typedef class BxMemory4
{
public:
	template<typename TYPE>
	global_func TYPE* _NewArray(const uint count, int type, string file, const int line, string func)
	{
		uint* DataArray = (uint*) _Alloc(sizeof(int) + Max(sizeof(uint), sizeof(TYPE) * count), type, file, line, func);
		DataArray[0] = count;
		TYPE* ClassArray = (TYPE*) &DataArray[1];
		for(uint i = 0; i < count; ++i)
			new(&ClassArray[i]) TYPE;
		return ClassArray;
	}

	template<typename TYPE>
	global_func void _Delete(const TYPE* ptr, int type, string file, const int line, string func)
	{
		if(ptr == null) return;
		ptr->~TYPE();
		_Free(ptr, type, file, line, func);
	}

	template<typename TYPE>
	global_func void _DeleteArray(const TYPE* ptr, int type, string file, const int line, string func)
	{
		if(ptr == null) return;
		const uint* DataArray = &((const uint*) ptr)[-1];
		for(uint i = 0, count = DataArray[0]; i < count; ++i)
			ptr[i].~TYPE();
		_Free(DataArray, type, file, line, func);
	}

	global_func inline void* _Alloc(const uint length, int type, string file, const int line, string func)
	{
		void* Ptr = MakeMethod()(length, type);
		LogMethod()(0, file, line, func);
		return Ptr;
	}

	global_func inline void _Free(const void* ptr, int type, string file, const int line, string func)
	{
		if(ptr == null) return;
		LogMethod()(1, file, line, func);
		FreeMethod()(ptr, type);
	}

	#define BXMEMORY_ARRAY_CORE(...) \
		uint* DataArray = (uint*) _Alloc(sizeof(int) + Max(sizeof(uint), sizeof(TYPE) * count), type, file, line, func); \
		DataArray[0] = count; \
		TYPE* ClassArray = (TYPE*) &DataArray[1]; \
		for(uint i = 0; i < count; ++i) \
			new(&ClassArray[i]) TYPE(__VA_ARGS__); \
		return ClassArray
	template<typename TYPE, typename PARAM1>
	global_func TYPE* _NewArrayParam(const uint count, int type, string file, const int line, string func, PARAM1 value1)
	{BXMEMORY_ARRAY_CORE(value1);}
	template<typename TYPE, typename PARAM1, typename PARAM2>
	global_func TYPE* _NewArrayParam(const uint count, int type, string file, const int line, string func, PARAM1 value1, PARAM2 value2)
	{BXMEMORY_ARRAY_CORE(value1, value2);}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3>
	global_func TYPE* _NewArrayParam(const uint count, int type, string file, const int line, string func, PARAM1 value1, PARAM2 value2, PARAM3 value3)
	{BXMEMORY_ARRAY_CORE(value1, value2, value3);}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	global_func TYPE* _NewArrayParam(const uint count, int type, string file, const int line, string func, PARAM1 value1, PARAM2 value2, PARAM3 value3, PARAM4 value4)
	{BXMEMORY_ARRAY_CORE(value1, value2, value3, value4);}
	template<typename TYPE, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4, typename PARAM5>
	global_func TYPE* _NewArrayParam(const uint count, int type, string file, const int line, string func, PARAM1 value1, PARAM2 value2, PARAM3 value3, PARAM4 value4, PARAM4 value5)
	{BXMEMORY_ARRAY_CORE(value1, value2, value3, value4, value5);}
	global_func inline bool DoMemLogger() {return (DLLHandle() != null);}

private:
	template<typename TYPE> global_func inline TYPE* GetModel() {global_data TYPE Type; return &Type;}
	global_func inline id_library& DLLHandle() {global_data id_library Handle = null; return Handle;}
	typedef int (*LogType)(int type, string file, int line, string func);
	typedef void* (*MakeType)(uint size, int type);
	typedef void (*FreeType)(const void* ptr, int type);
	global_func inline LogType& LogMethod() {global_data LogType Log = BeginLog; return Log;}
	global_func inline MakeType& MakeMethod() {global_data MakeType Make = BeginMake; return Make;}
	global_func inline FreeType& FreeMethod() {global_data FreeType Free = BeginFree; return Free;}
	global_func int BeginLog(int type, string file, int line, string func)
	{
		InitMemory();
		return LogMethod()(type, file, line, func);
	}
	global_func int NullLog(int type, string file, int line, string func)
	{
		return 0;
	}
	global_func void* BeginMake(uint size, int type)
	{
		InitMemory();
		return MakeMethod()(size, type);
	}
	global_func void BeginFree(const void* ptr, int type)
	{
		BxAssert("BxMemory", false);
	}
	global_func void InitMemory()
	{
		global_data bool IsInit = false;
		if(IsInit) return;
		IsInit = true;
		#if defined(__BX_DEBUG) && !defined(__GNUC__) && !defined(__ARMCC_VERSION)
			DLLHandle() = BxCore::Library::Open("../bxdll/CodeLogger2.dll");
			if(DLLHandle())
			{
				int MemSize = 0;
				BxCore::System::GetConfigInt("s3e", "MemSizeBx", &MemSize);
				id_memory Pool = BxCore::Util::Malloc(MemSize); // 동일한 조건
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
	// BxMemoryPack
	#define BXMEMORY_MEMSET_ON						(0)
	// FLAG
	#define BXMEMORY_CHUNK_HEADER_USEDFLAG			(0x80000000)
	#define BXMEMORY_CHUNK_HEADER_PACKFLAG			(0x40000000)
	#define BXMEMORY_CHUNK_HEADER_SIZEAREA			(0x3FFFFFFF)
	#define BXMEMORY_PACK_HEADER_IDAREA				(0xFF000000)
	#define BXMEMORY_PACK_HEADER_COUNTAREA			(0x00FFF000)
	#define BXMEMORY_PACK_HEADER_FREEAREA			(0x00000FFF)
	#define BXMEMORY_UNITSIZE						(sizeof(MemUint))
	// CHUNK HEADER
	#define BXMEMORY_IS_USED(CHUNK_PTR)				(*(CHUNK_PTR) & BXMEMORY_CHUNK_HEADER_USEDFLAG)
	#define BXMEMORY_IS_PACK(CHUNK_PTR)				(*(CHUNK_PTR) & BXMEMORY_CHUNK_HEADER_PACKFLAG)
	#define BXMEMORY_GET_SIZE(CHUNK_PTR)			(*(CHUNK_PTR) & BXMEMORY_CHUNK_HEADER_SIZEAREA)
	#define BXMEMORY_SET_USED(CHUNK_PTR)			(*(CHUNK_PTR) |= BXMEMORY_CHUNK_HEADER_USEDFLAG)
	#define BXMEMORY_SET_PACK(CHUNK_PTR)			(*(CHUNK_PTR) |= BXMEMORY_CHUNK_HEADER_PACKFLAG)
	#define BXMEMORY_SET_FREE(CHUNK_PTR)			(*(CHUNK_PTR) &= ~BXMEMORY_CHUNK_HEADER_USEDFLAG)
	// PACK HEADER
	#define BXMEMORY_GET_PACK_ID(PACK_PTR)			((*(PACK_PTR) & BXMEMORY_PACK_HEADER_IDAREA) >> 24)
	#define BXMEMORY_GET_PACK_COUNT(PACK_PTR)		((*(PACK_PTR) & BXMEMORY_PACK_HEADER_COUNTAREA) >> 12)
	#define BXMEMORY_GET_PACK_FREE(PACK_PTR)		(*(PACK_PTR) & BXMEMORY_PACK_HEADER_FREEAREA)
	#define BXMEMORY_SET_PACK_FREE(PACK_PTR, NUM)	((*(PACK_PTR) & (BXMEMORY_PACK_HEADER_IDAREA | BXMEMORY_PACK_HEADER_COUNTAREA)) | ((NUM) & BXMEMORY_PACK_HEADER_FREEAREA))

	// ETC
	#define BXMEMORY_PACK_INFO(CHUNK_PTR)			(*((CHUNK_PTR) + BXMEMORY_CHUNK_HEADER_SIZE))
	#define BXMEMORY_PACK_FOCUS(CHUNK_PTR)			(*((CHUNK_PTR) + BXMEMORY_CHUNK_HEADER_SIZE + BXMEMORY_PACK_HEADER_INFO_SIZE))
	#define BXMEMORY_PACK_DATA_PTR(CHUNK_PTR)		((CHUNK_PTR) + BXMEMORY_CHUNK_HEADER_SIZE + BXMEMORY_PACK_HEADER_INFO_SIZE + BXMEMORY_PACK_HEADER_FOCUS_SIZE)
	#define BXMEMORY_NEXT_CHUNK(CHUNK_PTR)			((CHUNK_PTR) + BXMEMORY_GET_SIZE(CHUNK_PTR))
	#define BXMEMORY_CHUNK_HEADER_SIZE				(1)
	#define BXMEMORY_PACK_HEADER_INFO_SIZE			(1)
	#define BXMEMORY_PACK_HEADER_FOCUS_SIZE			(1)
	#define BXMEMORY_RECENT_COUNT					(4)

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

	// Ptr을 포함하는 청크 검색
	global_func MemUint* FindChunk(MemUint* Ptr)
	{
		if(Ptr < Pool() || Pool() + PoolLength() <= Ptr)
			return null;
		MemUint* ChunkFocus = Pool();
		while(BXMEMORY_NEXT_CHUNK(ChunkFocus) <= Ptr)
			ChunkFocus = BXMEMORY_NEXT_CHUNK(ChunkFocus);
		return ChunkFocus;
	}

	// 신규청크 확보
	global_func MemUint* LockChunk(unsigned Size, unsigned AddCount)
	{
		const unsigned PackID = Size - 1; // Size가 256까지라서 1바이트에 기록될 수 있게 함
		const unsigned PackCount = Max((unsigned) 1, AddCount); // 팩의 최소수량
		const bool IsPack = (Size <= PackMaximum);
		unsigned NeedSize = BXMEMORY_CHUNK_HEADER_SIZE + ((!IsPack)? Size
			: BXMEMORY_PACK_HEADER_INFO_SIZE + BXMEMORY_PACK_HEADER_FOCUS_SIZE + Size * PackCount);
		// 공간검색
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
				// 공간할당을 위한 청크병합
				else
				{
					*(ChunkFocus - SumFreeChunkSize) = SumFreeChunkSize + BXMEMORY_GET_SIZE(ChunkFocus);
					ChunkFocus -= SumFreeChunkSize;
					break;
				}
			}
			else
			{
				// 검색속도를 위한 청크병합
				if(1 < SumFreeChunkCount)
					*(ChunkFocus - SumFreeChunkSize) = SumFreeChunkSize;
				SumFreeChunkSize = 0;
				SumFreeChunkCount = 0;
			}
			ChunkFocus = BXMEMORY_NEXT_CHUNK(ChunkFocus);
			if(ChunkFocus == PoolEnd)
			{
				BxAssert("BxMemory<메모리가 부족합니다>", false); // 메모리부족
				return null;
			}
		}
		// 공간분할
		if(BXMEMORY_CHUNK_HEADER_SIZE < BXMEMORY_GET_SIZE(ChunkFocus) - NeedSize)
			*(ChunkFocus + NeedSize) = BXMEMORY_GET_SIZE(ChunkFocus) - NeedSize;
		else NeedSize = BXMEMORY_GET_SIZE(ChunkFocus);
		*(ChunkFocus) = NeedSize;
		BXMEMORY_SET_USED(ChunkFocus);
		#if(BXMEMORY_MEMSET_ON == 1)
			BxCore::Util::MemSet(ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE, 0, BXMEMORY_BXMEMORY_UNITSIZE * (NeedSize - BXMEMORY_CHUNK_HEADER_SIZE));
		#endif
		// 팩구성
		if(IsPack)
		{
			// 청크헤더에 팩여부기입 및 팩헤더구성
			BXMEMORY_SET_PACK(ChunkFocus);
			BXMEMORY_PACK_INFO(ChunkFocus) =
				((PackID << 24) & BXMEMORY_PACK_HEADER_IDAREA) |
				((PackCount << 12) & BXMEMORY_PACK_HEADER_COUNTAREA) |
				(PackCount & BXMEMORY_PACK_HEADER_FREEAREA);
			BXMEMORY_PACK_FOCUS(ChunkFocus) = (MemUint) BXMEMORY_PACK_DATA_PTR(ChunkFocus);
			// 데이터구성
			MemUint* DataPtr = BXMEMORY_PACK_DATA_PTR(ChunkFocus);
			for(unsigned Focus = 0; Focus < PackCount; ++Focus)
			{
				if(Focus < PackCount - 1)
					*(DataPtr) = (MemUint) (DataPtr + Size);
				else *(DataPtr) = null;
				DataPtr = (MemUint*) *(DataPtr);
			}
		}
		// 청크정보 처리
		++NumChunk();
		return ChunkFocus + BXMEMORY_CHUNK_HEADER_SIZE;
	}

	// 해당청크 반환
	global_func void UnlockChunk(MemUint* ChunkPtr)
	{
		// 존재유무
		if(BXMEMORY_IS_USED(ChunkPtr))
		{
			BXMEMORY_SET_FREE(ChunkPtr);
			// 청크정보 처리
			--NumChunk();
		}
	}

	// 해당팩 검색
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

	// 팩내 신규유니트 확보
	global_func MemUint* LockUnit(MemUint* PackPtr)
	{
		const unsigned PackID = BXMEMORY_GET_PACK_ID(PackPtr);
		const unsigned PackCount = BXMEMORY_GET_PACK_COUNT(PackPtr);
		const unsigned PackFree = BXMEMORY_GET_PACK_FREE(PackPtr);
		MemUint* PackFocus = (MemUint*) BXMEMORY_PACK_FOCUS(PackPtr - BXMEMORY_CHUNK_HEADER_SIZE);

		MemUint* LockedMemory = PackFocus;
		if(PackFocus)
		{
			// 팩정보 처리
			*(PackPtr) = BXMEMORY_SET_PACK_FREE(PackPtr, PackFree - 1);
			// 팩현황 처리
			--Pack(PackID).TotalFreeUnit;
			// 포커스 이동
			BXMEMORY_PACK_FOCUS(PackPtr - BXMEMORY_CHUNK_HEADER_SIZE) = *(PackFocus);
		}
		return LockedMemory;
	}

	// 팩내 해당유니트 반환
	global_func bool UnlockUnit(MemUint* PackPtr, MemUint* Ptr)
	{
		const unsigned PackID = BXMEMORY_GET_PACK_ID(PackPtr);
		const unsigned PackCount = BXMEMORY_GET_PACK_COUNT(PackPtr);
		const unsigned PackFree = BXMEMORY_GET_PACK_FREE(PackPtr);
		MemUint* PackFocus = (MemUint*) BXMEMORY_PACK_FOCUS(PackPtr - BXMEMORY_CHUNK_HEADER_SIZE);

		// 팩정보 처리
		*(PackPtr) = BXMEMORY_SET_PACK_FREE(PackPtr, PackFree + 1);
		// 팩현황 처리
		++Pack(PackID).TotalFreeUnit;
		// 포커스 이동
		*(Ptr) = (MemUint) PackFocus;
		BXMEMORY_PACK_FOCUS(PackPtr - BXMEMORY_CHUNK_HEADER_SIZE) = (MemUint) Ptr;
		return (PackFree + 1 == PackCount); // 모든 유니트 반환여부
	}

	global_func void* MakePack(unsigned size, int)
	{
		// 초기화
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
		// 대용량데이터 처리(non packed)
		if(PackMaximum < Size)
			return LockChunk(Size, 0);
		// 소용량데이터 처리(packed)
		if(0 < Size)
		{
			MemUint* NewPack = null;
			const unsigned PackID = Size - 1; // Size가 256까지라서 1바이트에 기록될 수 있게 함
			PackState* PackInfo = &Pack(PackID);
			int RecentFocus = BXMEMORY_RECENT_COUNT;
			// 신규청크 확보
			if(PackInfo->TotalFreeUnit == 0)
			{
				unsigned AddCount = Min(Max((unsigned)(UnitSizeMinimum * 2 / Size),
					(unsigned)(PackInfo->TotalCountUnit - PackInfo->TotalFreeUnit)), (unsigned)(UnitCountMaximum * 2));
				while(!(NewPack = LockChunk(Size, AddCount /= 2)))
					if(AddCount == 0)
					{
						BxAssert("BxMemory<메모리가 부족합니다>", false); // 메모리부족
						return null;
					}
				// 팩정보 처리
				++PackInfo->NumPack;
				// 팩현황 처리
				PackInfo->TotalCountUnit += BXMEMORY_GET_PACK_COUNT(NewPack);
				PackInfo->TotalFreeUnit += BXMEMORY_GET_PACK_FREE(NewPack);
			}
			// 기존청크 접근
			else
			{
				// 대상팩 선정
				for(RecentFocus = 0; RecentFocus < BXMEMORY_RECENT_COUNT && !NewPack; ++RecentFocus)
					if(PackInfo->Recent[RecentFocus] && BXMEMORY_GET_PACK_FREE(PackInfo->Recent[RecentFocus]))
						NewPack = PackInfo->Recent[RecentFocus];
				// 대상팩 검색
				if(!NewPack) NewPack = FindPack(PackID);
				BxAssert("BxMemory<TotalFreeUnit정보 불일치>", NewPack != null);
			}
			// 우선순위 처리
			for(int i = RecentFocus - 1; 0 < i; --i)
				PackInfo->Recent[i] = PackInfo->Recent[i - 1];
			PackInfo->Recent[0] = NewPack;
			// 결과값 반환
			return LockUnit(NewPack);
		}
		return null;
	}

	global_func void FreePack(const void* ptr, int)
	{
		// 청크 검색
		MemUint* DelChunk = FindChunk((MemUint*) ptr);
		if(DelChunk && BXMEMORY_IS_USED(DelChunk))
		{
			// 해당유니트 반환
			if(BXMEMORY_IS_PACK(DelChunk))
			{
				MemUint* DelPack = DelChunk + BXMEMORY_CHUNK_HEADER_SIZE;
				const unsigned PackID = BXMEMORY_GET_PACK_ID(DelPack);
				PackState* PackInfo = &Pack(PackID);
				if(UnlockUnit(DelPack, (MemUint*) ptr))
				{
					// 팩리스트 삭제
					int RecentFocus = -1;
					while(++RecentFocus < BXMEMORY_RECENT_COUNT)
						if(PackInfo->Recent[RecentFocus] == DelPack)
						{
							PackInfo->Recent[RecentFocus] = null;
							break;
						}
					// 우선순위 처리
					for(int i = RecentFocus; i < BXMEMORY_RECENT_COUNT - 1; ++i)
						PackInfo->Recent[i] = PackInfo->Recent[i + 1];
					// 팩정보 처리
					--PackInfo->NumPack;
					// 팩현황 처리
					PackInfo->TotalCountUnit -= BXMEMORY_GET_PACK_COUNT(DelPack);
					PackInfo->TotalFreeUnit -= BXMEMORY_GET_PACK_FREE(DelPack);
				}
				else return;
			}
			// 청크 반환
			UnlockChunk(DelChunk);
		}
	}
} BxMemory;
/// @endcond
