#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"
// 
/* �뷫���� ����
1. �ϴ� SRSRenderTarget �غ�
2. ShaderAnalyzer�� �м��� ���͸��� ����
3. Material ����
4. �׸���.
5. DirectX Render �κ��� �������ؽ��ĸ� �����ͼ� �׸���.
*/
/*

�׸��� ���̴��� Ʋ����.. ��� ���͸��� ������ �޽� Draw�� ���̴� Draw�� �и�..
���͸���->SetMesh() �Լ��� ���� �������� Ż���� �����ϰ� ����
Mesh Ŭ������ ���� ������ ��( �޽� ���� ������ ������ �� �� �̰� �̷�α���ϰ� �޽����� ����) <- ���� mrt ������ ���� �����ϰ� fbx sdk �̿��Ͽ� �޽�
//**** ���ҽ� �Ŵ��� ������Ʈ�� �ϳ� �߰�..
�ؽ��Ŀ��� ( ���ҽ���� ���÷��� ���� �������..)
// fbx sdk �˾ƺ���
*/
namespace JGRC
{
	class JGSRSystem
	{
	private:
		class DirectX* m_Dx;
		std::function<void()> m_RenderFunc;
		std::unique_ptr<class SRSRenderTarget> m_SRSRT;
		std::vector<real> m_ClearColor;
	public:
		JGSRSystem();
		~JGSRSystem();
		void BindingRenderFunc(const std::function<void()>& func);
		bool InitSRSystem(const int width, const int height);
		void Render();
		void SetClearColor(const real r, const real g, const real b, const real a);
		SRSRenderTarget* GetRenderTarget() const;
	};
}
