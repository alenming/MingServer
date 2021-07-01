#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;
#define  GAME_PLAYER 2
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0									//花色掩码
#define	LOGIC_MASK_VALUE			0x0F									//数值掩码
#define MAX_INDEX 54
static BYTE m_cbHCardD[GAME_PLAYER][MAX_INDEX] = {
	{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

};
static vector<BYTE> m_vUserBoom;
BYTE GetCardValue(BYTE bCardData)
{
	return bCardData&LOGIC_MASK_VALUE;
}
BYTE SwitchToCardValue(BYTE index)
{
	if (index == 53)
		return 0x4F;
	else if (index == 52)
		return 0x4E;
	return (((index / 13) << 4) | (index % 13)) + 1;
}
void SwitchTo13(vector<BYTE>& vecCard, BYTE vecCardIndex[])
{
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (vecCardIndex[i] <= 0) continue;
		if (i>51)
		{
			vecCard.at(13) += vecCardIndex[i];
		}
		else
		{
			BYTE value = GetCardValue(SwitchToCardValue(i));
			vecCard.at(value) += vecCardIndex[i];
		}
	}
}

void calEndHandCardJiang()
{
	std::vector<BYTE> temp;

	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		//if (m_cbHCardC[i] > 0)
		{
			for (BYTE j = 0; j < 15; j++)
				temp.push_back(0);
			SwitchTo13(temp, (m_cbHCardD[i]));
			for (BYTE j = 0; j < temp.size(); j++)
			{
				if (j == 14 && temp.at(j)>3)
				{
					BYTE realCardCount = temp.at(j);
					realCardCount += 4;
					realCardCount -= 8;
					realCardCount += 1;
					m_vUserBoom.at(i) +=(realCardCount);
				}
				if (temp.at(j) >= 8)
				{
					BYTE realCardCount = temp.at(j);
					realCardCount -= 8;
					realCardCount += 1;
					m_vUserBoom.at(i) +=(realCardCount);
				}
			}
		}
	}
}

int main()
{
	m_vUserBoom.push_back(0);
	m_vUserBoom.push_back(0);
	calEndHandCardJiang();
	while (1)
	{
		//cout << "what fuck +" << i  << endl;
	}
}

