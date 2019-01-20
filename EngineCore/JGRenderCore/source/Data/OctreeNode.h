#pragma once
#include"DxCommon/DxCommon.h"


namespace JGRC
{
	// �ø� �ȵ� ���鸸 ������Ʈ Array �����ͼ� Draw
	// �� ������ BoundingBox�� ������ �ִ�.
	// ����ü�� �̿��� �ڽ� �з�( BoundingFrustom )
	//  BoundingBox::Contain Ȱ�� <- �Ҷ� ��ü�� ��ǥ�� ���� ��ǥ�� �̵�
	// ����� ������Ʈ�� �׸���.

	class RCORE_EXPORT OctreeNode
	{
	private:
		OctreeNode(const OctreeNode& rhs)            = delete;
		OctreeNode& operator=(const OctreeNode& rhs) = delete;
	private:
		std::vector<class JGRCObject*> m_StaticObjects;
		std::list<class JGRCObject*>   m_DynamicObjects;
		std::vector<OctreeNode*> m_ChildeNodes;
		DirectX::BoundingBox     m_BoundBox;
		OctreeNode*   m_ParentNode = nullptr;
		bool bCulling = false;
	public:
		void BuildOctree(UINT vCenter, UINT vHalfWidth, UINT Level);
		void DistributeObject(std::vector<class JGRCObject*>& ObjArr);
		bool IsIncludeNode(Camera* MainCamera);
		std::vector<JGRCObject*>& GetArray();
	};


}