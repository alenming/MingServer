#ifndef __SUPPORT_BASE64_H__
#define __SUPPORT_BASE64_H__
#include "../Common/CommonHead.h"
/*
	base64º”√‹¬Î∑Ω∑®
*/



int base64Decode(const unsigned char *in, unsigned int inLength, unsigned char **out);
    
int base64Encode(const unsigned char *in, unsigned int inLength, char **out);


   

#endif // __SUPPORT_BASE64_H__
