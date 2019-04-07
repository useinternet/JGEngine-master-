#include<PCH.h>
#include"AssetExtracter.h"
#include<fstream>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<Data/Data_IO.h>
#include<Util/StringUtil.h>
using namespace JgAsset;
using namespace std;
using namespace Common;
using namespace DirectX;

XMFLOAT4X4 AiToXm(const aiMatrix4x4& m)
{
	DirectX::XMFLOAT4X4 mat;
	mat._11 = m.a1; mat._12 = m.a2; mat._13 = m.a3; mat._14 = m.a4;
	mat._21 = m.b1; mat._22 = m.b2; mat._23 = m.b3; mat._24 = m.b4;
	mat._31 = m.c1; mat._32 = m.c2; mat._33 = m.c3; mat._34 = m.c4;
	mat._41 = m.d1; mat._42 = m.d2; mat._43 = m.d3; mat._44 = m.d4;
	return mat;
}
XMFLOAT3 AiToXm(const aiVector3D& v)
{
	return XMFLOAT3(v.x, v.y, v.z);
}
XMFLOAT4 AiToXm(const aiQuaternion& q)
{
	return XMFLOAT4(q.x, q.y, q.z, q.w);
}

bool AssetExtracter::Extract(const string& path, const string& outputpath, EAssetTypes type)
{

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path,
		aiProcess_JoinIdenticalVertices    |  // ������ ������ ����, �ε��� ����ȭ
		aiProcess_ValidateDataStructure    |  // �δ��� ����� ����
		aiProcess_ImproveCacheLocality     |  // ��� ������ ĳ����ġ�� ����
		aiProcess_RemoveRedundantMaterials |  // �ߺ��� ���͸��� ����
		aiProcess_GenUVCoords              |  // ����, ������, ���� �� ��� ������ ������ UV�� ��ȯ
		aiProcess_TransformUVCoords        |  // UV ��ȯ ó���� (�����ϸ�, ��ȯ...)
		aiProcess_FindInstances            |  // �ν��Ͻ��� �Ž��� �˻��Ͽ� �ϳ��� �����Ϳ� ���� ������ ����
		aiProcess_LimitBoneWeights         |  // ������ ���� ����ġ�� �ִ� 4���� ����
		aiProcess_OptimizeMeshes           |  // ������ ��� ���� �Ž��� ����
		aiProcess_GenSmoothNormals         |  // �ε巯�� �븻����(��������) ����
		aiProcess_SplitLargeMeshes         |  // �Ŵ��� �ϳ��� �Ž��� �����Ž���� ��Ȱ(����)
		aiProcess_Triangulate              |  // 3�� �̻��� �𼭸��� ���� �ٰ��� ���� �ﰢ������ ����(����)
		aiProcess_ConvertToLeftHanded      |  // D3D�� �޼���ǥ��� ��ȯ
		aiProcess_SortByPType              |  // ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����
		aiProcess_CalcTangentSpace         ); // ź��Ʈ ���� ��� 

	if (pScene == nullptr)
		return false;

	else
	{
		if (type & AssetType_Mesh)
		{
			AssetMeshPackage package;
			LoadMesh(pScene->mRootNode, pScene, package);
			OutputFile_Mesh(outputpath, package);
		}
		if (type & AssetType_BoneHierarchy)
		{
			AssetBoneHierarchyPackage package;
			LoadBoneHierarchy(pScene->mRootNode, nullptr, pScene);

			for (auto& bone : m_AssetBoneNodeByAi)
				package.BoneNodes.push_back(bone.second);

			m_AssetBoneNodeByAi.clear();
			m_BoneIndex = 0;
			package.Name = Util::Path_ExtractFileName_NoFormat(path);
				

			OutputFile_BoneHierarchy(outputpath, package);
		}
		if (type & AssetType_SkeletalMesh)
		{
			AssetSkeletalMeshPackage package;
			LoadSkeletalMesh(pScene->mRootNode, pScene, package);
			OutputFile_SkMesh(outputpath, package);
		}
		if (type & AssetType_Animation)
		{
			aiMatrix4x4 aiMat = pScene->mRootNode->mTransformation;
			aiMat.Transpose().Inverse();
			for (uint32_t i = 0; i < pScene->mNumAnimations; ++i)
			{
				AssetAnimation anim;
				anim.InvGlobalTransform = AiToXm(aiMat);
				LoadAnimation(pScene->mAnimations[i], anim);
				OutputFile_Animation(outputpath, anim);
			}
		}
	}

	return true;
}

