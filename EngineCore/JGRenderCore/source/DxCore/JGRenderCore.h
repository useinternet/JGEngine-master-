#pragma once
#include"DxCommon/DxCommon.h"
#include"DxSetting.h"
#include"UserInterface/Interface_Scene.h"
class Camera;


// �ν��Ͻ� ���� ���
// �Ǻ� �� -> �Ǻ� �� �͸� ������Ʈ(UpdateNotify -- �߰� )
namespace JGRC
{
	class DxCore;
	class Scene;

	class RCORE_EXPORT JGRenderCore
	{
	private:
		std::unique_ptr<DxCore>      m_DxCore;
		std::vector<std::unique_ptr<Scene>> m_SceneMems;
		std::unordered_map<std::string, Scene*> m_Scenes;

	private:
		JGRenderCore(const JGRenderCore& copy) = delete;
		JGRenderCore& operator=(const JGRenderCore& copy) = delete;
	public:
		JGRenderCore();
		~JGRenderCore();
		bool Init(const DxSetting& set);
		IF_Scene CreateScene(const std::string& SceneName);
		IF_Scene GetScene(const std::string& SceneName);
		void Build(const GameTimer& gt);
		void Update(const GameTimer& Timer);
		void ReSize(int width, int height);
		void Draw();
	};
}
