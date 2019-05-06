#pragma once
#include"DirectXCommon/DirectXCommon.h"

namespace DirectX
{
	class ScratchImage;
}
namespace Dx12
{
	namespace DxUtil
	{
		// Create Command Objects
		
		/* ID3D12CommandQueue ����
		@param type : Ŀ�ǵ� ����Ʈ Ÿ�� */
		ComPtr<ID3D12CommandQueue>        CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type);


		/* ID3D12GraphicsCommandList ����
		@param type : Ŀ�ǵ� ����Ʈ Ÿ�� 
		@param CmdAllocator : Ŀ�ǵ� �Ҵ��� */
		ComPtr<ID3D12GraphicsCommandList> CreateCommandList(
			D3D12_COMMAND_LIST_TYPE type,
			ComPtr<ID3D12CommandAllocator> CmdAllocator);

		/* ID3D12CommandAllocator ����
        @param type : Ŀ�ǵ� ����Ʈ Ÿ�� */
		ComPtr<ID3D12CommandAllocator>    CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type);


		/* ���̴� ������
		@param filename :  hlsl ���� path
		@param define   : shader define ����
		@param entrypoint : ���� �Լ�(main) �̸�
		@param target   : ���̴� �� */
		ComPtr<ID3DBlob> CompileShader(
			const std::wstring& filename,
			const D3D_SHADER_MACRO* defines,
			const std::string& entrypoint,
			const std::string& target,
			bool* isSuccess = nullptr,
			std::string* errorStr = nullptr);


		/* ��Ʈ ���� ����  */
		ComPtr<ID3D12RootSignature> CreateRootSignature(CD3DX12_ROOT_SIGNATURE_DESC* rootSigDesc);


		UINT CalcConstantBufferByteSize(UINT byteSize);
	}

}
