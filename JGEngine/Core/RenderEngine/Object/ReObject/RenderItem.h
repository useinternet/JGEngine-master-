#pragma once
#include "Object/ReObject.h"



namespace RE
{

	class ReMesh;
	class ReMaterial;
	// RenderItem�� renderengine�� ����
	// ��� Mesh, Material �� ���� �Ŵ������� ����
	// ��� module, pass ������ ���¸Ŵ������� ����
	// ��� �ؽ���, ���� ����, �ִϸ��̼� ������ ���¸Ŵ������� ����
	class RenderItem : public ReObject
	{
		
	public:
		RenderItem(const std::string& name);



	public:

		std::shared_ptr<ReMaterial>            Material;
		std::shared_ptr<ReMesh>                Mesh;
		// 
		JMatrix TempWorld = JMatrix::Identity();
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