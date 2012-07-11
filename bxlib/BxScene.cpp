#include "BxScene.hpp"
#include "s3e.h"

local_func bool Procedure();
local_func int OnKeyboard(void* systemData, void* userData);
local_func int OnPointer(void* systemData, void* userData);
local_func int OnPointerMotion(void* systemData, void* userData);
local_func int OnPointer_Multi(void* systemData, void* userData);
local_func int OnPointerMotion_Multi(void* systemData, void* userData);

/// @cond SECTION_NAME
local_func keykind GetKeyKind(s3eKey keycode)
{
	switch(keycode)
	{
	case s3eKeyEsc: return keykind_esc;
	case s3eKeyTab: return keykind_tab;
	case s3eKeyBackspace: return keykind_bspace;
	case s3eKeyEnter: return keykind_enter;
	case s3eKeyLeftShift: return keykind_shift;
	case s3eKeyLeftControl: return keykind_ctrl;
	case s3eKeyReserved: return keykind_null;
	case s3eKeySpace: return keykind_space;
	case s3eKeyLeft: return keykind_left;
	case s3eKeyUp: return keykind_up;
	case s3eKeyRight: return keykind_right;
	case s3eKeyDown: return keykind_down;
	case s3eKey0: return keykind_0;
	case s3eKey1: return keykind_1;
	case s3eKey2: return keykind_2;
	case s3eKey3: return keykind_3;
	case s3eKey4: return keykind_4;
	case s3eKey5: return keykind_5;
	case s3eKey6: return keykind_6;
	case s3eKey7: return keykind_7;
	case s3eKey8: return keykind_8;
	case s3eKey9: return keykind_9;
	case s3eKeyA: return keykind_a;
	case s3eKeyB: return keykind_b;
	case s3eKeyC: return keykind_c;
	case s3eKeyD: return keykind_d;
	case s3eKeyE: return keykind_e;
	case s3eKeyF: return keykind_f;
	case s3eKeyG: return keykind_g;
	case s3eKeyH: return keykind_h;
	case s3eKeyI: return keykind_i;
	case s3eKeyJ: return keykind_j;
	case s3eKeyK: return keykind_k;
	case s3eKeyL: return keykind_l;
	case s3eKeyM: return keykind_m;
	case s3eKeyN: return keykind_n;
	case s3eKeyO: return keykind_o;
	case s3eKeyP: return keykind_p;
	case s3eKeyQ: return keykind_q;
	case s3eKeyR: return keykind_r;
	case s3eKeyS: return keykind_s;
	case s3eKeyT: return keykind_t;
	case s3eKeyU: return keykind_u;
	case s3eKeyV: return keykind_v;
	case s3eKeyW: return keykind_w;
	case s3eKeyX: return keykind_x;
	case s3eKeyY: return keykind_y;
	case s3eKeyZ: return keykind_z;
	case s3eKeyF1: return keykind_f1;
	case s3eKeyF2: return keykind_f2;
	case s3eKeyF3: return keykind_f3;
	case s3eKeyF4: return keykind_f4;
	case s3eKeyF5: return keykind_f5;
	case s3eKeyF6: return keykind_f6;
	case s3eKeyF7: return keykind_f7;
	case s3eKeyF8: return keykind_f8;
	case s3eKeyF9: return keykind_f9;
	case s3eKeyF10: return keykind_f10;
	case s3eKeyNumPad0: return keykind_pad_0;
	case s3eKeyNumPad1: return keykind_pad_1;
	case s3eKeyNumPad2: return keykind_pad_2;
	case s3eKeyNumPad3: return keykind_pad_3;
	case s3eKeyNumPad4: return keykind_pad_4;
	case s3eKeyNumPad5: return keykind_pad_5;
	case s3eKeyNumPad6: return keykind_pad_6;
	case s3eKeyNumPad7: return keykind_pad_7;
	case s3eKeyNumPad8: return keykind_pad_8;
	case s3eKeyNumPad9: return keykind_pad_9;
	case s3eKeyNumPadPlus: return keykind_pad_plus;
	case s3eKeyNumPadMinus: return keykind_pad_minus;
	case s3eKeyNumPadEnter: return keykind_pad_enter;
	case s3eKeyRSK: return keykind_soft_right;
	case s3eKeyLSK: return keykind_soft_left;
	case s3eKeyLS: return keykind_shoulder_left;
	case s3eKeyRS: return keykind_shoulder_right;
	case s3eKeyHash: return keykind_sharp;
	case s3eKeyStar: return keykind_star;
	case s3eKeyOk: return keykind_ok;
	case s3eKeyCLR: return keykind_clr;
	case s3eKeyVolUp: return keykind_vol_up;
	case s3eKeyVolDown: return keykind_vol_down;
	}
	return keykind_null;
}
/// @endcond

