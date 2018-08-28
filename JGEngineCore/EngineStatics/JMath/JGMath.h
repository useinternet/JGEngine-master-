#pragma once

/*
Class : JMath 
Exp : ���� ���� ���� �Լ����� �� ���⿡~~!*/
class JGMath
{
private:
	static const float ErrorRange;
public:
	/*
	Exp : �������� ���Ѵ�.*/
	static float Sqrt(float num);
	/*
	Exp : ���� �������� �Ǻ��Ѵ�.(���������ȿ���) */
	static bool IsZero(const float num);
	/*
	Exp : �ΰ��� ���� ������ �Ǻ��Ѵ�.(�������� �ȿ���)*/
	static bool Equal(const float num1,const float num2);
	/*
	Exp : ���밪�� ���Ѵ�. */
	static float Abs(const float num);

};