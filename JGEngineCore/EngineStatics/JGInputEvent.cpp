#include"JGInputEvent.h"
#include"JGLog.h"


JGInputEvent::JGInputEvent() {}
JGInputEvent::~JGInputEvent() {}
void JGInputEvent::DoEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	// �̺�Ʈ
	switch (message)
	{
	case WM_LBUTTONDOWN:
		JGLog::Write(ELogLevel::Default, TT("���콺 Ŭ��"));
		break;
	}
}
