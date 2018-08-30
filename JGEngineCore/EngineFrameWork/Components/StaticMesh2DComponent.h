#pragma once
#include"Motivated2DComponent.h"
class JG2DMesh;
class JGTexture;

class StaticMesh2DObject;

// ���� �� ��
// �״㿡��2D���͸� ������ �������غ���.
// �׸��� ���� ���� �̿��ؼ� ������ �غ���..



/*
Class : StaticMesh2DComponent 
@m wstring m_ShaderName : ������ ���̴� �̸�
@m JGTexture& m_Texture : �ؽ���
@m JG2DMesh m_Mesh : �޽� 
@m int m_ZOrder : �׸��� ����(���� �������� ���߿� �׷�����.) */
class ENGINE_EXPORT StaticMesh2DComponent : public Motivated2DComponent
{
private:
	std::wstring m_ShaderName = TT("None");
	JGTexture* m_Texture = nullptr;
	JG2DMesh* m_Mesh     = nullptr;
public:
	StaticMesh2DComponent();
	virtual ~StaticMesh2DComponent();
	/*
	Exp : 2D�� �޽� �� �ؽ���, ���̴��� �����Ѵ�. 
	// ���߿� ���� Ŭ����(ConstructHelper Ŭ������ ��ü)
	@param const wstring& TexturePath : �ؽ��İ�� *///(������ ����)
	bool ConstructMesh2D(const std::wstring& TexturePath);



	/*
	Exp : StaticMesh2DComponent�� �������� �����Ѵ�. 
	@param StaticMesh2DObject* Object : JGConstructHelper::StaticMesh2D Ŭ������ �������� ������Ʈ ������ */
	void SetStaticMesh2DComponent(StaticMesh2DObject* Object);
	/*
	Exp : �ؽ����� ���� ���̸� �����´�. */
	float GetTextureWdith();
	/*
	Exp : �ؽ����� ���� ���̸� �����´�. */
	float GetTextureHeight();
	/*
	Exp : �޽ø� �����´�. */
	JG2DMesh* GetMesh();
	/*
	Exp : �ؽ��ĸ� �����´�. */
	JGTexture* GetTexture();
	/*
	Exp : ������ �Ѵ�.(���̴��� �ٸ��ٸ� ��� �޾Ƽ� Ŀ���� �ϼ� 
	���!! : Component�� Render�Լ��� ȣ���Ұ�..) */
	virtual void Render() override;
};