#include <vector>
#include <iostream>
#include <string>
//#include "TestClient.h"
//#include "GameNetworkNode.h"
using namespace std;
//扑克数据 6全部取出

const int	m_bCardListData[8 * 54] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F,
};
#define FTK_ERROR					0									//错误类型
#define FTK_SINGLE					1									//单牌类型
#define FTK_DOUBLE					2									//对牌类型
#define FTK_THREE					3									//三张类型


#define FTK_DOUBLE_DUI				4									//(姐妹对)
#define FTK_THREE_DUI       		5									//(姐妹对)
#define FTK_510K_FU					6									//510k副
#define FTK_510K_ZHENG				7									//510k正
#define FTK_BOOM       				8									//炸弹
#define FTK_LIAN_BOOM       		9									//连炸
#define MAX_INDEX 54

#define	FTK_BIG_KING				0x4F
#define	FTK_LITTLE_KING				0x4E
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

#define	FTK_COLOR_FANGKUAI				0
#define	FTK_COLOR_MEIHUA				1
#define	FTK_COLOR_HONGTAO				2
#define	FTK_COLOR_HEITAO				3
#define FTK_COLOR_KING					4
typedef char BYTE;

template <class T>
bool isSameElm(T & x)
{
	if (x.empty()) return false;
	auto be = x.front();
	for (const auto &v : x)
	{
		if (v != be)
			return false;
	}
	return true;
}

int GetCardValue(int bCardData)
{
	return bCardData&LOGIC_MASK_VALUE;
}
//获取花色0,1,2,3 方块,梅花,红桃,黑桃
int GetCardColor(int bCardData)
{
	return bCardData&LOGIC_MASK_COLOR;
}

//扑克字面转换索引
int SwitchToCardIndex(int cbCardData)
{
	int t = 0;
	int color = GetCardColor(cbCardData);//0-3  王是4
	int value = GetCardValue(cbCardData);//-13  王是1415

	int temp =  color >> 4;

	if (cbCardData == 0x4E)
		t = 52;
	else if (cbCardData == 0x4F)
		t = 53;
	else
		t = (temp * 13 + value) - 1;//从0开始

	cout << t << " " << temp << " " << value << endl;
	return t;
}

//扑克转换
char SwitchToCardIndex(int cbCardData[], int cbCardCount, int cbCardIndex[])
{
	//设置变量
	memset(cbCardIndex, 0, sizeof(int) * 54);
	//转换扑克
	for (int i = 0; i < cbCardCount; i++)
	{
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}
void RandCardList(int cbCardData[], int cbMaxCount)
{
	int cbRandCount = 0;
	int cbCount = 5;// random_int(2, 3);
	do
	{
		for (int i = cbMaxCount - 1; i > 0; i--)
		{
			int index = rand()%i;
			int tmp = cbCardData[i];
			cbCardData[i] = cbCardData[index];
			cbCardData[index] = tmp;
		}
		cbRandCount++;
	} while (cbRandCount < cbCount);
}


//扑克字面转换索引
char SwitchToCardValue(char index)
{
	if (index == 53)
	{
		return 0x4E;
	}
	else if (index == 54)
	{
		return 0x4F;
	}

	return (((index / 13) << 4) | (index % 13)) + 1;
}

void SwitchToCardValue(vector<char>& cbCardData, char cbCardIndex[])
{

	for (char i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] <= 0) continue;
		for (char j = 0; j < cbCardIndex[i]; j++)
		{
			cbCardData.push_back(SwitchToCardValue(i));
		}
	}
}

bool IsHaveThisMathCard(char bCardData[], char bCardCount, char targetCard)
{
	for (char i = 0; i < bCardCount; i++)
	{
		if (GetCardValue(bCardData[i]) == GetCardValue(targetCard))
		{
			return true;
		}
	}
	return false;
}

bool IsSameColorCard(char bCardData[], char bCardCount)
{//510K专用

	vector<char> vecColor;
	for (char i = 1; i < MAX_INDEX; i++)
	{
		if (bCardData[i] <= 0) continue;

		char temp = SwitchToCardValue(i);

		vecColor.push_back(GetCardColor(temp));
	}
	char tempColor = vecColor.at(0);
	for (vector<char>::iterator i = vecColor.begin(); i < vecColor.end(); i++)
	{
		if (tempColor != (*i))
		{
			return false;
		}
	}
	return true;
}

