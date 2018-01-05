#ifndef UrlEncode_h__
#define UrlEncode_h__

#include "../Common/CommonHead.h"


//#include <iostream>
//#include <string>
//#include <fstream>
//#include <ctype.h>
//#include <stdlib.h>
//#include <assert.h>

using namespace std;

class UrlEncode
{
public:
	static string Encode(const std::string& str);
	static string Decode(const std::string& str);
};



#endif
