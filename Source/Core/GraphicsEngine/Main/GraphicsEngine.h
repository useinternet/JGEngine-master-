#pragma once
#ifdef GRAPHICSENGINE_EXPORT

#define GRAPHICSENGINE_API __declspec(dllexport)

#else

#define GRAPHICSENGINE_API __declspec(dllimport)

#endif
#include "CommonCore.h"
#include <ppl.h>
class GraphicsIF;

/*
1. ObjectRef�� ������ GraphicsEngine �� �����ϰ��ִٰ� Destroy�� ��� ���� (������ִ� 
ObjectRef�� ptr�� nullptr�� ����������.


2. �ٸ� �������̽� ���� ��

3. ������ �Ͳ� ���� ( GUI, _3D, Custom )

*/
namespace GE
{
	REGISTER_SYSTEM_CORE(GraphicsEngine, 0, 100, 100)



	class GraphicsDevice;
	class GEObject;
	class GraphicsRenderer;
	class Material;
	class Texture;
	class GRAPHICSENGINE_API GraphicsEngine : public SystemCore
	{
		friend GraphicsIF;
		template<class T> friend class GEObjectRef;
	public:
		GraphicsEngine() : SystemCore() {}
		GraphicsEngine(const SystemCoreDesc& desc) : SystemCore(desc) {}
		virtual ~GraphicsEngine() {}
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void Destroy() override;
	public:
		virtual void ReceiveMessage(const Message& msg) override;

	private:
		std::shared_ptr<Plugin> m_GraphicsAPIPlugin;
		std::unordered_map<std::wstring, Material*>  m_MaterialPool;
		std::shared_mutex                           m_MaterialMutex;
	

		
		std::unordered_map<std::wstring, Texture*>  m_TexturePool;
		std::shared_mutex                           m_TextureMutex;
		Texture* m_InVaildTexture = nullptr;
		

		GraphicsDevice*   m_GraphicsDevice   = nullptr;
		GraphicsRenderer* m_GraphicsRenderer = nullptr;

	
		Concurrency::task_group m_DrawCallTasks;
	};












}

