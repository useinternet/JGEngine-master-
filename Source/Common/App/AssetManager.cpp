#include "pch.h"
#include "AssetManager.h"
#include "Math/JBBox.h"
#include "Utill/StringUtill.h"
#include "Data/SDCollection.h"

#include<filesystem>
#include <ostream>
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"


#pragma comment(lib, "assimp-vc142-mt.lib")
using namespace Assimp;

struct StaticMeshData
{
	std::wstring name;
	std::vector<StaticVertex> v;
	std::vector<uint32_t>     i;
	JBBox boundingBox;
};



void ProcessStaticMesh(aiMesh* mesh, const aiScene* scene, StaticMeshData* meshData, std::wstring* errorCode);
void LoadStaticMesh(aiNode* node, const aiScene* scene, std::vector<StaticMeshData>* meshData, std::wstring* errorCode);
void WriteMesh(const std::wstring& output_path, const std::wstring& filename, std::vector<StaticMeshData>& meshData, std::wstring* errorCode);

bool AssetManager::Import(const std::wstring& path, const std::wstring& output_path, FBX_ImportFlags flag, std::wstring* errorCode)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(ws2s(path),
		aiProcess_JoinIdenticalVertices |  // ������ ������ ����, �ε��� ����ȭ
		aiProcess_ValidateDataStructure |  // �δ��� ����� ����
		aiProcess_ImproveCacheLocality |  // ��� ������ ĳ����ġ�� ����
		aiProcess_RemoveRedundantMaterials |  // �ߺ��� ���͸��� ����
		aiProcess_GenUVCoords |  // ����, ������, ���� �� ��� ������ ������ UV�� ��ȯ
		aiProcess_TransformUVCoords |  // UV ��ȯ ó���� (�����ϸ�, ��ȯ...)
		aiProcess_FindInstances |  // �ν��Ͻ��� �Ž��� �˻��Ͽ� �ϳ��� �����Ϳ� ���� ������ ����
		aiProcess_LimitBoneWeights |  // ������ ���� ����ġ�� �ִ� 4���� ����
		aiProcess_OptimizeMeshes |  // ������ ��� ���� �Ž��� ����
		aiProcess_GenSmoothNormals |  // �ε巯�� �븻����(��������) ����
		aiProcess_SplitLargeMeshes |  // �Ŵ��� �ϳ��� �Ž��� �����Ž���� ��Ȱ(����)
		aiProcess_Triangulate |  // 3�� �̻��� �𼭸��� ���� �ٰ��� ���� �ﰢ������ ����(����)
		aiProcess_ConvertToLeftHanded |  // D3D�� �޼���ǥ��� ��ȯ
		aiProcess_SortByPType |  // ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����
		aiProcess_CalcTangentSpace); // ź��Ʈ ���� ��� 


	if (pScene == nullptr)
	{
		if (errorCode) *errorCode += TT(" : ������ �дµ� �����߽��ϴ�.\n");
		return false;
	}
	if (flag & FBX_ImportFlag_ImportMesh)
	{
	
		if (flag & FBX_ImportFlag_SkeletalMesh)
		{
			
		}
		else
		{
			std::vector<StaticMeshData> meshData;
			LoadStaticMesh(pScene->mRootNode, pScene, &meshData, errorCode);


			fs::path p(path);
			auto filename = p.filename().wstring();
			filename = filename.substr(0, filename.length() - p.extension().wstring().length());
			WriteMesh(output_path, filename, meshData,  errorCode);
		}
	}
	if (flag & FBX_ImportFlag_ImportAnimation)
	{

	}
	return true;
}

void ProcessStaticMesh(aiMesh* mesh, const aiScene* scene, StaticMeshData* meshData, std::wstring* errorCode)
{
	// Vertex Data ����
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
	{
		StaticVertex v;

		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;

		v.normal.x = mesh->mNormals[i].x;
		v.normal.y = mesh->mNormals[i].y;
		v.normal.z = mesh->mNormals[i].z;

		if (mesh->mTangents != nullptr)
		{
			v.tangent.x = mesh->mTangents[i].x;
			v.tangent.y = mesh->mTangents[i].y;
			v.tangent.z = mesh->mTangents[i].z;
		}
		else
		{
			if (errorCode) *errorCode += TT("Tangent ��Ҹ� �ҷ����� ���߽��ϴ�.");
		}



		if (mesh->mTextureCoords[0])
		{
			v.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			v.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
		else
		{
			if (errorCode) *errorCode += TT("TexCorrd ��Ҹ� �ҷ����� ���߽��ϴ�.");
		}

		meshData->boundingBox.min.x = std::min<float>(meshData->boundingBox.min.x, v.position.x);
		meshData->boundingBox.min.y = std::min<float>(meshData->boundingBox.min.y, v.position.y);
		meshData->boundingBox.min.z = std::min<float>(meshData->boundingBox.min.z, v.position.z);

		meshData->boundingBox.max.x = std::max<float>(meshData->boundingBox.max.x, v.position.x);
		meshData->boundingBox.max.y = std::max<float>(meshData->boundingBox.max.y, v.position.y);
		meshData->boundingBox.max.z = std::max<float>(meshData->boundingBox.max.z, v.position.z);


		meshData->v.push_back(v);
	}



	// Index Data ����
	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			meshData->i.push_back(face.mIndices[j]);
		}
	}

	//
	meshData->name = s2ws(mesh->mName.C_Str());

}

void LoadStaticMesh(aiNode* node, const aiScene* scene, std::vector<StaticMeshData>* meshData, std::wstring* errorCode)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		StaticMeshData data;
		ProcessStaticMesh(mesh, scene, &data, errorCode);
		meshData->push_back(data);
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i)
	{

		LoadStaticMesh(node->mChildren[i], scene, meshData, errorCode);
	}

}

void WriteMesh(const std::wstring& output_path, const std::wstring& filename, std::vector<StaticMeshData>& meshData, std::wstring* errorCode)
{
	uint32_t numOutput = (uint32_t)meshData.size();


	std::ofstream fout;
	fout.open(ws2s(output_path) + ws2s(filename) + MESH_FILEFORMAT, std::ios::binary);

	// �޽� ����
	fout.write(
		reinterpret_cast<const char*>(&numOutput),
		sizeof(uint32_t));


	// ���ɷ�Ż �޽� ����
	bool is_skeletal = false;
	fout.write(
		reinterpret_cast<const char*>(&is_skeletal),
		sizeof(bool));


	for (uint32_t i = 0; i < numOutput; ++i)
	{
		// Vertex �� ��
		uint32_t vertexCount = (uint32_t)meshData[i].v.size();
		fout.write(
			reinterpret_cast<const char*>(&vertexCount),
			sizeof(uint32_t)
		);

		// Index �� ��
		uint32_t indexCount = (uint32_t)meshData[i].i.size();
		fout.write(
			reinterpret_cast<const char*>(&indexCount),
			sizeof(uint32_t)
		);

		// Vertex 
		fout.write(
			reinterpret_cast<const char*>(meshData[i].v.data()),
			sizeof(StaticVertex) * vertexCount
		);

		// Index
		fout.write(
			reinterpret_cast<const char*>(meshData[i].i.data()),
			sizeof(uint32_t) * indexCount
		);

		// JBBox
		fout.write(
			reinterpret_cast<const char*>(&meshData[i].boundingBox),
			sizeof(JBBox)
		);



	}
	fout.close();

}
