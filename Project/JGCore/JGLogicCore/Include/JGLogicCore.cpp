#include"JGLogicCore.h"
#include"JGRenderCore.h"
#include"MaterialSystem/Mesh/Mesh.h"
#include"MaterialSystem/Shader/HlslEditor.h"
#include"MaterialSystem/Shader/ShaderWriter.h"
#include"MaterialSystem/Shader/ShaderReader.h"
#include"MaterialSystem/Shader/JGShaderArray.h"
#include"DirectX/DxWindow.h"
#include"MaterialSystem/Mesh/Mesh.h"
#include"JGRenderCore.h"
#include"DirectX/DirectX.h"
using namespace JGLC;
using namespace JGRC;
using namespace std;
JGLogicCore::~JGLogicCore()
{
	delete Cube;
}
void JGLogicCore::TestInit(JGRenderCore* Rc, HWND hWnd)
{
	this->hWnd = hWnd;
	this->Rc = Rc;
	HlslEditor* VertexHlsl = Rc->GetHlslEditor(EShaderType::Vertex, Game::path / "Engine/Shader/HLSL/StandardShader_vs.hlsl");
	

	// �Է� ���̾ƿ�
	InputLayout* Vertex_Il = VertexHlsl->CreateInputLayout();
	Vertex_Il->AddDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	Vertex_Il->AddDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	Vertex_Il->AddDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0);
	Vertex_Il->AddDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0);
	Vertex_Il->AddDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0);
	
	CBuffer* Cbf = VertexHlsl->CreateCBuffer();
	Cbf->SetName("MatrixBuffer");
	Cbf->AddVar("worldMatrix", 0, 15);
	Cbf->AddVar("viewMatrix", 16, 31);
	Cbf->AddVar("projectionMatrix", 32, 47);

	HlslEditor* PixelHlsl = Rc->GetHlslEditor(EShaderType::Pixel, Game::path / "Engine/Shader/HLSL/StandardShader_ps.hlsl");
	

	Texture2D* texture = PixelHlsl->CreateTexture2D();
	texture->Add("Default_Texture");

	SamplerState* Sampler = PixelHlsl->CreateSamplerState();
	Sampler->AddDefaultWrapMode();

	ShaderWriter writer;
	writer.AddEditor(VertexHlsl);
	writer.AddEditor(PixelHlsl);
	writer.Write(Game::path / "Engine/Shader/Shader/Sample");
	ShaderReader reader(hWnd);
	arr = reader.ReadShader(Game::path / "Engine/Shader/Shader/Sample.shader");

	arr->Get(EShaderType::Pixel)->AddTexture("Default_Texture",Game::path / "Engine/Texture/stone01.dds");
	arr->Get(EShaderType::Pixel)->AddTexture("Normal_Texture", Game::path / "Engine/Texture/bump01.dds");
	// ����
	Mesh* Cube = new Mesh;
	Cube->LoadModel(Game::path / "cube.txt",true);
	Cube->CreateBuffer();
	DxWindow* wn = Rc->GetDxWindow(hWnd);
	wn->AddMainEvent([Cube,this]()
	{
		Cube->Render();
		arr->Render(Cube->getIndexCount());
	});
	int num = 0;
}
void JGLogicCore::TestTick()
{
	static real yaw = 0;
	yaw += 0.0001f;
	jgMatrix4x4 rotation;
	rotation.rotationY(yaw);

	jgMatrix4x4 worldMatrix;
	worldMatrix.identity();
	worldMatrix = worldMatrix * rotation;
	worldMatrix.transpose();


	jgMatrix4x4 viewMatrix;
	viewMatrix.lookAtLH(jgVec3(0.0f, 0.0f, -5.0f), jgVec3(0.0f, 0.0f, 1.0f), jgVec3(0.0f, 1.0f, 0.0f));
	viewMatrix.transpose();
	DxWindow* wn = Rc->GetDxWindow(hWnd);
	jgMatrix4x4 projectionMatrix;

	projectionMatrix = wn->GetProjectionMatrix();
	projectionMatrix.transpose();

	arr->Get(EShaderType::Vertex)->SetParam("worldMatrix", &worldMatrix, 16);
	arr->Get(EShaderType::Vertex)->SetParam("viewMatrix", &viewMatrix, 16);
	arr->Get(EShaderType::Vertex)->SetParam("projectionMatrix", &projectionMatrix, 16);
}