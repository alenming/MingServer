#define  MAX_INDEX 13
#define  MAX_HANDCOUNT 8
#include <stdio.h>
#include <vector>
char TransToIndexModel(char const & _c)
{
//	return (_c / 10) * 9 + (_c % 10) - 1;
	return (_c & 0x0F) - 1;
}

void SwitchToCardValue(std::vector<char>&resVec, std::vector<char>& oldVec)
{
	resVec.clear();
	for (int i = 0; i < MAX_INDEX; i++)
	{
		resVec.push_back(0);
	}
	for(std::vector<char>::iterator i=oldVec.begin(); i!=oldVec.end(); ++i)
	{
		++resVec[TransToIndexModel(*i)];
	}
}
int TransToIndexArrayModel(char const src[MAX_HANDCOUNT], char Des[MAX_INDEX])
{
	memset(Des,0,sizeof(char)*MAX_INDEX);
	size_t AddCount = 0;
	for (size_t i = 0; i < MAX_HANDCOUNT;++i)
	{
		if(src[i]!=0)
		{
			char TempIndex = TransToIndexModel(src[i]);
			++Des[TempIndex];
			++AddCount;
		}
	}
	return AddCount;
}

//检查余下的牌是否能组成顺或者横
bool CheckCanContinuity(char const CardIndex[MAX_INDEX],  char GhostCount)
{
	char CardIndexTemp[MAX_INDEX] = { 0 };
	memcpy(CardIndexTemp, CardIndex, sizeof(CardIndexTemp));

	char GhostCardNeeded = 0;
	for (char i = 0; i < MAX_INDEX; ++i)
	{
		if (0 == CardIndexTemp[i])
			continue;
		if (CardIndexTemp[i] < 0)//有张牌大于0时,直接把后边两位置也减掉,然后用癞子补齐,不够 就是不能糊
		{
			GhostCardNeeded -= CardIndexTemp[i];  //因为牌数为负 所以用减
			CardIndexTemp[i] = 0;
			continue;
		}
		if (i >= 0 && i <= 10)  //这个判断是用在牌值在（1-11）之间(可以组成[i,i+1,i+2]的顺子)
		{
			switch (CardIndexTemp[i])
			{
				case 1:
				case 4:
				{
					//组成顺
					--CardIndexTemp[i + 1];
					--CardIndexTemp[i + 2];
					break;
				}
				case 2:
				{
					//230 203 200 201 210 211  补一个鬼让前边有三张成刻
					if ((CardIndexTemp[i + 1] < 2 && CardIndexTemp[i + 2] < 2) || (3 == CardIndexTemp[i + 1] && 0 == CardIndexTemp[i + 2]) || (0 == CardIndexTemp[i + 1] && 3 == CardIndexTemp[i + 2]))
					{
						//组成刻子
						++GhostCardNeeded;
					}
					else
					{
						//组成顺
						CardIndexTemp[i + 1] -= 2;
						CardIndexTemp[i + 2] -= 2;
					}
					break;
				}
				case 3:
				{
					break;
				}
			}
		}
		else if (11 == i || 12 == i)//这个判断是用在牌值在（12 13）之间(不可以组成[i,i+1,i+2]的顺子，可以组成横或者i为8的情况下[i-1,i,i+1])
		{
			switch (CardIndexTemp[i])
			{
				case 1:
				case 4:
				{
					if ((11 == i) && CardIndexTemp[i + 1] > 0)
					{
						--CardIndexTemp[i + 1];
						++GhostCardNeeded;
					}
					else
					{
						//如果i+1为0 则直接组成横
						GhostCardNeeded += 2;
					}
					break;
				}
				case 2:
				{
					++GhostCardNeeded; 
					break;
				}
				case 3:
				{
					break;
				}
			}
		}
//		else
//		{
//			//字牌和风牌不能成顺 只能组成横
//			if (CardIndexTemp[i] < 3)
//			{
//				GhostCardNeeded += (3 - CardIndexTemp[i]);
//			}
//			if (CardIndexTemp[i] > 3)//有4个,两对将,不能胡
//			{
//				return false;
//			}
//		}
		CardIndexTemp[i] = 0;
	}

	//检查需要鬼牌数量和实际鬼牌的数量
	if (GhostCardNeeded > GhostCount)
	{
		return false;
	}
	else
	{
		return true;
	}

}


bool CheckCanHu(char const HandCard[MAX_INDEX], size_t HandCardSize,size_t GhostCount)
{
	char TotalCount = HandCardSize + GhostCount;
	if ((TotalCount - 2) % 3 != 0 || TotalCount > MAX_HANDCOUNT)
	{
		//牌数，不满足3n+2，或大于14张
		return false;
	}
	//当该手牌中只有万能牌时
	if (TotalCount == GhostCount)
	{
		return true;
	}
	char CardIndexTemp[MAX_INDEX];
	memcpy(CardIndexTemp, HandCard, sizeof(CardIndexTemp));
	std::vector<char> PairsInfo;  //用于存放所有能构成胡牌的对子的值
	char CheckZero = ((GhostCount < 2) ? 2 : 0);//(0-未检查 1-成功 2-失败 )
	//1.把手牌中的每一张当做对子（将牌），然后检查余下的牌是否能达到胡牌条件(组成顺子或者横)
	for (char i = 0; i < MAX_INDEX; ++i)
	{
		if (0 == CardIndexTemp[i])
		{
			//尝试检测一次鬼牌变成手牌里没有的牌
			if (CheckZero == 0)  
			{
				//CheckZero不为0说明检测过了 不再检测
				GhostCount -= 2;
				if (CheckCanContinuity(CardIndexTemp, GhostCount))
				{
					CheckZero = 1;
					//PairsInfo.clear();
					//PairsInfo.push_back(-1);
					//如果变换成手中没有的牌都能胡牌 则说明可以胡任意牌，无需再继续检测
					return true;
				}
				else
				{
					CheckZero = 2;
				}
				//检测失败，还原牌，进行下一轮检测
				GhostCount += 2;
			}
			continue;
		}
		if (CardIndexTemp[i] > 4)
		{
			//牌数超过4张
			return false;
		}
		if (CardIndexTemp[i] >= 2)
		{
			CardIndexTemp[i] -= 2;
			if (CheckCanContinuity(CardIndexTemp, GhostCount))
			{
				//PairsInfo.push_back(i);//如果需要知道详细的胡那些对子，则此处不要返回
				return true;
			}
			//检测失败，还原牌，进行下一轮检测
			CardIndexTemp[i] += 2;
		}
		else if (GhostCount > 0)  //只有一张牌,那么就要使用一张鬼牌的来代替
		{

			--GhostCount;
			--CardIndexTemp[i];
			if (CheckCanContinuity(CardIndexTemp, GhostCount))
			{
				//PairsInfo.push_back(i);  //如果需要知道详细的胡那些对子，则此处不要返回
				return true;
			}
			//检测失败，还原牌，进行下一轮检测
			++CardIndexTemp[i];
			++GhostCount; 
		}
	}
	return false;
	//return (PairsInfo.size() > 0);
}

