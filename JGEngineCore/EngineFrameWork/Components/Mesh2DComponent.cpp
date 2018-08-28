#include "Mesh2DComponent.h"
#include"../../RenderSystem/JGTexture.h"
#include"../../RenderSystem/JGHLSLShaderDevice/JGShader.h"
#include"../../RenderSystem/JGHLSLShaderDevice/JGHLSLShaderDevice.h"
#include"../../RenderSystem/JGHLSLShaderDevice/JGShaderData.h"
#include"../../RenderSystem/JGMaterial/JGMesh.h"
#include"../../RenderSystem/JGDeviceD.h"
using namespace std;
Mesh2DComponent::Mesh2DComponent()
{
	m_Texture = make_unique<JGTexture>();
	m_Mesh = make_unique<JGMesh>();

}

Mesh2DComponent::~Mesh2DComponent()
{
}

bool Mesh2DComponent::ConstructMesh2D(const std::wstring & TexturePath)
{
	// �ؽ��� �߰�
	bool result = m_Texture->Add(JGDeviceD::GetInstance(), TexturePath);
	if (!result)
	{
		return false;
	}
	// �޽� �����
	// ���̴� �����
	return true;
}

void Mesh2DComponent::Render()
{
	Motivated2DComponent::Render();
	// JGShaderData ���� ( �ѹ� �����ΰ� �Լ� �����ֵ��� �����.)
	JGShaderData Data;
	// �޽� ������
	m_Mesh->Render(JGDeviceD::GetInstance(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ���̴� ������
	JGHLSLShaderDevice::GetInstance()->GetShader(m_ShaderName)->Render(nullptr, m_Texture.get());

}
