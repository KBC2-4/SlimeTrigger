#include "AbstractScene.h"
#include "DxLib.h"

AbstractScene::AbstractScene() {
	static short initialized = 0;

	//プログラム開始時に2回呼ばれるのを無効化
	if (initialized > 1 ) {
		CommonProcess();
	}
	else {
		initialized++;
	}
}


void AbstractScene::CommonProcess() {
	
	//リソースをリセット
	InitFontToHandle();
	InitGraph();
	InitSoundMem();
}

int AbstractScene::GetDrawCenterX(const char* string, int fontHandle, int margin)const {

	//画面幅
	const int screenX = 1280;

	if (margin >= screenX || margin <= -screenX) {
		margin = 0;
	}

	if (fontHandle == -1) {
		fontHandle = DX_DEFAULT_FONT_HANDLE;
	}


	const int w = screenX / 2 + margin - (GetDrawFormatStringWidthToHandle(fontHandle, string) / 2);
	return w;
}

void AbstractScene::DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const
{
	
	float currentX = x;
	float centerY = y + 15;
	
	for (const auto& guide : guides)
	{
		int font = guide.font;
		if (font == NULL) {
			font = DX_DEFAULT_FONT_HANDLE;
		}
		
		DrawGuideElement(guide, currentX, y);

		int textWidth, textHeight;
		GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, guide.description.c_str(), guide.description.length(), font);
		float stringY = centerY - (textHeight / 2) + guide.offsetStringY;
		DrawStringToHandle(currentX, stringY, guide.description.c_str(), guide.color, font, guide.edgeColor);
		currentX += textWidth + elementSpacing;
	}
}

