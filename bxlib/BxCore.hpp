#pragma once
#include "BxType.hpp"

#if defined(__GNUC__) || defined(__ARMCC_VERSION) || !defined(_DEBUG)
	#define BxAssert(STRING, CHECK) do{} while(false)
#else
	#define BxAssert(STRING, CHECK) do{global_data bool IsIgnore = false; BxCore::System::Assert(STRING, IsIgnore, (CHECK) != 0, __FILE__, __LINE__);} while(false)
#endif

//! \brief Marmalade-SDK API��������
namespace BxCore
{
	//! \brief Marmalade-SDK System��Ʈ
	namespace System
	{
		/*!
		\brief �������� ��û����
		\return true-���ᰡ ��û��, false-�����û ����
		\see DoQuit
		*/
		bool IsQuit();

		/*!
		\brief �������� ��û
		\see IsQuit
		*/
		void DoQuit();

		/*!
		\brief ���μ��� ����(�и� ����/Ű�̺�Ʈ�� ó��)
		\param ms : �� �ð�(1/1000��)
		*/
		void Sleep(int ms);

		/*!
		\brief ����ð� ���ϱ�
		\return 1/1000�� ������ ���� ����ð�
		*/
		uhuge GetTimerMs();

		/*!
		\brief Ư�� Ű�� ���� ��������
		\param key : Ȯ���� Ű��
		\return true-������, false-�ȴ�����
		*/
		bool IsKeyDown(keykind key);

		/*!
		\brief ������ icf�� �ҷ�����
		\param group : �׷��(s3e, gx��)
		\param name : �ʵ��(MemSize, DispFixRot��)
		\param value : ���Ϲ��� ����
		*/
		void GetConfigInt(string group, string name, int* value);

		/*!
		\brief ���ڿ��� icf�� �ҷ�����
		\param group : �׷��
		\param name : �ʵ��
		\param value : ���Ϲ��� ����(256¥�� �迭����)
		*/
		void GetConfigStr(string group, string name, string_rw value256);

		/*!
		\brief ������ڵ�(Macro�Լ��� BxAssert�� ������)
		\param name : ��������
		\param flag : �����ۿ���(true-������, false-��������)
		\param filename : �ش����ϸ�(__FILE__�� ���)
		\param linenumber : �ش��ٹ�ȣ(__LINE__�� ���)
		*/
		void Assert(string name, bool& IsIgnore, bool flag, string filename, int linenumber);

		/*!
		\brief OS�̸� ���ϱ�
		\return �ڱ� �÷����� OS�̸�(��: SYMBIAN, BREW, WINDOWS, WINMOBILE, LINUX, WIPI, IPHONE, ANDROID)
		*/
		string GetOSName();

		/*!
		\brief �޴�����ȣ ���ϱ�
		\return �ڽ��� �޴�����ȣ(��: 01022223333)
		*/
		string GetPhoneNumber();

		/*!
		\brief ������������
		\param url : ������ ���ּ�(��: market://search?q=pname:com.popcap.pvz, tstore://PRODUCT_VIEW/0000252470/1)
		\param exitme : ���� ���α׷��� ���Ῡ��
		*/
		void PopupOSExecute(string url, bool exitme);

		/*!
		\brief ����Ű����� ��Ʈ�����
		\param result_str256 : ���� ��Ʈ��(256¥�� �迭����)
		\param default_str : ó�� �����Ϳ� ���� ��Ʈ��
		\param keyboard_name : ����Ű������ �̸�
		\param type : Ű��������(�⺻, �̸���, ����, ���, ���ּ�)
		\return ��������(������ OS�� ���)
		*/
		bool PopupOSKeyboard(string_rw result_str256, string default_str = "", string keyboard_name = "", oskeyboardtype type = oskeyboardtype_base);

		/// @cond SECTION_NAME
		class CallbackList
		{
		public:
			callback_frame Method;
			bool IsDataRef;
			void* Data;
			CallbackList* Next;
		public:
			CallbackList(callback_frame method, void* data, int datasize);
			virtual ~CallbackList();
			global_func CallbackList* GetFirst()
			{
				global_data CallbackList First(null, null, -1);
				return &First;
			}
		};
		/// @endcond

		/*!
		\brief �����Ӵ��� ������ �ݹ���(������)
		\param method : �ݹ��Լ�(ȣ��� �ݹ��Լ����� false���Ͻ� ����)
		\param data : �����Ϳ���
		\param datasize : -1�� ������, 0���� ���ų� ũ�� �����(false���Ͻ� �Բ� ����)
		*/
		void AddCallback(callback_frame method, void* data, int datasize = -1);

