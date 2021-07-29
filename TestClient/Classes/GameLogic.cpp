#include "GameLogic.h"


//扑克数据 6全部取出
const int	CGameLogic::m_bCardListData[8*54] =
{
	0x01, 0x02, 0x03, 0x04, 0x05,0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15,0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25,0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35,0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4
};

CGameLogic::CGameLogic(){}
//析构函数
CGameLogic::~CGameLogic(){}

bool CGameLogic::CompareCard(int nowCard[], int oldCard[], int nowCount, int oldCount)
{
	WriteHandCard("现在出的牌为:", nowCard, false);
	WriteHandCard("上家出的牌为:", oldCard, false);

	std::vector<int> copyNowCard;
	std::vector<int> copyOldCard;
	copyNowCard.clear();
	copyOldCard.clear();
	change(nowCard, copyNowCard);
	change(oldCard, copyOldCard);

	//获取类型
	int nowMaxCard = GetMaxCard(copyNowCard);	//最大牌现在出的
	int oldMaxCard = GetMaxCard(copyOldCard);		//最大牌上回合出的

	int nowType = GetCardType(copyNowCard);
	int oldType = GetCardType(copyOldCard);

	WriteLog("出的最大牌=0x%02x 上家出最大牌=0x%02x nowType=%d  oldType=%d nowCount=%d oldCount=%d\n",
		nowMaxCard, oldMaxCard, nowType, oldType, copyNowCard.size(), copyOldCard.size());
	//类型判断
	if (nowType == FTK_ERROR || oldType == FTK_ERROR) //两人出来牌有问题
		return false;

	if (nowType == FTK_SINGLE && oldType == FTK_SINGLE)
		return GetCardLogicValue(nowMaxCard) > GetCardLogicValue(oldMaxCard);
	else if ((nowType&0XF0) == FTK_DOUBLE && (oldType&0XF0) == FTK_DOUBLE)
	{
		if (oldType == FTK_DOUBLE_LITTLE_KING && nowType ==FTK_DOUBLE_ZA_KING)//之前是对小王,后边出了大小王.不能出,false
			return false;
		return GetCardLogicValue(nowMaxCard) > GetCardLogicValue(oldMaxCard);
	}
	else if ((nowType & 0XF0) == FTK_THREE && (oldType & 0XF0) == FTK_THREE)
	{
		if (oldType == FTK_THREE_LITTLE_KING && nowType == FTK_THREE_ZA_KING)//之前是三小王,后边出了大小王.不能出,false
			return false;
		return GetCardLogicValue(nowMaxCard) > GetCardLogicValue(oldMaxCard);
	}	
	else if (nowType == FTK_DOUBLE_DUI && oldType == FTK_DOUBLE_DUI)
	{
		if (nowCount != oldCount)
			return false;
		return GetCardLogicValue(nowMaxCard) > GetCardLogicValue(oldMaxCard);
	}
	else if (nowType == FTK_THREE_DUI && oldType == FTK_THREE_DUI)
	{
		if (nowCount != oldCount)
			return false;
		return GetCardLogicValue(nowMaxCard) > GetCardLogicValue(oldMaxCard);
	}
	else if ((nowType > FTK_THREE_DUI && nowType < FTK_BOOM) && (oldType > FTK_THREE_DUI &&  oldType < FTK_BOOM))//510K
		return nowType > oldType;
	else if (nowType == FTK_BOOM && oldType == FTK_BOOM)
	{
		int bnowCount = howmanyOneNumber(copyNowCard);
		int boldCount = howmanyOneNumber(copyOldCard);
		bnowCount = IsAllKing(copyNowCard) ? nowCount + 4 : bnowCount;
		boldCount = IsAllKing(copyOldCard) ? oldCount + 4 : boldCount;
		WriteLog("是否是王炸bnowCount=%d boldCount=%d\n", bnowCount, boldCount);
		if (bnowCount == boldCount)
			return GetCardLogicValue(nowMaxCard) > GetCardLogicValue(oldMaxCard);
		else
			return bnowCount > boldCount;
	}
	WriteLog("nowType > oldType\n");
	return nowType > oldType;
}
//获取出牌类型 参数: 出的牌列表, 出的张数(王炸,4张炸,三张炸,对子,单张)
int CGameLogic::GetCardType(int bCardData[])
{
	std::vector<int> copyCard;
	change(bCardData, copyCard);
	return GetCardType(copyCard);
}
//获取出牌类型 参数: 出的牌列表, 出的张数(王炸,4张炸,三张炸,对子,单张)
int CGameLogic::GetCardType(int bCardData[], int cardCount)
{
	std::vector<int> copyCard;
	change(bCardData, copyCard, cardCount);
	return GetCardType(copyCard);
}
//获取出牌类型 参数: 出的牌列表, 出的张数(王炸,4张炸,三张炸,对子,单张)
int CGameLogic::GetCardType(std::vector<int>& vecCard)
{
	BYTE bCardCount = vecCard.size();
	switch (bCardCount)
	{
	case FTK_ERROR: //错误
		return FTK_ERROR;
	case 1: //单牌
		return FTK_SINGLE;
	case 2://对牌
	{
		if (IsAllKing(vecCard))//是两只王
		{
			if (isSameElm(vecCard) && vecCard.at(0) == FTK_LITTLE_KING)
				return FTK_DOUBLE_LITTLE_KING;
			else if (isSameElm(vecCard) && vecCard.at(0) == FTK_DOUBLE_BIG_KING)
				return FTK_DOUBLE_BIG_KING;
			else if (!isSameElm(vecCard))
				return FTK_DOUBLE_ZA_KING;
		}
		return IsSameValueCard(vecCard) ? FTK_DOUBLE : FTK_ERROR;//是正常的一对牌
	}
	case 3://三张牌
	{
		if (IsAllKing(vecCard))//是三只王
		{
			if (IsSameValueCard(vecCard) && vecCard.at(0) == FTK_LITTLE_KING)
				return FTK_THREE_LITTLE_KING;
			else if (IsSameValueCard(vecCard) && vecCard.at(0) == FTK_BIG_KING)
				return FTK_THREE_BIG_KING;
			else if (!IsSameValueCard(vecCard))
				return FTK_THREE_ZA_KING;
		}
		else if (IsSameValueCard(vecCard))//是正常的三只牌
			return FTK_THREE;
		else
		{
			std::vector<int> vecTargetCard;
			vecTargetCard.push_back(0X05);
			vecTargetCard.push_back(0x0A);
			vecTargetCard.push_back(0x0D);
			bool isSameColor = IsSameColorCard(vecCard);
			if (IsHaveThisMathCard(vecCard, vecTargetCard))
				return isSameColor ? FTK_510K_ZHENG : FTK_510K_FU;
			return FTK_ERROR;
		}
	}
	}
	//4张牌起,先判断是不是王炸
	if (IsAllKing(vecCard))
	{
		//所有牌是不是一个数字  	//4张牌起
		if (IsSameValueCard(vecCard))
			return GetMaxCard(vecCard) == (FTK_LITTLE_KING) ? FTK_BOOM_LITTLE_KING : FTK_BOOM_BIG_kING;
		else
			return FTK_BOOM_ZA_KING;
	}
	else//再判断普通炸弹和姐妹队
	{
		if (IsSameValueCard(vecCard))//只有一个数字是普通炸弹
			return FTK_BOOM;
		else
		{
			//每个数字的牌数量是否相等,姐妹队用
			if (isEqualDiffNumber(vecCard))
			{
				BYTE temp = howmanyOneNumber(vecCard);
				if (temp == 2)
					return FTK_DOUBLE_DUI;
				else if (temp == 3)
					return FTK_THREE_DUI;
			}
		}
	}
	return FTK_ERROR;
}

