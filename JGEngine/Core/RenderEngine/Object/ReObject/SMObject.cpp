#include "pch.h"
#include "SMObject.h"
#include "ReMesh.h"
#include "Object/DxObject/PipelineState.h"
#include "ShaderCode.h"
using namespace std;

namespace RE
{
	// Light Depth //

	// ��� ������Ʈ
	// �׷��� ������Ʈ
	// ����Ʈ ������Ʈ
	// �о� ������Ʈ (���� �ʿ��� �����ֱ� -> �⺻ ������ ConstantBuffer) <- ��� ��⿡ ��

	// �Է�( ���͸���, ConstantBuffer, StructuredBuffer, �ؽ���)
	// ���
	// ����� (�ؽ���)
	void SMGBufferObject::ConnectObjectStruct(const SCStruct& _struct)
	{
		m_ConnectedObjectStruct = make_shared<SCStruct>(_struct);
		m_ObjectSBuffer = make_shared<SDStructuredBuffer>();
		m_ObjectSBuffer->Set(GetName() + "ObjectData", _struct, ShaderEnum::Visible_Vertex);
	}
	void SMGBufferObject::ConnectSkinnedStruct(const SCStruct& _struct)
	{
		m_ConnectedSkinnedStruct = make_shared<SCStruct>(_struct);
		m_SkinnedCBuffer = make_shared<SDConstantBuffer>();
		m_SkinnedCBuffer->Set(GetName() + "SkinnedData", ShaderEnum::Visible_Vertex);
	}

	void SMGBufferObject::RemoveInstance(uint32_t id)
	{
		m_ObjectSBuffer->RemoveData(id);
	}

	// ��� ����
	// GBuffer��� -> �׷��� ������Ʈ�� �Է����� �޴´�.
	// �� ��� -> �⺻������ �� ������Ʈ�� �Է����� �޴´�.
	// Light��� -> ����Ʈ ������Ʈ�� �Է����� �޴´�.
}