		/*!
		\brief �ڽ����μ����� ����
		\param process : ���μ����� �����ų �ݹ��Լ�
		\return ���࿩��(�������� �ڽ����μ����� ����� �� �����Ƿ� false�̸� �������)
		*/
		bool BeginChildProcess(callback_process process);

		/*!
		\brief JAVAŬ���� ����Ȯ��
		\param classpath : Ŭ�����ּ�(��: com/abc/ABC)
		\return ����� true, ������ false
		*/
		bool IsExistClassJNI(string classpath);

		/*!
		\brief C��JAVA �����Լ�ȣ��
		\param classpath : Ŭ�����ּ�(��: com/abc/ABC)
		\param methodname : �Լ��̸�
		\param param1_str1024 : ������ ��Ʈ��
		\param param2 : ������ ���
		\return JAVA�Լ����ϰ�, -90001���� -90002�� JNI�������, -90003�� classpath����
		*/
		int CallStaticMethodJNI(string classpath, string methodname, string param1_str1024, int param2);

		/*!
		\brief JAVA��C �ݹ���(�ִ� 8������ ���)
		\param classpath : Ŭ�����ּ�(��: com/abc/ABC)
		\param methodname : �Լ��̸�
		\param methodCB : �ݹ��Լ�
		\return 0-����, -90001���� -90002�� JNI�������, -90003�� classpath����, -90004�� methodname����, -90005�� �ݹ��ϼ����ʰ�
		*/
		int SetCallbackJNI(string classpath, string methodname, callback_jni methodCB);
	}
	//! \brief Marmalade-SDK Surface��Ʈ
	namespace Surface
	{
		/*!
		\brief ȭ�����
		\param dir : ȸ������(0/90/180/270/Native)
		*/
		void Init(dirtype dir);

		/*!
		\brief ������Ʈ �� ����۸� ȭ�鿡 ���
		\param ptr : ȭ�鿡 ����� �̹����� ������
		\param row : �̹����� �ʺ��� ����Ʈ����
		\param height : �̹����� �ȼ�����
		\param pitch : �̹����� ���̸� ������ �ʺ��� ����Ʈ����
		*/
		void Flush(void* ptr, const int row, const int height, const int pitch);

		/*!
		\brief �߰� ������Ʈ
		*/
		void MiddleFlush();

		/*!
		\brief ȭ��ʺ� ���ϱ�
		\param isfixedvalue : icf�� SurfaceWidthBx�� ������ ���������� ��������(null�̸� ����)
		\return �ʺ��� �ȼ�����
		*/
		int GetWidth(bool* isfixedvalue = null);

		/*!
		\brief ȭ����� ���ϱ�
		\param isfixedvalue : icf�� SurfaceHeightBx�� ������ ���������� ��������(null�̸� ����)
		\return ������ �ȼ�����
		*/
		int GetHeight(bool* isfixedvalue = null);

		/*!
		\brief �ϵ������ ȭ��ʺ� ���ϱ�
		\return �ʺ��� �ȼ�����
		*/
		int GetWidthHW();

		/*!
		\brief �ϵ������ ȭ����� ���ϱ�
		\return ������ �ȼ�����
		*/
		int GetHeightHW();
	}
	//! \brief Marmalade-SDK Sound��Ʈ
	namespace Sound
	{
		/*!
		\brief ����ID �Ҵ�(icf���� [game]SoundCacheSize������ ���)
		\param filename : ���ϸ�(mid, mp3, aac, qcp, wav)
		\param autoload : �ڵ��ε������ý���
		\return ����ID(null�� ����)
		\see Release
		*/
		id_sound Create(string filename, bool autoload = true);

		/*!
		\brief ����ID ��ȯ
		\param sound : ����ID
		\see Create
		*/
		void Release(id_sound sound);

		/*!
		\brief ����� ���
		\param sound : ����ID(�ĳ�󸶽� ù��° ����ID)
		\param interrupt : ������� ����ID�� ��� ������Ű�� ���� ���
		\param panoramaCount : �ĳ�󸶷� �߰��� ����ID�� ����
		\param ... : �߰��� ����ID��
		\see StopMusic
		*/
		void PlayMusic(id_sound sound, bool interrupt = false, uint panoramaCount = 0, ...);

		/*!
		\brief ����� ����
		\see PlayMusic
		*/
		void StopMusic();

		/*!
		\brief ����� ����
		\param percent : �����(����Ʈ�� 100)
		*/
		void SetMusicVolume(uint percent);