//获取最大的单牌,返回的是16进制的值
int CGameLogic::GetMaxCard(std::vector<int>& vecCard)
{
	int temp1, temp2, temp = 0x00;
	temp1 = GetCardLogicValue(*vecCard.begin());
	temp = *vecCard.begin();
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
	{
		temp2 = GetCardLogicValue(*i);
		if (temp2 > temp1)
		{
			temp1 = temp2;
			temp = *i;
		}
	}
	return temp;
}
//获取最小的牌
int CGameLogic::GetMinCard(int bCardData[])
{
	std::vector<int> vecCard;
	vecCard.clear();

	SwitchToCardValue(vecCard, bCardData);

	int temp1, temp2, temp = 0x00;
	temp1 = GetCardLogicValue(*vecCard.begin());
	temp = *vecCard.begin();
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
	{
		temp2 = GetCardLogicValue(*i);
		if (temp2 < temp1)
		{
			temp1 = temp2;
			temp = *i;
		}
	}
	return temp;
}
//牌中是否有某个字面数字的牌.4个颜色的510K等 。
bool CGameLogic::IsHaveThisMathCard(std::vector<int>& vecCard, int targetCard)
{
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
		if (GetCardValue(*i) == targetCard)
			return true;
	return false;
}
bool CGameLogic::IsHaveThisMathCard(std::vector<int>& vecCard, std::vector<int>& vecTargetCard)
{
	for (auto i = vecTargetCard.begin(); i != vecTargetCard.end(); ++i)
		if (!IsHaveThisMathCard(vecCard, *i))
			return false;
	return true;
}
//出的牌是不是同一个颜色.区分正副510K用
bool CGameLogic::IsSameColorCard(std::vector<int>& vecCard)
{
	std::vector<int> vecColor;
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
		vecColor.push_back(GetCardColor(*i));
	return isSameElm(vecColor);
}
//出的牌是否是同一个字面数字
bool CGameLogic::IsSameValueCard(std::vector<int>& vecCard)
{
	std::vector<int> vecValue;
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
		vecValue.push_back(GetCardValue(*i));
	return isSameElm(vecValue);
}
//是否全部都是大小王
bool CGameLogic::IsAllKing(std::vector<int>& vecCard)
{
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
	{
		if ((*i) < FTK_LITTLE_KING)
			return false;
	}
	return true;
}
//把54个位置的数量转换成一堆正式的牌
void CGameLogic::change(int vecCard[], std::vector<int> &result, int cardCount)
{
	if (cardCount == 0)
	{
		for (int i = 0; i < MAX_INDEX; i++)
		{
			if (vecCard[i] <= 0) continue;
			for (int j = 0; j < vecCard[i]; j++)
				result.push_back(SwitchToCardValue(i));
		}
	}
	else
	{
		for (int i = 0; i < cardCount; i++)
		{
			result.push_back(vecCard[i]);
		}
	}
}
//每个数字的牌数量是否相等,姐妹队用
bool CGameLogic::isEqualDiffNumber(std::vector<int>& vecCard)
{
	std::map<int, int> temp;
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
	{
		temp[GetCardValue(*i)] ++;
	}
	std::vector<int> vecValue;
	for (auto be = temp.begin(); be != temp.end(); ++be)
	{
		if (be->second <= 0) continue;
		vecValue.push_back(be->second);
	}
	return isSameElm(vecValue);;
}
//不论颜色一个数字的牌有多少张
int CGameLogic::howmanyOneNumber(std::vector<int>& vecCard)
{
	std::map<int, int> temp;
	for (auto i = vecCard.begin(); i != vecCard.end(); ++i)
		temp[GetCardValue(*i)]++;

	auto be = temp.begin();
	if (be != temp.end())
		return be->second;
	return 0;
}
//扑克索引转换字面
int CGameLogic::SwitchToCardIndex(int cbCardData)
{
	int t = 0;
	int color = (GetCardColor(cbCardData) >> 4);//0-3  王是4
	int value = GetCardValue(cbCardData);//1-13  王是1415

	if (cbCardData == 0x4E)
		t =  52;
	else if (cbCardData == 0x4F)
		t = 53;
	else
		t = color * 13 + value - 1;//从0开始
	return t;
}
//扑克索引转换字面
int CGameLogic::SwitchToCardIndex(const int cbCardData[], int cbCardCount, int cbCardIndex[])
{
	//设置变量
	ZeroMemory(cbCardIndex, sizeof(int)*MAX_INDEX);
	//转换扑克
	for (int i = 0; i < cbCardCount; i++)
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;

	return cbCardCount;
}
//扑克字面转换索引
int CGameLogic::SwitchToCardValue(int index)
{
	if (index == 53)
		return 0x4F;
	else if (index == 52)
		return 0x4E;
	return (((index / 13) << 4) | (index % 13)) + 1;
}
//扑克字面转换索引
void CGameLogic::SwitchToCardValue(std::vector<int>& vecCard, int cbCardIndex[])
{
	vecCard.clear();
	for (int i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] <= 0) continue;
		for (int j = 0; j < cbCardIndex[i]; j++)
			vecCard.push_back(SwitchToCardValue(i));
	}
}
//扑克字面转换索引
//混乱扑克
void CGameLogic::RandCardList(int cbCardData[], int cbMaxCount)
{
	//int cbRandCount = 0;
	////int cbCount = rand_(2, 3);
	//do
	//{
	//	for (int i = cbMaxCount - 1; i > 0; i--)
	//	{
	//	//	int index = random_int(0, i);
	//		int tmp = cbCardData[i];
	//		cbCardData[i] = cbCardData[index];
	//		cbCardData[index] = tmp;
	//	}
	//	cbRandCount++;
	//} while (cbRandCount < cbCount);
}
//删除扑克
bool CGameLogic::RemoveCard(const int bRemoveCard[], int bRemoveCount, int bCardData[], int bCardCount)
{
	WriteLog("删牌 bRemoveCount=%d bCardCount=%d\n", bRemoveCount, bCardCount);
	WriteHandCard("删除的牌为:\n", bRemoveCard, false);
	//检验数据
	//ASSERT(bRemoveCount <= bCardCount);
	//定义变量
	int bDeleteCount = 0;
	int bTempCardData[MAX_INDEX];

	CopyMemory(bTempCardData, bCardData, MAX_INDEX*sizeof(int));
	//置零扑克
	for (int i = 0; i < MAX_INDEX; i++)
	{
		if (bRemoveCard[i] <= 0)
			continue;
		if (bRemoveCard[i] <= bTempCardData[i])
		{
			bDeleteCount += bRemoveCard[i];
			bTempCardData[i] -= bRemoveCard[i];
			WriteLog("删除扑克 ------------>  0x%02x   删除了%d张    \n", SwitchToCardValue(i), bRemoveCard[i]);
		}
		else
			WriteLog("玩家手上并没有这么多牌!你的牌哪来的 ------------>  0x%02x   删除了%d张    \n", SwitchToCardValue(i), bRemoveCard[i]);
	}
	WriteLog("一共删了%d张牌\n", bDeleteCount);
	if (bDeleteCount != bRemoveCount) return false;
	WriteLog("删牌前后对比\n");
	WriteHandCard(bCardData);
	ZeroMemory(bCardData, sizeof(bCardData));
	CopyMemory(bCardData, bTempCardData, MAX_INDEX*sizeof(int));
	WriteHandCard(bCardData);
	return true;
}
//逻辑数值17 16 15 14 13 12 11 10 9 8 7 5 4, 3
int CGameLogic::GetCardLogicValue(int bCardData)
{
	if (bCardData == FTK_BIG_KING)//大王
		return 17;
	else if (bCardData == FTK_LITTLE_KING)//小王
		return 16;
	int bCardValue = GetCardValue(bCardData);
	return (bCardValue <= 2) ? (bCardValue + 13) : bCardValue;

}
//牌的花色
int CGameLogic::GetCardColor(int bCardData)
{
	return bCardData&LOGIC_MASK_COLOR;
}
//牌的字面大小
int CGameLogic::GetCardValue(int bCardData)
{
	return bCardData&LOGIC_MASK_VALUE;
}
//写日志
void CGameLogic::WriteLog(char * szFormat, ...)
{
	/*
	bool bShowTime = false;
	time_t time_seconds = time(NULL);
	struct tm pNow;
	localtime_s(&pNow, &time_seconds);
	CHAR fileDirectory[128];
	sprintf_s(fileDirectory, "Log//server_%d//%04d-%02d-%02d", m_cbserver_id, pNow.tm_year + 1900, pNow.tm_mon + 1, pNow.tm_mday);
	if (!PathIsDirectoryA(fileDirectory))
	{
		sprintf_s(fileDirectory, "Log");
		if (!PathIsDirectoryA(fileDirectory))
		{
			CreateDirectoryA(fileDirectory, NULL);
		}
		sprintf_s(fileDirectory, "Log//server_%d", m_cbserver_id);
		if (!PathIsDirectoryA(fileDirectory))
		{
			CreateDirectoryA(fileDirectory, NULL);
		}
		sprintf_s(fileDirectory, "Log//server_%d//%04d-%02d-%02d", m_cbserver_id, pNow.tm_year + 1900, pNow.tm_mon + 1, pNow.tm_mday);
		if (!PathIsDirectoryA(fileDirectory))
		{
			CreateDirectoryA(fileDirectory, NULL);
		}
	}
	FILE * file = NULL;
	CHAR filepath[128];
	sprintf_s(filepath, "Log//server_%d//%04d-%02d-%02d//table_%d.txt", m_cbserver_id, pNow.tm_year + 1900, pNow.tm_mon + 1, pNow.tm_mday, m_cbtable_id);
	file = fopen(filepath, "a");
	if (!file) return;
	if (bShowTime)
	{
		fprintf_s(file, "%04d-%02d-%02d %02d:%02d:%02d", pNow.tm_year + 1900,
			pNow.tm_mon + 1, pNow.tm_mday, pNow.tm_hour, pNow.tm_min, pNow.tm_sec);
	}
	va_list ap;
	va_start(ap, szFormat);
	vfprintf(file, szFormat, ap);
	va_end(ap);
	fclose(file);
	file = NULL;
	*/
}
//设置房间桌子
void CGameLogic::SetServer(int server_id, int table_id)
{
	m_cbserver_id = server_id;
	m_cbtable_id = table_id;
}

