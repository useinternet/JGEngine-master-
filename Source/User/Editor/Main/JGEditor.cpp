#include "pch.h"
#include "JGEditor.h"










void JGEditor::Awake()
{

}

void JGEditor::Start()
{
}

void JGEditor::Tick()
{

}

void JGEditor::Destroy()
{

}

void JGEditor::OnKeyUp(KeyCode code)
{
	static int num = 0;
	if (code == KeyCode::S)
	{
		CreateWindow<GUI::Form>(TT("�׽�Ʈ ��") + std::to_wstring(num++), GUI::WindowFlag_MainWindow);
	}
	if (code == KeyCode::A)
	{
		CreateWindow<GUI::Form>(TT("�׽�Ʈ ��") + std::to_wstring(num++), GUI::WindowFlag_ChildWindow);
	}



}