		/*!
		\brief ȿ���� ���
		\param sound : ����ID
		\param interrupt : ������� ȿ������ ������ ����ID�� ������ ��� ������Ű�� ���� ���
		\see StopEffect
		*/
		void PlayEffect(id_sound sound, bool interrupt = false);

		/*!
		\brief ��� ȿ���� ����
		\see PlayEffect
		*/
		void StopEffect();

		/*!
		\brief ȿ���� ����
		\param percent : �����(����Ʈ�� 100)
		*/
		void SetEffectVolume(uint percent);

		/*!
		\brief ��ü �����/ȿ���� �Ͻ�����
		*/
		void PauseAll();

		/*!
		\brief ��ü �����/ȿ���� �Ͻ����� ����
		*/
		void ResumeAll();
	}
	//! \brief Marmalade-SDK Util��Ʈ
	namespace Util
	{
		/*!
		\brief �޸��Ҵ�
		\param size : ��û�� ����Ʈ����
		\return �Ҵ�� �޸�ID, ���н� 0
		\see Free, GetPtr
		*/
		id_memory Malloc(int size);

		/*!
		\brief �޸𸮹�ȯ
		\param item : ��ȯ�� �޸�ID
		\see Malloc
		*/
		void Free(id_memory item);

		/*!
		\brief �޸�ID�� ���� �޸𸮹��� ���ϱ�
		\param item : ��û�� �޸�ID
		\return �ش� �޸�ID�� �޸𸮹���
		\see Malloc
		*/
		void* GetPtr(id_memory item);

		/*!
		\brief �޸� ����
		\param dst : �ٿ����� ���� �޸𸮹���
		\param src : ������ ������ ���� �޸𸮹���
		\param length : ������ ����Ʈ����
		*/
		void MemMove(const void* dst, const void* src, int length);

		/*!
		\brief �޸� �ʱ�ȭ
		\param dst : �ʱ�ȭ�� ���� �޸𸮹���
		\param value : �ʱ�ȭ��ų ��
		\param length : �ʱ�ȭ�� ����Ʈ����
		*/
		void MemSet(const void* dst, byte value, int length);

		/*!
		\brief ���â�� ��Ʈ�����
		\param src : printf����� ��(%d/%s/...)
		\param ... : src�� ���� ��������
		\return ��ϵ� ����Ʈ����
		*/
		int Printf(string src, ...);

		/*!
		\brief ��Ʈ���� ��Ʈ�����
		\param dst : ����� ���� �޸𸮹���
		\param length : dst�� ����
		\param src : sprintf����� ��(%d/%s/...)
		\param ... : src�� ���� ��������
		\return ��ϵ� ����Ʈ����
		*/
		int SnPrintf(string_rw dst, int length, string src, ...);

		/*!
		\brief ��Ʈ���� va_list�� ��Ʈ�����
		\param dst : ����� ���� �޸𸮹���
		\param length : dst�� ����
		\param src : sprintf����� ��(%d/%s/...)
		\param list : src�� ���� va_list�� ��������
		\return ��ϵ� ����Ʈ����
		*/
		int VsnPrintf(string_rw dst, int length, string src, va_list list);

		/*!
		\brief ���� �Լ��� �ݱ�������
		\return �ݱ���
		*/
		uint GetCurrentCallDepth();

		/*!
		\brief ���� �Լ��� �����ּ�����
		\return �����ּ�
		*/
		void* GetCurrentCallAddress();

		/*!
		\brief ���� �Լ��� �ݼ�������
		\return �ݼ���
		*/
		uint GetCurrentCallCount();

		/*!
		\brief �ݼ��� �ʱ�ȭ
		\param value : �ݼ���
		*/
		void SetCallCount(uint value);
	}
	//! \brief Marmalade-SDK File��Ʈ
	namespace File
	{
		/*!
		\brief ������ ���翩��
		\param filename : ���ϸ�
		\return ����� true, ������ false
		\see Open
		*/
		bool IsExist(string filename);

		/*!
		\brief ���Ͽ���
		\param filename : ���ϸ�
		\param mode : ����ɼ�(��:"r"/"w"/"a"/"r+"/"w+"/"a+", ��:"t"/"b"/"U")
		\return ������ ����ID, ���н� 0
		\see IsExist, Close, Attach
		*/
		id_file Open(string filename, string mode);

		/*!
		\brief ���ϴݱ�
		\param file : ���� ����ID
		\see Open
		*/
		void Close(id_file file);

		/*!
		\brief ���Ͽ��� �б�
		\param file : ����ID
		\param buffer : ���� ������ ������ �޸�
		\param length : �б��û�� ����Ʈ����
		\param progress : ���α׷����� ����(������Ʈ����, progress(-1, 0)�� ȣ��)
		\return ���� ����Ʈ����
		*/
		uint Read(id_file file, void* buffer, uint length, callback_progress progress = null);

