#include"JG2DAnimationShader.h"
#include"../JGHLSLShaderDevice/JGShaderCompiler.h"
#include"../JGHLSLShaderDevice/JGBuffer.h"
#include"../JGTexture.h"
#include"../JGDeviceD.h"
#include"HLSLConstantBufferDesc.h"
JG2DAnimationShader::JG2DAnimationShader()
{
}

JG2DAnimationShader::~JG2DAnimationShader()
{
}

bool JG2DAnimationShader::CreateShader()
{
	bool result = true;
	AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT);
	AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	JGShaderCompiler Compiler(
		GetVertexShaderAddress(),
		GetPixelShaderAddress(),
		GetInputElements(),
		GetElementCount(),
		GetInputLayoutAddress(),
		EntryPoint,
		Version);

	// ������
	result =Compiler.Compile(
		GetJGDevice(), TT("../JGEngineCore/RenderSystem/ShaderHLSL/2DAnimationShader_vs.hlsl"),
		TT("../JGEngineCore/RenderSystem/ShaderHLSL/2DAnimationShader_ps.hlsl"));
	if (!result)
	{
		return false;
	}
	// ��� ���� �߰��ϰ�
	AddConstantBuffer(TT("MatrixBuffer"), sizeof(SMatrixBuffer_VS));
	AddConstantBuffer(TT("AnimationIncrementBuffer"), sizeof(SAnimationIncrementBuffer_VS));
	AddConstantBuffer(TT("2DSpriteBuffer"), sizeof(S2DSpriteBuffer_PS));
	// ���÷� �߰��ϰ�..
	AddSamplerState(GetJGDevice(), TT("WrapSampler"), ESamplerType::Wrap);
	return true;
}

void JG2DAnimationShader::JGShaderDataSetting(JGShaderData * Data)
{
	// ������ ����
	Write_RegisterBuffer(TT("MatrixBuffer"), Data, EJGMapType::Write_Discard,
		ERegisterType::Vertex);
	Write_RegisterBuffer(TT("AnimationIncrementBuffer"), Data, EJGMapType::Write_Discard,
		ERegisterType::Vertex);
	Write_RegisterBuffer(TT("2DSpriteBuffer"), Data, EJGMapType::Write_Discard,
		ERegisterType::Pixel);
}

void JG2DAnimationShader::JGTextureSetting(JGTexture* Texture)
{
	// �ؽ��� ����
	GetJGDevice()->GetContext()->PSSetShaderResources(0, 1, Texture->GetEntry());
}
