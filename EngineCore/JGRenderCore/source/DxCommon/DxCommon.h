#pragma once
#ifdef JGRENDERCORE_EXPORTS
#define RCORE_EXPORT __declspec(dllexport)
#else
#define RCORE_EXPORT __declspec(dllimport)
#endif
#ifdef _DEBUG
#pragma comment(lib, "JGLibrary_d.lib")
#else
#pragma comment(lib, "JGLibrary.lib")
#endif

#pragma warning(disable: 4251 4996 4005) 
#pragma comment(lib,"assimp-vc140-mt")
#include"d3dUtil.h"
#include"GeometryGenerator.h"
#include"UploadBuffer.h"
#include"GameTimer.h"
#include"ResourceManagement/ResourceManager.h"
#include"Data/EngineFrameResource.h"
#include"Shader/Shader.h"
#include"Math/ObjectController/CoordinationController.h"