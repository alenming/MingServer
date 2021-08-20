#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "cintool.h"
#include "GameLogic.h"
int main()
{
//	start();
	CGameLogic* ga = new CGameLogic();
	
	int ca[13] = { 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};
	bool res = ga->CheckCanHu(ca, 5, 0);
	cout << res << endl;
	delete ga;
	int m = 4, n = 5, k;
	m = (n = 5);
	return 0;
}
