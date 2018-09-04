#pragma once
#include"../../../EngineStatics/Engine.h"
#include"../../../RenderSystem/JGMaterial/JG2DMesh.h"


class JGTexture;
class JGDeviceD;
class JGBufferManager;

class ENGINE_EXPORT JGFontVertexInformation
{
public:
	float Width = 0.0f;
	float Height = 0.0f;
	float TexWidth = 0.0f;
	float TexHeight = 0.0f;
	float TexU = 0.0f;
	float TexV = 0.0f;
	float XAdvance = 0.0f;
	bool Space = false;
};

class ENGINE_EXPORT JGFontLoader
{
private:
	class Font;
public:
	struct SFontMesh;
	struct SFontInformation;
	struct SFontCommonType;
	struct SFontType;
	struct SKerningType;
	typedef unsigned int CharID;
	typedef std::pair<const CharID, const CharID> PairID;
	typedef struct SFontInformation
	{
		/*
		FontName : ���� Ÿ�� ��Ʈ�� �̸��Դϴ�.
		FontSize : Ʈ�� Ÿ�� ��Ʈ�� ũ��.
		bold     : �۲��� ���� ǥ�õ˴ϴ�.
		italic   : �� ����� ���Դϴ�.
		charset  : ��� �� OEM ���� ��Ʈ�� �̸� (���� �ڵ尡 �ƴ� ���).
		unicode  : ���� �ڵ� ���� ��Ʈ�̸� 1�� �����Ͻʽÿ�.
		stretchH : �۲� ���̰� ������� �þ�ϴ�. 100 %�� ���༺�� ������ �ǹ��մϴ�.
		smoothing: smoothing�� ���� ������ 1�� �����Ͻʽÿ�.
		aa       : ��� �� ���� ���ø� ����. 1�� ���� ���ø��� ������ �ʾ� ���� �ǹ��մϴ�.
		padding[4] : �� ������ �е� (����, ������, �Ʒ���, ����).
		spacing[2] : �� ������ ���� (����, ����).
		outline    : ������ ������ �β��Դϴ�.
		*/
		std::string  FontName;
		size_t FontSize;
		size_t bold;
		size_t italic;
		std::string  charset;
		bool  unicode;
		int   stretchH;
		bool  smoothing;
		bool  aa;
		int padding[4];
		int spacing[2];
		size_t outline;
	}SFontInformation;
	typedef struct SFontCommonType
	{
		/*
		lineHeight : �� �ؽ�Ʈ �� ������ �Ÿ� (�ȼ� ����)�Դϴ�.
		base       : ���� ���� ��ܿ��� ������ �ر����� �ȼ� ���Դϴ�.
		scaleW     : �ؽ�ó�� ��. �Ϲ������� ���� �̹����� x pos ũ�⸦ �����ϴ� �� ���˴ϴ�.
		scaleH     : �ؽ�ó�� ����. �Ϲ������� ���� �̹����� y pos ũ�⸦ �����ϴ� �� ���˴ϴ�.
		pages      : �۲ÿ� ���� �� �ؽ��� �������� ��.
		packed     : �ܻ� ���ڰ� �� �ؽ�ó ä�ο� ����Ǿ� ������ 1�� �����մϴ�. �� ��� alphaChnl�� �� ä�ο� ����� ������ �����մϴ�.
		alphaChnl  : ä�ο� �׸� �� �����Ͱ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� ������ ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		redChnl    : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �� ������ �ܰ����� ���� �����ϰ��ִ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		greenChnl  : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� �ܰ����� ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		blueChnl   : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� �ܰ����� ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		pageID     : ������ ID.
		FileName   : �ؽ�ó ���� �̸�.
		*/
		float lineHeight;
		float base;
		float scaleW;
		float scaleH;
		float pages;
		float packed;
		float alphaChnl;
		float redChnl;
		float greenChnl;
		float blueChnl;
		float pageID;
		std::string FileName;
	}SFontCommonType;
	typedef struct SFontType
	{
		/*
		ID      : ���� ID�Դϴ�.
		x       : �ؽ����� ���� �̹����� ���� ��ġ.
		y       : �ؽ�ó�� ���� �̹����� ���� ��ġ�Դϴ�.
		Width   : �ؽ�ó�� ���� �̹����� �ʺ��Դϴ�.
		Height  : �ؽ�ó�� ���� �̹����� �����Դϴ�.
		XOffset : �ؽ�ó���� ȭ������ �̹����� ���� �� �� ���� ��ġ�� �󸶳� �����µǾ�� �ϴ����� �����մϴ�.
		YOffset : �ؽ�ó���� ȭ������ �̹����� ���� �� �� ���� ��ġ�� �󸶳� �����µǾ�� �ϴ����� �����մϴ�.
		XAdvance : ĳ���� �׸��� �� ���� ��ġ�� �󸶸�ŭ �մ�� ���մϴ�.
		Page    : ���� �̹������ִ� �ؽ��� ������.
		chnl    : ���� �̹������ִ� �ؽ�ó ä�� (1 = �Ķ���, 2 = ���, 4 = ������, 8 = ����, 15 = ��� ä��).

		*/
		CharID ID;
		size_t x;
		size_t y;
		size_t Width;
		size_t Height;
		float  XOffset;
		float  YOffset;
		float XAdvance;
		float Page;
		float chnl;
	}SFontType;
	typedef struct SKerningType
	{
		/*
		first  : ù ��° ���� ID�Դϴ�.
		second : �� ��° ���� ID�Դϴ�.
		amount : ù ��° ���� �ٷ� ������ �� ��° ���ڸ� �׸� �� x ��ġ�� �����ؾ��ϴ� �����Դϴ�.
		*/
		CharID first;
		CharID second;
		float amount;
	}SKerningType;
private:
	std::map<const std::string, std::shared_ptr<Font>> m_mFonts;
	std::map< const std::string, const std::wstring> m_mFontPath;
public:
	JGFontLoader();
	~JGFontLoader();
	static JGFontLoader* GetInstance();
	bool LoadFont(JGDeviceD* Device, const std::string& FontPath, const std::wstring& FontTexturePath);
	void OutputVertexInformation(const std::string& FontPath,const std::wstring& Text, float TextSize,std::vector<JGFontVertexInformation>* Array);
private:
	void LoadFontInformation(std::ifstream& fin,Font* font);
	void LoadFontCommonInformation(std::ifstream& fin, Font* font);
	void LoadFontData(std::ifstream& fin, Font* font);
	void LoadKerningInformation(std::ifstream& fin, Font* font);
	void EqualLoop(std::ifstream& fin);
private:
	class Font
	{
	public:
		std::unique_ptr<JGTexture> Texture;
		std::unique_ptr<SFontInformation> FontInformation;
		std::unique_ptr<SFontCommonType>  FontCommonInformation;
		std::map<const CharID, SFontType> mFontType;
		std::map<PairID, const float> KerningInformation;
		size_t CharCount;
		size_t KerningCount;

		Font();
		~Font();
	};
};