/// @cond SECTION_NAME
local_data BxDraw* Draw = null;
local_data bool IsChildProcessRun = false;
callback_process ChildProcess = null;
/// @endcond

/// @cond SECTION_NAME
int main()
{
	// Draw객체 할당
	Draw = BxNew(BxDraw);

	// 이벤트등록
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, OnKeyboard, null);
	int IsMultiTouch = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE);
	if(IsMultiTouch)
		s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, OnPointer_Multi, null);
	else s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, OnPointer, null);

	// First Scene Test
	const int FirstSceneCount = BxScene::__GetActiveSceneCount__();
	BxAssert("BxScene<최초 씬은 1개여야 합니다>", FirstSceneCount == 1);
	// Scene Init(SceneData Create)
	sysevent Event;
	Event.type = syseventtype_init;
	Event.init.w = BxCore::Surface::GetWidth();
	Event.init.h = BxCore::Surface::GetHeight();
	BxScene::__OnEvent__(*Draw, Event);

	uhuge CheckTime = BxCore::System::GetTimerMs();
	while(Procedure())
	{
		// 프레임슬립처리
		BxCore::System::Sleep(0);
		while(BxCore::System::GetTimerMs() - CheckTime < MILLISECOND_PER_FRAME)
			BxCore::System::Sleep(1);
		CheckTime = BxCore::System::GetTimerMs();
	}

	// Scene Quit
	Event.type = syseventtype_quit;
	BxScene::__OnEvent__(*Draw, Event);
	// All SceneData Remove
	BxScene::AllScene* OneScene = &BxScene::__GetAllScene__();
	while((OneScene = OneScene->Next) != null)
		OneScene->GetData(sysmethod_remove);

	// 이벤트해제
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, OnKeyboard);
	if(IsMultiTouch)
		s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, OnPointer_Multi);
	else s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, OnPointer);

	// Draw객체 해제
	BxDelete(Draw);

	// 전역객체 해제
	BxMemoryJoin<memoryjoin_global>::Release();

	// 메모리해제확인
	while(BxMemory::DoMemLogger())
	{
		void* Test = BxAlloc(1); // 메모리해제확인중...
		BxFree(Test); // 메모리해제확인중...
		BxCore::System::Sleep(10);
	}
	return 0;
}
/// @endcond

/// @cond SECTION_NAME
local_func bool ProcedureWithYield(int sleep)
{
	global_data uhuge LastTime = BxCore::System::GetTimerMs();
	const uhuge LimitTime = BxCore::System::GetTimerMs() + sleep;
	if(BxCore::System::IsQuit()) return false;
	do
	{
		const uhuge CurTime = BxCore::System::GetTimerMs();
		if(LastTime + MILLISECOND_PER_FRAME < CurTime)
		{
			LastTime = CurTime - BxUtilGlobal::Min((const int) (CurTime - (LastTime + MILLISECOND_PER_FRAME)), MILLISECOND_PER_FRAME);
			BxCore::System::Sleep(0);
			if(!Procedure()) return false;
		}
		if(sleep) BxCore::System::Sleep(1);
	}
	while(BxCore::System::GetTimerMs() < LimitTime);
	return true;
}
/// @endcond

/// @cond SECTION_NAME
bool Procedure()
{
	if(BxCore::System::IsQuit() || BxScene::__GetActiveSceneCount__() <= 0)
		return false;

	BxCore::Util::SetCallCount(0);

	BxScene::__OnUpdateRender__(*Draw);
	Draw->Flush();

	// 통합콜백처리
	BxCore::System::CallbackList* List = BxCore::System::CallbackList::GetFirst();
	while(List->Next)
	{
		if(!List->Next->Method(List->Next->Data))
		{
			BxCore::System::CallbackList* DeleteList = List->Next;
			List->Next = DeleteList->Next;
			DeleteList->Next = null;
			BxDelete(DeleteList);
		}
		List = List->Next;
	}

	// 자식프로세스의 실행
	if(!IsChildProcessRun && ChildProcess)
	{
		IsChildProcessRun = true;
		ChildProcess(ProcedureWithYield);
		ChildProcess = null;
		IsChildProcessRun = false;
	}
	return true;
}
/// @endcond