void AbstractScene::DrawGuideElement(const guideElement& guide, float& currentX, float y) const
{
	
	int font = guide.font;
	if (font == NULL) {
		font = DX_DEFAULT_FONT_HANDLE;
	}

		for (size_t i = 0; i < guide.buttons.size(); ++i)
		{
			const auto& button = guide.buttons[i];
			int textWidth, textHeight;
			GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);
			
			float padding = 2.f;

			if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_CIRCLE) {
				// サークルの半径を文字列の幅に基づいて計算
				float radius = (std::max)(textHeight, textWidth) / 2 + padding + guide.radiusOffset;
				
				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.lineColor, FALSE, guide.lineWidth);
				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
				float stringY = y + radius - (textHeight / 2) + guide.offsetButtonStringY;
				// 文字列をサークルの中央に配置
				DrawStringToHandle(currentX + radius - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
				currentX += radius * 2;
			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_BOX) {
				float boxWidth = textWidth + 2 * padding;  // padding を両側に追加
				float boxHeight = textHeight + 2 * padding;  // padding を上下に追加

				// 1対1の比率の正方形に固定
				// // boxWidth と boxHeight のうち、大きい方を選ぶ
				// float maxDimension = max(boxWidth, boxHeight);
				//
				// // 正方形にするために、boxWidth と boxHeight を maxDimension に設定
				// boxWidth = maxDimension;
				// boxHeight = maxDimension;

				// 縦長にならないように制限する
				// 縦長になる場合は、boxWidth を boxHeight と同じにする
				if (boxHeight > boxWidth) {
					boxWidth = boxHeight;
				}
				
				DrawBoxAA(currentX, y, currentX + boxWidth, y + boxHeight, guide.lineColor, FALSE, guide.lineWidth);
				DrawBoxAA(currentX, y, currentX + boxWidth, y + boxHeight, guide.buttonColor, TRUE);
				float stringY = y + (boxHeight / 2 - textHeight / 2) + guide.offsetButtonStringY;
				DrawStringToHandle(currentX + boxWidth / 2 - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
				currentX += boxWidth;
			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::FIXED_CIRCLE) {
				float radius = guide.width / 2 + guide.radiusOffset;
				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.lineColor, FALSE, guide.lineWidth);
				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
				float stringY = y + radius - (textHeight / 2) + guide.offsetButtonStringY;
				DrawStringToHandle(currentX + radius - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
				
				currentX += guide.width;
			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::FIXED_BOX) {
				DrawBoxAA(currentX, y, currentX + guide.width, y + guide.height, guide.lineColor, FALSE, guide.lineWidth);
				DrawBoxAA(currentX, y, currentX + guide.width, y + guide.height, guide.buttonColor, TRUE);
				float stringY = y + guide.height / 2 - (textHeight / 2) + guide.offsetButtonStringY;
				DrawStringToHandle(currentX + guide.width / 2 - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
				currentX += guide.width + guide.lineWidth;
			}
			else if (guide.shapeType == GUIDE_SHAPE_TYPE::ROUNDED_BOX) {
				
				// テキストのサイズを取得
				int textWidth, textHeight;
				GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);

				// 図形のサイズを動的に計算
				float dynamicHeight = textHeight + 5;
				float dynamicWidth = textWidth + 5;
				float radius = dynamicHeight / 2;  // 高さに基づいて半径を設定

				float rectX = currentX;
				float rectY = y;

				// 矩形の左上と右下の座標を計算
				float rectLeft = rectX + radius;
				float rectTop = rectY;
				float rectRight = rectX + dynamicWidth - radius;
				float rectBottom = rectY + dynamicHeight + 1.0f;

				// 外枠の色と太さを設定
				unsigned int borderColor = 0xEFEAF6;
				float borderWidth = 4.0f; // 太さ

				// 外枠の矩形を描画
				DrawBoxAA(rectLeft, rectTop, rectRight, rectBottom, borderColor, FALSE, borderWidth);

				// 左側の円を描画
				float circleLeftCenterX = rectLeft;
				float circleLeftCenterY = rectY + radius;
				// 左側の円の外枠を描画
				DrawCircleAA(circleLeftCenterX, circleLeftCenterY, radius, 20, borderColor, FALSE, borderWidth);
				
				DrawCircleAA(circleLeftCenterX, circleLeftCenterY, radius, 20, guide.color, TRUE);

				// 右側の円を描画
				float circleRightCenterX = rectRight;
				float circleRightCenterY = rectY + radius;
				// 右側の円の外枠を描画
				DrawCircleAA(circleRightCenterX, circleRightCenterY, radius, 20, borderColor, FALSE, borderWidth);
				DrawCircleAA(circleRightCenterX, circleRightCenterY, radius, 20, guide.color, TRUE);
				
				// 矩形を描画
				DrawBoxAA(rectLeft, rectTop, rectRight, rectBottom, guide.color, TRUE);

				float stringY = rectY + dynamicHeight / 2 - (textHeight / 2) + guide.offsetButtonStringY;
				// テキストを描画
				DrawStringToHandle(rectX + dynamicWidth / 2 - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);

				currentX += dynamicWidth;
			}
			else if (guide.shapeType == GUIDE_SHAPE_TYPE::JOYSTICK) {
				float joystickX = currentX;
				float joystickY = y;
				DrawOvalAA(joystickX, joystickY + 6.0f, 18, 10, 20, guide.buttonColor, TRUE);
				DrawBoxAA(joystickX - 5.0f, joystickY, joystickX + 7, joystickY + 23, guide.buttonColor, TRUE);
				DrawOvalAA(joystickX, joystickY + 23.0f, 22, 8, 20, guide.buttonColor, TRUE);
				DrawString(joystickX - 2.0f, joystickY - 2.0f, button.c_str(), guide.buttonStringColor);
				currentX += 25.0f;  // ジョイスティックの幅

			} 
			else { // Text
				DrawStringToHandle(currentX, y, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
				currentX += textWidth;
			}

			//複数のボタンがある場合最後のボタン以外の後に内部パディングを追加
			if (i < guide.buttons.size() - 1)
			{
				currentX += guide.innerPadding;
			}
			else
			{
				// ボタンと説明の間のパディングを追加
				currentX += padding;
			}
		}
}


