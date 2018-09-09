#pragma once
#include"Widget.h"


class StaticMesh2DComponent;
class InputComponent;
enum class EPivot;

/*
EnumClas : EButtonImageType
@e Ordinary : ���� ��ư �̹���
@e ButtonClick : ��ư Ŭ�� �� �̹���
@e MousePointerInButton : ���콺 �����Ͱ� ��ư �ȿ� ���� ���� �̹��� */
enum class EButtonImageType
{
	Ordinary,
	ButtonClick,
	MousePointerInButton
};
/*
Class : Button 
@m unique_ptr<StaticMesh2DComponent> m_OrdinaryImage    : ���� �̹���
@m unique_ptr<StaticMesh2DComponent> m_ButtonClickImage : ��ư Ŭ���� �̹���
@m unique_ptr<StaticMesh2DComponent> m_MousePointerInButtonImage : ���콺 �����Ͱ� ��ư �ȿ� ���� ���� �̹���
@m unique_ptr<InputComponent> m_InputComponent : �Է� ������Ʈ( ���콺 �Է� )
@m unique_ptr<SButtonRect> m_ButtonRect : ��ư �浹�� �簢��
@m EButtonImageType m_ButtonImageType : ��ư �̹��� Ÿ��  */
class ENGINE_EXPORT Button : public Widget
{
	typedef struct SButtonRect
	{
		float Top;
		float Left;
		float Right;
		float Bottom;
	}SButtonRect;
private:
	std::unique_ptr<StaticMesh2DComponent> m_OrdinaryImage;
	std::unique_ptr<StaticMesh2DComponent> m_ButtonClickImage;
	std::unique_ptr<StaticMesh2DComponent> m_MousePointerInButtonImage;
	std::unique_ptr<InputComponent> m_InputComponent;
	std::unique_ptr<SButtonRect> m_ButtonRect;
	EButtonImageType m_ButtonImageType;

	bool m_bPosInit = false;
public:
	Button();
	virtual ~Button();
	virtual void Tick(const float DeltaTime) override;

	/*
	Exp : ��ư ����
	@param const EPivot pivot
	@param cosnt wstring& OrdinaryImagePath : ���� ��ư �̹��� ���
	@param const wstring& ButtonClickImagePath : ��ư Ŭ���� �̹��� ���
	@param const wstring& MousePointerInButtonImagePath : ���콺 �����Ͱ� ��ư �簢���ȿ� ������ �̹��� ���*/
	void CreateButton(const EPivot pivot,const std::wstring& OrdinaryImagePath, const std::wstring& ButtonClickPath = TT("None"), const std::wstring& MousePointerInButtonImagePath = TT("None"));
	/*
	Exp : ���İ� ���� 
	@param const float a : ������ ���� ��
	@param const EButtonImageType buttonType : ������ ��ư Ÿ�� */
	void SetBlend(const float a,const EButtonImageType buttonType);
	/*
	Exp : ���콺 �����Ϳ� �浹 üũ*/
	bool CollisionCheck();
	/*
	Exp : ��ư �̹��� Ÿ�� ���� */
	void SetButtonImage(const EButtonImageType type);

	/*
	Exp : ���� ���콺 �����Ͱ� ��ư �浹 �簢�� �ȿ� �ִ��� ���� */
	bool IsMousePointerInButton();
	/*
	Exp : ��ưŬ�� ���� */
	bool IsMouseClickInButton();
	/*
	Exp : ���� ���� */
	bool IsNoneInButton();
};