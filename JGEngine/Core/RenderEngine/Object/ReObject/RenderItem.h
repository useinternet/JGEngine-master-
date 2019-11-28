#pragma once
#include "Object/ReObject.h"



namespace RE
{

	class ReMesh;
	class ReMaterial;
	class SBDStructuredBuffer;
	class STStruct;
	// RenderItem�� renderengine�� ����
	// ��� Mesh, Material �� ���� �Ŵ������� ����
	// ��� module, pass ������ ���¸Ŵ������� ����
	// ��� �ؽ���, ���� ����, �ִϸ��̼� ������ ���¸Ŵ������� ����
	class InstanceRenderItem;
	class ReMaterialController;
	enum class ERenderItemUsage
	{
		GUI,
		Static3D,
		Skeletal3D
	};
	class RenderItem : public ReObject
	{
		friend class StaticGBufferModule;
		friend class SkeletalGBufferModule;
		friend class GUIModule;
		static std::map<ERenderItemUsage, std::string> sm_ModuleNameByUsage;
	public:
		static const std::string& GetModuleNameByUsage(ERenderItemUsage usage);
		static void BindShaderModuleByUsage(ERenderItemUsage usage, const std::string& module_name);
	public:
		RenderItem(const std::string& name, ERenderItemUsage usage);
		~RenderItem();
		InstanceRenderItem* AddInstance();
		void RemoveInstance(InstanceRenderItem* instance);
		ReMaterialController* GetMaterial() const {
			return Material;
		}
		ReMesh* GetMesh() const {
			return Mesh.get();
		}
		void SetMesh(std::shared_ptr<ReMesh> mesh) {
			Mesh = mesh;
		}
		void SetMaterial(const std::string& mat_name);
		ERenderItemUsage GetUsage() const {
			return Usage;
		}
	private:
		ReMaterialController*                  Material = nullptr;
		std::shared_ptr<ReMesh>                Mesh;
		ERenderItemUsage                       Usage = ERenderItemUsage::Static3D;
		//  
		std::shared_ptr<SBDStructuredBuffer>              StructuredBuffer;
		std::vector<std::shared_ptr<InstanceRenderItem>>  InstanceItems;
		std::unordered_map<InstanceRenderItem*, uint32_t> InstanceMapByPointer;
		//
		// pipelinestate (�� blend����, depth����, ������ȭ�� ����)
		// material;
		   /*
		        1. 2���� �� ���͸����� ���� GUI ��, 3D��
				2. �⺻������ ��� Ÿ�Ժ� �⺻ ����� ����ְ�
				3. �ȼ� �ڵ常 ���� ����
		   */
		// mesh
		  /*
		     MeshŸ�Կ����� material�� 3d������ gui������ �Ǻ�

		  */
		// ��ǥ����(��ġ, ȸ��, ������)
	};


	class StaticRenderItem : public RenderItem
	{
	public:
		StaticRenderItem(const std::string& name) :
			RenderItem(name, ERenderItemUsage::Static3D) {}
	};
	class SkeletalRenderItem : public RenderItem
	{
	public:
		SkeletalRenderItem(const std::string& name) :
			RenderItem(name, ERenderItemUsage::Skeletal3D) {}
	};
	class GUIRenderItem : public RenderItem
	{
	public:
		GUIRenderItem(const std::string& name) :
			RenderItem(name, ERenderItemUsage::GUI) {}
	};



	class InstanceRenderItem : public ReObject
	{
		friend RenderItem;
	public:
		InstanceRenderItem(const std::string& name) : ReObject(name) {}
	public:
		void Set(const std::string& name, const JMatrix& m);



		bool Get(const std::string& name, JMatrix& m);
	private:
		STStruct* Element;
	};

	class RenderItemManager
	{
	public:
		RenderItemManager();
		StaticRenderItem*   CreateStaticItem(const std::string& name);
		SkeletalRenderItem* CreateSkeletalItem(const std::string& name);
		GUIRenderItem*      CreateGUIItem(const std::string& name);
		void DeleteItem(RenderItem* item);
	public:
		std::vector<RenderItem*> GetAllItems();
		std::vector<StaticRenderItem*> GetStaticItems();
		std::vector<SkeletalRenderItem*> GetSkeletaltems();
		std::vector<GUIRenderItem*> GetGUIItems();
	private:
		std::unordered_map<RenderItem*, std::shared_ptr<RenderItem>> m_RenderItemPool;
		std::unordered_map<RenderItem*, StaticRenderItem*>   m_StaticRIs;
		std::unordered_map<RenderItem*, SkeletalRenderItem*> m_SkeletalRIs;
		std::unordered_map<RenderItem*, GUIRenderItem*>      m_GUIRIs;
	};
}