// void AbstractScene::DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const
// {
// 	float currentX = x;
// 	float centerY = y + 15;
// 	
// 	for (const auto& guide : guides)
// 	{
// 		int font = guide.font;
// 		if (font == NULL) {
// 			font = DX_DEFAULT_FONT_HANDLE;
// 		}
//
// 		for (size_t i = 0; i < guide.buttons.size(); ++i)
// 		{
// 			const auto& button = guide.buttons[i];
// 			int textWidth, textHeight;
// 			GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);
// 			
// 			float padding = 2.f;
// 			float radius = textHeight / 2 + padding;
//
// 			if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_CIRCLE) {
// 				// サークルの半径を文字列の幅に基づいて計算
// 				float radius = (std::max)(textHeight, textWidth) / 2 + padding;
//
// 				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
// 				// 文字列をサークルの中央に配置
// 				DrawStringToHandle(currentX + radius - textWidth / 2, y + radius - textHeight / 2, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
// 				currentX += radius * 2;
// 			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_BOX) {
// 				float boxWidth = textWidth + 2 * padding;  // padding を両側に追加
// 				float boxHeight = textHeight + 2 * padding;  // padding を上下に追加
// 				DrawBoxAA(currentX, y, currentX + boxWidth, y + boxHeight, guide.buttonColor, TRUE);
// 				DrawStringToHandle(currentX + boxWidth / 2 - textWidth / 2, y + boxHeight / 2 - textHeight / 2, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
// 				currentX += boxWidth;
// 			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::FIXED_CIRCLE) {
// 				float radius = guide.width / 2 + guide.radiusOffset;
// 				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
// 				DrawStringToHandle(currentX + radius - textWidth / 2, y + radius - textHeight / 2, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
// 				currentX += guide.width;
// 			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::FIXED_BOX || guide.shapeType == GUIDE_SHAPE_TYPE::EDGE_BOX) {
// 				
// 				// エッジの太さ
// 				float lineWidth = 0;				
// 				if(guide.shapeType == GUIDE_SHAPE_TYPE::EDGE_BOX)
// 				{
// 					// 外枠
// 					lineWidth = 4;
// 					DrawBoxAA(currentX - lineWidth, y - lineWidth, currentX + guide.width + lineWidth, y + guide.height + lineWidth, 0xEFEAF6, TRUE);
// 				}
// 				DrawBoxAA(currentX, y, currentX + guide.width, y + guide.height, guide.buttonColor, TRUE);
// 				DrawStringToHandle(currentX + guide.width / 2 - textWidth / 2, y + guide.height / 2 - textHeight / 2, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
// 				currentX += guide.width + lineWidth;
// 			}
// 			else if (guide.shapeType == GUIDE_SHAPE_TYPE::ROUNDED_BOX) {
//
// 				// float rect_x = currentX;
// 				// float rect_y = y;
// 				// float height = guide.height;
// 				// float width = guide.width;
// 				// float radius = height / 2;  // 高さに基づいて半径を設定
// 				//
// 				// // 矩形の左上と右下の座標を計算
// 				// float rect_left = rect_x + radius;
// 				// float rect_top = rect_y;
// 				// float rect_right = rect_x + width - radius;
// 				// float rect_bottom = rect_y + height;
// 				//
// 				// // 矩形を描画（左右の円で覆われる部分を除く）
// 				// DrawBoxAA(rect_left, rect_top, rect_right, rect_bottom + 1.f, guide.color, TRUE);
// 				//
// 				// // 左側の円を描画
// 				// float circleLeftCenter_x = rect_left;
// 				// float circleLeftCenter_y = rect_y + radius;
// 				// DrawCircleAA(circleLeftCenter_x, circleLeftCenter_y, radius, 20, guide.color, TRUE);
// 				//
// 				// // 右側の円を描画
// 				// float circleRightCenter_x = rect_right;
// 				// float circleRightCenter_y = rect_y + radius;
// 				// DrawCircleAA(circleRightCenter_x, circleRightCenter_y, radius, 20, guide.color, TRUE);
// 				//
// 				// // ---
// 				// // ボックスの中心座標を計算
// 				// float boxCenterX = currentX + guide.width / 2.0f;
// 				// float boxCenterY = y + guide.height / 2.0f;
// 				//
// 				// // テキストのサイズを取得
// 				// int textWidth, textHeight;
// 				// GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);
// 				//
// 				// // テキストを描画する座標を計算
// 				// float textStartX = boxCenterX - textWidth / 2.0f;
// 				// float textStartY = boxCenterY - textHeight / 2.0f;
// 				//
// 				// // テキストを描画
// 				// DrawStringToHandle(textStartX, textStartY, button.c_str(), guide.color, font);
// 				//
// 				// currentX += width;
//
// 				// テキストのサイズを取得
// 				int textWidth, textHeight;
// 				GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);
//
// 				// 図形のサイズを動的に計算
// 				float dynamicHeight = textHeight + 10;  // 10は任意のマージン
// 				float dynamicWidth = textWidth + 20;  // 20は任意のマージン
// 				float radius = dynamicHeight / 2;  // 高さに基づいて半径を設定
//
// 				float rect_x = currentX;
// 				float rect_y = y;
//
// 				// 矩形の左上と右下の座標を計算
// 				float rect_left = rect_x + radius;
// 				float rect_top = rect_y;
// 				float rect_right = rect_x + dynamicWidth - radius;
// 				float rect_bottom = rect_y + dynamicHeight;
//
// 				// 矩形を描画（左右の円で覆われる部分を除く）
// 				DrawBoxAA(rect_left, rect_top, rect_right, rect_bottom, guide.color, TRUE);
//
// 				// 左側の円を描画
// 				float circleLeftCenter_x = rect_left;
// 				float circleLeftCenter_y = rect_y + radius;
// 				DrawCircleAA(circleLeftCenter_x, circleLeftCenter_y, radius, 20, guide.color, TRUE);
//
// 				// 右側の円を描画
// 				float circleRightCenter_x = rect_right;
// 				float circleRightCenter_y = rect_y + radius;
// 				DrawCircleAA(circleRightCenter_x, circleRightCenter_y, radius, 20, guide.color, TRUE);
//
// 				// テキストを描画
// 				DrawStringToHandle(rect_x + dynamicWidth / 2 - textWidth / 2, rect_y + dynamicHeight / 2 - textHeight / 2, button.c_str(), 0xFFFFFF, font);
//
// 				// 次の要素に進むための X 座標を更新
// 				currentX += dynamicWidth;
// 			}
// 			else if (guide.shapeType == GUIDE_SHAPE_TYPE::RIGHT_ROUNDED_RECTANGLE)
// 			{
// 				float rectX = currentX;
// 				float rectY = y;
// 				float radius = 15;
// 				DrawBoxAA(rectX, rectY, rectX + guide.width, rectY + guide.height, guide.color, TRUE);
// 				DrawCircleAA(rectX + guide.width, rectY + guide.height / 2, radius, 20, guide.color, TRUE);
// 				currentX += guide.width;
// 			}
// 			else if (guide.shapeType == GUIDE_SHAPE_TYPE::LEFT_ROUNDED_RECTANGLE)
// 			{
// 				float rectX = currentX;
// 				float rectY = y;
// 				float radius = 15;
// 				DrawBoxAA(rectX, rectY, rectX + guide.width, rectY + guide.height, guide.color, TRUE);
// 				DrawCircleAA(rectX, rectY + guide.height / 2, radius, 20, guide.color, TRUE);
// 				currentX += guide.width;
// 			}
// 			else if (guide.shapeType == GUIDE_SHAPE_TYPE::EDGE_CIRCLE)
// 			{
// 				float radius = guide.width / 2;
// 				// コントラストが付いたゲームパッドのボタンを描画
// 				DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
// 				//  コントラストが付いたゲームパッドのボタンを描画// 白より少し暗くする
// 				DrawCircleAA(currentX + radius, y + radius, radius - 2, 32, 0x000000, TRUE);
// 				currentX += guide.width;
// 			}
// 			else if (guide.shapeType == GUIDE_SHAPE_TYPE::JOYSTICK) {
// 				float joystickX = currentX;
// 				float joystickY = y;
// 				DrawOvalAA(joystickX, joystickY + 6.0f, 18, 10, 20, guide.buttonColor, TRUE);
// 				DrawBoxAA(joystickX - 5.0f, joystickY, joystickX + 7, joystickY + 23, guide.buttonColor, TRUE);
// 				DrawOvalAA(joystickX, joystickY + 23.0f, 22, 8, 20, guide.buttonColor, TRUE);
// 				DrawString(joystickX - 2.0f, joystickY - 2.0f, button.c_str(), guide.buttonStringColor);
// 				currentX += 25.0f;  // ジョイスティックの幅
//
// 			} else if (guide.shapeType == GUIDE_SHAPE_TYPE::IMAGE) {
// 				int imageWidth, imageHeight;
// 				GetGraphSize(guide.imageHandle, &imageWidth, &imageHeight);
//
// 				int textWidth, textHeight;
// 				GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), guide.description.length(), font);
//
// 				// 画像の中央にテキストを描画する座標を計算
// 				int textX = currentX + (imageWidth - textWidth) / 2;
// 				int textY = y;
// 				
// 				DrawGraph(currentX, y - (imageHeight / 2), guide.imageHandle, TRUE);
// 				DrawStringToHandle(textX, textY, button.c_str(), guide.color, font);
// 				
// 				currentX += imageWidth;
//
// 			}
// 			else { // Text
// 				DrawStringToHandle(currentX, y, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
// 				currentX += textWidth;
// 			}
//
// 			// 最後のボタン以外の後に内部パディングを追加
// 			if (i < guide.buttons.size() - 1)
// 			{
// 				currentX += guide.innerPadding;
// 			}
// 			else
// 			{
// 				// ボタンと説明の間のパディングを追加
// 				currentX += padding;
// 			}
// 		}
//
// 		int textWidth, textHeight;
// 		GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, guide.description.c_str(), guide.description.length(), font);
// 		DrawStringToHandle(currentX, centerY - textHeight / 2, guide.description.c_str(), guide.color, font);
// 		currentX += textWidth + elementSpacing;
// 	}
// }