		/*!
		\brief ���Ϸ� ����
		\param file : ����ID
		\param buffer : �� ������ ��� �ִ� �޸�
		\param length : �����û�� ����Ʈ����
		\param progress : ���α׷����� ����(������Ʈ����, progress(-1, 0)�� ȣ��)
		\return �� ����Ʈ����
		*/
		uint Write(id_file file, const void* buffer, uint length, callback_progress progress = null);

		/*!
		\brief ���Ϸ� ��Ʈ������
		\param file : ����ID
		\param buffer : �� ������ ��� �ִ� ��Ʈ��
		\param progress : ���α׷����� ����(������Ʈ����, progress(-1, 0)�� ȣ��)
		\return �� ����Ʈ����
		*/
		uint WriteString(id_file file, string buffer, callback_progress progress = null);

		/*!
		\brief ������ġ ��ŵ�ϱ�
		\param file : ����ID
		\param length : ��ŵ��û�� ����Ʈ����
		\return ��� ������ġ
		*/
		int Skip(id_file file, uint length);

		/*!
		\brief ������ ��ü���� ���
		\param file : ����ID
		\return ��ü ����Ʈ����
		*/
		int GetSize(id_file file);

		/*!
		\brief ������ ��ü���� ���
		\param filename : ���ϸ�
		\return ��ü ����Ʈ����
		*/
		int GetSize(string filename);

		/*!
		\brief DZ���������� ����(������ Attach�ϸ� ������ Attach�� �ֿ켱����)
		\param archivename : �������ϸ�
		\return ������ ��������
		\see Detach
		*/
		bool Attach(string archivename);

		/*!
		\brief DZ���������� ��������(Attach�� ������ Ƚ����ŭ ����)
		\see Attach
		*/
		void Detach();

		/*!
		\brief ���������
		\param dirname : ������
		\param isfile : '/'��ȣ�� ���� ���, �������� ���������� ����
		*/
		void MakeDirectory(string dirname, bool isfile);

		/*!
		\brief ���������
		\param filename : ���ϸ�
		*/
		void DeleteFile(string filename);
	}
	//! \brief Marmalade-SDK Socket��Ʈ
	namespace Socket
	{
		/*!
		\brief ����ID �Ҵ�
		\return ����ID(null�� ����)
		\see Release, Connect
		*/
		id_socket Create();

		/*!
		\brief ����ID ��ȯ, ��������
		\param sock : ����ID
		\see Create
		*/
		void Release(id_socket sock);

		/*!
		\brief ���� ����
		\param sock : ����ID
		\return �ش� ������ ����
		*/
		socketstate GetState(id_socket sock);

		/*!
		\brief ������ ����
		\param sock : ����ID
		\param addr : ������ �Ǵ� IP
		\param port : ��Ʈ��ȣ
		\param timeout : ����� ��ٸ� �ð�
		\param progress : ���α׷����� ����(������Ʈ����, progress(-1, 0)�� ȣ��)
		\return ����� ��ٸ� ��� dowait_connected/dowait_disconnected, �� ��ٸ� ��� connecting
		\see Disconnect, GetState
		*/
		connectresult Connect(id_socket sock, string addr, ushort port, uint timeout = 0, callback_progress progress = null);

		/*!
		\brief �������� ��������
		\param sock : ����ID
		\see Connect, GetState
		*/
		void Disconnect(id_socket sock);

		/*!
		\brief ������ �۽�
		\param sock : ����ID
		\param buffer : �۽��� ����
		\param len : �۽��� ����Ʈ����
		\return �۽ŵ� ����Ʈ����(-1�� �۽ſ���, �̶� GetState�� Create/Connect�� �Ǵ�)
		\see GetState
		*/
		int Send(id_socket sock, const void* buffer, int len);

		/*!
		\brief ������ ����
		\param sock : ����ID
		\param buffer : ������ ����
		\param len : ������ ����Ʈ����
		\return ���ŵ� ����Ʈ����(-1�� ���ſ���, �̶� GetState�� Create/Connect�� �Ǵ�)
		\see GetState
		*/
		int Recv(id_socket sock, void* buffer, int len);
	}
	//! \brief Marmalade-SDK Font��Ʈ
	namespace Font
	{
		/*!
		\brief ��Ʈ����(����)
		\param filename : TTF���ϸ�
		\param pointsize : ����ũ��
		\param numCacheGlyphs : ��������� ���� ĳ�ü�(icf�� [gxfont] CacheTextureMaxSize���� �۾ƾ� ��, -1�� �ڵ�ó��)
		\return ��ƮID(null�� ����)
		\see Close
		*/
		id_font Open(string filename, const int pointsize, const int numCacheGlyphs = -1);

