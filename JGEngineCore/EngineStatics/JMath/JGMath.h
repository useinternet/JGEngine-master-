#pragma once
#include"../Engine.h"
#include<random>
/*
Class : JMath 
Exp : ���� ���� ���� �Լ����� �� ���⿡~~!*/
class JGVector2D;
class ENGINE_EXPORT JGMath
{
public:
	static const float ErrorRange;
	static std::random_device rd;
public:
	/*
	Exp : �������� ���Ѵ�.*/
	static float Sqrt(float num);
	/*
	Exp : ���� �������� �Ǻ��Ѵ�.(���������ȿ���) */
	static bool IsZero(const float num);
	static bool IsZero(const float num,const float CustomErrorRange);
	/*
	Exp : �ΰ��� ���� ������ �Ǻ��Ѵ�.(�������� �ȿ���)*/
	static bool Equal(const float num1,const float num2);
	/*
	Exp : ���밪�� ���Ѵ�. */
	static float Abs(const float num);
	/*
	Exp : �ִ밪 �� ���Ѵ�.  */
	static float Max(const float num1, const float num2);
	/*
	Exp : �ּڰ��� ���Ѵ�. */
	static float Min(const float num1, const float num2);
	/*
	Exp : ������ ���Ѵ�.. */
	static void DrawRegion(float& target,const float min, const float max);

	/*
	Exp : �ش� �������� �������� ���ڰ� ���ϵȴ�.*/
	static int RandomDraw(const int minRange, const int maxRange);
	/*
	���� Direct 2D ��ǥ�踦 ��ũ�� ��ǥ��� �ٲ۴�. */
	static JGVector2D D2DToScreen(const float ScreenWidth, const float ScreenHeight,JGVector2D& vector);
	static JGVector2D D2DToScreen(const float ScreenWidth, const float ScreenHeight,const float x, const float y);
};