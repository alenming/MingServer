#ifndef __BUFFER_DATA_TOOL_H__
#define __BUFFER_DATA_TOOL_H__
#include "BufferData.h"
#include "Head.h"

BufferData* newBufferData(int mainCMD, int subCmd)
{
	BufferData* buffer = new BufferData();
	buffer->init(256);
	buffer->writeData(0); //Head::length
	buffer->writeData(MakeCommand(mainCMD, subCmd));
	buffer->writeData(0); //Head::id
	return buffer;

	/*
	char* bu = buffer->getBuffer();
	Head* head = reinterpret_cast<Head*>(bu);
	head->length = buffer->getDataLength();
	int mainCmd = head->MainCommand();
	int subCmd = head->SubCommand();
	*/
}

int deleteBufferData(BufferData* buffer)
{
	if (NULL != buffer)
	{
		delete buffer;
	}
	return 0;
}

#endif //__BUFFER_DATA_TOOL_H__
