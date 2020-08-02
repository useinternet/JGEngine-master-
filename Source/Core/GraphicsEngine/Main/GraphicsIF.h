#pragma once

#ifdef GRAPHICSENGINE_EXPORT

#define GRAPHICSENGINE_API __declspec(dllexport)

#else

#define GRAPHICSENGINE_API __declspec(dllimport)

#endif
#include "CommonCore.h"
#include "Interface/GEObject.h"
#include "Interface/GraphicsDevice.h"
#include "Interface/GraphicsRenderer.h"
#include "Interface/Material.h"
#include "Interface/Scene.h"
#include "Interface/SceneObject.h"
#include "Interface/Screen.h"
#include "Interface/Material.h"
#include "Interface/Texture.h"



class GraphicsConfig
{
public:
	std::string   graphics_api_dllPath;
	void* config_user_data = nullptr;
};

template class GE::GEObjectRef<GE::Screen>;
using ScreenRef =  GE::GEObjectRef<GE::Screen>;

template class GE::GEObjectRef<GE::Scene>;
using SceneRef = GE::GEObjectRef<GE::Scene>;


template class GE::GEObjectRef<GE::SceneObject>;
using SceneObjectRef = GE::GEObjectRef<GE::SceneObject>;

template class GE::GEObjectRef<GE::Material>;
using MaterialRef = GE::GEObjectRef<GE::Material>;


template class GE::GEObjectRef<GE::Texture>;
using TextureRef = GE::GEObjectRef<GE::Texture>;
// Scene ���� ī�޶� ����, ������ ���ε�, �ø�, ���콺 ��ŷ ���, ������� �ؽ��� ����
// SceneObject �޽�, �ν��Ͻ�, Ʈ������
// Material ������ �÷��� ����, ������, PSO 
// GraphicsRenderer�� RootSig, �⺻ PSO
// Renderer�� ���� Material Ŭ������ Ʋ����

// Texture Ŭ���� �߰�

enum class GraphicsAPI
{
	DirectX12,
	DirectX11,
	OpenGL,
	Vulkan
};

// #define INDEPENDENT_GRAPHICS_INTERFACE
class GRAPHICSENGINE_API GraphicsIF
{
public:
	//static void 
	static bool Create(GraphicsAPI api = GraphicsAPI::DirectX12);
	static bool RegisterInVaildTexture(const std::wstring& path);
	static TextureRef     GetInValidTexture();
	static ScreenRef      RequestScreen(HWND hWnd, int width, int height);
	static SceneObjectRef RequestSceneObject();
	static SceneRef       RequestScene();
	static MaterialRef    RequestMaterial(const std::wstring& matName, const GE::MaterialProperty& ppt = GE::MaterialProperty());
	static MaterialRef    GetMaterial(const std::wstring& matName);


	static bool PreCompileMaterial(GE::Material* m);
	static void       LoadTextures(const std::vector<std::wstring>& paths);
	static TextureRef LoadTexture(const std::wstring& path);
	static TextureRef GetTexture(const std::wstring& path);


	static void DrawCall(SceneRef& sceneRef);
	static void BeginDraw();
	static void EndDraw();

	static GraphicsAPI GetGraphicsAPI();
#ifdef INDEPENDENT_GRAPHICS_INTERFACE   
	static void Destroy();
#endif // !1

};