
// �����ڵ� ��ȯ
#define TT(x) L##x

// EnumClass �� int�� ��ȯ(�迭 �ε�����)
#define EnumClassToInt(x) \
static_cast<int>(x##)     \


// 
#define JG_PI    (3.14159265358979323846)
inline float OneByPI()
{
	return (1.0f / (float)JG_PI);
}
inline float ToRadian(const float Degree)
{
	return (Degree * ((float)JG_PI / 180.0f));
}

inline float ToDegree(const float Radian)
{
	return (Radian * (180.0f / (float)JG_PI));
}
