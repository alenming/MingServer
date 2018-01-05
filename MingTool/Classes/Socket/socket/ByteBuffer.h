
#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H
#include "Util.h"
MING_BEGIN
class ByteBuffer 
{

public:

	ByteBuffer(int capacity);
	ByteBuffer(char* data,int offset,int length);
	~ByteBuffer();

public:

	char* buffer;
	int position;
	int capacity;
	int limit;

public:

	void reset();
	
	int remaining();
	
	void setIntAt(int n,int index);
	
	void put(const char* bytes,int offset,int len);
	
	void put(int n);
	void putBoolean(bool b);
	void putByte(UnChar_t b);
	void putShort(short n);
	void putFloat(float n);
	void putInt(int n);
	void putLong(long long n);
	void putUTF(const char* str);
	void putUTF(const std::string& str);
	void putArray(vector<UnChar_t>& a);
	void putArray(vector<bool>& a);
	void putArray(vector<short>& a);
	void putArray(vector<int>& a);
	void putArray(vector<long long>& a);
	void putArray(vector<string>& a);
	
	void getArray(vector<UnChar_t>& a);
	void getArray(vector<bool>& a);
	void getArray(vector<short>& a);
	void getArray(vector<int>& a);
	void getArray(vector<long long>& a);
	void getArray(vector<string>& a);
	
	void getArray(vector<UnChar_t>& a,int len);
	void getArray(vector<bool>& a,int len);
	void getArray(vector<short>& a,int len);
	void getArray(vector<int>& a,int len);
	void getArray(vector<long long>& a,int len);
	void getArray(vector<string>& a,int len);
	
	
	
	void getUTF(std::string& str);
	std::string getUTF();
	
	int getPosition();
	
	void setPosition(int p);
//	void reset();
	int getLimit();
	
	int getCapacity();
	
	char* getBuffer();
	char* toByteArray();
	
	char get();
	UnChar_t getByte();
	bool getBoolean();
	int getInt();
	float getFloat();
	short getShort();
	
	//这个方法只能用于打开的是文本文件的时候
	std::string getLine();
	
	long long getLong();
	void get(char* bytes,int size);
	
	void get(char* bytes,int offset,int len);
	/**
	 * makes a buffer ready for a new sequence of channel-read or relative put operations: It sets the limit to the capacity and the position to zero.
	 */
    void clear();
    int getLength(int offset);
    void getAsBytes(UnChar_t* bytes);
	
   // /**
//     * flip() makes a buffer ready for a new sequence of channel-write or relative get operations: It sets the limit to the current position and then sets the position to zero.
//     */
    void flip();//
    
    void compact();
    
    void rewind();
	
	static int bytesToInt(UnChar_t* data);
	static UnChar_t* intToByte(int i);
};

MING_END
#endif
