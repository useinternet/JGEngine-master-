#pragma once
#include"../../../EngineStatics/Engine.h"
#include"../../../RenderSystem/JGMaterial/JG2DMesh.h"


class JGTexture;
class JGDeviceD;
class JGBufferManager;
/*
Class : JGFontVertexInformation 
Exp : ��Ʈ ������ ���� ���� �Դϴ�..( ���� ���α��̳� �ؽ��� uv ����, ���� ���� ��� ������ 
����ֽ��ϴ�. 
@m float Width     : ���� ���� ����
@m float Height    : ���� ���� ����
@m float TexWidth  : �ؽ��� UV ��ǥ ���� ����
@m float TexHeight : �ؽ��� UV ��ǥ ���� ����
@m float TexU      : �ؽ��� U ��ǥ
@m float TexV      : �ؽ��� V ��ǥ
@m float XAdvance  : X ��ǥ �� ���� ����
@m bool Space      : ���� ���ڰ� �����̽�(����) �ΰ� ����.    */
class ENGINE_EXPORT JGFontVertexInformation
{
public:
	float Width     = 0.0f;
	float Height    = 0.0f;
	float TexWidth  = 0.0f;
	float TexHeight = 0.0f;
	float TexU      = 0.0f;
	float TexV      = 0.0f;
	float XAdvance  = 0.0f;
	bool  Space     = false;
};
/*
Class : JGFontLoader 
Exp : ��ü ��Ʈ �����͸� �����ϴ� �δ� Ŭ���� �Դϴ�. 
@m map<const string, shared_ptr<Font>> m_mFonts    : ��Ʈ���� �������� �����ϴ� �迭
@m map<const string, const wstring>    m_mFontPath : ��Ʈ���� ��θ� �����ϴ� �迭       */
class ENGINE_EXPORT JGFontLoader
{
private:
	class Font;
private:
	typedef unsigned int CharID;
	typedef std::pair<const CharID, const CharID> PairID;
public:
	/// JGFontLoader���� ���� ����ü ��..
	struct SFontMesh;
	struct SFontInformation;
	struct SFontCommonType;
	struct SFontType;
	struct SKerningType;
	/*
	Struct : SFontInformation
	Exp : ��Ʈ ������ ���� ����ü �Դϴ�. 
	@s	FontName : ���� Ÿ�� ��Ʈ�� �̸��Դϴ�.
	@s	FontSize : Ʈ�� Ÿ�� ��Ʈ�� ũ��.
	@s	bold     : �۲��� ���� ǥ�õ˴ϴ�.
	@s	italic   : �� ����� ���Դϴ�.
	@s	charset  : ��� �� OEM ���� ��Ʈ�� �̸� (���� �ڵ尡 �ƴ� ���).
	@s	unicode  : ���� �ڵ� ���� ��Ʈ�̸� 1�� �����Ͻʽÿ�.
	@s	stretchH : �۲� ���̰� ������� �þ�ϴ�. 100 %�� ���༺�� ������ �ǹ��մϴ�.
	@s	smoothing: smoothing�� ���� ������ 1�� �����Ͻʽÿ�.
	@s	aa       : ��� �� ���� ���ø� ����. 1�� ���� ���ø��� ������ �ʾ� ���� �ǹ��մϴ�.
	@s	padding[4] : �� ������ �е� (����, ������, �Ʒ���, ����).
	@s	spacing[2] : �� ������ ���� (����, ����).
	@s	outline    : ������ ������ �β��Դϴ�.     */
	typedef struct SFontInformation
	{
		std::string  FontName;
		size_t FontSize;
		size_t bold;
		size_t italic;
		std::string  charset;
		bool   unicode;
		int    stretchH;
		bool   smoothing;
		bool   aa;
		int    padding[4];
		int    spacing[2];
		size_t outline;
	}SFontInformation;
	/*
	Struct : SFontCommonType
	Exp : ��Ʈ ���� Ÿ��( bmFont���� ���� �̸�..) ���� ��Ʈ �ؽ��Ŀ� ���õ� ������ ��� �ֽ��ϴ�.
	@s  lineHeight : �� �ؽ�Ʈ �� ������ �Ÿ� (�ȼ� ����)�Դϴ�.
	@s	base       : ���� ���� ��ܿ��� ������ �ر����� �ȼ� ���Դϴ�.
	@s	scaleW     : �ؽ�ó�� ��. �Ϲ������� ���� �̹����� x pos ũ�⸦ �����ϴ� �� ���˴ϴ�.
	@s	scaleH     : �ؽ�ó�� ����. �Ϲ������� ���� �̹����� y pos ũ�⸦ �����ϴ� �� ���˴ϴ�.
	@s	pages      : �۲ÿ� ���� �� �ؽ��� �������� ��.
	@s	packed     : �ܻ� ���ڰ� �� �ؽ�ó ä�ο� ����Ǿ� ������ 1�� �����մϴ�. �� ��� alphaChnl�� �� ä�ο� ����� ������ �����մϴ�.
	@s	alphaChnl  : ä�ο� �׸� �� �����Ͱ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� ������ ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
	@s	redChnl    : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �� ������ �ܰ����� ���� �����ϰ��ִ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
	@s	greenChnl  : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� �ܰ����� ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
	@s	blueChnl   : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� �ܰ����� ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
	@s	pageID     : ������ ID.
	@s	FileName   : �ؽ�ó ���� �̸�.*/
	typedef struct SFontCommonType
	{
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
	/*
	Struct : SFontType 
	Exp : ���� ���� ������ �޾ƿË� ���� ��Ʈ Ÿ�� �����Դϴ�. ( ���� �����͸� �޾� ��Ʈ �ؽ��Ŀ� ���ڸ� �޾ƿ�
	�����Ͱ� ����ֽ��ϴ�. 
	@s	ID      : ���� ID�Դϴ�.
	@s	x       : �ؽ����� ���� �̹����� ���� ��ġ.
	@s	y       : �ؽ�ó�� ���� �̹����� ���� ��ġ�Դϴ�.
	@s	Width   : �ؽ�ó�� ���� �̹����� �ʺ��Դϴ�.
	@s	Height  : �ؽ�ó�� ���� �̹����� �����Դϴ�.
	@s	XOffset : �ؽ�ó���� ȭ������ �̹����� ���� �� �� ���� ��ġ�� �󸶳� �����µǾ�� �ϴ����� �����մϴ�.
	@s	YOffset : �ؽ�ó���� ȭ������ �̹����� ���� �� �� ���� ��ġ�� �󸶳� �����µǾ�� �ϴ����� �����մϴ�.
	@s	XAdvance : ĳ���� �׸��� �� ���� ��ġ�� �󸶸�ŭ �մ�� ���մϴ�.
	@s	Page    : ���� �̹������ִ� �ؽ��� ������.
	@s	chnl    : ���� �̹������ִ� �ؽ�ó ä�� (1 = �Ķ���, 2 = ���, 4 = ������, 8 = ����, 15 = ��� ä��).  */
	typedef struct SFontType
	{
		CharID ID;
		size_t x;
		size_t y;
		size_t Width;
		size_t Height;
		float  XOffset;
		float  YOffset;
		float  XAdvance;
		float  Page;
		float  chnl;
	}SFontType;
	/*
	Struct : SKerningType
	Exp : ���� �� ���� ������ ���� ������ ����ֽ��ϴ�. 
	@s	first  : ù ��° ���� ID�Դϴ�.
	@s	second : �� ��° ���� ID�Դϴ�.
	@s	amount : ù ��° ���� �ٷ� ������ �� ��° ���ڸ� �׸� �� x ��ġ�� �����ؾ��ϴ� �����Դϴ�.*/
	typedef struct SKerningType
	{
		CharID first;
		CharID second;
		float amount;
	}SKerningType;
private:
	std::map<const std::string, std::shared_ptr<Font>> m_mFonts;
	std::map< const std::string, const std::wstring>   m_mFontPath;
public:
	JGFontLoader();
	~JGFontLoader();

	/*
	Exp : FontLoader�� �ν��Ͻ��� �����ɴϴ�. (���� ����) */
	static JGFontLoader* GetInstance();
	/*
	Exp : ��Ʈ�� �ҷ��ɴϴ�. 
	@param JGDeviceD* Device              : JGDevice ������
	@param const string& FontPath         : ��Ʈ ���
	@param const wstring& FontTexturePath : ��Ʈ �ؽ��� ���*/
	bool LoadFont(JGDeviceD* Device, const std::string& FontPath, const std::wstring& FontTexturePath);
	/*
	Exp : �ش� ���忡 �ش��ϴ� ���� ������ ����մϴ�. 
	@param const string& FontPath : �� ��Ʈ ���
	@param const wstring& Text    : ���� ������ �ٲ� ����
	@param const float TextSize   : ��Ʈ ũ��
	@param vector<JGFontVertexInformation>* Array : ������ ���� ���� �迭 */
	void OutputVertexInformation(const std::string& FontPath,const std::wstring& Text,const float TextSize,std::vector<JGFontVertexInformation>* Array);
private:
	/// BmFont ���α׷����� �� ��Ʈ �ҷ����� �Լ�..
	void LoadFontInformation(std::ifstream& fin,Font* font);
	void LoadFontCommonInformation(std::ifstream& fin, Font* font);
	void LoadFontData(std::ifstream& fin, Font* font);
	void LoadKerningInformation(std::ifstream& fin, Font* font);
	void EqualLoop(std::ifstream& fin);
private:
	/// ��Ʈ �ҷ��ö� �� �ӽ� Ŭ����..
	class Font
	{
	public:
		std::unique_ptr<JGTexture>        Texture;
		std::unique_ptr<SFontInformation> FontInformation;
		std::unique_ptr<SFontCommonType>  FontCommonInformation;
		std::map<const CharID, SFontType> mFontType;
		std::map<PairID, const float>     KerningInformation;
		size_t CharCount;
		size_t KerningCount;

		Font();
		~Font();
	};
};