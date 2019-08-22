#include"pch.h"
#include<filesystem>
#include"AssetExporter.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#pragma comment(lib, "assimp.lib")

using namespace std;
using namespace Assimp;
namespace fs = experimental::filesystem;
namespace AssetExporter
{
	// ������ Ÿ�� ���� 
	struct Vector3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	struct Vector2
	{
		float x = 0.0f;
		float y = 0.0f;
	};
	struct Matrix4x4
	{
		float m[4][4];
	};
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector3 tangent;
		Vector2 tex;
	};
	using Index = uint32_t;
	struct MeshData
	{
		std::string Name;
		std::vector<Vertex>   Vertices;
		std::vector<Index>    Indices;
	};


	void PushError(EError error, vector<EError>* outputError)
	{
		if (outputError)
		{
			outputError->push_back(error);
		}
	}

	// ������ �ε�
	void LoadProcessMesh(aiMesh* aimesh, MeshData& output, vector<EError>* outputError, uint32_t IndexOffset);
	void LoadMesh(aiNode* node, const aiScene* pScene, std::vector<MeshData>& output,vector<EError>* outputError);
	// ������ ����
	void WriteMeshFile(std::vector<MeshData>& output, const std::string& outputpath);
	void WriteMeshFile(std::vector<MeshData>& output, const std::string& outputpath, const std::string& filename);
	std::string ErrorToString(EError error)
	{
		switch (error)
		{
		case Failed_Read_File:
			return "Failed Read File..";
		case NoneTangent:
			return "None Tangent";
		case NoneTexcoord:
			return "None Texcoord";
		}
		return "None";
	}
	void Execute(const std::string& path, const std::string& outputpath, EOption option, vector<EError>* outputError)
	{
		Importer importer;

		const aiScene* pScene = importer.ReadFile(path,
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
			PushError(Failed_Read_File, outputError);
			return;
		}
		else
		{


			if (option & opt_Mesh)
			{
				vector<MeshData> output;
				LoadMesh(pScene->mRootNode, pScene, output, outputError);
				if (option & opt_MeshCombined)
				{
					fs::path fpath(path);
					std::string filename = fpath.filename().string();
					filename = filename.substr(0, filename.length() - 4);
					WriteMeshFile(output, outputpath, filename);
				}
				else
				{
					WriteMeshFile(output, outputpath);
				}
			
			}
		}


	}

	void LoadMesh(aiNode* node, const aiScene* pScene, std::vector<MeshData>& outputData, vector<EError>* outputError)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* aimesh = pScene->mMeshes[node->mMeshes[i]];
			MeshData data;
			LoadProcessMesh(aimesh, data, outputError, 0);
			outputData.push_back(data);
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			LoadMesh(node->mChildren[i], pScene, outputData, outputError);
		}

	}
	void LoadProcessMesh(aiMesh* aimesh, MeshData& output, vector<EError>* outputError, uint32_t IndexOffset)
	{
		// Vertex Data ����
		for (uint32_t i = 0; i < aimesh->mNumVertices; ++i)
		{
			Vertex v;

			v.position.x = aimesh->mVertices[i].x;
			v.position.y = aimesh->mVertices[i].y;
			v.position.z = aimesh->mVertices[i].z;

			v.normal.x = aimesh->mNormals[i].x;
			v.normal.y = aimesh->mNormals[i].y;
			v.normal.z = aimesh->mNormals[i].z;

			if (aimesh->mTangents != nullptr)
			{
				v.tangent.x = aimesh->mTangents[i].x;
				v.tangent.y = aimesh->mTangents[i].y;
				v.tangent.z = aimesh->mTangents[i].z;
			}
			else
			{
				PushError(NoneTangent, outputError);
			}



			if (aimesh->mTextureCoords[0])
			{
				v.tex.x = (float)aimesh->mTextureCoords[0][i].x;
				v.tex.y = (float)aimesh->mTextureCoords[0][i].y;
			}
			else
			{
				PushError(NoneTexcoord, outputError);
			}

			output.Vertices.push_back(v);
		}



		// Index Data ����
		for (uint32_t i = 0; i < aimesh->mNumFaces; ++i)
		{
			aiFace face = aimesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				output.Indices.push_back(IndexOffset + face.mIndices[j]);
			}
		}

		//
		output.Name = aimesh->mName.C_Str();
	}
	void WriteMeshFile(std::vector<MeshData>& output,const std::string& outputpath)
	{
		uint32_t numOutput = (uint32_t)output.size();


		for (uint32_t i = 0; i < numOutput; ++i)
		{
			MeshData& data = output[i];
			auto& vertices = data.Vertices;
			auto& indices = data.Indices;
			string& name = data.Name;
			//Vector3 maxV = { FLT_MIN , FLT_MIN };
			//Vector3 minV = { FLT_MAX, FLT_MAX };
			//for (int j = 0; j < vertices.size(); ++j)
			//{

			//}
			ofstream fout;
			fout.open(outputpath + name + MESH_FILEFORMAT, ios::binary);

			// �޽� ������ ����
			uint32_t meshCount = 1;

			fout.write(
				reinterpret_cast<const char*>(&meshCount),
				sizeof(uint32_t)
			);


			bool is_skeletal = false;

			// ���ɷ�Ż �޽� ����
			fout.write(
				reinterpret_cast<const char*>(&is_skeletal),
				sizeof(bool)
			);
			// �޽� �̸� ����
			uint32_t len = (uint32_t)name.length() + 1;
			fout.write(
				reinterpret_cast<const char*>(&len),
				sizeof(uint32_t)
			);

			// �޽� �̸�
			const char* c_name = name.c_str();

			fout.write(
				reinterpret_cast<const char*>(c_name),
				sizeof(char) * len
			);

			// Vertex �� ��
			uint32_t vertexCount = (uint32_t)vertices.size();
			fout.write(
				reinterpret_cast<const char*>(&vertexCount),
				sizeof(uint32_t)
			);

			// Index �� ��

			uint32_t indexCount = (uint32_t)indices.size();
			fout.write(
				reinterpret_cast<const char*>(&indexCount),
				sizeof(uint32_t)
			);

			// Vertex 

			fout.write(
				reinterpret_cast<const char*>(vertices.data()),
				sizeof(Vertex) * vertexCount
			);

			// Index
			fout.write(
				reinterpret_cast<const char*>(indices.data()),
				sizeof(Index) * indexCount
			);


			fout.close();
		}


	}
	void WriteMeshFile(std::vector<MeshData>& output, const std::string& outputpath, const std::string& filename)
	{
		uint32_t numOutput = (uint32_t)output.size();
		ofstream fout;
		fout.open(outputpath + filename + MESH_FILEFORMAT, ios::binary);

		// �޽� ����
		fout.write(
			reinterpret_cast<const char*>(&numOutput),
			sizeof(uint32_t)
		);
		// ���ɷ�Ż �޽� ����
		bool is_skeletal = false;
		fout.write(
			reinterpret_cast<const char*>(&is_skeletal),
			sizeof(bool)
		);

		for (uint32_t i = 0; i < numOutput; ++i)
		{
			MeshData& data = output[i];
			auto& vertices = data.Vertices;
			auto& indices = data.Indices;
			string& name = data.Name;
			



			// �޽� �̸� ����
			uint32_t len = (uint32_t)name.length() + 1;
			fout.write(
				reinterpret_cast<const char*>(&len),
				sizeof(uint32_t)
			);

			// �޽� �̸�
			const char* c_name = name.c_str();

			fout.write(
				reinterpret_cast<const char*>(c_name),
				sizeof(char) * len
			);

			// Vertex �� ��
			uint32_t vertexCount = (uint32_t)vertices.size();
			fout.write(
				reinterpret_cast<const char*>(&vertexCount),
				sizeof(uint32_t)
			);

			// Index �� ��

			uint32_t indexCount = (uint32_t)indices.size();
			fout.write(
				reinterpret_cast<const char*>(&indexCount),
				sizeof(uint32_t)
			);

			// Vertex 

			fout.write(
				reinterpret_cast<const char*>(vertices.data()),
				sizeof(Vertex) * vertexCount
			);

			// Index
			fout.write(
				reinterpret_cast<const char*>(indices.data()),
				sizeof(Index) * indexCount
			);



		}





		fout.close();


	}
}



