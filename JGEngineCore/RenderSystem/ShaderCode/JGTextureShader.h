#pragma once
#include"../JGHLSLShaderDevice/JGShader.h"




// �ϴ��� ���̴� �ڵ� �ϼ������ϱ�
// �㿡�� ���̴� �����ؼ� ������ �غ���
// 



class JGTextureShader : public JGShader
{
public:
	JGTextureShader();
	virtual ~JGTextureShader();

protected:
	virtual bool CreateShader() override;
	virtual void JGShaderDataSetting(JGShaderData* Data) override;
	virtual void JGTextureSetting(JGTexture* Texture) override;
};