/*
* 通讯协议 主要定义主命令和包头
*/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum MAIN_CMD
{
	CMD_LOGIN_SERVER = 1,      // 登录
};

enum LOGIN_CMD
{
	CMD_CS_BEIGN = 0,
	CMD_LOGIN_CS,

	CMD_CS_END,

	CMD_SC_BGINE = 100,
	CMD_LOGIN_SC,
	CMD_SC_END,
};

struct LOGIN_DATA
{
	int accountId;
	int passwd;
};

inline int MakeCommand(int main, int sub)
{
    return (main << 16) | (sub & 0x0000ffff);
}

struct Head
{
    int length;
    int cmd;
	int uid; //服务器用

    inline void MakeCommand(int main, int sub)
    {
        cmd = (main << 16) | (sub & 0x0000ffff);
    }

    inline int SubCommand()
    {
        return cmd & 0x0000ffff;
    }

    inline int MainCommand()
    {
        return cmd >> 16;
    }

    inline void* data()
    {
        return this + 1;
    }
};

#endif
