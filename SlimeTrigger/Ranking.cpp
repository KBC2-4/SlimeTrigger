#include "Ranking.h"
#include "DxLib.h"

#define FILEPATH "Resource/BestTime/BestTime.bin"

int RANKING::bestTime[3];
void RANKING::Insert(int time, int stage)
{
	ReadRanking();
	if ((time < bestTime[stage - 1]) || (bestTime[stage - 1] == NULL))
	{
		bestTime[stage - 1] = time;
		SaveRanking();
	}
}

void RANKING::SaveRanking(void) {
	FILE* fp = NULL;

	//ファイルオープン
	if (fopen_s(&fp, FILEPATH, "wb") != 0)
	{
		fopen_s(&fp, FILEPATH, "wb");

		if (fp == NULL)
		{
			throw FILEPATH;
		}

	}

	//ベストタイムを書き込む
	for (int i = 0; i < 3; i++)
	{
		fwrite(&bestTime[i], sizeof(bestTime) / sizeof(bestTime[0]), 3, fp);
	}
	fclose(fp);
}


void RANKING::ReadRanking(void) {
	FILE* fp = NULL;

	//ファイルオープン
	if (fopen_s(&fp, FILEPATH, "rb") != 0)
	{
		fopen_s(&fp, FILEPATH, "wb");

		if (fp == NULL)
		{
			throw FILEPATH;
		}
	}
	//ベストタイムを読み込む
	for (int i = 0; i < 3; i++)
	{
		fread(&bestTime[i], sizeof(bestTime) / sizeof(bestTime[0]), 3, fp);
	}
	fclose(fp);
}