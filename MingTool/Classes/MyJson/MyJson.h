#ifndef _MY_JSON_H__
#define _MY_JSON_H__

#include "../Common/CommonHead.h"
#include "json/json.h"
/*
1:在jsoncpp库的基础上封装了一层自己习惯的api


*/



class MyJson
{

public:
	MyJson();
	MyJson(Json::Value&);
	MyJson(const char * data);
	virtual ~MyJson();

public:
	int    asInt(string key);
	float  asFloat(string key);
	double asDouble(string key);
	bool   asBool(string key);
	string asString(string key);
	MyJson asValue(string key);
	MyJson asValue(int key);

	void writeInt(string key, int value);
	void writeDouble(string key, double value);
	void writeBool(string key, bool value);
	void writeFloat(string key, float value);
	void writeString(string key, string value);
	void writeValue(string key, MyJson& value);
	void writeValueFor(string key, MyJson& value);

	bool removeMember(string key);

	string getString();

	size_t getSize()
	{
		return mValue.size();
	}

	MyJson at(size_t size)
	{
		if ((mValue.size()>0) && (size < mValue.size()))
		{
			return MyJson(mValue.asValue(size));
		}
		return nullptr;
	}
	
	void operator=(MyJson& my)
	{
		this->mValue = my.getValue();
	}

	MING_SYNTHESIZE_READONLY(Json::Value, mValue, Value);
private:
	bool init();

	Json::Reader mReader;
	Json::FastWriter mWriter;
	//Json::Value  mValue;
	const char * mData;
	bool isParse;
};



#endif // _MY_JSON_H__
