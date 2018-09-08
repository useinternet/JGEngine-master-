#pragma once
#include"Component.h"
#include"../../SoundSystem/JGSound.h"





// ���߿� ������ġ�� �̿��Ͽ� ȿ���� �� ������ �־� ���� ����ϰ� �����..( ���� ���� )
class ENGINE_EXPORT SoundQueComponent : public Component
{
private:
	std::vector<std::shared_ptr<JGSound>> m_vSounds;



public:
	SoundQueComponent();
	virtual ~SoundQueComponent();
	bool AddSound(const std::string& SoundPath,ESoundMode mode = ESoundMode::Sound2D);

};