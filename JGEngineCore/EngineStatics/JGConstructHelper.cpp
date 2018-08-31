#include"JGConstructHelper.h"
#include"../RenderSystem/JGTexture.h"
#include"../RenderSystem/JGMaterial/JG2DMesh.h"
#include"../EngineFrameWork/Components/StaticMesh2DComponent.h"
#include"../EngineStatics/JGLog.h"
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

