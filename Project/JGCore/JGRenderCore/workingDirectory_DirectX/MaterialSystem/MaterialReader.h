#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"
#include"Shader/ShaderTool/ShaderObject.h"
// ���͸��� ������ ���� �ؼ� �Ϸ�
// �̰� ���� ���͸���ý��� �Ϸ���
// �ؽ���(��� ����) <- ������ �� ���ϱ� �ؼ� �𵨸� �غ���
// �׸��� ���α��� �� �غ���
// ���� MRT ����

namespace JGRC
{
	class CORE_EXPORT MaterialReader
	{
	private:
		static class MaterialSystem* m_MtSys;
		static class ShaderObjectManager* m_ObjManager;
		
	public:
		MaterialReader();
		~MaterialReader();

		class Material* Read(const std::string& materialPath);

	private:
		bool ReadShaderType(const std::string& buffer, std::fstream& fin, EShaderType* type);
		bool ReadHlslPath(const std::string& buffer, std::fstream& fin, std::string* path);
		bool ReadInputLayout(const std::string& buffer, std::fstream& fin, class Material* mt);
		bool ReadCBuffer(const std::string& buffer, std::fstream& fin, class Material* mt, EShaderType type);
		bool ReadSamplerState(const std::string& buffer, std::fstream& fin, class Material* mt);
		bool ReadTexture(const std::string& buffer, std::fstream& fin, class Material* mt);
		bool Move(const std::string& buffer, const std::string& keyword, std::fstream& fin);
	};
}