void CGameLogic::WriteHandCard(std::string heard, const int cbCardData[], bool isEnter)
{
	std::string temp = "";
	temp += heard;
	int temp1, temp2 = 0;
	for (int i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardData[i] <= 0)
			continue;
		temp1 = GetCardColor(SwitchToCardValue(i));
		if (isEnter && temp1 > temp2) temp += "\n";
		temp2 = temp1;
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
			temp += std::to_string(GetCardValue(SwitchToCardValue(i))) + ":";

		temp += std::to_string(cbCardData[i]) + "张  ";
	}
	temp += "\n";
	WriteLog(const_cast<char*>(temp.c_str()));
}

void CGameLogic::WriteHandCard(int bCardData[])
{
	WriteLog("1 2 3 4 5 6 7 8 9 1 J Q K 1 2 3 4 5 6 7 8 9 1 J Q K 1 2 3 4 5 6 7 8 9 1 J Q K 1 2 3 4 5 6 7 8 9 1 J Q K 小 大\n");
	WriteLog("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	bCardData[0], bCardData[1], bCardData[2], bCardData[3], bCardData[4], bCardData[5], bCardData[6], bCardData[7], bCardData[8], bCardData[9], bCardData[10], bCardData[11], bCardData[12],
	bCardData[13], bCardData[14], bCardData[15], bCardData[16], bCardData[17], bCardData[18], bCardData[19], bCardData[20], bCardData[21], bCardData[22], bCardData[23], bCardData[24], bCardData[25],
	bCardData[26], bCardData[27], bCardData[28], bCardData[29], bCardData[30], bCardData[31], bCardData[32], bCardData[33], bCardData[34], bCardData[35], bCardData[36], bCardData[37], bCardData[38],
	bCardData[39], bCardData[40], bCardData[41], bCardData[42], bCardData[43], bCardData[44], bCardData[45], bCardData[46], bCardData[47], bCardData[48], bCardData[49], bCardData[50], bCardData[51],
	bCardData[52], bCardData[53]);
}

