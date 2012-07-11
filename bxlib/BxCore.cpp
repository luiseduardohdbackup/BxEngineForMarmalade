#include "BxCore.hpp"
#include "BxMemory.hpp"
#include "BxAutoBuffer.hpp"
#include "BxUtil.hpp"
#include "s3e.h"
#include "netdb.h"
#include "s3eLibrary.h"
#include "s3eOSExec.h"
#include "s3eOSReadString.h"
#include "s3eAndroidJNI.h"
#include "../edk/h/jni.h"
#include "string.h"
#include "derbh.h"
#include "arpa/inet.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "IwGxFontContainer.h"
#include "IwUTF8.h"
#include "IwDebug.h"

/// @cond SECTION_NAME
template<int ID>
class CallbackListJNI
{
	local_func string_rw ClassPath() {global_data char Name[256] = {'\0',}; return Name;}
	local_func string_rw MethodName() {global_data char Name[256] = {'\0',}; return Name;}

	local_func callback_jni& MethodJNI() {global_data callback_jni MethodJNI = null; return MethodJNI;}

	local_func jint CallbackMethod(JNIEnv* ENV, jobject OBJ, jstring param1_str1024, jint param2)
	{
		if(!MethodJNI()) return -90001;
		wchar temp_UTF16[1024];
		char result_str1024[1024];
		jboolean IsCopy = false;
		string UTFChars = ENV->GetStringUTFChars(param1_str1024, &IsCopy);
		BxUtil::UTF8ToUTF16(UTFChars, -1, temp_UTF16);
		ENV->ReleaseStringUTFChars(param1_str1024, UTFChars);
		BxUtil::UTF16ToCP949(temp_UTF16, -1, result_str1024, 1024);
		return (jint) MethodJNI()(result_str1024, (int) param2);
	}
public:
	local_func bool GetClass(string classpath, string methodname)
	{
		if(ClassPath()[0] == '\0' || MethodName()[0] == '\0')
		{
			BxUtilGlobal::StrCpy(ClassPath(), classpath);
			BxUtilGlobal::StrCpy(MethodName(), methodname);
			return true;
		}
		return (BxUtilGlobal::StrCmp(ClassPath(), classpath) == same && BxUtilGlobal::StrCmp(MethodName(), methodname) == same);
	}

	local_func int SetRegister(JNIEnv* env, jclass jclass, string methodname, callback_jni methodCB)
	{
		MethodJNI() = methodCB;
		global_data const JNINativeMethod JMETHOD = {methodname, "(Ljava/lang/String;I)I", (void*) &CallbackMethod};
		if(env->RegisterNatives(jclass, &JMETHOD, 1))
			return -90004;
		return 0;
	}
};
/// @endcond

/// @cond SECTION_NAME
extern callback_process ChildProcess;
/// @endcond

/// @cond SECTION_NAME
namespace BxCore
{
	namespace System
	{
		bool IsQuit()
		{
			return (s3eDeviceCheckQuitRequest() == S3E_TRUE);
		}

		void DoQuit()
		{
			s3eDeviceRequestQuit();
		}

		void Sleep(int ms)
		{
			s3eDeviceYield(ms);
		}

		uhuge GetTimerMs()
		{
			return (uhuge) s3eTimerGetUTC();
		}

		bool IsKeyDown(keykind key)
		{
			s3eKey Key = s3eKeyFirst;
			switch(key)
			{
			case keykind_esc: Key = s3eKeyEsc; break;
			case keykind_tab: Key = s3eKeyTab; break;
			case keykind_space: Key = s3eKeySpace; break;
			case keykind_bspace: Key = s3eKeyBackspace; break;
			case keykind_enter: Key = s3eKeyEnter; break;
			case keykind_shift: Key = s3eKeyLeftShift; break;
			case keykind_ctrl: Key = s3eKeyLeftControl; break;
			case keykind_left: Key = s3eKeyLeft; break;
			case keykind_up: Key = s3eKeyUp; break;
			case keykind_right: Key = s3eKeyRight; break;
			case keykind_down: Key = s3eKeyDown; break;
			case keykind_0: Key = s3eKey0; break;
			case keykind_1: Key = s3eKey1; break;
			case keykind_2: Key = s3eKey2; break;
			case keykind_3: Key = s3eKey3; break;
			case keykind_4: Key = s3eKey4; break;
			case keykind_5: Key = s3eKey5; break;
			case keykind_6: Key = s3eKey6; break;
			case keykind_7: Key = s3eKey7; break;
			case keykind_8: Key = s3eKey8; break;
			case keykind_9: Key = s3eKey9; break;
			case keykind_a: Key = s3eKeyA; break;
			case keykind_b: Key = s3eKeyB; break;
			case keykind_c: Key = s3eKeyC; break;
			case keykind_d: Key = s3eKeyD; break;
			case keykind_e: Key = s3eKeyE; break;
			case keykind_f: Key = s3eKeyF; break;
			case keykind_g: Key = s3eKeyG; break;
			case keykind_h: Key = s3eKeyH; break;
			case keykind_i: Key = s3eKeyI; break;
			case keykind_j: Key = s3eKeyJ; break;
			case keykind_k: Key = s3eKeyK; break;
			case keykind_l: Key = s3eKeyL; break;
			case keykind_m: Key = s3eKeyM; break;
			case keykind_n: Key = s3eKeyN; break;
			case keykind_o: Key = s3eKeyO; break;
			case keykind_p: Key = s3eKeyP; break;
			case keykind_q: Key = s3eKeyQ; break;
			case keykind_r: Key = s3eKeyR; break;
			case keykind_s: Key = s3eKeyS; break;
			case keykind_t: Key = s3eKeyT; break;
			case keykind_u: Key = s3eKeyU; break;
			case keykind_v: Key = s3eKeyV; break;
			case keykind_w: Key = s3eKeyW; break;
			case keykind_x: Key = s3eKeyX; break;
			case keykind_y: Key = s3eKeyY; break;
			case keykind_z: Key = s3eKeyZ; break;
			case keykind_f1: Key = s3eKeyF1; break;
			case keykind_f2: Key = s3eKeyF2; break;
			case keykind_f3: Key = s3eKeyF3; break;
			case keykind_f4: Key = s3eKeyF4; break;
			case keykind_f5: Key = s3eKeyF5; break;
			case keykind_f6: Key = s3eKeyF6; break;
			case keykind_f7: Key = s3eKeyF7; break;
			case keykind_f8: Key = s3eKeyF8; break;
			case keykind_f9: Key = s3eKeyF9; break;
			case keykind_f10: Key = s3eKeyF10; break;
			case keykind_pad_0: Key = s3eKeyNumPad0; break;
			case keykind_pad_1: Key = s3eKeyNumPad1; break;
			case keykind_pad_2: Key = s3eKeyNumPad2; break;
			case keykind_pad_3: Key = s3eKeyNumPad3; break;
			case keykind_pad_4: Key = s3eKeyNumPad4; break;
			case keykind_pad_5: Key = s3eKeyNumPad5; break;
			case keykind_pad_6: Key = s3eKeyNumPad6; break;
			case keykind_pad_7: Key = s3eKeyNumPad7; break;
			case keykind_pad_8: Key = s3eKeyNumPad8; break;
			case keykind_pad_9: Key = s3eKeyNumPad9; break;
			case keykind_pad_plus: Key = s3eKeyNumPadPlus; break;
			case keykind_pad_minus: Key = s3eKeyNumPadMinus; break;
			case keykind_pad_enter: Key = s3eKeyNumPadEnter; break;
			case keykind_soft_left: Key = s3eKeyLSK; break;
			case keykind_soft_right: Key = s3eKeyRSK; break;
			case keykind_shoulder_left: Key = s3eKeyLS; break;
			case keykind_shoulder_right: Key = s3eKeyRS; break;
			case keykind_star: Key = s3eKeyStar; break;
			case keykind_sharp: Key = s3eKeyHash; break;
			case keykind_ok: Key = s3eKeyOk; break;
			case keykind_clr: Key = s3eKeyCLR; break;
			case keykind_vol_up: Key = s3eKeyVolUp; break;
			case keykind_vol_down: Key = s3eKeyVolDown; break;
			}
			return ((s3eKeyboardGetState(Key) & S3E_KEY_STATE_DOWN) != null);
		}

		void GetConfigInt(string group, string name, int* value)
		{
			s3eConfigGetInt(group, name, value);
		}

		void GetConfigStr(string group, string name, string_rw value256)
		{
			s3eConfigGetString(group, name, value256);
		}

		void Assert(string name, bool& IsIgnore, bool flag, string filename, int linenumber)
		{
			#ifdef IwDebugHandleStop
			if(IsIgnore || flag) return;
			switch(IwDebugAssertShow(name, filename, linenumber, 1))
			{
			case S3E_ERROR_SHOW_STOP:
				IwDebugHandleStop();
				break;
			case S3E_ERROR_SHOW_IGNORE:
				IsIgnore = true;
				break;
			}
			#endif
		}

		string GetOSName()
		{
			return s3eDeviceGetString(S3E_DEVICE_OS);
		}

		string GetPhoneNumber()
		{
			return s3eDeviceGetString(S3E_DEVICE_PHONE_NUMBER);
		}

		void PopupOSExecute(string url, bool exitme)
		{
			s3eOSExecExecute(url, exitme);
		}

		bool PopupOSKeyboard(string_rw result_str256, string default_str, string keyboard_name, oskeyboardtype type)
		{
			if(!s3eOSReadStringAvailable())
				return false;
			int Flag = 0;
			switch(type)
			{
			case oskeyboardtype_base: Flag = 0; break;
			case oskeyboardtype_email: Flag = S3E_OSREADSTRING_FLAG_EMAIL; break;
			case oskeyboardtype_number: Flag = S3E_OSREADSTRING_FLAG_NUMBER; break;
			case oskeyboardtype_password: Flag = S3E_OSREADSTRING_FLAG_PASSWORD; break;
			case oskeyboardtype_url: Flag = S3E_OSREADSTRING_FLAG_URL; break;
			}
			wchar temp_UTF16[1024];
			char default_str_UTF8[1024];
			char keyboard_name_UTF8[1024];
			BxUtil::CP949ToUTF16(default_str, -1, temp_UTF16, 1024);
			BxUtil::UTF16ToUTF8(temp_UTF16, -1, default_str_UTF8);
			BxUtil::CP949ToUTF16(keyboard_name, -1, temp_UTF16, 1024);
			BxUtil::UTF16ToUTF8(temp_UTF16, -1, keyboard_name_UTF8);
			string Result = s3eOSReadStringUTF8WithDefault(keyboard_name_UTF8, default_str_UTF8, Flag);
			if(!Result) return false;
			BxUtil::UTF8ToUTF16(Result, -1, temp_UTF16);
			BxUtil::UTF16ToCP949(temp_UTF16, -1, result_str256, 1024);
			return true;
		}

