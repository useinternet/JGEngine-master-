#pragma once
#include"Motivated2DComponent.h"
class JG2DMesh;
class JGTexture;
class JGShaderData;
class ConsructObject;

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

	std::unique_ptr<struct SMatrixBuffer_VS>   m_MatrixBufferDesc;
	std::unique_ptr<struct S2DSpriteBuffer_PS> m_2DSpriteBufferDesc;
public:
	StaticMesh2DComponent();
	virtual ~StaticMesh2DComponent();
	/*
	Exp : StaticMesh2DComponent�� �������� �����Ѵ�. 
	@param StaticMesh2DObject* Object : JGConstructHelper::StaticMesh2D Ŭ������ �������� ������Ʈ ������ */
	virtual void SetConstructObject(ConsructObject* Object);
	/*
	Exp : �ؽ����� ���� ���̸� �����´�. */
	float GetTextureWdith();
	/*
	Exp : �ؽ����� ���� ���̸� �����´�. */
	float GetTextureHeight();
	/*
	Exp : ��������Ʈ ������ Ű�� ��������Ʈ ���� ���Ѵ�. */
	void SpriteSwichOn(const float r, const float g, const float b);
	/*
	Exp : ��������Ʈ ������ Ű�� ��������Ʈ ���� ���Ѵ�. */
	void SpriteSwichOn(JGVector3D& color);
	/*
	Exp : �÷������ �����Ѵ�. */
	void SetColorRize(const float r, const float g, const float b);
	/*
	Exp : �÷������ �����Ѵ�. */
	void SetColorRize(JGVector3D& color);
	/*
	Exp : ���İ��� �����Ѵ�. */
	void SetAlphaBlend(const float a);
public:
	/*
	Exp : ������ �Ѵ�.(���̴��� �ٸ��ٸ� ��� �޾Ƽ� Ŀ���� �ϼ� 
	���!! : Component�� Render�Լ��� ȣ���Ұ�..) */
	virtual void Render() override;
protected:
	/*
	Exp : ���̴� ��������� �Ķ���͵��� �����Ѵ�. ( ��� �޾Ƽ� ������ / �θ� �Լ� ȣ�� ���ص��ǿ�.. */
	virtual void ShaderParamSetting(JGShaderData* Data);
	/*
	Exp : �޽ø� �����´�. */
	JG2DMesh * GetMesh();
	/*
	Exp : �޽ø� �����Ѵ�. */
	void SetMesh(JG2DMesh* mesh);
	/*
	Exp : �ؽ��ĸ� �����´�. */
	JGTexture* GetTexture();
	/*
	Exp : �ؽ��ĸ� �����Ѵ�. */
	void SetTexture(JGTexture* texture);
	/*
	Exp : �̿����� ���̴� �̸��� �����´�. */
	std::wstring& GetShaderName();
	/*
	Exp : ���̴��̸��� �����Ѵ�. */
	void SetShaderName(const std::wstring& ShaderName);
};