bool CGameLogic::IsDuiDuiHu(std::vector<BYTE>& handCard,BYTE weaveCount, BYTE curCard)
{
	std::vector<BYTE> copyVec;
	SwitchToCardValue(copyVec, handCard);
	for(std::vector<BYTE>::iterator i=copyVec.begin(); i!=copyVec.end(); ++i)
	{
		if(*i == 0) continue;
		if(*i == 1 || *i == 3)
			return false;
	}
	return true;
}

//扑克字面转换索引
void CGameLogic::SwitchToCardValue(std::vector<BYTE>&resVec, std::vector<BYTE>& oldVec)
{
	resVec.clear();
	FOR_I(15)
	{
		resVec.push_back(0);
	}
	for(std::vector<BYTE>::iterator i=oldVec.begin(); i!=oldVec.end(); ++i)
	{
		BYTE val = GetCardValue(*i);
		++resVec[(val-1)];
	}
}
bool CGameLogic::AnalyseHu(std::vector<BYTE>& handCard, BYTE weaveCount)
{
	BYTE cbCardCount = handCard.size();
	//效验数目
	if ((cbCardCount<2) || (cbCardCount>MAX_HAND_COUNT) || ((cbCardCount - 2) % 3 != 0))
		return false;
	//转换扑克
	std::vector<BYTE> copyVec;
	SwitchToCardValue(copyVec, handCard);

	//单调
	BYTE lessKindItem = (cbCardCount - 2) / 3; 
	if (lessKindItem == 0)
	{
		for(std::vector<BYTE>::iterator i=copyVec.begin(); i!=copyVec.end(); ++i)
		{
			if (*i == 0) continue;
			if(*i == 2)
				return true;
		}
	}
	BYTE cbJiangPai = 0;
	if ((cbCardCount >= 2) &&(cbCardCount - 2) % 3 == 0)
	{
		for (BYTE g = 0; g < 13; g++)
		{
			std::vector<BYTE> tempVec = copyVec;
			if (AnalyseJiangCard(tempVec, g) && AnalysePerutCard(tempVec))//选取将牌,判断是否满足n*ABC+z*EEE
			{
				cbJiangPai++;break;	//记录所有花色中可以做将牌数量
			}
		}	
	}
	//牌数量3n，无将牌
	else if (cbCardCount % 3 == 0)
	{
		return false;
	}
	//牌数量不满足3n+2或3n一定不能胡牌
	else
	{
		return false;
	}
	//说明所有将牌组合动不能满足n*ABC+z*EEE,或者花色（万条同）能做将牌的不止一对
	if (cbJiangPai != 1)
		return false;

	return true;
}