		/*!
		\brief ��Ʈ����(����)
		\param buffer : TTF����
		\param buffersize : TTF���ۻ�����
		\param pointsize : ����ũ��
		\param numCacheGlyphs : ��������� ���� ĳ�ü�(icf�� [gxfont] CacheTextureMaxSize���� �۾ƾ� ��, -1�� �ڵ�ó��)
		\return ��ƮID(null�� ����)
		\see Close
		*/
		id_font Open(const byte* buffer, const int buffersize, const int pointsize, const int numCacheGlyphs = -1);

		/*!
		\brief ��Ʈ�ݱ�
		\param handle : ��ƮID
		\see Open
		*/
		void Close(id_font font);

		/*!
		\brief ���Ĺ�� ����
		\param fontsort : ����Ű�е� ���
		\see SetStyle, GetSize
		*/
		void SetSort(fontsort sort);

		/*!
		\brief ��Ÿ�� ����
		\param underline : ���� ����
		\param italic : ���Ÿ�ü ����
		\see SetSort, GetSize
		*/
		void SetStyle(bool underline, bool italic);

		/*!
		\brief ������ ���
		\param font : ��ƮID
		\param str : ��Ʈ��
		\param s : ��������
		\return �ش� ��Ʈ���� ��»�����
		\see SetSort, SetStyle
		*/
		size GetSize(id_font font, string str, const size s = size::full());

		/*!
		\brief ��Ʈ�� ���
		\param font : ��ƮID
		\param str : ��Ʈ��
		\param p : �����ġ
		\param s : ��������
		\param color : ����
		\param opacity : ������
		*/
		void Draw(id_font font, string str, const point p, const size s = size::full(), const color_x888 color = 0, const byte opacity = 0xFF);
	}
	//! \brief Marmalade-SDK Library��Ʈ
	namespace Library
	{
		/*!
		\brief ���̺귯�� ����
		\param filename : ���ϸ�
		\return ���̺귯��ID(null�� ����)
		\see Close
		*/
		id_library Open(string filename);

		/*!
		\brief ���̺귯�� �ݱ�
		\param handle : ���̺귯��ID
		\see Open
		*/
		void Close(id_library handle);

		/*!
		\brief �Լ�����
		\param handle : ���̺귯��ID
		\param name : �Լ���
		\see �Լ�������
		*/
		void* Link(id_library handle, string name);
	}
	/// @cond SECTION_NAME
	#ifdef __BX_OPENGL
	namespace OpenGL2D
	{
		void Init();
		void Quit();
		// View
		void SetFarZNearZ(int far, int near);
		void SetViewRotX(int angle);
		void SetViewRotY(int angle);
		void SetViewRotZ(int angle);
		// Make/Free
		id_opengl_form MakeForm_TextureMaterial(string filename);
		id_opengl_form MakeForm_TextureMaterial(color_a888* image, const int width, const int height, const int pitch);
		id_opengl_form MakeForm_ColorAmbient(const color_x888 color);
		void FreeForm(id_opengl_form form);
		id_opengl_outline MakeOutline_VertexUV(const rect& r, const rect& uvclip);
		id_opengl_outline MakeOutline_PolyVertexUV(const points& p, const rect& uvclip);
		id_opengl_outline MakeOutline_PolyVertexUV3D(const vertexs& v, const points& uv);
		void FreeOutline(id_opengl_outline outline);
		// Get
		color_x888 Form_ColorAmbient_GetColor(id_opengl_form form);
		void Outline_VertexUV_SetScale(id_opengl_outline outline, fint hoz, fint ver);
		void Outline_VertexUV_SetRotate(id_opengl_outline outline, int angle1024, bool doFlip);
		void Outline_PolyVertexUV_SetScale(id_opengl_outline outline, fint hoz, fint ver);
		void Outline_PolyVertexUV_SetRotate(id_opengl_outline outline, int angle1024, bool doFlip);
		// Render
		void Render(id_opengl_form form, id_opengl_outline outline, int x, int y, const byte opacity, const color_x888 color);
		void Render3D(id_opengl_form form, id_opengl_outline outline, int x, int y, int z, const byte opacity, const color_x888 color);
		// Option
		void Clip(rect r);
		// Property
		const int GetTextureMargin();
		const bool DoTextureInterpolation();
	}
	#endif
	/// @endcond
}
