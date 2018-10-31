#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"
#include"MaterialSystem/Shader/ShaderTool/ShaderObject.h"

namespace JGRC
{
	// enum class EShaderType;
	class CORE_EXPORT ShaderAnalyzer
	{
	private:
		class JGBufferManager* m_BufferMG;
		class DirectX*         m_DirectX;
	private:
		// hlsl path
		// �Է� ���̾ƿ�����
		// ��� ����
		std::unique_ptr<class LayoutInformation>  m_InputLayoutInfor;
		std::unique_ptr<class CBufferInformation> m_CBufferInfor;
		std::unique_ptr<class SamplerInformation> m_SamplerInfor;
		std::unique_ptr<class TextureInformation> m_TextureInfor;
		std::vector<class JGBuffer*> m_vBuffers;
		EShaderType m_ShaderType;
		bool m_bIgnore = false;
	public:
		ShaderAnalyzer();
		~ShaderAnalyzer();

		bool Analyze(const std::string& hlslPath, const EShaderType ShaderType);
		void CreateConstantBuffers();
		void WriteConstantBuffers();
		/*
		Exp : �Է� ���̾ƿ� �迭�� ����ϴ�. */
		void MakeInputLayoutArray(class InputLayout* ly);
		float*   GetParam(const std::string& paramName);
		void     SetParam(const std::string& paramName, void* value);
		uint     GetParamSize(const std::string& paramName);
	private:
		bool RemoveRemark(std::string& sentence);
		std::string IncludeAyz(std::string hlslPath, std::string& sentence);
	};
}