bool CGameLogic::AnalyseJiangCard(std::vector<BYTE>& handCard, BYTE index)
{
	//判断是否可为将牌
	if (handCard[index] >= 2)
	{
		//减去将牌
		handCard[index] -= 2;
		return true;
	}
	return false;
}

bool CGameLogic::AnalysePerutCard(std::vector<BYTE>& handCard)
{
	BYTE j = 0;
	while (j < 13)
	{
		//对应牌数量为零跳过
		if (handCard.at(j) == 0)
		{
			j++;
			continue;
		}
		/*if (j > 6 && cbCardIndexTemp[j] != 3) return false;*/
		//从第一个牌数量不为0开始判断
		else if (handCard.at(j) >= 4)
		{
			//如果之后相连的两张牌数量大于0则可组成ABC形式
				if (handCard.at(j + 1) >0 && handCard.at(j+2) > 0)
				{
					//从牌堆中减去满足要求的牌
					handCard.at(j) -= 4;
					handCard.at(j + 1) -= 1;
					handCard.at(j+2) -= 1;
					j++;
				}
				else
				{
					return false;
				}
		}
		//如果第一个牌数量>=3，只能以AAA形式存在
		else if (handCard.at(j) == 3)
		{
			//从牌堆中减去满足要求的牌
			handCard.at(j) -= 3;
			j++;
		}
		//如果第一个牌数量<=2，只能以之后相连的2张牌组成ABC形式存在
		else if (handCard.at(j) == 2)
		{
			//如果之后相连的两张牌数量大于0则可组成ABC形式
			if (j > 6) return false;
				if (handCard.at(j+1) >= 2 && handCard.at(j + 2) >= 2)
				{
					//从牌堆中减去满足要求的牌
					handCard.at(j) -= 2;
					handCard.at(j + 1) -= 2;
					handCard.at(j + 2) -= 2;
					j++;
				}
				else
				{
					return false;
				}
		}
		else if (handCard.at(j)== 1)
		{
			//如果之后相连的两张牌数量大于0则可组成ABC形式
			if (j > 9) return false;
				if (handCard.at(j + 1) >= 1 && handCard.at(j+ 2) >= 1)
				{
					//从牌堆中减去满足要求的牌
					handCard.at(j) -= 1;
					handCard.at(j + 1) -= 1;
					handCard.at(j + 2) -= 1;
					j++;
				}
				else
				{
					return false;
				}
		}
	}
	return true;
}

