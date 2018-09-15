#pragma once
#include"Object.h"



// ������
// �ӵ�
// �ǹ��� ���� ����� ������
// �߽� ��ǥ


/*
Class : ExistObject 
Exp : ȭ��� �׷����� ������Ʈ.. */
class ExistObject : public Object
{
private:
	class TextComponent* Frame;
	class InputComponent* Input;
	class TestAnim* TestAnimation;

	class Box2DCollisionComponent* SampleCollison;
	class ImageBox* image;
	class Box2DCollisionComponent* Ground;
	class TextComponent* MousePosText;
	class TextComponent* TestAnimationPosText;
public:
	ExistObject();
	virtual ~ExistObject();

	virtual void BeginObject(World* world) override;

	virtual void Tick(const float DeltaTime) override;
	void Right();
	void Left();
	void Up();
	void Down();
};
