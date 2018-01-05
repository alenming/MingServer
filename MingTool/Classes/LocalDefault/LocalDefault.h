#ifndef __LOCAL_DEFAULT_H__
#define __LOCAL_DEFAULT_H__

#include "../FileUtils/Data.h"
#include "../XML/XML.h"



#define GetIntFromXML		LocalDefault::getInstance()->getIntegerForKeyValue
#define SetIntToXML			LocalDefault::getInstance()->setIntegerForKeyValue
#define GetStringFromXML	LocalDefault::getInstance()->getStringForKeyValue
#define SetStringToXML		LocalDefault::getInstance()->setStringForKeyValue
#define GetBoolFromXML		LocalDefault::getInstance()->getBoolForKeyValue
#define SetBoolToXML		LocalDefault::getInstance()->setBoolForKeyValue
#define GetFloatFromXML		LocalDefault::getInstance()->getFloatForKeyValue
#define SetFloatToXML		LocalDefault::getInstance()->setFloatForKeyValue
#define GetDoubleFromXML	LocalDefault::getInstance()->getDoubleForKeyValue
#define SetDoubleToXML		LocalDefault::getInstance()->setDoubleForKeyValue
#define GetDataFromXML		LocalDefault::getInstance()->getDataForKeyValue
#define SetDataToXML		LocalDefault::getInstance()->setDataForKeyValue

class  LocalDefault
{
public:
	LocalDefault();
	~LocalDefault();
	static LocalDefault* getInstance();
	static void destroyInstance();
public:
	enum ENCRYPT_TYPE  //是否加密
	{
		IS_EN,
		NO_EN
	};
public:
	bool	getBoolForKeyValue(const string nKey);											//读bool值
	int		getIntegerForKeyValue(const string nKey);										//读int值
	float	getFloatForKeyValue(const string nKey);											//读float值
	double	getDoubleForKeyValue(const string nKey);											//读float值
	string	getStringForKeyValue(const string nKey);                                        //读string值
	Data	getDataForKeyValue(const char* pKey);

	void	setBoolForKeyValue(const string nKey, bool value) ;								//写bool值
	void	setIntegerForKeyValue(const string nKey, int value) ;							//写int值
	void	setFloatForKeyValue(const string nKey, float value) ;							//写float值
	void	setDoubleForKeyValue(const string nKey, double value);							//写double值
	void	setStringForKeyValue(const string nKey, const std::string& value) ;				//写string值
	void	setDataForKeyValue(const char* pKey, const Data& value) ;

	MING_SYNTHESIZE(ENCRYPT_TYPE, _mIsEn, Encryption);
private:

	const std::string& getXMLFilePath();
	bool isXMLFileExist();
    void initXMLFilePath();
	bool init();

	std::string	EncryptData(const char* szPass) const;		//字符串加密
	std::string	CrevasseData(const char* szEncryPass) const;

	void setValueForKey(const char* pKey, const char* pValue);
	string getValueForKey(const char* pKey);

    static LocalDefault* _userDefault;
    static std::string _filePath;
    static bool _isFilePathInitialized;
	XML* _mXML;

};




#endif // __LOCAL_DEFAULT_H__
