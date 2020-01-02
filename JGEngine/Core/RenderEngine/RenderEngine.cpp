#include "pch.h"
#include "RenderEngine.h"
#include "Object/DxObject/Fence.h"
#include "Object/DxObject/DescriptorAllocator.h"
#include "Object/DxObject/DescriptorHandle.h"
#include "Object/DxObject/RenderDevice.h"
#include "Object/DxObject/CommandQueue.h"
#include "Object/DxObject/CommandList.h"
#include "Object/DxObject/Resource.h"
#include "DxUtill.h"
#include "GUI.h"
#include "Object/ReObject/RenderTarget.h"
#include "Object/DxObject/Shader.h"
#include "Object/DxObject/PipelineState.h"
#include "Object/DxObject/RootSignature.h"
#include "Temp/DDSTextureLoader.h"
#include "Object/Shader/ShaderData.h"
#include "Object/Shader/ShaderLib.h"
#include "Object/Shader/ShaderDataType.h"
#include "Object/Shader/ShaderModule.h"
#include "Object/Shader/ShaderModuleManager.h"
#include "Object/ReObject/RenderItem.h"
#include "Object/ReObject/ReMesh.h"
#include "Object/ReObject/ReMaterial.h"
#include "Object/ReObject/ReCamera.h"
#include "Object/DxObject/DxScreen.h"
#include "Object/DxObject/TextureManager.h"
#include <d3dcompiler.h>
using namespace std;
using namespace DirectX;

namespace RE
{
	mutex g_ScreenMutex;
	mutex g_RIMutex;
	RenderEngine* g_RenderEngine = nullptr;

	DxScreen* RenderEngine::CreateDxScreen(HWND hWnd, uint32_t width, uint32_t height, DXGI_FORMAT format)
	{
		lock_guard<mutex> lock(g_ScreenMutex);
		return g_RenderEngine->m_RenderDevice->CreateDxScreen(hWnd, width, height, format);
	}
	void RenderEngine::DestroyDxScreen(HWND hWnd)
	{
		if (!g_RenderEngine)
			return;
		lock_guard<mutex> lock(g_ScreenMutex);
		g_RenderEngine->m_RenderDevice->DestroyDxScreen(hWnd);
	}
	void RenderEngine::DestroyDxScreen(DxScreen* screen)
	{
		if (!g_RenderEngine)
			return;
		lock_guard<mutex> lock(g_ScreenMutex);
		g_RenderEngine->m_RenderDevice->DestroyDxScreen(screen);
	}
	DxScreen* RenderEngine::FindDxScreen(HWND hWnd)
	{
		lock_guard<mutex> lock(g_ScreenMutex);
		return g_RenderEngine->m_RenderDevice->FindDxScreen(hWnd);
	}

	void RenderEngine::RegisterRIManager(uint64_t id)
	{
		lock_guard<mutex> lock(g_RIMutex);
		g_RenderEngine->m_RIManager[id].reset();
		g_RenderEngine->m_RIManager[id] = make_shared<RenderItemManager>(id);
	}

	RenderItem* RenderEngine::CreateRenderItem(uint64_t id, ERenderItemUsage usage, const std::string& name)
	{
		lock_guard<mutex> lock(g_RIMutex);
		if (g_RenderEngine->m_RIManager.find(id) == g_RenderEngine->m_RIManager.end())
			return nullptr;

		switch (usage)
		{
		case ERenderItemUsage::GUI:
			return g_RenderEngine->m_RIManager[id]->CreateGUIItem(name);
		case ERenderItemUsage::Skeletal3D:
			return g_RenderEngine->m_RIManager[id]->CreateSkeletalItem(name);
		case ERenderItemUsage::Static3D:
			return g_RenderEngine->m_RIManager[id]->CreateStaticItem(name);
		default:
			return nullptr;
		}
		return nullptr;
	}
	void        RenderEngine::DestroyRenderItem(uint64_t id, RenderItem* item)
	{
		if (!g_RenderEngine)
			return;
		lock_guard<mutex> lock(g_RIMutex);
		if (g_RenderEngine->m_RIManager.find(id) == g_RenderEngine->m_RIManager.end())
			return;
		g_RenderEngine->m_RIManager[id]->DeleteItem(item);
	}

	void RenderEngine::UnRegisterRIManager(uint64_t id)
	{
		if (!g_RenderEngine)
			return;
		lock_guard<mutex> lock(g_RIMutex);
		g_RenderEngine->m_RIManager.erase(id);
	}
	void  RenderEngine::MoveRenderItem(uint64_t old_id, uint64_t new_id)
	{
		lock_guard<mutex> lock(g_RIMutex);
		if (g_RenderEngine->m_RIManager.find(old_id) == g_RenderEngine->m_RIManager.end())
			return;
		if (g_RenderEngine->m_RIManager.find(new_id) == g_RenderEngine->m_RIManager.end())
			return;

		auto& riManager = g_RenderEngine->m_RIManager;
		riManager[new_id]->Merge(riManager[old_id].get());

	}

