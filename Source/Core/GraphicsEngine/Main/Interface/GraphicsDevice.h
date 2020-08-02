#pragma once
#include "GEObject.h"



/*
class : GraphicsDevice

ex) ���ҽ�, �޸�, ����� �����ϴ� ��ġ Ŭ����
*/

namespace GE
{
	class Texture;
	class GRAPHICSENGINE_API GraphicsDevice : public GEObject
	{
	public:
		virtual void Create(void* config_user_data) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Destroy() = 0;




		virtual bool LoadTextures(const std::vector<std::wstring>& paths, std::vector<GE::Texture*>& t) = 0;
	};
}