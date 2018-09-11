#pragma once
#include"Engine.h"


class StaticMesh2DComponent;
class JGTexture;
class JG2DMesh;
class JGDeviceD;
class JGBufferManager;
class JGFontLoader;
class JGSound;
class JGVector2D;
enum class EPivot;


class ENGINE_EXPORT ConstructObject
{
public:
	virtual ~ConstructObject();
};
/*
Class : StaticMesh2DObject 
Exp : ���� 2DMesh / ������Ʈ�� �����ϱ����� Ŭ����.
@m wstring               ShaderName : ������ ���̴� �̸�
@m shared_ptr<JGTexture> Texture    : �ؽ��� 
@m shared_ptr<JG2DMesh>  Mesh       : �޽� */
class ENGINE_EXPORT StaticMesh2DObject : public ConstructObject
{
public:
	std::wstring ShaderName = TT("None");
	std::shared_ptr<JGTexture> Texture;
	std::shared_ptr<JG2DMesh>  Mesh;
	std::unique_ptr<EPivot> Pivot;
	virtual ~StaticMesh2DObject();
};
class ENGINE_EXPORT AnimationMesh2DObject : public StaticMesh2DObject
{
public:
	size_t TotalFrame;
	size_t WidthFrame;
	size_t HeightFrame;
	float IncreaseWidth;
	float IncreaseHeight;
	virtual ~AnimationMesh2DObject();
};
/*
Class : TextObject 
Exp : �ؽ�Ʈ ������Ʈ���� �����ϱ����� ������Ʈ 
@m wstring ShaderName : ������ ���̴� �̸�
@m shared_ptr<JGTexture> Texture : ������ ��Ʈ �ؽ���
@m string FontPath : ������ ��Ʈ ���� ��� */
class ENGINE_EXPORT TextObject : public ConstructObject
{
public:
	std::wstring ShaderName = TT("None");
	std::shared_ptr<JGTexture> Texture;
	std::string FontPath = "None";
	virtual ~TextObject();
};

class ENGINE_EXPORT SoundObject : public ConstructObject
{
public:
	virtual ~SoundObject();
};


/*
Class : JGConstructHelper 
@m vector<shared_ptr<StaticMesh2DObject>> m_vStaticMesh2DObject : ����ƽ 2DMesh�� �ּҰ��� �����ϱ� ���� �迭 */
class ENGINE_EXPORT JGConstructHelper
{
public:
	std::vector<std::shared_ptr<ConstructObject>> m_vConstructObject;
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
			EPivot pivot, const std::wstring& TexturePath, const std::wstring& ShaderName = TT("2DSpriteShader"));
	};

	class AnimationMesh2D
	{
	public:
		AnimationMesh2DObject * Object;
		bool Success = true;
	public:
		AnimationMesh2D() = delete;
		AnimationMesh2D(const AnimationMesh2D& copy) = delete;
		/*
		@param JGDevice* Device : GetDevice() ȣ��
		@param JGBufferManager : GetBufferManager() ȣ��
		@param const wstring& ComponentName : GetComponentName() ȣ��
		@param EPivot pivot : ������
		@param const size_t TotalFrame : �� ������ ��
		@param const size_t WidthFrame : ���� ������ ��
		@param const size_t HeightFrame :  ���� ������ ��
		@param const std::wstring& TexturePath : �ؽ��� ���
		@param const std::wstring& ShaderName = ������ ���̴� �̸�(�⺻������ ������ �°� �����Ǿ�����) */
		AnimationMesh2D(JGDeviceD* Device, JGBufferManager* BufferManager,const std::wstring& ComponentName,
			EPivot pivot,const size_t TotalFrame, const size_t WidthFrame, const size_t HeightFrame,
			const std::wstring& TexturePath, const std::wstring& ShaderName = TT("2DAnimationShader"));
	};
	class TextFont
	{
	public:
		TextObject* Object;
		bool Success = true;
	public:
		TextFont() = delete;
		TextFont(const TextFont& copy) = delete;
		/*
		@param JGDevice* Device : GetDevice() ȣ��
		@param const string& FontPath :  ��Ʈ ���� ��� 
		@param const wstring& FontTexturePath : ��Ʈ �ؽ��� ��� 
		@param const wstring& ShaderName : ��Ʈ�� ������ ���̴� �̸�. */
		TextFont(JGDeviceD* Device, const std::string& FontPath, const std::wstring& FontTexturePath,
			const std::wstring& ShaderName = TT("TextShader"));
	};
};