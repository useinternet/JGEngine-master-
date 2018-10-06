#pragma once
#include"StaticMesh2DComponent.h"

/*
Class : TextComponent */
class ENGINE_EXPORT TextComponent : public StaticMesh2DComponent
{
private:
	std::unique_ptr<std::wstring> m_Text;
	std::unique_ptr<JG2DMesh> m_TextMesh;
	std::unique_ptr<struct STextBuffer_PS> m_TextBufferDesc;



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
	virtual void SetConstructObject(ConstructObject* Object);
	/*
	Exp : �ؽ�Ʈ������Ʈ�� �Է��Ѵ�. */
	void SetText(const wchar_t* Text, ...);
	/*
	Exp: �ؽ�Ʈ ������ �����Ѵ�. */
	void SetTextColor(const float r, const float g, const float b);
	/*
	Exp : �ؽ�Ʈ ���İ��� �����Ѵ�. */
	void SetTextAplha(const float a);
	/*
	Exp : �ؽ�Ʈ�� 1�ʴ� ��� ������Ʈ ��ų������ ���Ѵ�.*/
	void SetFramePerSecond(float Frame);
	/*
	Exp : �ؽ�Ʈ ����� ���Ѵ�. */
	void SetTextSize(const float TextSize);
private:
	/*
	Exp : ���̴� ��������� �Ķ���͵��� �����Ѵ�. ( ��� �޾Ƽ� ������ / �θ� �Լ� ȣ�� ���ص��ǿ�.. */
	virtual void ShaderParamSetting(JGShaderData* Data);


	bool CreateTextMesh();
};