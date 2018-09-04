#include"JGConstructHelper.h"
#include"../RenderSystem/JGTexture.h"
#include"../RenderSystem/JGMaterial/JG2DMesh.h"
#include"../EngineFrameWork/Components/StaticMesh2DComponent.h"
#include"../EngineStatics/JGLog.h"
#include"../EngineFrameWork/2D/Text/JGFontLoader.h"
using namespace std;

static JGConstructHelper* Instance = nullptr;

JGConstructHelper::JGConstructHelper()
{
	if (Instance == nullptr)
	{
		Instance = this;
	}
	else
	{
		JGLog::Write(ELogLevel::Warning, TT("JGConstructHelper has already been initialized."));
	}
}
JGConstructHelper* JGConstructHelper::GetInstance()
{
	return Instance;
}


JGConstructHelper::StaticMesh2D::StaticMesh2D(JGDeviceD* Device, JGBufferManager* BufferManager, const wstring& ComponentName,
	EPivot pivot, const wstring& TexturePath, const wstring& ShaderName)
{
	// ������Ʈ �����Ҵ�..
	unique_ptr<StaticMesh2DObject> TempObject = make_unique<StaticMesh2DObject>();
	TempObject->Texture = make_unique<JGTexture>();
	TempObject->Mesh = make_unique<JG2DMesh>();

	// �ؽ��� �ε�
	bool result = TempObject->Texture->Add(Device, TexturePath);
	if (!result)
	{
		Success = false;
	}
	// �޽� �ε�
	result = TempObject->Mesh->Construct2DMesh(
		BufferManager, ComponentName,
		(float)TempObject->Texture->GetInformation(0).Width,
		(float)TempObject->Texture->GetInformation(0).Height, pivot);
	if (!result)
	{
		Success = false;
	}
	// ���̴� �̸� ����
	TempObject->ShaderName = ShaderName;

	// ������ ����
	Object = TempObject.get();

	// �ּ� ���� �迭�� ����
	JGConstructHelper::GetInstance()->m_vStaticMesh2DObject.push_back(move(TempObject));
}



JGConstructHelper::TextFont::TextFont(JGDeviceD* Device, const std::string& FontPath, const std::wstring& FontTexturePath,
	const wstring& ShaderName)
{
	unique_ptr<TextObject> TempObject = make_unique<TextObject>();
	TempObject->Texture = make_unique<JGTexture>();
	// ��Ʈ �ؽ��� ����
	bool result = TempObject->Texture->Add(Device, FontTexturePath);
	if (!result)
	{
		Success = false;
	}
	// ��Ʈ ����
	result = JGFontLoader::GetInstance()->LoadFont(Device, FontPath, FontTexturePath);
	if (!result)
	{
		Success = false;
	}
	// ���̴� �̸� ����.
	TempObject->ShaderName = ShaderName;
	TempObject->FontPath = FontPath;
	// ������ ����
	Object = TempObject.get();
	JGConstructHelper::GetInstance()->m_vStaticMesh2DObject.push_back(move(TempObject));
}

StaticMesh2DObject::~StaticMesh2DObject()
{
}

TextObject::~TextObject()
{
}

ConsructObject::~ConsructObject()
{
}
