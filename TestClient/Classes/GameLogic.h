#ifndef GAME_LOGIC_HEAD_H
#define GAME_LOGIC_HEAD_H
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>

#define MAX_INDEX 54
#pragma once
//////////////////////////////////////////////////////////////////////////
/*
	游戏算法逻辑
*/
//////////////////////////////////////////////////////////////////////////

//扑克类型
#define FTK_ERROR					0x00									//错误类型

#define FTK_SINGLE					0x10									//单牌类型

#define FTK_DOUBLE					0x20									//对牌类型
#define FTK_DOUBLE_ZA_KING			0x21									//一对杂王
#define FTK_DOUBLE_LITTLE_KING		0x22									//一对小王
#define FTK_DOUBLE_BIG_KING			0x23									//一对大王

#define FTK_THREE					0x30									//三张类型
#define FTK_THREE_ZA_KING			0x31									//三只杂王
#define FTK_THREE_LITTLE_KING		0x32									//三只小王
#define FTK_THREE_BIG_KING			0x33									//三只大王

#define FTK_DOUBLE_DUI				0x41									//(姐妹对)
#define FTK_THREE_DUI       		0x42									//(姐妹对)
#define FTK_510K_FU					0x51									//510k副
#define FTK_510K_ZHENG				0x52									//510k正

#define FTK_BOOM       				0x60									//炸弹
#define FTK_BOOM_ZA_KING       		0x61									//杂王炸弹
#define FTK_BOOM_LITTLE_KING       	0x62									//小王炸弹
#define FTK_BOOM_BIG_kING       	0x63									//大王炸弹

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0									//花色掩码
#define	LOGIC_MASK_VALUE			0x0F									//数值掩码

#define	FTK_BIG_KING				0x4F									//大王
#define	FTK_LITTLE_KING				0x4E									//小王



//游戏逻辑类
class CGameLogic
{
public:
	CGameLogic();
	virtual ~CGameLogic();
	//获取类型
	int GetCardType(int bCardData[]);
	int GetCardType(std::vector<int>& vecCard);
	int GetCardType(int bCardData[], int cardCount);
	//获取最大的牌
	int GetMaxCard(std::vector<int>& vecCard);
	//获取最小的牌
	int GetMinCard(int bCardData[]);
	//获取数值0x01-0x0D
	int GetCardValue(int bCardData);
	//获取花色0,1,2,3 方块,梅花,红桃,黑桃
	int GetCardColor(int bCardData);
	//混乱扑克
	void RandCardList(int bCardDIndex[], int cbMaxCount);
	//删除扑克
	bool RemoveCard(const int bRemoveCard[], int bRemoveCount, int bCardData[], int bCardCount);
	//是否有这个数字的牌
	bool IsHaveThisMathCard(std::vector<int>& vecCard, int targetCard);
	bool IsHaveThisMathCard(std::vector<int>& vecCard, std::vector<int>& vecTargetCard);
	//是否同花色
	bool IsSameColorCard(std::vector<int>&  vecCard);
	//是否同数字
	bool IsSameValueCard(std::vector<int>& vecCard);
	//是否全部都是大小王
	bool IsAllKing(std::vector<int>&  vecCard);
	//把54个位置的数量转换成一堆正式的牌
	void change(int bCardData[], std::vector<int> &result, int cardCount=0);
	//每个数字的牌数量是否相等
	bool isEqualDiffNumber(std::vector<int>& vecCard);
	//一个数字的牌数量,用来判断2姐妹对还是三姐妹队
	int howmanyOneNumber(std::vector<int>& vecCard);
	//逻辑数值17 16 15 14 13 12 11 10 9 8 7 5 4, 3
	int GetCardLogicValue(int bCardData);
	//对比扑克,任意类型
	bool CompareCard(int nowCard[], int oldCard[], int nowCount, int oldCount);

	int SwitchToCardIndex(int cbCardData);
	int SwitchToCardIndex(const int cbCardData[], int cbCardCount, int cbCardIndex[]);

	int SwitchToCardValue(int cbCardData);
	void SwitchToCardValue(std::vector<int>&  vecCard, int cbCardIndex[]);
	void SwitchToCardValue(std::vector<int>&  vecCard, std::vector<int>&  vecCardIndex);
	//保存日志
	void WriteLog(char * szFormat, ...);
	void WriteHandCard(std::string heard, const int cbCardData[], bool isEnter = true);
	void WriteHandCard(int cbCardData[]);
	//设置房间桌子
	void SetServer(int server_id, int table_id);
	//容器中的值是否相等
	template <class T>
	bool isSameElm(T & x)
	{
		if (x.size() == 0) return false;
		T::iterator be = x.begin();
		for (T::iterator i = x.begin(); i != x.end(); i++)
		{
			if (*i != *be)
				return false;
		}
		return true;
	};
public:
	//变量定义
	static const int						m_bCardListData[8*MAX_INDEX];				//扑克数据
	int										m_cbtable_id;						//桌子id
	int										m_cbserver_id;						//房间id
};


#endif //GAME_LOGIC_HEAD_H
