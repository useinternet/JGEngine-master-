#pragma once
#include"Motivated2DComponent.h"
class JGMesh;
class JGTexture;

// ���� �� ��
// �޽� ������Ʈ�� �ϼ���Ű�� �������غ���
// �״㿡��2D���͸� ������ �������غ���.
// �״㿡�� �ؽ��� ������ ����� �������غ���
// �׸��� ���� ���� �̿��ؼ� ������ �غ���..
class Mesh2DComponent : public Motivated2DComponent
{
private:
	std::wstring m_ShaderName = TT("None");
	std::unique_ptr<JGShaderData> m_ShaderData;
	std::unique_ptr<JGTexture> m_Texture;
	std::unique_ptr<JGMesh> m_Mesh;

public:
	Mesh2DComponent();
	virtual ~Mesh2DComponent();

	bool ConstructMesh2D(const std::wstring& TexturePath);
	/*
	Exp : ������ �Ѵ�. ( ��ӹ��� ����. ) */
	virtual void Render() override;

};