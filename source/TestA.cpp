#include "BxScene.hpp"
#include "TestA.hpp"

CREATE_SCENE(TestAClass, "Test-A")

syseventresult OnEvent(TestAClass& Data, const sysevent& Event)
{
	if(Event.type == syseventtype_touch && Event.touch.type == systouchtype_down)
	{
		BxScene::SubRequest("Test-A");
		BxScene::AddRequest("Test-List");
		return syseventresult_done;
	}
	return syseventresult_pass;
}

void OnUpdate(TestAClass& Data)
{
}

void OnRender(TestAClass& Data, BxDraw& Draw)
{
	Draw.Rectangle(FILL, XYWH(0, 0, Draw.Width(), Draw.Height()), COLOR(128, 128, 192));
}
