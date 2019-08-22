#pragma once
#include<iostream>
#include<string>
#define MESH_FILEFORMAT ".mesh"
#define SKEELETAL_FILEFORMAT ".skeletal"
#define ANIM_FILEFORMAT ".anim"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiBone;
struct aiAnimMesh;
struct aiAnimation;
struct aiVectorKey;
struct aiQuatKey;
struct aiNodeAnim;



namespace AssetExporter
{

	/*    .mesh ������ 

	1. �޽� ������ ����
	1. bool is_skeletal : ���̷�Ż �޽�(true) �ΰ� ����ƽ �޽�(false) �ΰ�

	// -> ���� ���� ���� ���� �� �б�

	// -> ���� ���ʹ� �޽� ������ ���� ��ŭ �б�
	2. uint32_t len   : �޽� �̸� ���ڿ� ����
	3. char name[len] : �޽� �̸� 
	4. uint32_t vertexCount : Vertex ����
	5. uint32_t indexCount :  Index  ����
	6. �� ����ƽ�̳� ���̷�Ż �޽� ���� ������
	7. �� ����ƽ �̳� ���̷�Ż �޽� �ε��� ������

	*/

	enum EOption
	{
		opt_Mesh = 0x00001,
		opt_SkeletalMesh = 0x00002,
		opt_MeshCombined = 0x00004,
		opt_Animation = 0x0008
	};

	enum EError
	{
		Failed_Read_File,
		NoneTangent,
		NoneTexcoord
	};

	std::string ErrorToString(EError error);

	void Execute(const std::string& path, const std::string& outputpath, EOption option, std::vector<EError>* outputError = nullptr);;
};