	GraphicsShaderModule* RenderEngine::FindGraphicsShaderModule(const std::string& name)
	{
		// �̱���
		return nullptr;
	}
	ComputeShaderModule* RenderEngine::FindComputeShaderModule(const std::string& name)
	{
		// �̱���
		return nullptr;
	}

	GUIModule* RenderEngine::GetGUIModule()
	{
		return static_cast<GUIModule*>(g_RenderEngine->m_ShaderModuleManager->FindGraphicsModule("JGUI"));
	}

	void        RenderEngine::RegisterMaterial(const std::string& name, const ReMaterial& m)
	{
		ReMaterialManager::RegisterMaterial(name, m);
	}
	ReMaterial* RenderEngine::FindMaterial(const std::string& name)
	{
		return ReMaterialManager::GetMaterial(name);
	}
	RenderDevice* RenderEngine::GetDevice()
	{
		return g_RenderEngine->m_RenderDevice.get();
	}





	RenderEngine::RenderEngine(const GlobalLinkStream& stream) :
		EngineCore(stream)
	{
		g_RenderEngine = this;
	}
	RenderEngine::~RenderEngine()
	{
		std::string path = GlobalLinkData::_EngineConfig->InEngine("Shader\\Lib\\ShaderTypeLib.jg");
		m_ShaderLibManager->Save(path);

		path = GlobalLinkData::_EngineConfig->InEngine("Shader\\Module");
		m_ShaderModuleManager->Save(path);
	}
	void RenderEngine::Init(const RenderEngineConfig& config)
	{
		RenderDeviceDesc desc;
		desc.gui = nullptr;
		m_RenderDevice = make_shared<RenderDevice>(desc);
		m_ShaderLibManager    = make_shared<ShaderLibManager>();
		m_ShaderModuleManager = make_shared<ShaderModuleManager>();
		m_TextureManager      = make_shared<TextureManager>();
		m_MaterialManager     = make_shared<ReMaterialManager>();
		RE_LOG_INFO("RenderEngine Init Complete...");
		m_Config = config;
		//m_MainScreen = m_RenderDevice->CreateDxScreen(hWnd, width, height);
	}
	void RenderEngine::Load()
	{
		std::string path = GlobalLinkData::_EngineConfig->InEngine(m_Config.ShaderLibPath);
		m_ShaderLibManager->Load(path);
		

		path = GlobalLinkData::_EngineConfig->InEngine(m_Config.ShaderModulePath);
		m_ShaderModuleManager->Load(path);


		path = GlobalLinkData::_EngineConfig->InEngine(m_Config.TexturePath);
		m_TextureManager->InitLoad(path);


		//
		m_ShaderModuleManager->CreateGraphicsModule(m_Config.StaticEntryModuleName, EModuleFormat::G_StaticGBuffer);
		m_ShaderModuleManager->CreateGraphicsModule(m_Config.GUIEntryModuleName, EModuleFormat::G_GUI);

		RenderItem::BindShaderModuleByUsage(ERenderItemUsage::Static3D, m_Config.StaticEntryModuleName);
		RenderItem::BindShaderModuleByUsage(ERenderItemUsage::GUI, m_Config.GUIEntryModuleName);

		// ���͸��� �⺻ ���
		{
			auto depth_desc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			depth_desc.DepthEnable = false;
			ReMaterial default_Mat(RE_GUI_DefaultMaterial, ERenderItemUsage::GUI);
			D3D12_BLEND_DESC blend_desc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			blend_desc.RenderTarget[0].BlendEnable = true;
			blend_desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blend_desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;


			// �ؽ��� ���� �⺻ ���͸���
			{
				default_Mat.AddFloat4InMaterialCB("Color", { 1.0f,1.0f,1.0f,1.0f });
				default_Mat.SetCode(R"(
    Output output;
    output.Screen = input.Color * Color;
    return output;
)");
				default_Mat.SetBlendState(blend_desc);
				default_Mat.SetDepthStencilState(depth_desc);
				default_Mat.Compile();



			}


			ReMaterial default_tMat(RE_GUI_OneTextureDefault, ERenderItemUsage::GUI);
			// �ؽ��� �ִ� ���͸���
			{
			
			default_tMat.AddFloat4InMaterialCB("Color", { 1.0f,1.0f,1.0f,1.0f });
			default_tMat.AddTexture("Image");
		
			default_tMat.SetCode(R"(
    Output output;
    float4 img = Image.Sample( AnisotropicSampler, input.TexC);
    output.Screen = input.Color * img * Color;
    return output;
)");
			default_tMat.SetDepthStencilState(depth_desc);
			default_tMat.SetBlendState(blend_desc);
			default_tMat.Compile();
			//
			}
		
			ReMaterialManager::RegisterMaterial(default_Mat.GetName(), default_Mat);
			ReMaterialManager::RegisterMaterial(default_tMat.GetName(), default_tMat);
		}
	}

	void RenderEngine::Update()
	{
		ENGINE_PERFORMANCE_TIMER("Application", "RenderEngine");

		m_RenderDevice->Update();
	}
	void RenderEngine::OnEvent(Event& e)
	{
		
	}





}

