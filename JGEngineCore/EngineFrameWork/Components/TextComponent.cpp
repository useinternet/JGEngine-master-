#include"TextComponent.h"
#include"../../RenderSystem/JGMaterial/JG2DMesh.h"
#include"../../EngineStatics/JGConstructHelper.h"
#include"../2D/Text/JGFontLoader.h"
#include"../../RenderSystem/JGTexture.h"
using namespace std;

TextComponent::TextComponent()
{
	RegisterComponentID(typeid(this));
	// �ؽ�Ʈ ������Ʈ�� �ʱ�ȭ
	m_Text = make_unique<wstring>();

	*m_Text = TT("None");
	m_TextSize = 20.0f;
	m_FramePerSecond = 10.0f;
}
TextComponent::~TextComponent()
{

}
void TextComponent::Tick(const float DeltaTime)
{
	StaticMesh2DComponent::Tick(DeltaTime);

	// ������ 1�ʴ� ������ �������� �ؽ�Ʈ ������Ʈ�Ѵ�.
	m_AccSecond += DeltaTime;
	if (m_AccSecond >= (1.0f / m_FramePerSecond))
	{
		m_AccSecond = 0.0f;
		CreateTextMesh();
	}
}
void TextComponent::SetConstructObject(ConsructObject* Object)
{
	TextObject* object = dynamic_cast<TextObject*>(Object);
	if (object)
	{
		m_FontPath = object->FontPath;
		SetTexture(object->Texture.get());
		SetShaderName(object->ShaderName);
	}
}

void TextComponent::SetText(const wchar_t* Text, ...)
{
	WCHAR str[256];
	va_list Arglist;
	va_start(Arglist, Text);
	wvsprintf(str, Text, Arglist);
	va_end(Arglist);

	*m_Text = str;
}
void TextComponent::SetFramePerSecond(float Frame)
{
	m_FramePerSecond = Frame;
}

void TextComponent::SetTextSize(const float TextSize)
{
	m_TextSize = TextSize;
}

bool TextComponent::CreateTextMesh()
{
	if (m_TextMesh != nullptr)
	{
		m_TextMesh.reset();
		m_TextMesh = make_unique<JG2DMesh>();
	}
	else
	{
		m_TextMesh = make_unique<JG2DMesh>();
	}
	// �ؽ�Ʈ �޽� ����

	vector<JGFontVertexInformation> vertexInfo;
	JGFontLoader::GetInstance()->OutputVertexInformation(m_FontPath,
		*m_Text, m_TextSize, &vertexInfo);

	bool result = m_TextMesh->ConstructTextMesh(
		GetBufferManager(), GetComponentName(), vertexInfo);
	if (!result)
	{
		return false;
	}
	SetMesh(m_TextMesh.get());
	return true;
}
