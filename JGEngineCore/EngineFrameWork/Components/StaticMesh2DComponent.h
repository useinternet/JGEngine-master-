#pragma once
#include"Motivated2DComponent.h"
class JGMesh;
class JGTexture;




// ���� �� ��
// �״㿡��2D���͸� ������ �������غ���.
// ConstructHelper �����
// �׸��� ���� ���� �̿��ؼ� ������ �غ���..


class ENGINE_EXPORT StaticMesh2DComponent : public Motivated2DComponent
{
private:
	typedef struct Mesh2DVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 tex;
	}Mesh2DVertexType;
private:
	std::wstring m_ShaderName = TT("None");
	std::unique_ptr<JGTexture> m_Texture;
	std::unique_ptr<JGMesh> m_Mesh;
public:
	StaticMesh2DComponent();
	virtual ~StaticMesh2DComponent();
	/*
	Exp : 2D�� �޽� �� �ؽ���, ���̴��� �����Ѵ�. 
	// ���߿� ���� Ŭ����(ConstructHelper Ŭ������ ��ü)
	@param const wstring& TexturePath : �ؽ��İ�� */
	bool ConstructMesh2D(const std::wstring& TexturePath);

	float GetTextureWdith();
	float GetTextureHeight();

	/*
	Exp : ������ �Ѵ�. ( ��ӹ��� ����. ) */
	virtual void Render() override;
private:
	void Create2DMesh();
};