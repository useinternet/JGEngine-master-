#pragma once

#include"Widget.h"
#include"../../../RenderSystem/JGMaterial/JG2DMesh.h"
class JGTexture;
class StaticMesh2DComponent;
class ConstructObject;
/*
Class : ImageBox
Exp : UI �� �̹��� �ڽ� .. 
@m unique_ptr<StaticMesh2DComponent> m_Image :  �̹��� ������Ʈ */
class ENGINE_EXPORT ImageBox : public Widget
{
private:
	static std::map<std::wstring, ConstructObject*> m_mImagePath;
	std::unique_ptr<StaticMesh2DComponent> m_Image;
	float m_BlendAlpha = 1.0f;
public:
	ImageBox();
	virtual ~ImageBox();

	/*
	Exp : �̹��� �ڽ��� �����մϴ�. 
	@param const wstring& ImagePath : �̹��� ��� 
	@param const EPivot pivot : ������ ���� */
	bool CreateImage(const std::wstring& ImagePath, const EPivot pivot);
	/*
	Exp : ���� �� ���� */
	void SetBlend(const float a);
	/*
	Exp : ���� ���� �ش簪 ��ŭ ���ϱ� */
	void AddBlend(const float a);


	void ReSizeImage(const float width, const float height);

};