BYTE IsTwoOrThreeSister(BYTE bCardData[], BYTE bCardCount)
{
	for (BYTE i = 1; i < MAX_INDEX; i++)
	{
		if (bCardData[i] <= 0) continue;

		return bCardData[i];//2或者3
	}
	return 0;
}
bool IsSameValueCard(char bCardData[], char bCardCount)
{
	vector<BYTE> vecColor;
	for (char i = 1; i < MAX_INDEX; i++)
	{
		if (bCardData[i] <= 0) continue;

		char temp = SwitchToCardValue(i);

		vecColor.push_back(GetCardValue(temp));
	}
	return isSameElm(vecColor);
}

bool IsKingBoom(BYTE bCardData[], BYTE bCardCount)
{
	if (bCardCount < 4) return false;

	for (BYTE i = 1; i < MAX_INDEX; i++)
	{
		if (bCardData[i] <= 0) continue;
		if (i < 52)
			return false;
	}
	return true;
}
//获取出牌类型 参数: 出的牌列表, 出的张数(王炸,4张炸,三张炸,对子,单张)
char GetCardType(char bCardData[], char bCardCount)
{
	switch (bCardCount)
	{
	case FTK_ERROR: //错误
		return FTK_ERROR;
	case FTK_SINGLE: //单牌
		return FTK_SINGLE;
	case FTK_DOUBLE://对牌
		return (GetCardValue(bCardData[0]) == GetCardValue(bCardData[1])) ? FTK_DOUBLE : FTK_ERROR;
	case FTK_THREE://三张牌
	{
		if (GetCardValue(bCardData[0]) == GetCardValue(bCardData[1]) && GetCardValue(bCardData[0]) == GetCardValue(bCardData[2]))
		{
			return FTK_THREE;
		}
		else
		{
			bool tem1 = IsHaveThisMathCard(bCardData, bCardCount, 0x05);
			bool tem2 = IsHaveThisMathCard(bCardData, bCardCount, 0x0A);
			bool tem3 = IsHaveThisMathCard(bCardData, bCardCount, 0x0D);
			bool isSameColor = IsSameColorCard(bCardData, bCardCount);
			if (tem1&&tem2&&tem3)
			{
				return isSameColor ? FTK_510K_FU : FTK_510K_ZHENG;
			}
			return FTK_ERROR;
		}
	}
	}
	//123三张
	bool isSameValue = IsSameValueCard(bCardData, bCardCount);
	BYTE duiCount = IsTwoOrThreeSister(bCardData, bCardCount);

	if (duiCount > 3 || isSameValue)//同数字,炸弹
	{
		return duiCount > 3 && !isSameValue ? FTK_LIAN_BOOM : FTK_BOOM;
	}
	else//不同数字,姐妹对
	{
		if (duiCount == 2)
		{
			return FTK_DOUBLE_DUI;
		}
		else if (duiCount == 3)
		{
			return FTK_THREE_DUI;
		}
	}
	return FTK_ERROR;
}

