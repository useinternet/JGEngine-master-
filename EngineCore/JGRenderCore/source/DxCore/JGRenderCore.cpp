#include"JGRenderCore.h"
#include"DxCore.h"
#include"Data/Scene.h"
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace JGRC;
using namespace std;

JGRenderCore::JGRenderCore()
{

}
JGRenderCore::~JGRenderCore()
{
}

bool JGRenderCore::Init(const DxSetting& set)
{
	hWnd = set.hWnd;
	m_DxCore = make_unique<DxCore>(set);
	if (!m_DxCore->InitDirect3D())
	{
		// ���� ó��
		return false;
	}
	ThrowIfFailed(m_DxCore->CommandList()->Reset(m_DxCore->CommandAllocator(), nullptr));

	m_Scene = make_unique<Scene>(m_DxCore.get());
	m_Camera = m_Scene->GetMainCamera();
	BuildTextures();
	BuildLandGeometry();
	BuildLight();
	m_Scene->BuildScene();
	
	// ��� ����Ʈ&ť  �ʱ�ȭ
	ThrowIfFailed(m_DxCore->CommandList()->Close());
	ID3D12CommandList* cmdsList[] = { m_DxCore->CommandList() };
	m_DxCore->CommandQueue()->ExecuteCommandLists(_countof(cmdsList), cmdsList);


	for (int i = 0; i < 3; ++i)
	{
		m_Scene->Update(GameTimer());
		m_Scene->Draw();
	}


	return true;
}
void JGRenderCore::Update(const GameTimer& gt)
{
	InputCamera(gt);
	DirLight->SetRotation(0.0f, gt.TotalTime() * 5, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		if (i % 2 == 0)
		{
			if (PointLight[i]->GetLocation().x > 150.0f)
				PointSwitch[i] = false;
			else if (PointLight[i]->GetLocation().x < -150.0f)
				PointSwitch[i] = true;


			if (!PointSwitch[i])
				PointLight[i]->OffsetLocation(-gt.DeltaTime() * 20.0f, 0.0f, 0.0f);
			else
				PointLight[i]->OffsetLocation(gt.DeltaTime() * 20.0f, 0.0f, 0.0f);
		}
		else
		{
			if (PointLight[i]->GetLocation().z > 150.0f)
				PointSwitch[i] = false;
			else if (PointLight[i]->GetLocation().z < -150.0f)
				PointSwitch[i] = true;

			if (!PointSwitch[i])
				PointLight[i]->OffsetLocation(0.0f, 0.0f, -gt.DeltaTime() * 20.0f);
			else
				PointLight[i]->OffsetLocation(0.0f, 0.0f, gt.DeltaTime() * 20.0f);
		}
	}
	SpotLight[0]->OffsetRotation(0.0f, gt.DeltaTime() * 40.0f, 0.0f);
	SpotLight[1]->OffsetRotation(0.0f, gt.DeltaTime() * 40.0f, 0.0f);
	SpotLight[2]->OffsetRotation(0.0f, gt.DeltaTime() * 40.0f, 0.0f);
	SpotLight[3]->OffsetRotation(0.0f, gt.DeltaTime() * 40.0f, 0.0f);
	//for (int i = 0; i < 4; ++i)
	//{
	//	SpotLight[i]->OffsetRotation(0.0f, gt.DeltaTime() * 40.0f, 0.0f);
	//}
	m_Scene->Update(gt);
}
void JGRenderCore::ScreenReSize(int width, int height)
{
	if (width == 0 || height == 0)
		return;
	//m_DxCore->OnResize(width, height);
	//m_Camera->SetLens(4 / MathHelper::Pi, m_DxCore->AspectRatio(), m_Camera->GetNearZ(),
	//	m_Camera->GetFarZ());
	//m_Scene->OnReSize(width, height);
}
void JGRenderCore::Draw()
{

	m_Scene->Draw();


}
void JGRenderCore::InputCamera(const GameTimer& gt)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_Camera->Walk(200.0f * gt.DeltaTime());
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_Camera->Walk(-200.0f * gt.DeltaTime());
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_Camera->Strafe(-200.0f * gt.DeltaTime());
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_Camera->Strafe(200.0f * gt.DeltaTime());
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_Camera->Up(-200.0f * gt.DeltaTime());
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_Camera->Up(200.0f * gt.DeltaTime());
	}
}
void JGRenderCore::BuildTextures()
{
	m_Scene->AddTexture(L"../Contents/Engine/Textures/tile.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/tile_nmap.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/bricks2.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/bricks2_nmap.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/ice.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/water1.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/waves0.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/waves1.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/Arissa_DIFF_diffuse.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/Arissa_NM_normal.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/Arissa_SPEC_specular.dds");
	m_Scene->AddTexture(L"../Contents/Engine/Textures/sunsetcube1024.dds", ETextureType::Cube);
}
void JGRenderCore::BuildLight()
{
	DirLight = m_Scene->AddLight(ELightType::Direction);
	DirLight->SetLightColor(0.7f, 0.75f, 0.9f);
	DirLight->SetDirection(-1.0f, -1.0f, 1.0f);



	for (int i = 0; i < 4; ++i)
	{
		PointSwitch[i] = false;
		PointLight[i] = m_Scene->AddLight(ELightType::Point,ELightExercise::Dynamic);
		PointLight[i]->SetFalloffStart(0.0f);
		PointLight[i]->SetFalloffEnd(100.0f);
		SpotLight[i] = m_Scene->AddLight(ELightType::Spot, ELightExercise::Dynamic);
		SpotLight[i]->SetFalloffStart(50.0f);
		SpotLight[i]->SetFalloffEnd(100.0f);

		SpotLight[i]->SetDirection(1.0f, -1.0f, 0.0f);
		switch (i)
		{
		case 0:
			PointLight[i]->SetLightColor(1.0f, 0.0f, 0.0f);
			PointLight[i]->SetLocation(150.0f, 5.0f, 0.0f);
			SpotLight[i]->SetLightColor(0.0f, 1.0f, 0.0f);
			SpotLight[i]->SetLocation(100.0f, 10.0f, 0.0f);
			break;
		case 1:
			PointLight[i]->SetLightColor(0.0f, 1.0f, 0.0f);
			PointLight[i]->SetLocation(0.0f, 5.0f, 150.0f);
			SpotLight[i]->SetLightColor(0.928f, 0.34f, 1.0f);
			SpotLight[i]->SetLocation(0.0f, 10.0f, 100.0f);
			break;
		case 2:
			PointLight[i]->SetLightColor(1.0f, 1.0f, 0.0f);
			PointLight[i]->SetLocation(-150.0f, 5.0f, 0.0f);
			SpotLight[i]->SetLightColor(1.0f, 1.0f, 1.0f);
			SpotLight[i]->SetLocation(-100.0f, 10.0f, 0.0f);
			break;
		case 3:
			PointLight[i]->SetLightColor(0.0f, 1.0f, 1.0f);
			PointLight[i]->SetLocation(0.0f, 5.0f, -150.0f);
			SpotLight[i]->SetLightColor(1.0f, 0.0f, 1.0f);
			SpotLight[i]->SetLocation(0.0f, 10.0f, -100.0f);
			break;
		}
	}
}
void JGRenderCore::BuildLandGeometry()
{
	JGStaticMesh* GroundMesh = m_Scene->AddStaticMesh();
	GroundMesh->AddGridArg("Grid", 1000, 1000, 500, 500);


	JGSkeletalMesh* SphereMesh = m_Scene->AddSkeletalMesh();
	SphereMesh->AddFbxMeshArg("../Contents/Engine/Meshs/Bellydancing.fbx");
	JGSkeletalMesh* SphereMesh2 = m_Scene->AddSkeletalMesh();
	SphereMesh2->AddFbxMeshArg("../Contents/Engine/Meshs/Female Tough Walk.fbx");
	JGSkeletalMesh* SphereMesh3 = m_Scene->AddSkeletalMesh();
	SphereMesh3->AddFbxMeshArg("../Contents/Engine/Meshs/Hip Hop Dancing.fbx");
	JGSkeletalMesh* SphereMesh4 = m_Scene->AddSkeletalMesh();
	SphereMesh4->AddFbxMeshArg("../Contents/Engine/Meshs/Hip Hop Dancing2.fbx");
	JGStaticMesh* SkyMesh = m_Scene->AddStaticMesh();
	SkyMesh->AddBoxArg("Sky", 1.0f, 1.0f, 1.0f, 1);
	

	string AnimName  = m_Scene->AddAnimation("../Contents/Engine/Meshs/Bellydancing.fbx")[0];
	string AnimName2 = m_Scene->AddAnimation("../Contents/Engine/Meshs/Female Tough Walk.fbx")[0];
	string AnimName3 = m_Scene->AddAnimation("../Contents/Engine/Meshs/Hip Hop Dancing.fbx")[0];
	string AnimName4 = m_Scene->AddAnimation("../Contents/Engine/Meshs/Hip Hop Dancing2.fbx")[0];
	SphereMesh->SetAnimation(AnimName);
	SphereMesh2->SetAnimation(AnimName2);
	SphereMesh3->SetAnimation(AnimName3);
	SphereMesh4->SetAnimation(AnimName4);

	MaterialDesc Desc;
	Desc.Name = "GroundMat";
	Desc.ShaderPath = L"../Contents/Engine/Shaders/Default.hlsl";
	JGMaterial* GroundMat = m_Scene->AddMaterial(Desc);
	GroundMat->SetTexture(ETextureSlot::Diffuse, L"../Contents/Engine/Textures/tile.dds");
	GroundMat->SetTexture(ETextureSlot::Normal, L"../Contents/Engine/Textures/tile_nmap.dds");
	GroundMat->SetDiffuseAlbedo(1.0f, 1.0f, 1.0f, 1.0f);
	GroundMat->SetFresnelR0(1.0f, 1.0f, 1.0f);
	GroundMat->SetRoughness(1.0f);


	Desc.Name = "SphereMat";
	Desc.bCubeMapStatic = true;
	JGMaterial* SphereMat = m_Scene->AddMaterial(Desc);
	SphereMat->SetDiffuseAlbedo(1.0f, 0.0f, 0.4f, 1.0f);
	SphereMat->SetFresnelR0(1.0f, 1.0f, 1.0f);
	SphereMat->SetRoughness(0.7f);

	Desc.Name = "InstanceMat";
	Desc.bCubeMapStatic = true;
	Desc.Mode = EPSOMode::INSTANCE;
	JGMaterial* InsMat = m_Scene->AddMaterial(Desc);
	InsMat->SetTexture(ETextureSlot::Diffuse, L"../Contents/Engine/Textures/bricks2.dds");
	InsMat->SetTexture(ETextureSlot::Normal, L"../Contents/Engine/Textures/bricks2_nmap.dds");
	InsMat->SetDiffuseAlbedo(1.0f, 0.0f, 0.0f, 1.0f);
	InsMat->SetFresnelR0(1.0f, 1.0f, 1.0f);
	InsMat->SetRoughness(0.05f);
	InsMat->SetRefractive(1.0f);


	JGRCObject* Obj1 = m_Scene->CreateObject(GroundMat, GroundMesh);
	Obj1->SetLocation(0.0f, -1.0f, 0.0f);

	JGRCObject* Obj2 = m_Scene->CreateObject(SphereMat, SphereMesh);
	Obj2->SetLocation(0.0f, 0.0f, 0.0f);

	JGRCObject* Obj3 = m_Scene->CreateObject(SphereMat, SphereMesh2);
	Obj3->SetLocation(-250.0f, 0.0f, 0.0f);

	JGRCObject* Obj4 = m_Scene->CreateObject(SphereMat, SphereMesh3);
	Obj4->SetLocation(250.0f, 0.0f, 0.0f);

	JGRCObject* Obj5 = m_Scene->CreateObject(SphereMat, SphereMesh4);
	Obj5->SetLocation(0.0f, 0.0f, -250.0f);


	JGRCObject* SkyBox = m_Scene->CreateSkyBox(L"../Contents/Engine/Textures/sunsetcube1024.dds");
	m_Scene->SetMainSkyBox(SkyBox);
}
void JGRenderCore::DrawRenderItems()
{
	m_Scene->Draw();
}
void JGRenderCore::UpdateLook(WPARAM btnState, int x, int y, POINT& mLastMousePos)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		m_Camera->Pitch(dy);
		m_Camera->RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}