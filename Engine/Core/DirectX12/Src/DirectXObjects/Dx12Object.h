#pragma once
#include"DirectXCommon/DirectXCommon.h"
#include"ShaderCommonDefines.h"
#include"Mesh.h"
#include"Material.h"



namespace Dx12
{
	/*
	1. root ���� ���� �ۼ�
	2. �ν��Ͻ� �� �ִϸ��̼�
	3. Object �迭�� ���̴��� ���ε�
	4. ���ɷ�Ż�� instance �ϳ��� ���ɷ�Ż �ִϸ��̼� �ϳ� �Ҵ�
	*/
	class CommandList;
	class Camera;
	class Dx12Object;
	class ObjectFragments;
	
	class Dx12Object
	{
		using ObjectCBArray  = std::vector<ObjectCB>;
		using SkinnedCBArray = std::vector<SkinnedCB>;
		using ObjectFragmentsArray = std::vector<std::shared_ptr<ObjectFragments>>;
	private:
		std::string m_Name;
		uint32_t m_InstanceCount;
		Mesh     m_Mesh;
		SkinnedCB            m_SkinnedCB;
		ObjectCBArray        m_ObjectCBDatas;
		ObjectFragmentsArray m_ObjectFragments;
		GraphicsPSO          m_PSO;
		bool m_BuildOK;
	public:
		Dx12Object(const std::string& name = "Dx12Object");
	public:
		const std::string& GetName() const {
			
			return m_Name;
		}
		const Mesh* GetMesh_c() const {
			return &m_Mesh;
		}
		Mesh* GetMesh() {
			return &m_Mesh;
		}
		uint32_t GetInstanceCount() const {
			return (uint32_t)m_ObjectFragments.size();
		}
	public:
		void SetMesh(const Mesh& mesh);
		void SetName(const std::string& name);
		ObjectFragments* AddObjectFragments();
		ObjectFragments* GetObjectFragments(uint32_t instanceID);
		void RemoveObjectFragments(ObjectFragments* obj);
		void Build(CommandList* commandList);
		void Update(SkinnedCB* skinnedCB = nullptr);
		void Draw(CommandList* commandList);
		void Draw(CommandList* commandList, const GraphicsPSO& pso);
	};
	class ObjectFragments
	{
		friend Dx12Object;
	private:
		Dx12Object* m_OwnerObject;
		std::string m_Name;
		uint32_t    m_InstanceID;
		Common::Transformation m_Transform;
		// JMatrix4x4 �� ����� Ʈ�������� ���� �׸��� Set �Ҷ����� �ʱ�ȭ
	public:
		ObjectFragments(Dx12Object* obj) : m_OwnerObject(obj){}
		const std::string& GetName() const {
			return m_Name;
		}
		uint32_t GetInstanceID() const {
			return m_InstanceID;
		}
		Common::Transformation& GetTransform() {
			return m_Transform;
		}
		const Common::Transformation& GetTransform_c() const{
			return m_Transform;
		}
		Common::Location& GetLocation() {
			return m_Transform._Location();
		}
		const Common::Location& GetLocation() const {
			return m_Transform._Location_c();
		}
		Common::Rotation& GetRotation() {
			return m_Transform._Rotation();
		}
		const Common::Rotation& GetRotation_c() const {
			return m_Transform._Rotation_c();
		}
		Common::Scale& GetScale() {
			return m_Transform._Scale();
		}
		const Common::Scale& GetScale() const {
			return m_Transform._Scale_c();
		}
	};
}