		CallbackList::CallbackList(callback_frame method, void* data, int datasize)
		{
			Method = method;
			IsDataRef = (datasize <= -1);
			if(!IsDataRef)
			{
				if(0 < datasize && data)
				{
					Data = BxAlloc(datasize);
					BxCore::Util::MemMove(Data, data, datasize);
				}
				else Data = null;
			}
			else Data = data;
			Next = null;
		}

		CallbackList::~CallbackList()
		{
			if(!IsDataRef && Data) BxFree(Data);
			if(Next) BxDelete(Next);
		}

		void AddCallback(callback_frame method, void* data, int datasize)
		{
			CallbackList* List = (CallbackList*) CallbackList::GetFirst();
			while(List->Next) List = List->Next;
			List->Next = BxNew_Param(CallbackList, method, data, datasize);
		}

		bool BeginChildProcess(callback_process process)
		{
			if(ChildProcess) return false;
			ChildProcess = process;
			return true;
		}

		bool IsExistClassJNI(string classpath)
		{
			if(!s3eAndroidJNIAvailable()) return false;
			JavaVM* JVM = (JavaVM*) s3eAndroidJNIGetVM();
			if(!JVM) return false;
			JNIEnv* ENV = null;
			JVM->GetEnv((void**) &ENV, JNI_VERSION_1_6);
			jclass JCLASS = ENV->FindClass(classpath);
			if(!JCLASS) return false;
			ENV->DeleteLocalRef(JCLASS);
			return true;
		}

		int CallStaticMethodJNI(string classpath, string methodname, string param1_str1024, int param2)
		{
			if(!s3eAndroidJNIAvailable()) return -90001;
			JavaVM* JVM = (JavaVM*) s3eAndroidJNIGetVM();
			if(!JVM) return -90002;
			JNIEnv* ENV = null;
			JVM->GetEnv((void**) &ENV, JNI_VERSION_1_6);
			jclass JCLASS = ENV->FindClass(classpath);
			if(!JCLASS) return -90003;

			jmethodID JMETHOD = ENV->GetStaticMethodID(JCLASS, methodname, "(Ljava/lang/String;I)I");
			wchar temp_UTF16[1024];
			char default_str_UTF8[1024];
			BxUtil::CP949ToUTF16(param1_str1024, -1, temp_UTF16, 1024);
			BxUtil::UTF16ToUTF8(temp_UTF16, -1, default_str_UTF8);
			jstring JSTRING = ENV->NewStringUTF(default_str_UTF8);
			jint JINT = ENV->CallStaticIntMethod(JCLASS, JMETHOD, JSTRING, param2);
			ENV->DeleteLocalRef(JCLASS);
			ENV->DeleteLocalRef(JSTRING);
			return JINT;
		}

