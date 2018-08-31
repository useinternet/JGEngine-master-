#pragma once
#include"Engine.h"


class StaticMesh2DComponent;
class JGTexture;
class JG2DMesh;
class JGDeviceD;
class JGBufferManager;
enum class EPivot;



/*
Class : StaticMesh2DObject 
Exp : ���� 2DMesh / ������Ʈ�� �����ϱ����� Ŭ����.
@m wstring               ShaderName : ������ ���̴� �̸�
@m shared_ptr<JGTexture> Texture    : �ؽ��� 
@m shared_ptr<JG2DMesh>  Mesh       : �޽� */
class ENGINE_EXPORT StaticMesh2DObject
{
public:
	std::wstring ShaderName = TT("None");
	std::shared_ptr<JGTexture> Texture;
	std::shared_ptr<JG2DMesh>  Mesh;
};


/*
Class : JGConstructHelper 
@m vector<shared_ptr<StaticMesh2DObject>> m_vStaticMesh2DObject : ����ƽ 2DMesh�� �ּҰ��� �����ϱ� ���� �迭 */
class ENGINE_EXPORT JGConstructHelper
{
public:
	std::vector<std::shared_ptr<StaticMesh2DObject>> m_vStaticMesh2DObject;
public:
	JGConstructHelper();
	static JGConstructHelper* GetInstance();
public:
	class StaticMesh2D
	{
	public:
		StaticMesh2DObject* Object;
		bool Success = true;
	public:
		StaticMesh2D() = delete;
		StaticMesh2D(const StaticMesh2D& copy) = delete;
	
		/*
		@param JGDevice* Device : GetDevice() ȣ�� 
		@param JGBufferManager : GetBufferManager() ȣ�� 
		@param const wstring& ComponentName : GetComponentName() ȣ�� 
		@param EPivot pivot : ������
		@param const std::wstring& TexturePath : �ؽ��� ���
		@param const std::wstring& ShaderName = ������ ���̴� �̸�(�⺻������ ������ �°� �����Ǿ�����) */
		StaticMesh2D(JGDeviceD* Device, JGBufferManager* BufferManager, const std::wstring& ComponentName,
			EPivot pivot, const std::wstring& TexturePath, const std::wstring& ShaderName = TT("SampleShader"));
	};
};