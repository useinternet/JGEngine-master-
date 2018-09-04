#pragma once
#include"StaticMesh2DComponent.h"

/*
Class : TextComponent */
class TextComponent : public StaticMesh2DComponent
{
private:
	std::unique_ptr<std::wstring> m_Text;
	std::unique_ptr<JG2DMesh> m_TextMesh;

	std::string m_FontPath;
	float m_TextSize = 0.0f;


	//
	float m_FramePerSecond  = 0.0f;
	float m_AccSecond = 0.0f;
public:
	TextComponent();
	virtual ~TextComponent();
	/*
	Exp : �� ������ �ѹ� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ Tick�� ȣ���ؾ��Ѵ�. )*/
	virtual void Tick(const float DeltaTime) override;
	/*
	Exp : ������ ������Ʈ�� �����Ѵ�. 
	@param ConstructObject* Object : ������ ������Ʈ */
	virtual void SetConstructObject(ConsructObject* Object);
	/*
	Exp : �ؽ�Ʈ������Ʈ�� �Է��Ѵ�. */
	void SetText(const wchar_t* Text, ...);
	/*
	Exp : �ؽ�Ʈ�� 1�ʴ� ��� ������Ʈ ��ų������ ���Ѵ�.*/
	void SetFramePerSecond(float Frame);
	/*
	Exp : �ؽ�Ʈ ����� ���Ѵ�. */
	void SetTextSize(const float TextSize);
private:
	bool CreateTextMesh();
};