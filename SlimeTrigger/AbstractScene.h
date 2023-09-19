#pragma once
#include <vector>
#include <string>

enum class GUIDE_SHAPE_TYPE
{
	TEXT,			//テキスト
	DYNAMIC_CIRCLE,	//動的な円
	DYNAMIC_BOX,	//動的な四角形
	FIXED_CIRCLE,	//固定サイズの円
	FIXED_BOX,		//固定サイズの四角形
	ROUNDED_BOX,		//丸みを帯びた四角形
	JOYSTICK,		//ジョイスティック形
};

// ガイド要素構造体
struct guideElement
{
	std::vector<std::string> buttons; // ボタン
	std::string description; // 説明
	GUIDE_SHAPE_TYPE shapeType;
	// 0: TEXT, 1: DYNAMIC_CIRCLE, 2: DYNAMIC_BOX, 3: FIXED_CIRCLE, 4: FIXED_BOX, 5: ROUNDED_BOX, 6: JOYSTICK
	int font; // フォントハンドル
	unsigned int buttonColor; // ボタンの色
	unsigned int buttonStringColor; // ボタンの文字色
	unsigned int color; // 説明の色
	unsigned int edgeColor; // 説明の輪郭線の色
	float innerPadding; // 複数のボタンがある場合のボタン間隔
	float width; // 固定サイズのBox、Circle用横幅
	float height; // 固定サイズのBox、Circle用縦幅
	float offsetStringY; // 文字列のY座標の微調整
	float offsetButtonStringY; // ボタン文字のY座標の微調整
	float radiusOffset; // 固定サイズのCircle用半径の微調整
	float padding;		// ボタンと説明の間のパディング
	float lineWidth; // 輪郭線の太さ
	unsigned int lineColor; // 輪郭線の色
	
	guideElement(const std::vector<std::string>& buttons = {""}, const std::string& description = "",
	             GUIDE_SHAPE_TYPE shapeType = GUIDE_SHAPE_TYPE::TEXT, int font = NULL,
	             unsigned int buttonColor = 0xffffff,
	             unsigned int buttonStringColor = 0x000000,
	             unsigned int color = 0xffffff, unsigned int edgeColor = 0x000000,
	             float innerPadding = 10.f, float width = 30.f, float height = 30.f, float offsetStringY = 0.f,  float offsetButtonStringY = 0.f, float radiusOffset = -2.f,
	             float padding = 2.f, float lineWidth = 4.f,
	             unsigned int lineColor = 0xEFEAF6)
		: buttons(buttons), description(description), shapeType(shapeType), font(font), buttonColor(buttonColor),
		  buttonStringColor(buttonStringColor), color(color), edgeColor(edgeColor),
		  innerPadding(innerPadding), width(width), height(height), offsetStringY(offsetStringY), offsetButtonStringY(offsetButtonStringY),
		  radiusOffset(radiusOffset), padding(padding), lineWidth(lineWidth), lineColor(lineColor)
	{
	}
};

class AbstractScene
{
public:
	//コンストラクタ
	AbstractScene();
	// CommonProcessを実行しないようにする引数有りの空コンストラクタ
	AbstractScene(bool notInitialized) {};
	//デストラクタ
	virtual ~AbstractScene() {};
	//描画以外の更新を実行
	virtual AbstractScene* Update() = 0;
	//描画に関することを実装
	virtual void Draw() const = 0;

	/// <summary>
	/// 全シーン共通のコンストラクタで行う処理
	/// </summary>
	void CommonProcess();

	/// <summary>
	/// 文字の描画するX座標が中心になるX座標を取得する。
	/// </summary>
	/// <param name="string">描画する文字列</param>
	/// <param name="fontHandle">描画に使用するフォントハンドル(未入力又は0の場合はデフォルトフォントハンドルを使用</param>
	/// <param name="margin">中央のX座標に間隔を空ける値</param>
	int GetDrawCenterX(const char* string, int fontHandle = -1, int margin = 0)const;

	/// <summary>
	/// ガイド表示する
	/// </summary>
	/// <param name="guides">ガイド要素</param>
	/// <param name="x">描画の基準となるX座標</param>
	/// <param name="y">描画の基準となるY座標</param>
	/// <param name="padding">ボタンと説明の間の間隔</param>
	/// <param name="elementSpacing">各要素の間隔</param>
	void DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const;
	void DrawGuideElement(const guideElement& guide, float& currentX, float y) const;
};