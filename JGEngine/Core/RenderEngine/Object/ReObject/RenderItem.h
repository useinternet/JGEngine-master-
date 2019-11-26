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
	class RenderItem : public ReObject
	{
		
	public:
		RenderItem(const std::string& name);

		InstanceRenderItem* AddInstance();
		void RemoveInstance(InstanceRenderItem* instance);
	public:

		std::shared_ptr<ReMaterial>            Material;
		std::shared_ptr<ReMesh>                Mesh;
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


	enum class EReMeshType;

	class RenderItemManager
	{
	public:
		RenderItemManager();
		RenderItem* CreateItem(const std::string& name);
		void DeleteItem(RenderItem* item);
	public:
		std::vector<RenderItem*> GetAllItem();
		std::vector<RenderItem*> GetItemByMesh(EReMeshType type);
	private:
		std::unordered_map<RenderItem*, std::shared_ptr<RenderItem>> m_RenderItemPool;
	};
}