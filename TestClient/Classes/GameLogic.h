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
	��Ϸ�㷨�߼�
*/
//////////////////////////////////////////////////////////////////////////

//�˿�����
#define FTK_ERROR					0x00									//��������

#define FTK_SINGLE					0x10									//��������

#define FTK_DOUBLE					0x20									//��������
#define FTK_DOUBLE_ZA_KING			0x21									//һ������
#define FTK_DOUBLE_LITTLE_KING		0x22									//һ��С��
#define FTK_DOUBLE_BIG_KING			0x23									//һ�Դ���

#define FTK_THREE					0x30									//��������
#define FTK_THREE_ZA_KING			0x31									//��ֻ����
#define FTK_THREE_LITTLE_KING		0x32									//��ֻС��
#define FTK_THREE_BIG_KING			0x33									//��ֻ����

#define FTK_DOUBLE_DUI				0x41									//(���ö�)
#define FTK_THREE_DUI       		0x42									//(���ö�)
#define FTK_510K_FU					0x51									//510k��
#define FTK_510K_ZHENG				0x52									//510k��

#define FTK_BOOM       				0x60									//ը��
#define FTK_BOOM_ZA_KING       		0x61									//����ը��
#define FTK_BOOM_LITTLE_KING       	0x62									//С��ը��
#define FTK_BOOM_BIG_kING       	0x63									//����ը��

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0									//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F									//��ֵ����

#define	FTK_BIG_KING				0x4F									//����
#define	FTK_LITTLE_KING				0x4E									//С��



//��Ϸ�߼���
class CGameLogic
{
public:
	CGameLogic();
	virtual ~CGameLogic();
	//��ȡ����
	int GetCardType(int bCardData[]);
	int GetCardType(std::vector<int>& vecCard);
	int GetCardType(int bCardData[], int cardCount);
	//��ȡ������
	int GetMaxCard(std::vector<int>& vecCard);
	//��ȡ��С����
	int GetMinCard(int bCardData[]);
	//��ȡ��ֵ0x01-0x0D
	int GetCardValue(int bCardData);
	//��ȡ��ɫ0,1,2,3 ����,÷��,����,����
	int GetCardColor(int bCardData);
	//�����˿�
	void RandCardList(int bCardDIndex[], int cbMaxCount);
	//ɾ���˿�
	bool RemoveCard(const int bRemoveCard[], int bRemoveCount, int bCardData[], int bCardCount);
	//�Ƿ���������ֵ���
	bool IsHaveThisMathCard(std::vector<int>& vecCard, int targetCard);
	bool IsHaveThisMathCard(std::vector<int>& vecCard, std::vector<int>& vecTargetCard);
	//�Ƿ�ͬ��ɫ
	bool IsSameColorCard(std::vector<int>&  vecCard);
	//�Ƿ�ͬ����
	bool IsSameValueCard(std::vector<int>& vecCard);
	//�Ƿ�ȫ�����Ǵ�С��
	bool IsAllKing(std::vector<int>&  vecCard);
	//��54��λ�õ�����ת����һ����ʽ����
	void change(int bCardData[], std::vector<int> &result, int cardCount=0);
	//ÿ�����ֵ��������Ƿ����
	bool isEqualDiffNumber(std::vector<int>& vecCard);
	//һ�����ֵ�������,�����ж�2���öԻ��������ö�
	int howmanyOneNumber(std::vector<int>& vecCard);
	//�߼���ֵ17 16 15 14 13 12 11 10 9 8 7 5 4, 3
	int GetCardLogicValue(int bCardData);
	//�Ա��˿�,��������
	bool CompareCard(int nowCard[], int oldCard[], int nowCount, int oldCount);

	int SwitchToCardIndex(int cbCardData);
	int SwitchToCardIndex(const int cbCardData[], int cbCardCount, int cbCardIndex[]);

	int SwitchToCardValue(int cbCardData);
	void SwitchToCardValue(std::vector<int>&  vecCard, int cbCardIndex[]);
	void SwitchToCardValue(std::vector<int>&  vecCard, std::vector<int>&  vecCardIndex);
	//������־
	void WriteLog(char * szFormat, ...);
	void WriteHandCard(std::string heard, const int cbCardData[], bool isEnter = true);
	void WriteHandCard(int cbCardData[]);
	//���÷�������
	void SetServer(int server_id, int table_id);
	//�����е�ֵ�Ƿ����
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
	//��������
	static const int						m_bCardListData[8*MAX_INDEX];				//�˿�����
	int										m_cbtable_id;						//����id
	int										m_cbserver_id;						//����id
};


#endif //GAME_LOGIC_HEAD_H
