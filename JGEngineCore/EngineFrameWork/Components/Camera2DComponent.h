#pragma once
#include"Motivated2DComponent.h"
// ��ǥ�谡 �ȸ���..
class ENGINE_EXPORT Camera2DComponent : public Motivated2DComponent
{
private:
	friend class World;
	std::unique_ptr<JGMatrix> m_ViewMatrix;

public:
	Camera2DComponent();
	virtual ~Camera2DComponent();
	virtual void Tick(const float DeltaTime) override;
	void CalculateViewMatrix();

private:
	JGMatrix* GetViewMatrixPointer();
};