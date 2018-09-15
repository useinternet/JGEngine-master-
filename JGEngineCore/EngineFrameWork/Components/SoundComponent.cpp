#include"SoundComponent.h"
#include"../../SoundSystem/SoundSystem.h"
using namespace std;
SoundComponent::SoundComponent()
{
	RegisterComponentID(typeid(this));
	m_Sound = make_unique<JGSound>();
}

SoundComponent::~SoundComponent()
{

}
bool SoundComponent::CreateSound(const string& SoundPath, const ESoundMode mode)
{
	bool result = true;
	result = m_Sound->CreateSound(GetSoundSystem()->GetSoundDevice(), mode, SoundPath);
	if (!result)
	{
		return false;
	}
	return true;
}
void SoundComponent::Play()
{
	if (m_Paused)
	{
		m_Paused = false;
		m_Sound->Pause(false);
		m_Play = true;
		return;
	}
	m_Sound->Play();
	m_Play = true;
}
void SoundComponent::Pause() 
{
	m_Play = false;
	m_Paused = true;
	m_Sound->Pause(true);
}
void SoundComponent::Stop()
{
	m_Stop = true;
	m_Play = false;
	m_Paused = false;
	m_Sound->Stop();
}
bool SoundComponent::IsPause()
{
	return m_Paused;
}
bool SoundComponent::IsPlaying()
{
	return m_Play;
}
void SoundComponent::SetVolume(const float Volume)
{
	m_Sound->SetVolume(Volume);
}
void SoundComponent::AddVolume(const float Volume)
{
	m_Sound->AddVolume(Volume);
}