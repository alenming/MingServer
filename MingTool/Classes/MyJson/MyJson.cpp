#include "MyJson.h"



MyJson::MyJson():
isParse(false),
mData(nullptr)
{
	mValue = Json::Value();
	mWriter = Json::FastWriter();
}
MyJson::MyJson(const char * data)
{
	isParse = false;
	mData = data;
	mReader =  Json::Reader();
	mValue  =  Json::Value();

	init();
}

MyJson::MyJson(Json::Value& value)
{
	mValue = value;
}

MyJson::~MyJson()
{
	isParse = false;
	mData = nullptr;
	mValue.clear();
}

bool MyJson::init()
{
	if (!isParse)
	{
		isParse = mReader.parse(mData, mValue);
	}
	return isParse;
}

int MyJson::asInt(string key)
{
	return mValue[key].asInt();
}

float MyJson::asFloat(string key)
{
	return (float)mValue[key].asDouble();
}

double MyJson::asDouble(string key)
{
	return mValue[key].asDouble();
}

bool MyJson::asBool(string key)
{
	return mValue[key].asBool();
}

std::string MyJson::asString(string key)
{
	return mValue[key].asString();
}

MyJson MyJson::asValue(string key)
{
	return  MyJson(mValue.asValue(key.c_str()));
}

//取数组下标
MyJson MyJson::asValue(int key)
{
	return MyJson(mValue.asValue(key));
}

void MyJson::writeInt(string key, int value)
{
	mValue[key] = value;
}

void MyJson::writeDouble(string key, double value)
{
	mValue[key] = value;
}

void MyJson::writeBool(string key, bool value)
{
	mValue[key] = value;
}

void MyJson::writeFloat(string key, float value)
{
	mValue[key] = value;
}

void MyJson::writeString(string key, string value)
{
	mValue[key] = value;
}

void MyJson::writeValue(string key, MyJson& value)
{
	mValue[key] = value.mValue;
}

//为循环插入数组使用
void MyJson::writeValueFor(string key, MyJson& value)
{
	mValue[key].append(value.mValue);
}

string MyJson::getString()
{
	return mWriter.write(mValue);
}

bool MyJson::removeMember(string key)
{
	return mValue.removeMember(key) != Json::Value::null;
}




