#pragma once
#include"Object.h"

/*
Class : ExistObject 
Exp : ȭ��� �׷����� ������Ʈ.. */
class ExistObject : public Object
{
private:
	// ������ �غ���..
	class StaticMesh2DComponent* Breath;
	class TextComponent* Frame;
	class InputComponent* Input;
public:
	ExistObject();
	virtual ~ExistObject();

	virtual void BeginObject() override;

	virtual void Tick(const float DeltaTime) override;

	void Move();
};