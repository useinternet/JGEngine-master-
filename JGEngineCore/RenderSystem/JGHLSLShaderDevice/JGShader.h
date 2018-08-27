#pragma once
#include"../../EngineStatics/Engine.h"
#include"JGShaderData.h"


class JGDeviceD;
class JGBufferManager;
class JGTexture;
enum class EJGMapType;


/*
Class : JGShader
@m string EntryPoint : ���̴� ���� ����( Default : "main" ) 
@m string Version    : �������� ���̴� ���� ( Default : "5_0" )
@m ComPtr<ID3D11VertexShader> m_VertexShader : ���ؽ����̴� 
@m ComPtr<ID3D11PixelShader> m_PixelShader : �ȼ� ���̴�
@m vector<D3D11_INPUT_ELEMENT_DESC> m_ElementDesc : ���̴� ��� ��� �迭
@m vector<D3DSamplerState> m_vSamplerStates : ���÷�������Ʈ �迭 */
class ENGINE_EXPORT JGShader
{
protected:
	/*
	EnumClass : ESamplerType 
	@e Wrap : Wrap ����
	@e Clamp : Clamp ���� */
	enum class ESamplerType
	{
		Wrap, Clamp
	};
	/*
	ERegisterType 
	@e Vertex : VertexShader�� ���
	@e Pixel : PixelShader�� ��� */
	enum class ERegisterType
	{
		Vertex, Pixel
	};
private:
	typedef std::pair<const std::wstring, Microsoft::WRL::ComPtr<ID3D11SamplerState>> D3DSamplerState;
protected:
	const std::string EntryPoint = "main";
	const std::string Version    = "5_0";
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_PixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_InputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC>      m_ElementDesc;
	std::vector<D3DSamplerState>               m_vSamplerStates;
	JGBufferManager* m_pBufferManager;
	JGDeviceD* m_pDevice;

	UINT m_VertexNumber = 0;
	UINT m_PixelNumber = 0;
public:
	JGShader();
	virtual ~JGShader();
	bool Create(JGDeviceD* Device, JGBufferManager* BufferManager);
	void Render(JGShaderData* Data,JGTexture* Texture);
protected:
	virtual bool CreateShader();
	virtual void JGShaderDataSetting(JGShaderData* Data);
	virtual void JGTextureSetting(JGTexture* Texture);
	/*
	Exp : ���÷�������Ʈ�� �߰��Ѵ�.
	@param const char* SemanticName : �ø�ƽ �̸�
	@param UINT SemanticIndex       : �ø�ƽ �ε���
	@param DXGI_FORMAT Format       : ����
	@param UINT InputSlot           : �Է� ����
	@param D3D11_INPUT_CLASSIFICATION InputSlotClass : �Է½���Ŭ����(�ν��Ͻ���)
	@param UINT InstanceDataStepRate : �ν��Ͻ���(���߿�)  */
	virtual void AddInputElement( const char* SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, 
		UINT InputSlot = 0, D3D11_INPUT_CLASSIFICATION InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
		UINT InstanceDataStepRate = 0);

	/*
	Exp : ��� ���۸� �߰��Ѵ�. 
	@param const std::wstring& BufferName : ���� �̸�
	@param EJGUsageType usage : ���� �뵵
	@param EJGCPUType CPUtype : ���� Cpu Ÿ��
	@param size_t size : ���ε��� ������ Ÿ�� ũ�� */
	virtual void AddConstantBuffer(const std::wstring& BufferName, size_t size);
	/*
	Exp : ���÷�������Ʈ�� �߰��Ѵ�.
	@param : JGDeviceD* Device  : JGDevice ��ġ ������
	@param : const wstring name : �߰��� ���÷� �̸� 
	@param : ESamplerType       : ���÷� Ÿ��  */
	virtual void AddSamplerState(JGDeviceD* Device,const std::wstring& name, ESamplerType type);
	/*
	Exp : ������۸� ���� ���̴��� ����Ѵ�. 
	@param const UINT idx : ������� ��� ��ġ 
	@param JGShaderData* Data : ���̴� ������
	@param EJGMapType : ���� Ÿ��(��Ծ�����)
	@param ERegisterType : ���� ��� Ÿ�� (���ؽ� / �ȼ�) */
	virtual void Write_RegisterBuffer(const std::wstring& BufferName, JGShaderData* Data, EJGMapType MapType, ERegisterType RegisterType);
	/*
	Exp : ���� �Ŵ����� �����´�. */
	JGBufferManager* GetBufferManager();
	/*
	Exp : JGDevice��ġ�� �����´�. */
	JGDeviceD* GetJGDevice();
	/*
	Exp : ���ؽ����̴������͸� �����´�. */
	ID3D11VertexShader* GetVertexShader();
	/*
	Exp : ���ؽ����̴������� �ּҸ� �����´�. */
	ID3D11VertexShader** GetVertexShaderAddress();
	/*
	Exp : �ȼ� ���̴� �����͸� �����´�. */
	ID3D11PixelShader* GetPixelShader();
	/*
	Exp : �ȼ� ���̴� ������ �ּҸ� �����´�. */
	ID3D11PixelShader** GetPixelShaderAddress();
	/*
	Exp :  �Է� ���̾ƿ� �����͸� �����´�. */
	ID3D11InputLayout* GetInputLayout();
	/*
	Exp : �Է� ���̾ƿ� �������ּҸ� �����´�. */
	ID3D11InputLayout** GetInputLayoutAddress();
	/*
	Exp : ��� ����� �����´�. */
	D3D11_INPUT_ELEMENT_DESC* GetInputElements();
	/*
	Exp : �߰��� ��� ������ �����´�. */
	UINT GetElementCount();
	/*
	Exp : ���÷�������Ʈ �����͸� �����´�. */
	ID3D11SamplerState* GetSamplerState(const std::wstring& name);
	/*
	Exp : ���÷� ������Ʈ ������ �ּҸ� �����´�. */
	ID3D11SamplerState** GetSamplerStateAddress(const std::wstring& name);
};