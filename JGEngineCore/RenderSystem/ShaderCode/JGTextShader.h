#pragma once
#include"../JGHLSLShaderDevice/JGShader.h"




// �ϴ��� ���̴� �ڵ� �ϼ������ϱ�
// �㿡�� ���̴� �����ؼ� ������ �غ���
// 



class JGTextShader : public JGShader
{
public:
	JGTextShader();
	virtual ~JGTextShader();

protected:
	virtual bool CreateShader() override;
	virtual void JGShaderDataSetting(JGShaderData* Data) override;
	virtual void JGTextureSetting(JGTexture* Texture) override;
};