void WriteHandCard(string heard, const BYTE cbCardData[])
{
	string temp = "";
	temp += heard;
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardData[i] <= 0)
			continue;
		int temp1 = GetCardColor(SwitchToCardValue(i));
		switch (temp1)
		{
		case 0x00:
		{
			temp += "方块";
			break;
		}
		case 0x10:
		{
			temp += "梅花";
			break;
		}
		case 0x20:
		{
			temp += "红桃";
			break;
		}
		case 0x30:
		{
			temp += "黑桃";
			break;
		}
		}
		if (i == 52)
			temp += "小王:";
		else if (i == 53)
			temp += "大王:";
		else
			temp += to_string(GetCardValue(SwitchToCardValue(i))) + ":";

		temp += to_string(cbCardData[i]) + "张# ";
	}
	temp += "\n";
	cout << temp;
}
BYTE GetCardLogicValue(BYTE bCardData)
{
	if (bCardData == FTK_BIG_KING)//大王
		return 17;
	else if (bCardData == FTK_LITTLE_KING)//小王
		return 16;
	BYTE bCardValue = GetCardValue(bCardData);
	return (bCardValue <= 2) ? (bCardValue + 13) : bCardValue;

}
BYTE GetMaxCard(BYTE bCardData[])
{
	vector<BYTE> vecTemp;
	vecTemp.clear();

	SwitchToCardValue(vecTemp, bCardData);

	BYTE temp1, temp2, temp = 0x00;
	temp1 = GetCardLogicValue(vecTemp.at(0));
	temp = vecTemp.at(0);
	for (BYTE i = 1; i < vecTemp.size(); i++)
	{
		temp2 = GetCardLogicValue(vecTemp.at(i));
		if (temp2 > temp1)
		{
			temp1 = temp2;
			temp = vecTemp.at(i);
		}
	}
	return temp;
}
bool CompareCard(BYTE nowCard[], BYTE oldCard[], BYTE nowCount, BYTE oldCount)
{
	WriteHandCard("现在出的牌为:", nowCard);
	WriteHandCard("上家出的牌为:", oldCard);
	//TEST
	return true;
	vector<BYTE> temp1, temp2;
	temp1.clear();
	SwitchToCardValue(temp1, nowCard);

	temp2.clear();
	SwitchToCardValue(temp2, oldCard);

	//获取类型
	BYTE bFirstMaxCard = GetMaxCard(nowCard);	//最大牌现在出的
	BYTE bNextMaxCard = GetMaxCard(oldCard);		//最大牌上回合出的

	BYTE bFirstType = GetCardType(nowCard, nowCount);
	BYTE bNextType = GetCardType(oldCard, oldCount);

	printf( "出的最大牌=0x%02x 上家出的牌=0x%02x bFirstType=%d  bNextType=%d \n", bFirstMaxCard, bNextMaxCard, bFirstType, bNextType);
	//类型判断
	if (bFirstType == FTK_ERROR || bNextType == FTK_ERROR) //两人出来牌有问题
		return false;

	//先进行单牌,对牌判定,他两只能同类型才能出.别外的类型,是大的就可以打小的
	if (bFirstType == FTK_SINGLE && bNextType == FTK_SINGLE)
		return GetCardLogicValue(bFirstMaxCard) > GetCardLogicValue(bNextMaxCard);
	if (bFirstType == FTK_DOUBLE && bNextType == FTK_DOUBLE)
		return GetCardLogicValue(bFirstMaxCard) > GetCardLogicValue(bNextMaxCard);
	if (bFirstType == FTK_THREE && bNextType == FTK_THREE)
		return GetCardLogicValue(bFirstMaxCard) > GetCardLogicValue(bNextMaxCard);
	if (bFirstType == FTK_DOUBLE_DUI && bNextType == FTK_DOUBLE_DUI)
		return GetCardLogicValue(bFirstMaxCard) > GetCardLogicValue(bNextMaxCard);
	if (bFirstType == FTK_THREE_DUI && bNextType == FTK_THREE_DUI)
		return GetCardLogicValue(bFirstMaxCard) > GetCardLogicValue(bNextMaxCard);

	if (bFirstType > bNextType) //上家出牌的类型大于本家,打不过的意思 
		return true;



	return false;
}


int main()
{

	char m_cbCardD[54] = {1};
	memset(m_cbCardD, 1, sizeof(m_cbCardD));
	//m_cbCardD[2] = 8;
	//m_cbCardD[3] = 8;
	//m_cbCardD[4] = 8;
	//m_cbCardD[5] = 8;

	char m_cbCardD1[54] = { 0 };
	memset(m_cbCardD1, 0, sizeof(m_cbCardD));
	m_cbCardD1[52] = 4;
	//m_cbCardD1[53] = 1;
	//m_cbCardD1[24] = 8;
	//m_cbCardD1[25] = 8;
	//m_cbCardD1[26] = 8;

	cout << IsKingBoom(m_cbCardD1,4);


	while (1)
	{

	}
}