void ProcessMesh(aiMesh* aimesh, const aiScene* scene, AssetMesh& assetMesh)
{
	// ���� ������ 
	assetMesh.AssetVertexs.resize(aimesh->mNumVertices);
	for (uint32_t i = 0; i < aimesh->mNumVertices; ++i)
	{
		AssetVertex v;
		v.position.x = aimesh->mVertices[i].x;
		v.position.y = aimesh->mVertices[i].y;
		v.position.z = aimesh->mVertices[i].z;
		v.normal.x = aimesh->mNormals[i].x;
		v.normal.y = aimesh->mNormals[i].y;
		v.normal.z = aimesh->mNormals[i].z;

		if (aimesh->mTangents != nullptr)
		{
			assetMesh.IsTangents = true;
			v.tangent.x = aimesh->mTangents[i].x;
			v.tangent.y = aimesh->mTangents[i].y;
			v.tangent.z = aimesh->mTangents[i].z;
		}
		else
			assetMesh.IsTangents = false;

		if (aimesh->mTextureCoords[0])
		{
			assetMesh.IsTexcoords = true;
			v.texcoord.x = (float)aimesh->mTextureCoords[0][i].x;
			v.texcoord.y = (float)aimesh->mTextureCoords[0][i].y;
		}
		else
			assetMesh.IsTexcoords = false;
		
		assetMesh.AssetVertexs[i] = v;
	}

	// �ε��� ������
	for (uint32_t i = 0; i < aimesh->mNumFaces; ++i)
	{
		aiFace face = aimesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			assetMesh.AssetIndices.push_back(face.mIndices[j]);
		}
	}
}
void AssetExtracter::LoadMesh(aiNode* node, const aiScene* scene, AssetMeshPackage& Meshpackage)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		AssetMesh assetMesh;
		ProcessMesh(aimesh, scene, assetMesh);

		Meshpackage.AssetMeshs.push_back(assetMesh);
		Meshpackage.AssetMeshNames.push_back(aimesh->mName.C_Str());
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		LoadMesh(node->mChildren[i], scene, Meshpackage);
	}
}
void AssetExtracter::OutputFile_Mesh(const string& outputpath, AssetMeshPackage& Meshpackage)
{
	for (uint32_t i = 0; i < Meshpackage.AssetMeshs.size(); ++i)
	{
		Data_IO dataOutput(EIOType::Write);
		AssetMeshStream stream;

		stream.MeshName = Meshpackage.AssetMeshNames[i];
		stream.MeshData = Meshpackage.AssetMeshs[i];

		dataOutput << stream;
		dataOutput.Write(outputpath + stream.MeshName + ASSET_MESH_FILEFORMAT);
	}
}
void AssetExtracter::LoadBoneHierarchy(aiNode* node, aiNode* parent, const aiScene* scene)
{
	if (node->mName.length != 0 && node->mNumMeshes == 0 && node != scene->mRootNode)
	{
		AssetBoneNode bone;
		bone.ID = m_BoneIndex++;
		bone.Name = string(node->mName.C_Str());
		aiMatrix4x4 mat = node->mTransformation;
		bone.Transform = AiToXm(mat.Transpose());

		
		m_AssetBoneNodeByAi[node] = bone;
	

		// �θ� ��尡 �����ϴ� ��� ���
		if (m_AssetBoneNodeByAi.find(parent) != m_AssetBoneNodeByAi.end())
		{
			AssetBoneNode* parentBone = &m_AssetBoneNodeByAi[parent];
			AssetBoneNode* meBone     = &m_AssetBoneNodeByAi[node];


			parentBone->ChildIDs.push_back(meBone->ID);
			parentBone->ChildCount++;

			meBone->ParentID = parentBone->ID;
		}

		parent = node;
	}
	for (uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		LoadBoneHierarchy(node->mChildren[i], parent, scene);
	}
}
void AssetExtracter::OutputFile_BoneHierarchy(const std::string& outputPath, AssetBoneHierarchyPackage& bonePackage)
{
	AssetBoneHierarchyStream stream;
	stream.Bones = move(bonePackage.BoneNodes);
	stream.Name  = move(bonePackage.Name);
	Data_IO dataOutput(EIOType::Write);
	dataOutput << stream;
	dataOutput.Write(outputPath + stream.Name + ASSET_SKELETALHIERARCHY_FILEFORMAT);
}