		int SetCallbackJNI(string classpath, string methodname, callback_jni methodCB)
		{
			if(!s3eAndroidJNIAvailable()) return -90001;
			JavaVM* JVM = (JavaVM*) s3eAndroidJNIGetVM();
			if(!JVM) return -90002;
			JNIEnv* ENV = null;
			JVM->GetEnv((void**) &ENV, JNI_VERSION_1_6);
			jclass JCLASS = ENV->FindClass(classpath);
			if(!JCLASS) return -90003;

			if(CallbackListJNI<0>::GetClass(classpath, methodname)) return CallbackListJNI<0>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<1>::GetClass(classpath, methodname)) return CallbackListJNI<1>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<2>::GetClass(classpath, methodname)) return CallbackListJNI<2>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<3>::GetClass(classpath, methodname)) return CallbackListJNI<3>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<4>::GetClass(classpath, methodname)) return CallbackListJNI<4>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<5>::GetClass(classpath, methodname)) return CallbackListJNI<5>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<6>::GetClass(classpath, methodname)) return CallbackListJNI<6>::SetRegister(ENV, JCLASS, methodname, methodCB);
			if(CallbackListJNI<7>::GetClass(classpath, methodname)) return CallbackListJNI<7>::SetRegister(ENV, JCLASS, methodname, methodCB);
			ENV->DeleteLocalRef(JCLASS);
			return -90005;
		}
	}
	namespace Surface
	{
		local_func inline const bool IsOSIPhone() {global_data const bool OSResult = (BxUtilGlobal::StrICmp(System::GetOSName(), "IPHONE") == same); return OSResult;}
		local_func inline const bool IsOSAndroid() {global_data const bool OSResult = (BxUtilGlobal::StrICmp(System::GetOSName(), "ANDROID") == same); return OSResult;}
		local_func inline const bool IsOSWindows() {global_data const bool OSResult = (BxUtilGlobal::StrICmp(System::GetOSName(), "WINDOWS") == same); return OSResult;}
		local_func inline bool& IsOffscreenSurfaceValid() {global_data bool IsValid = false; return IsValid;}
		local_func inline CIwGxSurface& OffscreenSurface() {global_data CIwGxSurface Surface; return Surface;}
		local_func void SetRenderMode(rendermode mode)
		{
			global_data rendermode LastRenderMode = rendermode_null;
			if(LastRenderMode != mode)
			{
				#ifdef __BX_OPENGL
				if(LastRenderMode != rendermode_null)
					IwGxFlush();
				if(mode == rendermode_2d)
					IwGxSetSortMode(IW_GX_SORT_BY_SLOT);
				else if(mode == rendermode_3d)
					IwGxSetSortMode(IW_GX_SORT_BY_Z);
				#endif
				LastRenderMode = mode;
			}
		}

		void Init(dirtype dir)
		{
			if(IsOSAndroid()) dir = dir_rot0;
			#ifndef __BX_OPENGL
			#ifdef __BX_PIXEL16
				s3eSurfacePixelType Type = S3E_SURFACE_PIXEL_TYPE_RGB565;
			#else
				s3eSurfacePixelType Type = S3E_SURFACE_PIXEL_TYPE_RGB888;
			#endif
			s3eSurfaceBlitDirection Direction = S3E_SURFACE_BLIT_DIR_NORMAL;
			switch(dir)
			{
			case dir_rot0: Direction = S3E_SURFACE_BLIT_DIR_NORMAL; break;
			case dir_rot90: Direction = S3E_SURFACE_BLIT_DIR_ROT90; break;
			case dir_rot180: Direction = S3E_SURFACE_BLIT_DIR_ROT180; break;
			case dir_rot270: Direction = S3E_SURFACE_BLIT_DIR_ROT270; break;
			case dir_native: Direction = S3E_SURFACE_BLIT_DIR_NATIVE; break;
			}
			s3eSurfaceSetup(Type, 0, NULL, Direction);
			#else
			switch(dir)
			{
			case dir_rot0:
				IwGxSetScreenOrient(IW_GX_ORIENT_NONE);
				break;
			case dir_rot90:
				IwGxSetScreenOrient(IW_GX_ORIENT_90);
				break;
			case dir_rot180:
				IwGxSetScreenOrient(IW_GX_ORIENT_180);
				break;
			case dir_rot270:
				IwGxSetScreenOrient(IW_GX_ORIENT_270);
				break;
			}
			IwGxLightingOff();
			IwGxSetColStream(null);
			IwGxSetColClear(0x00, 0x00, 0x00, 0xFF);
			BxCore::Surface::SetRenderMode(rendermode_2d);
			IwGxClear();
			if(GetWidth() != GetWidthHW() || GetHeight() != GetHeightHW())
			{
				if(!OffscreenSurface().CreateSurface(NULL, GetWidth(), GetHeight(),
					CIwGxSurface::EXACT_MATCH_F | CIwGxSurface::PERSISTENT_F))
					BxAssert("BxCore::Surface<FBO의 초기화에 실패하였습니다>", false);
				else
				{
					IsOffscreenSurfaceValid() = true;
					OffscreenSurface().MakeCurrent();
					IwGxLightingOff();
					IwGxSetColStream(null);
					IwGxSetColClear(0x00, 0x00, 0x00, 0xFF);
					IwGxClear();
				}
			}
			#endif
			s3eDeviceBacklightOn();
		}

		void Flush(void* ptr, const int row, const int height, const int pitch)
		{
			#ifndef __BX_OPENGL
			void* ScreenPtr = s3eSurfacePtr();
			const int ScreenPitch = s3eSurfaceGetInt(S3E_SURFACE_PITCH);
			for(int y = 0; y < height; ++y)
			{
				memcpy(ScreenPtr, ptr, row);
				ScreenPtr = ((byte*) ScreenPtr) + ScreenPitch;
				ptr = ((byte*) ptr) + pitch;
			}
			s3eSurfaceShow();
			#else
			BxCore::Surface::SetRenderMode(rendermode_null);
			if(IsOffscreenSurfaceValid())
			{
				CIwGxSurface::MakeDisplayCurrent();
				CIwMaterial* MaterialFBO = IW_GX_ALLOC_MATERIAL();
				MaterialFBO->SetTexture(OffscreenSurface().GetTexture());
				MaterialFBO->SetModulateMode(CIwMaterial::MODULATE_RGB);
				MaterialFBO->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				MaterialFBO->SetBlendMode(CIwMaterial::BLEND_MODULATE);
				MaterialFBO->SetColAmbient(0xFF, 0xFF, 0xFF, 0xFF);
				MaterialFBO->SetCullMode(CIwMaterial::CULL_NONE);
				IwGxSetMaterial(MaterialFBO);
				// Vertex
				CIwSVec2* Vertex = IW_GX_ALLOC(CIwSVec2, 4);
				Vertex[0].x = 0;
				Vertex[0].y = 0;
				Vertex[1].x = 0;
				Vertex[1].y = GetHeightHW();
				Vertex[2].x = GetWidthHW();
				Vertex[2].y = GetHeightHW();
				Vertex[3].x = GetWidthHW();
				Vertex[3].y = 0;
				// UV
				CIwSVec2* UV = IW_GX_ALLOC(CIwSVec2, 4);
				UV[0] = CIwSVec2(0, 0);
				UV[1] = CIwSVec2(0, (1 << 12) * GetHeight() / OffscreenSurface().GetHeight());
				UV[2] = CIwSVec2((1 << 12) * GetWidth() / OffscreenSurface().GetWidth(),
					(1 << 12) * GetHeight() / OffscreenSurface().GetHeight());
				UV[3] = CIwSVec2((1 << 12) * GetWidth() / OffscreenSurface().GetWidth(), 0);
				IwGxSetVertStreamScreenSpace(Vertex, 4);
				IwGxSetUVStream(UV);
				IwGxDrawPrims(IW_GX_QUAD_LIST, null, 4);
				IwGxFlush();
			}
			IwGxSwapBuffers();
			if(IsOffscreenSurfaceValid())
				OffscreenSurface().MakeCurrent();
			#endif
			s3eDeviceYield();
			s3eKeyboardUpdate();
			s3ePointerUpdate();
			s3eDeviceBacklightOn();
		}

		void MiddleFlush()
		{
			#ifdef __BX_OPENGL
			IwGxFlush();
			#endif
		}

		int GetWidth(bool* isfixedvalue)
		{
			global_data int SurfaceWidth = -1;
			if(SurfaceWidth == -1)
			{
				BxCore::System::GetConfigInt("s3e", "SurfaceWidthBx", &SurfaceWidth);
				SurfaceWidth = BxUtilGlobal::Max(0, SurfaceWidth);
			}
			if(SurfaceWidth != 0)
			{
				if(isfixedvalue) *isfixedvalue = true;
				return SurfaceWidth;
			}
			else if(isfixedvalue) *isfixedvalue = false;
			return GetWidthHW();
		}

		int GetHeight(bool* isfixedvalue)
		{
			global_data int SurfaceHeight = -1;
			if(SurfaceHeight == -1)
			{
				BxCore::System::GetConfigInt("s3e", "SurfaceHeightBx", &SurfaceHeight);
				SurfaceHeight = BxUtilGlobal::Max(0, SurfaceHeight);
			}
			if(SurfaceHeight != 0)
			{
				if(isfixedvalue) *isfixedvalue = true;
				return SurfaceHeight;
			}
			else if(isfixedvalue) *isfixedvalue = false;
			return GetHeightHW();
		}

		int GetWidthHW()
		{
			#ifdef __BX_OPENGL
			if(IsOSIPhone()) return IwGxGetScreenWidth();
			else return s3eSurfaceGetInt(S3E_SURFACE_DEVICE_WIDTH);
			#else
			return s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
			#endif
		}

		int GetHeightHW()
		{
			#ifdef __BX_OPENGL
			if(IsOSIPhone()) return IwGxGetScreenHeight();
			else return s3eSurfaceGetInt(S3E_SURFACE_DEVICE_HEIGHT);
			#else
			return s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
			#endif
		}
	}
	namespace Sound
	{
		class SoundData : public BxAutoBuffer<autobuffer_sound>
		{
		private:
			bool IsAudio;
			string FileName;
			int FileSize;
		public:
			int16* _ref_ WaveData;
			uint WaveLength;
			uint SamplesPerSec;

		private:
			class Initializer
			{
			public:
				Initializer()
				{
					int SoundCacheSize = -1;
					BxCore::System::GetConfigInt("s3e", "SoundCacheSizeBx", &SoundCacheSize);
					BufferPoolSize() = SoundCacheSize;
					SetMusicVolume(100);
					SetEffectVolume(100);
					for(int i = GetEffectChannelBegin(); i < GetChannelCount(); ++i)
						System::AddCallback(EffectChannelCB, (void*) i);
				}
				virtual ~Initializer() {}
				local_func void Run() {global_data Initializer Init;}
			};

			struct RIFFHeader
			{
				uint typeID;
				uint length;
				uint subTypeID;
			};
			struct RIFFChunkHeader
			{
				uint typeID;
				uint length;
			};
			struct WAVEFormatChunk
			{
				ushort formatTag;
				ushort channels;
				uint samplesPerSec;
				uint avgBytesPerSec;
				ushort blockAlign;
				ushort bitsPerSample;
			};
			struct WAVEFormatChunkADPCM
			{
				ushort formatTag;
				ushort channels;
				uint samplesPerSec;
				uint avgBytesPerSec;
				ushort blockAlign;
				ushort bitsPerSample;
				ushort cbSize;
				ushort samplesPerBlock;
			};
			void LoadWave(const byte* sounddata, uint length)
			{
				if(((RIFFHeader*) sounddata)->typeID != VAL4('R', 'I', 'F', 'F') || ((RIFFHeader*) sounddata)->subTypeID != VAL4('W', 'A', 'V', 'E'))
					return;
				const byte* DataEnd = sounddata + length;
				sounddata += sizeof(RIFFHeader);
				while(sounddata + sizeof(RIFFChunkHeader) <= DataEnd)
				{
					const byte* NextData = sounddata + sizeof(RIFFChunkHeader) + ((RIFFChunkHeader*) sounddata)->length;
					switch(((RIFFChunkHeader*) sounddata)->typeID)
					{
					case VAL4('f', 'm', 't', ' '):
						sounddata += sizeof(RIFFChunkHeader);
						if(((WAVEFormatChunkADPCM*) sounddata)->bitsPerSample == 16)
							SamplesPerSec = ((WAVEFormatChunkADPCM*) sounddata)->samplesPerSec;
						break;
					case VAL4('d', 'a', 't', 'a'):
						WaveData = (int16*)(sounddata + sizeof(RIFFChunkHeader));
						WaveLength = ((RIFFChunkHeader*) sounddata)->length;
						break;
					case VAL4('f', 'a', 'c', 't'):
						sounddata += sizeof(RIFFChunkHeader);
						WaveLength = *((uint32*) sounddata);
						break;
					default:
						break;
					}
					sounddata = NextData;
				}
			}

		public:
			SoundData(string filename, bool audio, bool autoload)
			{
				Initializer::Run();
				IsAudio = audio;
				FileName = (string) BxAlloc(BxUtilGlobal::StrLen(filename) + 1);
				BxUtilGlobal::StrCpy((string_rw) FileName, filename);
				FileSize = (int) s3eFileGetFileInt(FileName, S3E_FILE_SIZE);
				WaveData = null;
				WaveLength = 0;
				SamplesPerSec = s3eSoundGetInt(S3E_SOUND_DEFAULT_FREQ);
				if(!autoload)
				{
					id_file File = File::Open(FileName, "rb");
					BxAssert("BxCore::Sound<존재하지 않는 파일입니다>", File);
					File::Read(File, Alloc(FileSize, true), FileSize);
					File::Close(File);
					LoadWave((const byte*) GetBuffer(), FileSize);
				}
			}

			virtual ~SoundData()
			{
				if(FileName) BxFree(FileName);
			}

			int16* LockSoundData()
			{
				if(!GetBuffer())
				{
					id_file File = File::Open(FileName, "rb");
					BxAssert("BxCore::Sound<존재하지 않는 파일입니다>", File);
					File::Read(File, Alloc(FileSize, true), FileSize);
					File::Close(File);
					LoadWave((const byte*) GetBuffer(), FileSize);
				}
				Lock();
				return (int16*) GetBuffer();
			}

			inline void UnlockSoundData()
			{
				Unlock();
			}

			inline int16* GetSoundData()
			{
				return (IsAudio)? (int16*) GetBuffer() : WaveData;
			}

			inline int GetSoundDataSize()
			{
				return ((IsAudio)? FileSize : WaveLength);
			}

			inline int GetSoundDataCount()
			{
				return ((IsAudio)? FileSize : WaveLength) / sizeof(int16);
			}

			inline bool IsSoundDataByAudio()
			{
				return IsAudio;
			}

			local_func s3eResult EffectChannelPlay(int channel, id_sound sound)
			{
				ChannelState(channel, false) = sound;
				ChannelState(channel, true) = null;
				BxAssert("BxCore::Sound<효과음은 Audio포맷이 지원불가합니다>", !((SoundData*) sound)->IsSoundDataByAudio());
				((SoundData*) sound)->LockSoundData();
				s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, ((SoundData*) sound)->SamplesPerSec);
				return s3eSoundChannelPlay(channel, ((SoundData*) sound)->GetSoundData(), ((SoundData*) sound)->GetSoundDataCount(), 1, 0);
			}

			local_func void EffectChannelPlayReserve(int channel, id_sound sound)
			{
				SoundData* Sound = (SoundData*) ChannelState(channel, true);
				if(Sound)
				{
					BxAssert("BxCore::Sound<효과음은 Audio포맷이 지원불가합니다>", !Sound->IsSoundDataByAudio());
					Sound->LockSoundData();
					s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, Sound->SamplesPerSec);
					s3eResult Result = s3eSoundChannelPlay(channel, Sound->GetSoundData(), Sound->GetSoundDataCount(), 1, 0);
					BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
				}
				ChannelState(channel, false) = ChannelState(channel, true);
				ChannelState(channel, true) = sound;
			}

			local_func s3eResult EffectChannelStop(int channel)
			{
				SoundData* CurSound = (SoundData*) SoundData::ChannelState(channel, false);
				if(CurSound) CurSound->UnlockSoundData();
				ChannelState(channel, false) = null;
				ChannelState(channel, true) = null;
				return s3eSoundChannelStop(channel);
			}

			local_func id_sound& ChannelState(int channel, bool isReserve)
			{
				global_data id_sound Channel[32][2] = {{null,},};
				return Channel[channel][isReserve];
			}

			local_func bool EffectChannelCB(void* data)
			{
				// 확인
				const int Channel = (int) data;
				SoundData* CurSound = (SoundData*) SoundData::ChannelState(Channel, false);
				if(!CurSound) return true;
				if(s3eSoundChannelGetInt(Channel, S3E_CHANNEL_PAUSED)) return true;
				if(s3eSoundChannelGetInt(Channel, S3E_CHANNEL_STATUS)) return true;
				CurSound->UnlockSoundData();
				SoundData::ChannelState(Channel, false) = null;
				// 처리
				SoundData* Sound = (SoundData*) ChannelState(Channel, true);
				if(Sound)
				{
					BxAssert("BxCore::Sound<효과음은 Audio포맷이 지원불가합니다>", !Sound->IsSoundDataByAudio());
					Sound->LockSoundData();
					s3eSoundChannelSetInt(Channel, S3E_CHANNEL_RATE, Sound->SamplesPerSec);
					s3eResult Result = s3eSoundChannelPlay(Channel, Sound->GetSoundData(), Sound->GetSoundDataCount(), 1, 0);
					BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
				}
				ChannelState(Channel, false) = ChannelState(Channel, true);
				ChannelState(Channel, true) = null;
				return true;
			}

			local_func inline int GetEffectChannelBegin()
			{
				return 1;
			}

			local_func inline int GetChannelCount()
			{
				global_data const int ChannelCount = s3eSoundGetInt(S3E_SOUND_NUM_CHANNELS);
				return ChannelCount;
			}

			local_func int GetFreeEffectChannel()
			{
				for(int i = GetEffectChannelBegin(); i < GetChannelCount(); ++i)
					if(ChannelState(i, false) == null && ChannelState(i, true) == null)
						return i;
				return 0;
			}

			local_func int GetSameEffectChannel(id_sound sound)
			{
				for(int i = GetEffectChannelBegin(); i < GetChannelCount(); ++i)
					if(ChannelState(i, false) == sound && ChannelState(i, true) == null)
						return i;
				for(int i = GetEffectChannelBegin(); i < GetChannelCount(); ++i)
					if(ChannelState(i, true) == sound)
						return i;
				return 0;
			}

			local_func int GetImminentEffectChannel()
			{
				int iBest = 1;
				bool IsReserveBest = true;
				int ValueBest = 0x7FFFFFFF;
				for(int i = GetEffectChannelBegin(); i < GetChannelCount(); ++i)
				{
					SoundData* Sound1 = (SoundData*) ChannelState(i, false);
					SoundData* Sound2 = (SoundData*) ChannelState(i, false);
					bool IsReserve = (Sound2 != null);
					int Value = ((Sound1)? Sound1->GetSoundDataSize() : 0) + ((Sound2)? Sound2->GetSoundDataSize() : 0);
					if(IsReserve < IsReserveBest || ((IsReserve == IsReserveBest) && Value < ValueBest))
					{
						iBest = i;
						IsReserveBest = IsReserve;
						ValueBest = Value;
					}
				}
				return iBest;
			}
		};

		class PanoramaList
		{
			id_sound Sound;
			PanoramaList* Next;
		public:
			enum Command {None, RemoveAll, GoNext, GoFirst};

			PanoramaList()
			{
				Sound = null;
				Next = this;
				System::AddCallback(MusicChannelCB, null);
			}

			virtual ~PanoramaList()
			{
				RemoveList();
			}

			void RemoveList()
			{
				PanoramaList* Focus = Next;
				while(Focus != this)
				{
					PanoramaList* NextFocus = Focus->Next;
					BxFree(Focus);
					Focus = NextFocus;
				}
				Next = this;
			}

			int GetLength()
			{
				int Length = 0;
				PanoramaList* Focus = this;
				while((Focus = Focus->Next) != this)
					++Length;
				return Length;
			}

			void AddNext(id_sound sound)
			{
				PanoramaList* NewData = (PanoramaList*) BxAlloc(sizeof(PanoramaList));
				NewData->Sound = sound;
				NewData->Next = Next;
				Next = NewData;
			}

			local_func PanoramaList* Focus(Command cmd = None)
			{
				global_data PanoramaList ListBegin;
				global_data PanoramaList* ListFocus = &ListBegin;
				switch(cmd)
				{
				case RemoveAll:
					ListBegin.RemoveList();
					ListFocus = &ListBegin;
					break;
				case GoNext:
					ListFocus = ListFocus->Next;
					break;
				case GoFirst:
					ListFocus = ListBegin.Next;
					break;
				}
				return ListFocus;
			}

			local_func bool MusicChannelCB(void*)
			{
				// 확인
				SoundData* CurSound = (SoundData*) SoundData::ChannelState(0, false);
				if(!CurSound) return true;
				if(CurSound->IsSoundDataByAudio())
				{
					s3eAudioStatus Status = (s3eAudioStatus) s3eAudioGetInt(S3E_AUDIO_STATUS);
					if(Status == S3E_AUDIO_PAUSED) return true;
					if(Status == S3E_AUDIO_PLAYING) return true;
				}
				else
				{
					if(s3eSoundChannelGetInt(0, S3E_CHANNEL_PAUSED)) return true;
					if(s3eSoundChannelGetInt(0, S3E_CHANNEL_STATUS)) return true;
				}
				CurSound->UnlockSoundData();
				SoundData::ChannelState(0, false) = null;
				// 처리
				if(!Focus(GoNext)->Sound) Focus(GoNext);
				if(Focus()->Sound)
				{
					SoundData::ChannelState(0, false) = Focus()->Sound;
					SoundData* CurSound = (SoundData*) Focus()->Sound;
					if(CurSound)
					{
						if(CurSound->IsSoundDataByAudio()) s3eAudioStop();
						else s3eSoundChannelStop(0);
					}
					CurSound->LockSoundData();
					if(CurSound->IsSoundDataByAudio())
					{
						s3eResult Result = s3eAudioPlayFromBuffer(CurSound->GetSoundData(), CurSound->GetSoundDataSize(), 1);
						BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
					}
					else
					{
						s3eSoundChannelSetInt(0, S3E_CHANNEL_RATE, CurSound->SamplesPerSec);
						s3eResult Result = s3eSoundChannelPlay(0, CurSound->GetSoundData(), CurSound->GetSoundDataCount(), 1, 0);
						BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
					}
				}
				return true;
			}
		};

		id_sound Create(string filename, bool autoload)
		{
			const int Length = BxUtilGlobal::StrLen(filename);
			BxAssert("BxCore::Sound<파일명이 너무 짧습니다>", 4 <= Length);
			const bool IsAudio = (BxUtilGlobal::StrICmp(filename + Length - 4, ".wav") != same);
			SoundData* Data = BxNew_Param(SoundData, filename, IsAudio, autoload);
			return (id_sound) Data;
		}

		void Release(id_sound sound)
		{
			BxDelete_ByType(SoundData, sound);
		}

		void PlayMusic(id_sound sound, bool interrupt, uint panoramaCount, ...)
		{
			if(PanoramaList::Focus()->GetLength() == 0)
				interrupt = true;
			PanoramaList::Focus(PanoramaList::RemoveAll)->AddNext((SoundData*) sound);
			va_list List;
			va_start(List, panoramaCount);
			for(uint i = 0; i < panoramaCount; ++i)
			{
				SoundData* iSound = (SoundData*) va_arg(List, id_sound);
				PanoramaList::Focus(PanoramaList::GoNext)->AddNext(iSound);
			}
			va_end(List);
			PanoramaList::Focus(PanoramaList::GoFirst);
			if(interrupt)
			{
				PanoramaList::Focus(PanoramaList::GoNext);
				SoundData* CurSound = (SoundData*) SoundData::ChannelState(0, false);
				if(CurSound)
				{
					if(CurSound->IsSoundDataByAudio()) s3eAudioStop();
					else s3eSoundChannelStop(0);
				}
				SoundData::ChannelState(0, false) = sound;
				((SoundData*) sound)->LockSoundData();
				if(((SoundData*) sound)->IsSoundDataByAudio())
				{
					s3eResult Result = s3eAudioPlayFromBuffer(((SoundData*) sound)->GetSoundData(), ((SoundData*) sound)->GetSoundDataSize(), 1);
					BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
				}
				else
				{
					s3eSoundChannelSetInt(0, S3E_CHANNEL_RATE, ((SoundData*) sound)->SamplesPerSec);
					s3eResult Result = s3eSoundChannelPlay(0, ((SoundData*) sound)->GetSoundData(), ((SoundData*) sound)->GetSoundDataCount(), 1, 0);
					BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
				}
			}
		}

		void StopMusic()
		{
			PanoramaList::Focus(PanoramaList::RemoveAll);
			SoundData* CurSound = (SoundData*) SoundData::ChannelState(0, false);
			if(CurSound)
			{
				if(CurSound->IsSoundDataByAudio()) s3eAudioStop();
				else s3eSoundChannelStop(0);
				CurSound->UnlockSoundData();
			}
			SoundData::ChannelState(0, false) = null;
		}

		void SetMusicVolume(uint percent)
		{
			global_data const int DefaultVolumeSound = s3eSoundGetInt(S3E_SOUND_VOLUME_DEFAULT);
			int VolumeSound = DefaultVolumeSound * percent / 100;
			if(VolumeSound < 0) VolumeSound = 0;
			else if(S3E_SOUND_MAX_VOLUME < VolumeSound) VolumeSound = S3E_SOUND_MAX_VOLUME;
			s3eSoundChannelSetInt(0, S3E_CHANNEL_VOLUME, VolumeSound);
			// Audio
			global_data const int DefaultVolumeAudio = s3eAudioGetInt(S3E_AUDIO_VOLUME_DEFAULT);
			int VolumeAudio = DefaultVolumeAudio * percent / 100;
			if(VolumeAudio < 0) VolumeAudio = 0;
			else if(S3E_AUDIO_MAX_VOLUME < VolumeAudio) VolumeAudio = S3E_AUDIO_MAX_VOLUME;
			s3eAudioSetInt(S3E_AUDIO_VOLUME, VolumeAudio);
		}

		void PlayEffect(id_sound sound, bool interrupt)
		{
			int Channel = SoundData::GetFreeEffectChannel();
			if(Channel) interrupt = true;
			else if(!(Channel = SoundData::GetSameEffectChannel(sound)))
				Channel = SoundData::GetImminentEffectChannel();
			if(interrupt)
			{
				SoundData::EffectChannelStop(Channel);
				s3eResult Result = SoundData::EffectChannelPlay(Channel, sound);
				BxAssert("BxCore::Sound<해당 사운드를 출력할 수 없습니다>", Result != S3E_RESULT_ERROR);
			}
			else SoundData::EffectChannelPlayReserve(Channel, sound);
		}

		void StopEffect()
		{
			for(int i = 1; i < SoundData::GetChannelCount(); ++i)
				SoundData::EffectChannelStop(i);
		}

		void SetEffectVolume(uint percent)
		{
			global_data const int DefaultVolume = s3eSoundGetInt(S3E_SOUND_VOLUME_DEFAULT);
			int Volume = DefaultVolume * percent / 100;
			if(Volume < 0) Volume = 0;
			if(S3E_SOUND_MAX_VOLUME < Volume) Volume = S3E_SOUND_MAX_VOLUME;
			for(int i = 1; i < SoundData::GetChannelCount(); ++i)
				s3eSoundChannelSetInt(i, S3E_CHANNEL_VOLUME, Volume);
		}

		void PauseAll()
		{
			s3eSoundPauseAllChannels();
			s3eAudioPause();
		}

		void ResumeAll()
		{
			s3eSoundResumeAllChannels();
			s3eAudioResume();
		}
	}
	namespace Util
	{
		id_memory Malloc(int size)
		{
			return (id_memory) s3eMalloc(size);
		}

		void Free(id_memory item)
		{
			s3eFree((void*) item);
		}

		void* GetPtr(id_memory item)
		{
			return (void*) item;
		}

		void MemMove(const void* dst, const void* src, int length)
		{
			if(dst) memmove((void*) dst, src, length);
		}

		void MemSet(const void* dst, byte value, int length)
		{
			if(dst) memset((void*) dst, value, length);
		}

		int Printf(string src, ...)
		{
			#if defined(__GNUC__) || defined(__ARMCC_VERSION) || !defined(_DEBUG)
			return 0;
			#else
			va_list List;
			va_start(List, src);
			int Result = vprintf(src, List);
			va_end(List);
			return Result;
			#endif
		}

		int SnPrintf(string_rw dst, int length, string src, ...)
		{
			va_list List;
			va_start(List, src);
			const int Result = vsnprintf(dst, length, src, List);
			BxAssert("BxCore::Util<DST의 길이가 부족합니다>", !dst || (0 <= Result && Result < length));
			BxAssert("BxCore::Util<DST의 예상길이를 얻는데 실패하였습니다>", dst || 0 <= Result);
			va_end(List);
			return Result;
		}

		int VsnPrintf(string_rw dst, int length, string src, va_list list)
		{
			const int Result = vsnprintf(dst, length, src, list);
			BxAssert("BxCore::Util<DST의 길이가 부족합니다>", !dst || (0 <= Result && Result < length));
			BxAssert("BxCore::Util<DST의 예상길이를 얻는데 실패하였습니다>", dst || 0 <= Result);
			return Result;
		}

		uint __CallStack__[20480] = {0,};
		uint* __CallStackFocus__ = __CallStack__;
		uint __CallCount__ = 0;

		#if !defined(__GNUC__) && !defined(__ARMCC_VERSION)
		// 컴파일옵션필요(/Gh)
		extern "C" void __declspec(naked) _cdecl _penter(void)
		{
			_asm
			{
				push edi
				push esi
					mov edi, __CallStackFocus__
					mov esi, [esp] + 8
					mov [edi], esi
					mov esi, __CallCount__
					mov [edi + 4], esi
				pop esi
				pop edi
				inc __CallCount__
				add __CallStackFocus__, 8
				ret
			}
		}
		// 컴파일옵션필요(/GH)
		extern "C" void __declspec(naked) _cdecl _pexit(void)
		{
			_asm
			{
				sub __CallStackFocus__, 8
				ret
			}
		}
		#endif

		uint GetCurrentCallDepth()
		{
			if(!__CallCount__) return 0;
			return ((__CallStackFocus__ - __CallStack__) >> 1) - 2;
		}

		void* GetCurrentCallAddress()
		{
			if(!__CallCount__) return null;
			return (void*) __CallStackFocus__[-4];
		}

		uint GetCurrentCallCount()
		{
			if(!__CallCount__) return 0;
			return __CallStackFocus__[-3];
		}

		void SetCallCount(uint value)
		{
			if(!__CallCount__) return;
			__CallCount__ = value;
		}
	}
	namespace File
	{
		bool IsExist(string filename)
		{
			return (s3eFileCheckExists(filename) == true);
		}

		id_file Open(string filename, string mode)
		{
			return (id_file) s3eFileOpen(filename, mode);
		}

		void Close(id_file file)
		{
			s3eFileClose((s3eFile*) file);
		}

		uint Read(id_file file, void* buffer, uint length, callback_progress progress)
		{
			if(!progress)
				return s3eFileRead(buffer, length, 1, (s3eFile*) file);
			uint Result = 0;
			for(uint i = 0; i < length; i += 4096)
			{
				Result += s3eFileRead(&((byte*) buffer)[i], BxUtilGlobal::Min(length - i, 4096), 1, (s3eFile*) file);
				if(progress && !progress(-1, 0)) return Result; // 업데이트
			}
			return Result;
		}

		uint Write(id_file file, const void* buffer, uint length, callback_progress progress)
		{
			if(!progress)
				return s3eFileWrite(buffer, length, 1, (s3eFile*) file);
			uint Result = 0;
			for(uint i = 0; i < length; i += 4096)
			{
				Result += s3eFileWrite(&((const byte*) buffer)[i], BxUtilGlobal::Min(length - i, 4096), 1, (s3eFile*) file);
				if(progress && !progress(-1, 0)) return Result; // 업데이트
			}
			return Result;
		}

		uint WriteString(id_file file, string buffer, callback_progress progress)
		{
			return Write(file, buffer, BxUtilGlobal::StrLen(buffer), progress);
		}

		int Skip(id_file file, uint length)
		{
			return s3eFileSeek((s3eFile*) file, length, S3E_FILESEEK_CUR);
		}

		int GetSize(id_file file)
		{
			return s3eFileGetSize((s3eFile*) file);
		}

		int GetSize(string filename)
		{
			s3eFile* File = s3eFileOpen(filename, "rb");
			const int Result = (File)? s3eFileGetSize(File) : 0;
			if(File) s3eFileClose(File);
			return Result;
		}

		bool Attach(string archivename)
		{
			return (((s3eResult) dzArchiveAttach(archivename)) == S3E_RESULT_SUCCESS);
		}

		void Detach()
		{
			dzArchiveDetach();
		}

		void MakeDirectory(string dirname, bool isfile)
		{
			char PathName[1024];
			PathName[0] = '\0';
			for(int i = 0; dirname[i]; ++i)
			{
				if(dirname[i] == '/' || dirname[i] == '\\')
				{
					if(*PathName) s3eFileMakeDirectory(PathName);
					PathName[i] = '/';
				}
				else PathName[i] = dirname[i];
				PathName[i + 1] = '\0';
			}
			if(*PathName && !isfile)
				s3eFileMakeDirectory(PathName);
		}

		void DeleteFile(string filename)
		{
			s3eFileDelete(filename);
		}
	}
	namespace Socket
	{
		class SocketData
		{
			s3eSocket* Client;
			s3eSocket* Server;
			socketstate State;
		public:
			SocketData()
			{
				Client = null;
				Server = null;
				State = socketstate_null;
				ResetClient();
			}
			virtual ~SocketData()
			{
				Disconnect();
			}
			inline socketstate GetState()
			{
				return State;
			}
			inline void SetState(socketstate state)
			{
				State = state;
			}
			inline s3eSocket* GetClient()
			{
				return Client;
			}
			inline void ResetClient()
			{
				Disconnect();
				Client = s3eSocketCreate(S3E_SOCKET_TCP);
				State = (Client)? socketstate_created : socketstate_null;
			}
			inline s3eSocket* GetServer()
			{
				return Server;
			}
			void SetServer(s3eSocket* server)
			{
				if(Server = server)
					State = socketstate_connected;
				else State = socketstate_disconnected;
			}
			inline void Disconnect()
			{
				if(Client) s3eSocketClose(Client);
				Client = null;
				Server = null;
				State = socketstate_null;
			}
		};

		id_socket Create()
		{
			SocketData* Data = BxNew(SocketData);
			if(Data->GetState() == socketstate_created)
				return (id_socket) Data;
			BxDelete(Data);
			return null;
		}

		void Release(id_socket sock)
		{
			if(sock)
				BxDelete_ByType(SocketData, sock);
		}

		socketstate GetState(id_socket sock)
		{
			if(sock)
				return ((SocketData*) sock)->GetState();
			return socketstate_null;
		}

		int32 _ConnectCB(s3eSocket* s, void* systemData, void* userData)
		{
			if(userData)
				((SocketData*) userData)->SetServer(s);
			return 0;
		}

		bool _Error(id_socket sock)
		{
			switch(s3eSocketGetError())
			{
			case S3E_SOCKET_ERR_PARAM:
			case S3E_SOCKET_ERR_SHUTDOWN: ((SocketData*) sock)->SetState(socketstate_null); break;
			case S3E_SOCKET_ERR_WOULDBLOCK: return true;
			case S3E_SOCKET_ERR_NOTCONN: ((SocketData*) sock)->SetState(socketstate_disconnected); break;
			}
			return false;
		}

		connectresult Connect(id_socket sock, string addr, ushort port, uint timeout, callback_progress progress)
		{
			if(!sock) return connect_error_param;
			else if(((SocketData*) sock)->GetState() == socketstate_null)
			{
				((SocketData*) sock)->ResetClient();
				if(((SocketData*) sock)->GetState() != socketstate_created)
					return connect_error_param;
			}
			else if(((SocketData*) sock)->GetState() == socketstate_connecting) return connect_connecting;
			else if(((SocketData*) sock)->GetState() == socketstate_connected) return connect_connected;
			s3eInetAddress SockAddr = {0,};
			hostent* HostEnt = gethostbyname(addr);
			BxAssert("BxCore::Socket<알 수 없는 도메인입니다>", HostEnt);
			SockAddr.m_IPAddress = *((uint*) HostEnt->h_addr_list[0]);
			SockAddr.m_Port = ((port & 0x00FF) << 8) | ((port & 0xFF00) >> 8);
			((SocketData*) sock)->SetState(socketstate_connecting);
			s3eResult Result = s3eSocketConnect(((SocketData*) sock)->GetClient(), &SockAddr, _ConnectCB, sock);
			if(Result == S3E_RESULT_ERROR)
			{
				switch(s3eSocketGetError())
				{
				case S3E_SOCKET_ERR_PARAM: ((SocketData*) sock)->SetState(socketstate_disconnected); return connect_error_param;
				case S3E_SOCKET_ERR_ALREADY: ((SocketData*) sock)->SetState(socketstate_disconnected); return connect_error_already;
				case S3E_SOCKET_ERR_NETDOWN: ((SocketData*) sock)->SetState(socketstate_disconnected); return connect_error_netdown;
				case S3E_SOCKET_ERR_TIMEDOUT: ((SocketData*) sock)->SetState(socketstate_disconnected); return connect_error_timeout;
				case S3E_SOCKET_ERR_INPROGRESS: break;
				case S3E_SOCKET_ERR_CONNREFUSED: ((SocketData*) sock)->SetState(socketstate_disconnected); return connect_error_refused;
				default: ((SocketData*) sock)->SetState(socketstate_disconnected); return connect_error_unknown;
				}
			}
			// 결과확인
			if(0 < timeout)
			{
				uhuge LimitTime = BxCore::System::GetTimerMs() + timeout;
				while(BxCore::System::GetTimerMs() < LimitTime && ((SocketData*) sock)->GetState() == socketstate_connecting)
				{
					if(progress && !progress(-1, 0)) // 업데이트
						return connect_error_unknown;
					BxCore::System::Sleep(10);
				}
			}
			else BxCore::System::Sleep(0);
			switch(((SocketData*) sock)->GetState())
			{
			case socketstate_connecting: return connect_connecting;
			case socketstate_connected: return connect_connected;
			case socketstate_disconnected: return connect_disconnected;
			}
			return connect_error_unknown;
		}

		void Disconnect(id_socket sock)
		{
			if(!sock) return;
			((SocketData*) sock)->Disconnect();
		}

		int Send(id_socket sock, const void* buffer, int len)
		{
			if(!sock || ((SocketData*) sock)->GetState() != socketstate_connected) return -1;
			int Result = s3eSocketSend(((SocketData*) sock)->GetServer(), (string) buffer, len, 0);
			if(Result < 0 && _Error(sock)) return 0;
			return Result;
		}

		int Recv(id_socket sock, void* buffer, int len)
		{
			if(!sock || ((SocketData*) sock)->GetState() != socketstate_connected) return -1;
			int Result = s3eSocketRecv(((SocketData*) sock)->GetServer(), (string_rw) buffer, len, 0);
			if(Result < 0 && _Error(sock)) return 0;
			return Result;
		}
	}
	namespace Font
	{
		local_func int& RefCount() {global_data int Count = 0; return Count;}
		local_func CIwGxFont*& TempFont() {global_data CIwGxFont* Font = 0; return Font;} // GxFont버그회피용

		id_font Open(string filename, const int pointsize, const int numCacheGlyphs)
		{
			#ifndef __BX_OPENGL
			return null;
			#else
			if(++RefCount() == 1)
			{
				IwGxFontInit();
				IwGxFontResetState();
				IwGxFontSetEncoding(IW_GX_FONT_CP949);
				IwGxFontInitEncodings();
				TempFont() = IwGxFontCreateTTFont(filename, 1, -1);
			}
			CIwGxFont* NewFont = IwGxFontCreateTTFont(filename, pointsize, numCacheGlyphs);
			if(!NewFont)
			{
				if(--RefCount() == 0)
				{
					IwGxFontDestroyTTFont(TempFont());
					IwGxFontTerminate();
				}
			}
			return (id_font) NewFont;
			#endif
		}

		id_font Open(const byte* buffer, const int buffersize, const int pointsize, const int numCacheGlyphs)
		{
			#ifndef __BX_OPENGL
			return null;
			#else
			if(++RefCount() == 1)
			{
				IwGxFontInit();
				IwGxFontResetState();
				IwGxFontSetEncoding(IW_GX_FONT_CP949);
				IwGxFontInitEncodings();
				TempFont() = IwGxFontCreateTTFontFromBuffer(buffer, buffersize, 1, -1);
			}
			CIwGxFont* NewFont = IwGxFontCreateTTFontFromBuffer(buffer, buffersize, pointsize, numCacheGlyphs);
			if(!NewFont)
			{
				if(--RefCount() == 0)
				{
					IwGxFontDestroyTTFont(TempFont());
					IwGxFontTerminate();
				}
			}
			return (id_font) NewFont;
			#endif
		}

		void Close(id_font font)
		{
			if(!font) return;
			IwGxFontDestroyTTFont((CIwGxFont*) font);
			if(--RefCount() == 0)
			{
				IwGxFontDestroyTTFont(TempFont());
				IwGxFontTerminate();
			}
		}

		void SetSort(fontsort sort)
		{
			#ifdef __BX_OPENGL
			switch(sort)
			{
			case fontsort_pad1: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_LEFT); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_BOTTOM); break;
			case fontsort_pad2: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_BOTTOM); break;
			case fontsort_pad3: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_RIGHT); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_BOTTOM); break;
			case fontsort_pad4: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_LEFT); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE); break;
			case fontsort_pad5: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE); break;
			case fontsort_pad6: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_RIGHT); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE); break;
			case fontsort_pad7: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_LEFT); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_TOP); break;
			case fontsort_pad8: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_TOP); break;
			case fontsort_pad9: IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_RIGHT); IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_TOP); break;
			}
			#endif
		}

		void SetStyle(bool underline, bool italic)
		{
			#ifdef __BX_OPENGL
			if(underline) IwGxFontSetFlags(IW_GX_FONT_UNDERLINE_F);
			else IwGxFontClearFlags(IW_GX_FONT_UNDERLINE_F);
			if(italic) IwGxFontSetFlags(IW_GX_FONT_ITALIC_F);
			else IwGxFontClearFlags(IW_GX_FONT_ITALIC_F);
			#endif
		}

		size GetSize(id_font font, string str, const size s)
		{
			size Size = {0, 0};
			if(!font || !str) return Size;
			CIwGxFontPreparedData Data;
			IwGxFontSetFont((CIwGxFont*) font);
			IwGxFontSetRect(CIwRect(0, 0, s.w, s.h));
			IwGxFontPrepareText(Data, str);
			Size.w = Data.GetWidth();
			Size.h = Data.GetHeight();
			return Size;
		}

		void Draw(id_font font, string str, const point p, const size s, const color_x888 color, const byte opacity)
		{
			if(!font || !str) return;
			global_data id_font LastFont = null;
			global_data color_x888 LastColor = 0;
			global_data byte LastOpacity = 0xFF;
			if(LastFont != font || LastColor != color || LastOpacity != opacity)
			{
				LastFont = font;
				LastColor = color;
				LastOpacity = opacity;
				for(int i = 0, iend = ((CIwGxFont*) font)->GetNumberMaterials(); i < iend; ++i)
					((CIwGxFont*) font)->GetMaterial(i)->SetColAmbient((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, opacity);
			}
			IwGxFontSetFont((CIwGxFont*) font);
			IwGxFontSetRect(CIwRect(p.x, p.y, s.w, s.h));

			BxCore::Surface::SetRenderMode(rendermode_2d);
			IwGxFontDrawText(str);
		}
	}
	namespace Library
	{
		id_library Open(string filename)
		{
			return (id_library) s3eExtLibraryOpen(filename);
		}

		void Close(id_library handle)
		{
			s3eExtLibraryClose((s3eDLLHandle*) handle);
		}

		void* Link(id_library handle, string name)
		{
			return s3eExtLibraryGetSymbol((s3eDLLHandle*) handle, name);
		}
	}

	#ifdef __BX_OPENGL
	namespace OpenGL2D
	{
		// OpenGLForm
		class OpenGLForm
		{
		public:
			enum OpenGLFormType {IMAGE, COLOR} Type;
			OpenGLForm(OpenGLFormType type) : Type(type) {}
			virtual ~OpenGLForm() {}
		};
		class TextureMaterial : public OpenGLForm
		{
		public:
			CIwTexture Texture;
			CIwMaterial Material;
			TextureMaterial() : OpenGLForm(IMAGE) {}
			virtual ~TextureMaterial() {}
		};
		class ColorAmbient : public OpenGLForm
		{
		public:
			color_x888 Color;
			CIwMaterial Material;
			ColorAmbient() : OpenGLForm(COLOR) {}
			virtual ~ColorAmbient() {}
		};

		// OpenGLOutline
		class OpenGLOutline
		{
		public:
			enum OpenGLOutlineType {RECT, POLYGON, POLYGON3D} Type;
			OpenGLOutline(OpenGLOutlineType type) : Type(type) {}
			virtual ~OpenGLOutline() {}
		};
		class VertexUV : public OpenGLOutline
		{
		public:
			pointf Vertex[4];
			CIwSVec2 UV[4];
			VertexUV() : OpenGLOutline(RECT) {}
			virtual ~VertexUV() {}
		};
		class PolyVertexUV : public OpenGLOutline
		{
		public:
			int Count;
			pointf* Vertex;
			CIwSVec2* UV;
			PolyVertexUV(int count) : OpenGLOutline(POLYGON)
			{
				Count = count;
				Vertex = BxNew_Array(pointf, count);
				UV = BxNew_Array(CIwSVec2, count);
			}
			virtual ~PolyVertexUV()
			{
				BxDelete_Array(Vertex);
				BxDelete_Array(UV);
			}
		};
		class PolyVertexUV3D : public OpenGLOutline
		{
		public:
			int Count;
			CIwSVec3* Vertex;
			CIwSVec2* UV;
			PolyVertexUV3D(int count) : OpenGLOutline(POLYGON3D)
			{
				Count = count;
				Vertex = BxNew_Array(CIwSVec3, count);
				UV = BxNew_Array(CIwSVec2, count);
			}
			virtual ~PolyVertexUV3D()
			{
				BxDelete_Array(Vertex);
				BxDelete_Array(UV);
			}
		};

		void Init()
		{
			IwGxInit();
			IwGxSetPerspMul(0x100);
			IwGxSetFarZNearZ(0x400, 0x10);
			CIwMat ViewMatrix = CIwMat::g_Identity;
			ViewMatrix.t.z = -0x100;
			IwGxSetViewMatrix(&ViewMatrix);
		}

		void Quit()
		{
			IwGxTerminate();
		}

		void SetFarZNearZ(int far, int near)
		{
			IwGxSetFarZNearZ(far, near);
		}

		void SetViewRotX(int angle)
		{
			CIwMat ViewMatrix = CIwMat::g_Identity;
			ViewMatrix.SetRotX(angle);
			IwGxSetViewMatrix(&ViewMatrix);
		}

		void SetViewRotY(int angle)
		{
			CIwMat ViewMatrix = CIwMat::g_Identity;
			ViewMatrix.SetRotY(angle);
			IwGxSetViewMatrix(&ViewMatrix);
		}

		void SetViewRotZ(int angle)
		{
			CIwMat ViewMatrix = CIwMat::g_Identity;
			ViewMatrix.SetRotZ(angle);
			IwGxSetViewMatrix(&ViewMatrix);
		}

		id_opengl_form MakeForm_TextureMaterial(string filename)
		{
			TextureMaterial* Result = BxNew(TextureMaterial);
			Result->Texture.LoadFromFile(filename);
			Result->Texture.Upload();
			Result->Material.SetTexture(&Result->Texture);
			Result->Material.SetModulateMode(CIwMaterial::MODULATE_RGB);
			Result->Material.SetAlphaMode(CIwMaterial::ALPHA_BLEND);
			Result->Material.SetBlendMode(CIwMaterial::BLEND_MODULATE);
			Result->Material.SetColAmbient(0xFF, 0xFF, 0xFF, 0xFF);
			return (id_opengl_form) Result;
		}

		id_opengl_form MakeForm_TextureMaterial(color_a888* image, const int width, const int height, const int pitch)
		{
			TextureMaterial* Result = BxNew(TextureMaterial);
			Result->Texture.CopyFromBuffer(width, height, CIwImage::ARGB_8888, sizeof(color_a888) * pitch, (uint8*) image, null);
			Result->Texture.Upload();
			Result->Material.SetTexture(&Result->Texture);
			Result->Material.SetModulateMode(CIwMaterial::MODULATE_RGB);
			Result->Material.SetAlphaMode(CIwMaterial::ALPHA_BLEND);
			Result->Material.SetBlendMode(CIwMaterial::BLEND_MODULATE);
			Result->Material.SetColAmbient(0xFF, 0xFF, 0xFF, 0xFF);
			return (id_opengl_form) Result;
		}

		id_opengl_form MakeForm_ColorAmbient(const color_x888 color)
		{
			ColorAmbient* Result = BxNew(ColorAmbient);
			Result->Color = color;
			Result->Material.SetAlphaMode(CIwMaterial::ALPHA_BLEND);
			Result->Material.SetBlendMode(CIwMaterial::BLEND_MODULATE);
			Result->Material.SetColAmbient((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 0xFF);
			return (id_opengl_form) Result;
		}

		void FreeForm(id_opengl_form form)
		{
			OpenGLForm* Form = (OpenGLForm*) form;
			BxDelete(Form);
		}

		id_opengl_outline MakeOutline_VertexUV(const rect& r, const rect& uvclip)
		{
			VertexUV* Result = BxNew(VertexUV);
			// Vertex
			Result->Vertex[0].x = r.l << 16;
			Result->Vertex[0].y = r.t << 16;
			Result->Vertex[1].x = r.l << 16;
			Result->Vertex[1].y = r.b << 16;
			Result->Vertex[2].x = r.r << 16;
			Result->Vertex[2].y = r.b << 16;
			Result->Vertex[3].x = r.r << 16;
			Result->Vertex[3].y = r.t << 16;
			// UV
			Result->UV[0] = CIwSVec2(uvclip.l, uvclip.t);
			Result->UV[1] = CIwSVec2(uvclip.l, uvclip.b);
			Result->UV[2] = CIwSVec2(uvclip.r, uvclip.b);
			Result->UV[3] = CIwSVec2(uvclip.r, uvclip.t);
			return (id_opengl_outline) Result;
		}

		id_opengl_outline MakeOutline_PolyVertexUV(const points& p, const rect& uvclip)
		{
			PolyVertexUV* Result = BxNew_Param(PolyVertexUV, p.count);
			BxAssert("BxCore::OpenGL2D<파라미터 p의 count는 3 이상이어야 합니다>", 3 <= p.count);
			// Vertex
			rectf Bound = {0, 0, 0, 0};
			const bool IsClockwise = BxUtil::IsClockwise(p.pt[0], p.pt[1], p.pt[2]);
			for(int i = 0; i < p.count; ++i)
			{
				const int j = (IsClockwise)? (p.count - i) % p.count : i;
				Result->Vertex[i].x = p.pt[j].x << 16;
				Result->Vertex[i].y = p.pt[j].y << 16;
				Bound.l = (i == 0)? Result->Vertex[i].x : BxUtilGlobal::Min(Bound.l, Result->Vertex[i].x);
				Bound.t = (i == 0)? Result->Vertex[i].y : BxUtilGlobal::Min(Bound.t, Result->Vertex[i].y);
				Bound.r = (i == 0)? Result->Vertex[i].x : BxUtilGlobal::Max(Bound.r, Result->Vertex[i].x);
				Bound.b = (i == 0)? Result->Vertex[i].y : BxUtilGlobal::Max(Bound.b, Result->Vertex[i].y);
			}
			if(Bound.r <= Bound.l || Bound.b <= Bound.t)
			{
				BxDelete(Result);
				return null;
			}
			// UV
			for(int i = 0; i < p.count; ++i)
				Result->UV[i] = CIwSVec2(
					uvclip.l + (short)(((uvclip.r - uvclip.l)) * (huge)(Result->Vertex[i].x - Bound.l) / (Bound.r - Bound.l)),
					uvclip.t + (short)(((uvclip.b - uvclip.t)) * (huge)(Result->Vertex[i].y - Bound.t) / (Bound.b - Bound.t)));
			return (id_opengl_outline) Result;
		}

		id_opengl_outline MakeOutline_PolyVertexUV3D(const vertexs& v, const points& uv)
		{
			PolyVertexUV3D* Result = BxNew_Param(PolyVertexUV3D, v.count);
			BxAssert("BxCore::OpenGL2D<파라미터 v의 count는 3 이상이어야 합니다>", 3 <= v.count);
			BxAssert("BxCore::OpenGL2D<파라미터 v와 uv의 count는 동일해야 합니다>", v.count == uv.count);
			// Vertex
			for(int i = 0; i < v.count; ++i)
			{
				Result->Vertex[i].x = v.vt[i].x;
				Result->Vertex[i].y = v.vt[i].y;
				Result->Vertex[i].z = v.vt[i].z;
			}
			// UV
			for(int i = 0; i < uv.count; ++i)
				Result->UV[i] = CIwSVec2(uv.pt[i].x, uv.pt[i].y);
			return (id_opengl_outline) Result;
		}

		void FreeOutline(id_opengl_outline outline)
		{
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			BxDelete(Outline);
		}

		color_x888 Form_ColorAmbient_GetColor(id_opengl_form form)
		{
			OpenGLForm* Form = (OpenGLForm*) form;
			BxAssert("BxCore::OpenGL2D<파라미터가 ColorAmbient형이 아닙니다>", Form->Type == OpenGLForm::COLOR);
			return ((ColorAmbient*) Form)->Color;
		}

		void Outline_VertexUV_SetScale(id_opengl_outline outline, fint hoz, fint ver)
		{
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			BxAssert("BxCore::OpenGL2D<파라미터가 VertexUV형이 아닙니다>", Outline->Type == OpenGLOutline::RECT);
			if(hoz == 0x10000 && ver == 0x10000) return;
			VertexUV* VUV = (VertexUV*) Outline;
			for(int i = 0; i < 4; ++i)
			{
				VUV->Vertex[i].x = (fint)((VUV->Vertex[i].x * (huge) hoz + 0x8000) >> 16);
				VUV->Vertex[i].y = (fint)((VUV->Vertex[i].y * (huge) ver + 0x8000) >> 16);
			}
		}

		void Outline_VertexUV_SetRotate(id_opengl_outline outline, int angle1024, bool doFlip)
		{
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			BxAssert("BxCore::OpenGL2D<파라미터가 VertexUV형이 아닙니다>", Outline->Type == OpenGLOutline::RECT);
			if(angle1024 == 0 && !doFlip) return;
			VertexUV* VUV = (VertexUV*) Outline;
			const huge Cos = BxUtil::Cos(angle1024);
			const huge Sin = BxUtil::Sin(angle1024);
			fint MinX = 0, MaxX = 0;
			if(doFlip)
			for(int i = 0; i < 4; ++i)
			{
				MinX = (i == 0)? VUV->Vertex[i].x : BxUtilGlobal::Min(MinX, VUV->Vertex[i].x);
				MaxX = (i == 0)? VUV->Vertex[i].x : BxUtilGlobal::Max(MaxX, VUV->Vertex[i].x);
			}
			for(int i = 0; i < 4; ++i)
			{
				const fint TempX = (doFlip)? MinX + MaxX - VUV->Vertex[i].x : VUV->Vertex[i].x;
				VUV->Vertex[i].x = (fint)((TempX * Cos - VUV->Vertex[i].y * Sin + 0x8000) >> 16);
				VUV->Vertex[i].y = (fint)((TempX * Sin + VUV->Vertex[i].y * Cos + 0x8000) >> 16);
			}
		}

		void Outline_PolyVertexUV_SetScale(id_opengl_outline outline, fint hoz, fint ver)
		{
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			BxAssert("BxCore::OpenGL2D<파라미터가 PolyVertexUV형이 아닙니다>", Outline->Type == OpenGLOutline::POLYGON);
			if(hoz == 0x10000 && ver == 0x10000) return;
			PolyVertexUV* PolyVUV = (PolyVertexUV*) Outline;
			for(int i = 0; i < PolyVUV->Count; ++i)
			{
				PolyVUV->Vertex[i].x = (fint)((PolyVUV->Vertex[i].x * (huge) hoz + 0x8000) >> 16);
				PolyVUV->Vertex[i].y = (fint)((PolyVUV->Vertex[i].y * (huge) ver + 0x8000) >> 16);
			}
		}

		void Outline_PolyVertexUV_SetRotate(id_opengl_outline outline, int angle1024, bool doFlip)
		{
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			BxAssert("BxCore::OpenGL2D<파라미터가 PolyVertexUV형이 아닙니다>", Outline->Type == OpenGLOutline::POLYGON);
			if(angle1024 == 0 && !doFlip) return;
			PolyVertexUV* PolyVUV = (PolyVertexUV*) Outline;
			const huge Cos = BxUtil::Cos(angle1024);
			const huge Sin = BxUtil::Sin(angle1024);
			fint MinX = 0, MaxX = 0;
			if(doFlip)
			for(int i = 0; i < PolyVUV->Count; ++i)
			{
				MinX = (i == 0)? PolyVUV->Vertex[i].x : BxUtilGlobal::Min(MinX, PolyVUV->Vertex[i].x);
				MaxX = (i == 0)? PolyVUV->Vertex[i].x : BxUtilGlobal::Max(MaxX, PolyVUV->Vertex[i].x);
			}
			for(int i = 0; i < PolyVUV->Count; ++i)
			{
				const fint TempX = (doFlip)? MinX + MaxX - PolyVUV->Vertex[i].x : PolyVUV->Vertex[i].x;
				PolyVUV->Vertex[i].x = (fint)((TempX * Cos - PolyVUV->Vertex[i].y * Sin + 0x8000) >> 16);
				PolyVUV->Vertex[i].y = (fint)((TempX * Sin + PolyVUV->Vertex[i].y * Cos + 0x8000) >> 16);
			}
		}

		void _RenderRect(OpenGLForm* TM, VertexUV* VUV, int x, int y, const byte opacity, const color_x888 color)
		{
			BxCore::Surface::SetRenderMode(rendermode_2d);
			// Material
			if(TM->Type == OpenGLForm::IMAGE)
			{
				CIwMaterial& Material = ((TextureMaterial*) TM)->Material;
				CIwColour& Color = Material.GetColAmbient();
				Color.a = opacity;
				Color.r = (color >> 16) & 0xFF;
				Color.g = (color >>  8) & 0xFF;
				Color.b = (color >>  0) & 0xFF;
				Material.SetColAmbient(Color);
				if(Material.GetCullMode() != CIwMaterial::CULL_NONE)
					Material.SetCullMode(CIwMaterial::CULL_NONE);
				IwGxSetMaterial(&Material);
			}
			else if(TM->Type == OpenGLForm::COLOR)
			{
				CIwMaterial& Material = ((ColorAmbient*) TM)->Material;
				CIwColour& Color = Material.GetColAmbient();
				Color.a = opacity;
				Color.r = (color >> 16) & 0xFF;
				Color.g = (color >>  8) & 0xFF;
				Color.b = (color >>  0) & 0xFF;
				Material.SetColAmbient(Color);
				if(Material.GetCullMode() != CIwMaterial::CULL_NONE)
					Material.SetCullMode(CIwMaterial::CULL_NONE);
				IwGxSetMaterial(&Material);
			}
			// Vertex
			CIwSVec2* Vertex = IW_GX_ALLOC(CIwSVec2, 4);
			for(int i = 0; i < 4; ++i)
			{
				Vertex[i].x = ((VUV->Vertex[i].x + 0x8000) >> 16) + x;
				Vertex[i].y = ((VUV->Vertex[i].y + 0x8000) >> 16) + y;
			}
			IwGxSetVertStreamScreenSpace(Vertex, 4);
			// UV
			if(TM->Type == OpenGLForm::IMAGE)
			{
				const int RateX = (1 << 11) * (((TextureMaterial*) TM)->Texture.GetWidth() - GetTextureMargin() * 2) / ((TextureMaterial*) TM)->Texture.GetWidth();
				const int RateY = (1 << 11) * (((TextureMaterial*) TM)->Texture.GetHeight() - GetTextureMargin() * 2) / ((TextureMaterial*) TM)->Texture.GetHeight();
				CIwSVec2* UV = IW_GX_ALLOC(CIwSVec2, 4);
				for(int i = 0; i < 4; ++i)
				{
					UV[i].x = (VUV->UV[i].x - (1 << 11)) * RateX / (1 << 11) + (1 << 11);
					UV[i].y = (VUV->UV[i].y - (1 << 11)) * RateY / (1 << 11) + (1 << 11);
				}
				IwGxSetUVStream(UV);
			}
			else IwGxSetUVStream(VUV->UV);
			IwGxDrawPrims(IW_GX_QUAD_LIST, null, 4);
		}

		void _RenderPoly(OpenGLForm* TM, PolyVertexUV* PolyVUV, int x, int y, const byte opacity, const color_x888 color)
		{
			BxCore::Surface::SetRenderMode(rendermode_2d);
			// Material
			if(TM->Type == OpenGLForm::IMAGE)
			{
				CIwMaterial& Material = ((TextureMaterial*) TM)->Material;
				CIwColour& Color = Material.GetColAmbient();
				Color.a = opacity;
				Color.r = (color >> 16) & 0xFF;
				Color.g = (color >>  8) & 0xFF;
				Color.b = (color >>  0) & 0xFF;
				Material.SetColAmbient(Color);
				if(Material.GetCullMode() != CIwMaterial::CULL_NONE)
					Material.SetCullMode(CIwMaterial::CULL_NONE);
				IwGxSetMaterial(&Material);
			}
			else if(TM->Type == OpenGLForm::COLOR)
			{
				CIwMaterial& Material = ((ColorAmbient*) TM)->Material;
				CIwColour& Color = Material.GetColAmbient();
				Color.a = opacity;
				Color.r = (color >> 16) & 0xFF;
				Color.g = (color >>  8) & 0xFF;
				Color.b = (color >>  0) & 0xFF;
				Material.SetColAmbient(Color);
				if(Material.GetCullMode() != CIwMaterial::CULL_NONE)
					Material.SetCullMode(CIwMaterial::CULL_NONE);
				IwGxSetMaterial(&Material);
			}
			// Vertex
			CIwSVec2* Vertex = IW_GX_ALLOC(CIwSVec2, PolyVUV->Count);
			for(int i = 0; i < PolyVUV->Count; ++i)
			{
				Vertex[i].x = ((PolyVUV->Vertex[i].x + 0x8000) >> 16) + x;
				Vertex[i].y = ((PolyVUV->Vertex[i].y + 0x8000) >> 16) + y;
			}
			IwGxSetVertStreamScreenSpace(Vertex, PolyVUV->Count);
			// UV
			if(TM->Type == OpenGLForm::IMAGE)
			{
				const int RateX = (1 << 11) * (((TextureMaterial*) TM)->Texture.GetWidth() - GetTextureMargin() * 2) / ((TextureMaterial*) TM)->Texture.GetWidth();
				const int RateY = (1 << 11) * (((TextureMaterial*) TM)->Texture.GetHeight() - GetTextureMargin() * 2) / ((TextureMaterial*) TM)->Texture.GetHeight();
				CIwSVec2* UV = IW_GX_ALLOC(CIwSVec2, PolyVUV->Count);
				for(int i = 0; i < PolyVUV->Count; ++i)
				{
					UV[i].x = (PolyVUV->UV[i].x - (1 << 11)) * RateX / (1 << 11) + (1 << 11);
					UV[i].y = (PolyVUV->UV[i].y - (1 << 11)) * RateY / (1 << 11) + (1 << 11);
				}
				IwGxSetUVStream(UV);
			}
			else IwGxSetUVStream(PolyVUV->UV);
			IwGxDrawPrims(IW_GX_QUAD_LIST, null, PolyVUV->Count);
		}

		void _RenderPoly3D(OpenGLForm* TM, PolyVertexUV3D* PolyVUV3D, int x, int y, int z, const byte opacity, const color_x888 color)
		{
			BxCore::Surface::SetRenderMode(rendermode_3d);
			// Material
			if(TM->Type == OpenGLForm::IMAGE)
			{
				CIwMaterial& Material = ((TextureMaterial*) TM)->Material;
				CIwColour& Color = Material.GetColAmbient();
				Color.a = opacity;
				Color.r = (color >> 16) & 0xFF;
				Color.g = (color >>  8) & 0xFF;
				Color.b = (color >>  0) & 0xFF;
				Material.SetColAmbient(Color);
				if(Material.GetCullMode() != CIwMaterial::CULL_FRONT)
					Material.SetCullMode(CIwMaterial::CULL_FRONT);
				IwGxSetMaterial(&Material);
			}
			else if(TM->Type == OpenGLForm::COLOR)
			{
				CIwMaterial& Material = ((ColorAmbient*) TM)->Material;
				CIwColour& Color = Material.GetColAmbient();
				Color.a = opacity;
				Color.r = (color >> 16) & 0xFF;
				Color.g = (color >>  8) & 0xFF;
				Color.b = (color >>  0) & 0xFF;
				Material.SetColAmbient(Color);
				if(Material.GetCullMode() != CIwMaterial::CULL_FRONT)
					Material.SetCullMode(CIwMaterial::CULL_FRONT);
				IwGxSetMaterial(&Material);
			}
			// Vertex
			CIwMat ModelMatrix;
			ModelMatrix.SetRotX(0);
			CIwSVec3* Move = IW_GX_ALLOC(CIwSVec3, 1);
			Move[0].x = x;
			Move[0].y = y;
			Move[0].z = z;
			ModelMatrix.SetRotX(0x800 * 180 / 360); // 테스트
			ModelMatrix.SetTrans(Move[0]);
			IwGxSetModelMatrix(&ModelMatrix);
			CIwSVec3* Vertex = IW_GX_ALLOC(CIwSVec3, 4);
			for(int i = 0; i < PolyVUV3D->Count; ++i)
			{
				Vertex[i].x = PolyVUV3D->Vertex[i].x;
				Vertex[i].y = PolyVUV3D->Vertex[i].y;
				Vertex[i].z = PolyVUV3D->Vertex[i].z;
			}
			IwGxSetVertStreamModelSpace(Vertex, PolyVUV3D->Count);
			// UV
			if(TM->Type == OpenGLForm::IMAGE)
			{
				const int RateX = (1 << 11) * (((TextureMaterial*) TM)->Texture.GetWidth() - GetTextureMargin() * 2) / ((TextureMaterial*) TM)->Texture.GetWidth();
				const int RateY = (1 << 11) * (((TextureMaterial*) TM)->Texture.GetHeight() - GetTextureMargin() * 2) / ((TextureMaterial*) TM)->Texture.GetHeight();
				CIwSVec2* UV = IW_GX_ALLOC(CIwSVec2, PolyVUV3D->Count);
				for(int i = 0; i < PolyVUV3D->Count; ++i)
				{
					UV[i].x = (PolyVUV3D->UV[i].x - (1 << 11)) * RateX / (1 << 11) + (1 << 11);
					UV[i].y = (PolyVUV3D->UV[i].y - (1 << 11)) * RateY / (1 << 11) + (1 << 11);
				}
				IwGxSetUVStream(UV);
			}
			else IwGxSetUVStream(PolyVUV3D->UV);
			IwGxDrawPrims(IW_GX_QUAD_LIST, null, PolyVUV3D->Count);
		}

		void Render(id_opengl_form form, id_opengl_outline outline, int x, int y, const byte opacity, const color_x888 color)
		{
			OpenGLForm* Form = (OpenGLForm*) form;
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			if(Outline->Type == OpenGLOutline::RECT)
				_RenderRect(Form, (VertexUV*) Outline, x, y, opacity, color);
			else if(Outline->Type == OpenGLOutline::POLYGON)
				_RenderPoly(Form, (PolyVertexUV*) Outline, x, y, opacity, color);
		}

		void Render3D(id_opengl_form form, id_opengl_outline outline, int x, int y, int z, const byte opacity, const color_x888 color)
		{
			OpenGLForm* Form = (OpenGLForm*) form;
			OpenGLOutline* Outline = (OpenGLOutline*) outline;
			if(Outline->Type == OpenGLOutline::POLYGON3D)
				_RenderPoly3D(Form, (PolyVertexUV3D*) Outline, x, y, z, opacity, color);
		}

		void Clip(rect r)
		{
			if(0 < r.r - r.l && 0 < r.b - r.t)
				_IwGxSetViewRect(r.l, r.t, r.r - r.l, r.b - r.t);
		}

		const int GetTextureMargin()
		{
			global_data int TextureMargin = -1;
			if(TextureMargin == -1)
			{
				BxCore::System::GetConfigInt("s3e", "TextureMarginBx", &TextureMargin);
				if(TextureMargin < 0) TextureMargin = 0;
			}
			return TextureMargin;
		}

		const bool DoTextureInterpolation()
		{
			global_data int IsTextureInterpolation = -1;
			if(IsTextureInterpolation == -1)
			{
				BxCore::System::GetConfigInt("s3e", "TextureInterpolationBx", &IsTextureInterpolation);
				if(IsTextureInterpolation < 0) IsTextureInterpolation = 0;
			}
			return (IsTextureInterpolation != 0);
		}
	}
	#endif
}
/// @endcond
