#pragma once
#include <Windows.h>
#include <vector>

#define MAP_CEllSIZE 80

class PLAYER;
class ELEMENT;

class STAGE
{
private:
	
	int stageImage[5];			//ステージ背景画像
	int halfWayPointSe;		//中間地点SE
	float mapX, mapY;			//マップ描画座標
	POINT clearBox;		//クリアになるボックス状範囲の座標
	int aniTimer;
	POINT halWayPointBox;		//中間地点座標
	POINT spawnPoint;			//スポーン地点座標

	bool clearFlag;			//クリア判定フラグ
	int clearCount;		//ステージクリア時のカウントタイマー
	bool halfWayPoint;		//中間地点フラグ
	int halfWayTimer;		//中間地点用タイマー
	float playerXOld, playerYOld;		//旧プレイヤー座標
	float playerLongOld;	//数フレーム前のプレイヤー座標
	int countTimer;		//カウント用タイマー
	float playerVectorX;					//プレイヤーの移動方向x
	float playerVectorY;					//プレイヤーの移動方向y
	float scrollSpeedX;					//スクロール移動量(x)
	float scrollSpeedY;					//スクロール移動量(y)

protected:
	int blockImage1[110];		//ステージブロック画像
	std::vector<std::vector<int>> mapData;
	std::vector<std::vector<int>> doorInfo;
	std::vector<std::vector<int>> buttonInfo;
	float scrollX, scrollY;	//マップスクロール量
	int temporaryHit;		//一時的な当たり判定
	const char* stageName;		//ステージ名
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	STAGE(const char* stageName = "StageSelect", bool restert = false);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~STAGE();
	/// <summary>
	/// ステージの更新
	/// </summary>
	void Update(PLAYER* player, ELEMENT* element);
	/// <summary>
	/// ステージの描画
	/// </summary>
	virtual void Draw(ELEMENT* element) const;
	/// <summary>
	/// マップ画像番号Geter
	/// </summary>
	/// <returns>int型：画像整理番号</returns>	
	int GetMapImage(int num) { return blockImage1[num]; }
	/// <summary>
	/// マップデータの読み込み
	/// </summary>
	/// <param name="stageName">ステージのインスタンス</param>
	void LoadMapData(const char* stageName);
	/// <summary>
	/// マップサイズのGeter
	/// </summary>
	/// <returns>構造体型：マップサイズ</returns>	
	POINT GetMapSize(void) { POINT mapsize{ mapData.size(),mapData.at(0).size(), };  return mapsize; }
	/// <summary>
	/// マップ配列のGetter
	/// </summary>
	/// <param name="y">マップデータの横要素数</param>
	/// <param name="x">マップデータの縦要素数</param>
	/// <returns>int型：マップ配列番号</returns>	
	int GetMapData(int y, int x);
	/// <summary>
	/// マップ配列の値のSetter
	/// </summary>
	/// <param name="y">マップデータの横要素数</param>
	/// <param name="x">マップデータの縦要素数</param>
	/// <param name="mapchip_num">マップ配列番号</param>
	void SetMapData(int y, int x, int mapchip_num) { mapData[y][x] = mapchip_num; }


	/// <summary>
	/// カメラワーク
	/// </summary>
	/// 	/// <param name="player">プレイヤーのインスタンス</param>
	/// <param name="element">エレメントのインスタンス</param>
	void CameraWork(PLAYER* player, ELEMENT* element);
	/// <summary>
	/// スクロールXのSetter
	/// </summary>
	void SetScrollX(float scrollX) { if (scrollX < 0) { this->scrollX = scrollX; } }
	/// <summary>
	/// スクロールYのSetter
	/// </summary>
	void SetScrollY(float scrollY) { if (scrollY < 0) { this->scrollY = scrollY; } }
	/// <summary>
	/// 画面スクロール座標XのSetter
	/// </summary>
	bool SetScrollPos(int moveX);
	/// <summary>
	/// 画面スクロール座標XのGetter
	/// </summary>
	float GetScrollX() { return scrollX; };
	/// <summary>
	/// 画面スクロール座標YのGetter
	/// </summary>
	float GetScrollY() { return scrollY; }


	/// <summary>
	/// マップの当たり判定
	/// </summary>
	/// <param name="y">マップデータの横要素数</param>
	/// <param name="x">マップデータの縦要素数</param>
	bool HitMapDat(int y, int x);
	/// <summary>
	/// スライムのかけらの当たり判定
	/// </summary>
 	/// <param name="y">マップデータの横要素数</param>
	/// <param name="x">マップデータの縦要素数</param>
	bool HitThrowSlime(int y, int x);

	/// <summary>
	/// スポーン地点座標のGetter
	/// </summary>
	/// <returns>構造体型：スポーン地点座標</returns>	
	POINT GetSpawnPoint(void) { return spawnPoint; }
	/// <summary>
	/// ステージクリア判定処理
	/// </summary>
	void StageClear(PLAYER* player);
	/// <summary>
	/// クリアしたかのフラグのGetter
	/// </summary>
	bool GetClearFlg(void) { return clearFlag; };
	/// <summary>
	/// 中間地点判定処理
	/// </summary>
	void HalfwayPoint(PLAYER* player);
	/// <summary>
	/// 中間地点座標のGetter
	/// </summary>
	/// <returns>構造体型：中間地点座標</returns>	
	POINT GetHalfwayPoint(void) {return POINT(halWayPointBox); }
	/// <summary>
	/// 中間地点通過したか判定フラグのGetter
	/// </summary>
	bool GetHalfwayPointFlg(void) { return halfWayPoint; }
	/// <summary>
	/// 一時的な当たり判定のSetter
	/// </summary>
	/// <param name="mapdata">マップ配列番号</param>
	void SetTemporary_Hit(int mapdata) { temporaryHit = mapdata; }
};