void AddBoneWeightAndVertexID(UINT Boneindex, float w, UINT vertexId, AssetSkeletalMesh& skMesh)
{

	if (skMesh.AssetVertexs[vertexId].boneWeights[0] == 0)
	{
		skMesh.AssetVertexs[vertexId].bondId[0] = Boneindex;
		skMesh.AssetVertexs[vertexId].boneWeights[0] = w;
		return;
	}
	if (skMesh.AssetVertexs[vertexId].boneWeights[1] == 0)
	{
		skMesh.AssetVertexs[vertexId].bondId[1] = Boneindex;
		skMesh.AssetVertexs[vertexId].boneWeights[1] = w;
		return;
	}
	if (skMesh.AssetVertexs[vertexId].boneWeights[2] == 0)
	{
		skMesh.AssetVertexs[vertexId].bondId[2] = Boneindex;
		skMesh.AssetVertexs[vertexId].boneWeights[2] = w;
		return;
	}
	if (skMesh.AssetVertexs[vertexId].boneWeights[3] == 0)
	{
		skMesh.AssetVertexs[vertexId].bondId[3] = Boneindex;
		skMesh.AssetVertexs[vertexId].boneWeights[3] = w;
		return;
	}

}
void ProcessSkMesh(aiMesh* mesh, const aiScene* scene, AssetSkeletalMesh& skMesh, AssetBoneOffsets& BoneOffsets)
{
	skMesh.AssetVertexs.resize(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
	{
		AssetSkeletalVertex v;
		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;
		v.normal.x = mesh->mNormals[i].x;
		v.normal.y = mesh->mNormals[i].y;
		v.normal.z = mesh->mNormals[i].z;

		if (mesh->mTangents != nullptr)
		{
			skMesh.IsTangents = true;
			v.tangent.x = mesh->mTangents[i].x;
			v.tangent.y = mesh->mTangents[i].y;
			v.tangent.z = mesh->mTangents[i].z;
		}
		else
			skMesh.IsTangents = false;

		if (mesh->mTextureCoords[0])
		{
			skMesh.IsTexcoords = true;
			v.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			v.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
		else
			skMesh.IsTexcoords = false;

		skMesh.AssetVertexs[i] = v;
	}
	// ��
	UINT BoneCount = 0;
	UINT BoneIndex = BoneCount;
	std::map<string, uint32_t> BoneMapping;
	for (UINT i = 0; i < mesh->mNumBones; ++i)
	{
		aiBone* CurrBone = mesh->mBones[i];
		string boneName = CurrBone->mName.C_Str();
		if (BoneMapping.find(boneName) == BoneMapping.end())
		{
			aiMatrix4x4 aiMat = CurrBone->mOffsetMatrix;
			XMFLOAT4X4 offset = AiToXm(aiMat.Transpose());

			BoneIndex = BoneCount++;

			AssetBoneOffset boneData;
			boneData.Name   = boneName;
			boneData.Offset = offset;
			boneData.ID = BoneIndex;
			BoneOffsets.push_back(boneData);

			BoneMapping[boneName] = BoneIndex;
		}
		else
		{
			BoneIndex = BoneMapping[boneName];
		}
		for (uint32_t j = 0; j < CurrBone->mNumWeights; ++j)
		{
			UINT vID = CurrBone->mWeights[j].mVertexId;
			float weight = CurrBone->mWeights[j].mWeight;
			AddBoneWeightAndVertexID(BoneIndex, weight, vID, skMesh);
		}
	}

	// �ε���
	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			skMesh.AssetIndices.push_back(face.mIndices[j]);
		}
	}
}
void AssetExtracter::LoadSkeletalMesh(aiNode* node, const aiScene* scene,
	AssetSkeletalMeshPackage& Meshpackage)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		AssetSkeletalMesh skMesh;
		AssetBoneOffsets boneOffsets;
		ProcessSkMesh(aimesh, scene, skMesh, boneOffsets);
		Meshpackage.AssetMeshs.push_back(skMesh);
		Meshpackage.AssetBoneOffsets.push_back(boneOffsets);
		Meshpackage.AssetMeshNames.push_back(aimesh->mName.C_Str());
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		LoadSkeletalMesh(node->mChildren[i], scene, Meshpackage);
	}
}
void AssetExtracter::OutputFile_SkMesh(const std::string& outputpath, AssetSkeletalMeshPackage& Meshpackage)
{
	for (uint32_t i = 0; i < Meshpackage.AssetMeshs.size(); ++i)
	{
		Data_IO dataOutput(EIOType::Write);
		AssetSkeletalMeshStream stream;
		stream.MeshName = move(Meshpackage.AssetMeshNames[i]);
		stream.MeshData = move(Meshpackage.AssetMeshs[i]);
		stream.BoneOffsets = move(Meshpackage.AssetBoneOffsets[i]);

		dataOutput << stream;
		dataOutput.Write(outputpath + stream.MeshName + ASSET_SKELETALMESH_FILEFORMAT);
	}
}



