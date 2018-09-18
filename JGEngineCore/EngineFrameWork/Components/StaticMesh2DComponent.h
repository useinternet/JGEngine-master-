#pragma once
#include"Motivated2DComponent.h"
class JG2DMesh;
class JGTexture;
class JGShaderData;
class ConstructObject;
enum class EPivot;

/*
Class : StaticMesh2DComponent 
@m wstring m_ShaderName : 적용할 셰이더 이름
@m JGTexture& m_Texture : 텍스쳐
@m JG2DMesh m_Mesh   : 메시 
@m EPivot*  m_pPivot : 메시 기준점 */
class ENGINE_EXPORT StaticMesh2DComponent : public Motivated2DComponent
{
private:
	std::wstring m_ShaderName = TT("None");
	JGTexture* m_Texture = nullptr;
	JG2DMesh* m_Mesh     = nullptr;
	EPivot*  m_pPivot    = nullptr;

	/// 셰이더 데이터 용 변수
	std::unique_ptr<struct SMatrixBuffer_VS>   m_MatrixBufferDesc;
	std::unique_ptr<struct S2DSpriteBuffer_PS> m_2DSpriteBufferDesc;
public:
	StaticMesh2DComponent();
	virtual ~StaticMesh2DComponent();
	/*
	Exp : StaticMesh2DComponent의 변수들을 셋팅한다. 
	@param StaticMesh2DObject* Object : JGConstructHelper::StaticMesh2D 클래스가 생성해준 오브젝트 포인터 */
	virtual void SetConstructObject(ConstructObject* Object);
	/*
	Exp : 텍스쳐의 가로 길이를 가져온다. */
	float GetTextureWdith();
	/*
	Exp : 텍스쳐의 세로 길이를 가져온다. */
	float GetTextureHeight();
	/*
	Exp : 스프라이트 설정을 키고 스프라이트 색을 정한다. */
	void SpriteSwichOn(const float r, const float g, const float b);
	/*
	Exp : 스프라이트 설정을 키고 스프라이트 색을 정한다. */
	void SpriteSwichOn(JGVector3D& color);
	/*
	Exp : 컬러라이즈를 설정한다. */
	void SetColorRize(const float r, const float g, const float b);
	/*
	Exp : 컬러라이즈를 설정한다. */
	void SetColorRize(JGVector3D& color);
	/*
	Exp : 알파값을 설정한다. */
	void SetAlphaBlend(const float a);

	/*
	Exp : 해당 매쉬의 중심점을 가져온다. */
	virtual JGVector2D GetCenterPoint();
public:
	/*
	Exp : 렌더링 한다.(셰이더가 다르다면 상속 받아서 커스텀 하셈 
	경고!! : Component의 Render함수를 호출할것..) */
	virtual void Render() override;
protected:
	/*
	Exp : 셰이더 상수버퍼의 파라미터들을 설정한다. ( 상속 받아서 쓰세요 / 부모 함수 호출 안해도되요.. */
	virtual void ShaderParamSetting(JGShaderData* Data);
	/*
	Exp : 메시를 가져온다. */
	JG2DMesh * GetMesh();
	/*
	Exp : 메시를 셋팅한다. */
	void SetMesh(JG2DMesh* mesh);
	/*
	Exp : 텍스쳐를 가져온다. */
	JGTexture* GetTexture();
	/*
	Exp : 텍스쳐를 셋팅한다. */
	void SetTexture(JGTexture* texture);
	/*
	Exp : 이용중인 셰이더 이름을 가저온다. */
	std::wstring& GetShaderName();
	/*
	Exp : 셰이더이름을 셋팅한다. */
	void SetShaderName(const std::wstring& ShaderName);
	/*
	Exp : 적용중인 Pivot을 가져온다. */
	EPivot GetPivot();
	/*
	Exp : 적용중인 Pivot을 셋팅한다. */
	void SetPivot(EPivot* pivot);
private:
	virtual bool Render2DCurling() override;
};