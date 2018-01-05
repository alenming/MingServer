#ifndef __SESSION_MODULE_H__
#define __SESSION_MODULE_H__
/************************************************************************/
/*                  这是个与SessionServer的连接模型                     */
/************************************************************************/

#include "BaseModule.h"

class LoginSessionModule : public BaseModule
{
public:
    LoginSessionModule(void);
    virtual ~LoginSessionModule(void);

    //处理客户端发来消息
    virtual void processLogic(char* buffer, unsigned int len, IKxComm *target);
    //客户端与Session服务器断开，通知游戏服务器，并清理相关资源
    virtual void processError(IKxComm *target);

private:
    //玩家断线
    void userDisconnect(IKxComm *target);
};

#endif
