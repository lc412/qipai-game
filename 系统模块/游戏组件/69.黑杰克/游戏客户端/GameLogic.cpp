#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const BYTE	CGameLogic::m_bCardArray[52] =									//扑克数组
{
	 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
	14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
};

//获取扑克数值
BYTE CGameLogic::GetCardValue(BYTE bCard)
{
	BYTE bValue = bCard%13;
	if (bValue == 0 || bValue == 11 || bValue == 12) bValue=10;
	if (bValue == 1) bValue = 11;
	return bValue;
}

//获取扑克总值
BYTE CGameLogic::GetCardSum(const BYTE bCardBuffer[], const BYTE bCardCount)
{
	BYTE cbCardSum = 0;
	
	for(int i = 0; i < bCardCount; i++)
	{
		cbCardSum += GetCardValue(bCardBuffer[i]);
	}
	
	if(cbCardSum > 21)
	{
		for(int i = 0; i < bCardCount; i++)
		{
			//是否有A
			if(GetCardValue(bCardBuffer[i]) == 11)
			{
				//按1计算
				cbCardSum -= 10;
			}
		}
	}

	return cbCardSum;
}

//是否爆牌
bool CGameLogic::IsBurst(const BYTE bCardBuffer[], BYTE bCardCount)
{
	WORD wCardSum = 0;
	
	for(int i = 0; i < bCardCount; i++)
	{
		wCardSum += GetCardValue(bCardBuffer[i]);
	}

	if(wCardSum > 21)
	{
		for(int i = 0; i < bCardCount; i++)
		{
			//是否有A
			if(GetCardValue(bCardBuffer[i]) == 11)
			{
				//按1计算
				wCardSum -= 10;
			}
		}
	}

	if(wCardSum > 21) return true;
	
	return false;
}

//能否分牌
bool CGameLogic::CanCutCard(const BYTE bFirstCard, const BYTE bLastCard)
{
	BYTE bFirstValue = bFirstCard % 13;
	BYTE bLastValue  = bLastCard  % 13;
	if (bFirstValue == bLastValue) return true;
	return false;
}

//是否BlackJack
bool CGameLogic::IsBlackJack(const BYTE bCardBuffer[])
{
	BYTE bCardSum=0;
	if((GetCardValue(bCardBuffer[0]) == 1) && (GetCardValue(bCardBuffer[1]) == 10)) return true;
	if((GetCardValue(bCardBuffer[1]) == 1) && (GetCardValue(bCardBuffer[0]) == 10)) return true;
	return false;
}

//对比单只扑克
bool CGameLogic::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstValue = GetCardValue(bFirstCard);
	BYTE bLastValue  = GetCardValue(bLastCard);
	if (bFirstValue == bLastValue) return bFirstValue > bLastValue;
	return bFirstValue > bLastValue;
}

//推断胜者
BYTE CGameLogic::DeduceWiner(const BYTE bBankerCard[],const BYTE bBankerCardCount,
							 const BYTE bIdleCard[],  const BYTE bIdleCardCount)
{
	//平
	if(IsBlackJack(bBankerCard) && IsBlackJack(bIdleCard)) return 0;

	//庄家赢
	if(IsBlackJack(bBankerCard) && !IsBlackJack(bIdleCard)) return 1;

	//闲家赢
	if(!IsBlackJack(bBankerCard) && IsBlackJack(bIdleCard)) return 2;

	//比较大小
	BYTE bBankerSum = 0;
	BYTE bIdleSum   = 0;
	
	for(int i=0;i<bBankerCardCount;i++)
	{
		bBankerSum += GetCardValue(bBankerCard[i]);
	}

	for(int i=0;i<bIdleCardCount;i++)
	{
		bIdleSum += GetCardValue(bIdleCard[i]);
	}

	if(bBankerSum > bIdleSum) 
		return 1;
	else if(bBankerSum < bIdleSum)
		return 2;
	else
		return 0;
}

//混乱扑克
void CGameLogic::RandCard(BYTE bCardBuffer[], BYTE bBufferCount)
{
	BYTE bSend = 0,bStation = 0, bCardList[CountArray(m_bCardArray)];

	CopyMemory(bCardList, m_bCardArray, sizeof(m_bCardArray));
	
	static long int dwRandCount = 0L;
	
	srand((unsigned)time(NULL) + dwRandCount++);
	
	do
	{
		bStation = rand() % (CountArray(m_bCardArray) - bSend);

		bCardBuffer[bSend++] = bCardList[bStation];
		bCardList[bStation]  = bCardList[CountArray(m_bCardArray)-bSend];
	
	} while (bSend < bBufferCount);	
}

//排列扑克
void CGameLogic::SortCard(BYTE bCardList[], BYTE bCardCount)
{
	bool bSorted     = true;
	BYTE bTemp,bLast = bCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < bLast; i++) 
		{
			if (CompareOnlyOne(bCardList[i],bCardList[i+1]) == FALSE)
			{	
				bTemp = bCardList[i];
				bCardList[i]   = bCardList[i+1];
				bCardList[i+1] = bTemp;
				bSorted = false;
			}	
		}
		bLast--;
	} while (bSorted == false);

	return;
}
//////////////////////////////////////////////////////////////////////////