void AssetExtracter::LoadAnimation(aiAnimation* anim,AssetAnimation& output)
{
	output.Duration = (float)anim->mDuration;
	output.TickPerSecond = (float)anim->mTicksPerSecond;
	output.Name = anim->mName.C_Str();




	for (uint32_t i = 0; i < anim->mNumChannels; ++i)
	{
		AssetAnimChannel channel;
		aiNodeAnim* CurrNode = anim->mChannels[i];

		channel.Name = CurrNode->mNodeName.C_Str();

		for (uint32_t j = 0; j < CurrNode->mNumPositionKeys; ++j)
		{
			AssetVectorFrame frame;
			frame.timepos = (float)CurrNode->mPositionKeys[j].mTime;
			frame.value = AiToXm(CurrNode->mPositionKeys[j].mValue);

			channel.AnimPositionFrames.push_back(frame);
		}
		for (uint32_t j = 0; j < CurrNode->mNumRotationKeys; ++j)
		{
			AssetQuatFrame frame;
			frame.timepos = (float)CurrNode->mRotationKeys[j].mTime;
			frame.value = AiToXm(CurrNode->mRotationKeys[j].mValue);

			channel.AnimRotationFrames.push_back(frame);
		}
		for (uint32_t j = 0; j < CurrNode->mNumScalingKeys; ++j)
		{
			AssetVectorFrame frame;
			frame.timepos = (float)CurrNode->mScalingKeys[j].mTime;
			frame.value = AiToXm(CurrNode->mScalingKeys[j].mValue);

			channel.AnimScaleFrames.push_back(frame);
		}

		output.AnimChannels.push_back(channel);
	}

}
void AssetExtracter::OutputFile_Animation(const std::string& outputPath, AssetAnimation& animation)
{
	AssetAnimationStream stream = animation;
	Data_IO dataOutput(EIOType::Write);
	dataOutput << stream;
	dataOutput.Write(outputPath + stream.Name + ASSET_ANIM_FILEFORMAT);

}

