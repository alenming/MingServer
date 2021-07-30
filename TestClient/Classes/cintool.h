#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "log/LogManager.h"
#include "log/LogFileHandler.h"
using namespace std;
std::string getStringFromFile(const std::string &path)
{
	//FILE *fp = fopen(path.c_str(), "rb");
	FILE *fp = fopen(path.c_str(), "rt");
	if (NULL == fp)
	{
		return "";
	}

	char* buffer = NULL;
	int size = 0;
	int readsize = 0;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp) + 1;
	fseek(fp, 0, SEEK_SET);

    buffer = (char*)malloc(size);
    memset(buffer, 0, size);

	readsize = fread(buffer, sizeof(unsigned char), size, fp);
	fclose(fp);

	std::string ret(buffer, readsize);
    free(buffer);
	return ret;
}
static int m_nDocStep = 0;
static int m_nDocOffset = 0;
static int m_nLineOffset = 0;


std::string NextLine(std::string m_Document)
{
	std::string m_CurLine = "";
	// 换行这里需要考虑\r\n的问题
	m_nDocStep = m_Document.find_first_of('\n', m_nDocOffset);
	if (std::string::npos == m_nDocStep)
	{
		// 这里还需要考虑最后一行找不到\n的情况
        if (m_nDocOffset < m_Document.size())
        {
            // 将最后剩余的一行内容取出
            m_CurLine = m_Document.substr(m_nDocOffset, m_Document.size() - m_nDocOffset);
            m_nDocOffset = m_Document.size();
            return m_CurLine;
 		}
		return "";
	}

	if (m_Document.at(m_nDocStep - 1) == '\r')
	{
		m_CurLine = m_Document.substr(m_nDocOffset, m_nDocStep - m_nDocOffset - 1);
	}
	else
	{
		m_CurLine = m_Document.substr(m_nDocOffset, m_nDocStep - m_nDocOffset);
	}
	m_nLineOffset = 0;
	m_nDocOffset = m_nDocStep + 1;

	return m_CurLine;
}
bool SplitStrToVector(const std::string &str, char sep, std::vector<std::string>& out)
{
	int pos = 0;
	int step = 0;

	while (static_cast<unsigned int>(pos) < str.length() && step != -1)
	{
		step = str.find_first_of(sep, pos);
		std::string seg = str.substr(pos, step-pos);
		out.push_back(seg);
		pos = step + 1;
	}

	return out.size() > 0;
}


void cal(std::string Document)
{
	std::string data;

	int DocStep = Document.find_first_of('\t', 0);

	std::string head = Document.substr(0, DocStep);
	if (head.length() != 6)
		return;
	std::string CurLine = Document.substr(DocStep+1, Document.size());

	data = head + "\t"+ CurLine;
	KX_LOGDEBUG(data.c_str());
//	std::vector<std::string> out;
//	out.clear();
//	if (SplitStrToVector(CurLine, '\t', out))
//	{
//		for (auto i = out.begin(); i != out.end(); ++i)
//		{
//			if ((*i).length() == 6)
//			{
//				std::string data = *i + "\t"+head;
//				KX_LOGDEBUG(data.c_str());
//			}
//		}
//	}
}
//合并同类项
void cal1(std::string Document, std::map<string,string>& mapData)
{
	std::string data;

	int DocStep = Document.find_first_of(' ', 0);

	std::string head = Document.substr(0, DocStep);
	std::string CurLine = Document.substr(DocStep+1, Document.size());
	if (mapData.find(data) != mapData.end())
		mapData[head] += " " + CurLine;
	else
		mapData[data] = CurLine;
}


int start()
{
	LogFileHandler* log = new LogFileHandler();
	LogManager::getInstance()->addHandler(log);

	std::string data = getStringFromFile("F:\\myCode\\MingServer\\Resources\\aa.txt");
	string str = NextLine(data);
	std::map<string, string> out;
	out.clear();
	do 
	{
//		cal1(str,out);
		cal(str);
		str = NextLine(data);
	} while (str != "");
	for (auto i = out.begin(); i != out.end(); ++i)
	{
		std::string data = i->first + i->second;
		KX_LOGDEBUG(data.c_str());
	}
	return 0;
}