/// @cond SECTION_NAME
int OnKeyboard(void* systemData, void* userData)
{
	s3eKeyboardEvent* EventData = (s3eKeyboardEvent*) systemData;
	sysevent Event;
	Event.type = syseventtype_key;
	Event.key.type = (EventData->m_Pressed)? syskeytype_down : syskeytype_up;
	Event.key.code = GetKeyKind(EventData->m_Key);
	BxScene::__OnEvent__(*Draw, Event, (EventData->m_Pressed)? syseventset_do_enable : syseventset_need_enable);
	return 0;
}
/// @endcond

/// @cond SECTION_NAME
int OnPointer(void* systemData, void* userData)
{
	s3ePointerEvent* EventData = (s3ePointerEvent*) systemData;
	sysevent Event;
	Event.type = syseventtype_touch;
	Event.touch.type = (EventData->m_Pressed)? systouchtype_down : systouchtype_up;
	Event.touch.id = 0;
	Event.touch.x = EventData->m_x * BxCore::Surface::GetWidth() / BxCore::Surface::GetWidthHW();
	Event.touch.y = EventData->m_y * BxCore::Surface::GetHeight() / BxCore::Surface::GetHeightHW();
	while(BxScene::__OnEvent__(*Draw, Event, (EventData->m_Pressed)? syseventset_do_enable : syseventset_need_enable));

	if(EventData->m_Pressed)
		s3ePointerRegister(S3E_POINTER_MOTION_EVENT, OnPointerMotion, null);
	else s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, OnPointerMotion);
	return 0;
}
/// @endcond

/// @cond SECTION_NAME
int OnPointerMotion(void* systemData, void* userData)
{
	s3ePointerMotionEvent* EventData = (s3ePointerMotionEvent*) systemData;
	sysevent Event;
	Event.type = syseventtype_touch;
	Event.touch.type = systouchtype_move;
	Event.touch.id = 0;
	Event.touch.x = EventData->m_x * BxCore::Surface::GetWidth() / BxCore::Surface::GetWidthHW();
	Event.touch.y = EventData->m_y * BxCore::Surface::GetHeight() / BxCore::Surface::GetHeightHW();
	BxScene::__OnEvent__(*Draw, Event, syseventset_need_enable);
	return 0;
}
/// @endcond

/// @cond SECTION_NAME
int OnPointer_Multi(void* systemData, void* userData)
{
	global_data bool IsPressed[S3E_POINTER_TOUCH_MAX] = {false,};
	s3ePointerTouchEvent* EventData = (s3ePointerTouchEvent*) systemData;
	sysevent Event;
	Event.type = syseventtype_touch;
	Event.touch.type = (EventData->m_Pressed)? systouchtype_down : systouchtype_up;
	Event.touch.id = EventData->m_TouchID;
	Event.touch.x = EventData->m_x * BxCore::Surface::GetWidth() / BxCore::Surface::GetWidthHW();
	Event.touch.y = EventData->m_y * BxCore::Surface::GetHeight() / BxCore::Surface::GetHeightHW();
	while(BxScene::__OnEvent__(*Draw, Event, (EventData->m_Pressed)? syseventset_do_enable : syseventset_need_enable));

	IsPressed[EventData->m_TouchID] = (EventData->m_Pressed != 0);
	uint PressCount = 0;
	for(int i = 0; i < S3E_POINTER_TOUCH_MAX; ++i)
		if(IsPressed[i]) ++PressCount;
	if(EventData->m_Pressed)
	{
		if(PressCount == 1)
			s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, OnPointerMotion_Multi, null);
	}
	else if(PressCount == 0)
		s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, OnPointerMotion_Multi);
	return 0;
}
/// @endcond

/// @cond SECTION_NAME
int OnPointerMotion_Multi(void* systemData, void* userData)
{
	s3ePointerTouchMotionEvent* EventData = (s3ePointerTouchMotionEvent*) systemData;
	sysevent Event;
	Event.type = syseventtype_touch;
	Event.touch.type = systouchtype_move;
	Event.touch.id = EventData->m_TouchID;
	Event.touch.x = EventData->m_x * BxCore::Surface::GetWidth() / BxCore::Surface::GetWidthHW();
	Event.touch.y = EventData->m_y * BxCore::Surface::GetHeight() / BxCore::Surface::GetHeightHW();
	BxScene::__OnEvent__(*Draw, Event, syseventset_need_enable);
	return 0;
}
/// @endcond
