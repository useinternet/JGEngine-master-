#pragma once
#include"Common/dllExportDefine.h"
namespace JGRC
{
	class JGDevice;
	/*
	EnumClass : EJGBufferType
	@e ConstantBuffer :  ��� ����
	@e VertexBuffer   :  ���� ����
	@e IndexBuffer    :  �ε��� ����
	@e InstanceBuffer :  �ν��Ͻ� ����
	*/
	enum class CORE_EXPORT EBufferType
	{
		ConstantBuffer = 0,
		VertexBuffer = 1,
		IndexBuffer = 2,
		InstanceBuffer = 3
	};
	/*
	EnumClass : EJGUsageType
	@e Static  : ������ Ÿ��
	@e Dynamic : ������ Ÿ��
	*/
	enum class CORE_EXPORT EUsageType
	{
		Static = 0,
		Dynamic = 1
	};
	/*
	EnumClass : EJGCPUType
	@e Access_Write : ����� ����
	@e Access_Read  : �б�� ����
	@e None         : ����X
	*/
	enum class CORE_EXPORT ECPUType
	{
		Access_Write = 0,
		Access_Read = 1,
		None = 2
	};
	/*
	EnumClass : EJGMapType
	@e Read  :  �б� ���
	@e Write :  ���� ���
	@e Read_Write : �б� ���� ��� ���
	@e Write_Discard : ���� ��� ( ���� �����͸� ��� �����.)
	@e Write_NoOverWrite : ���� ���( ���� �����͸� ������ �ʴ´�.)
	*/
	enum class CORE_EXPORT EMapType
	{
		Read = 0,
		Write = 1,
		Read_Write = 2,
		Write_Discard = 3,
		Write_NoOverWrite = 4
	};
}