/*
 * SessionServer 主要负责维护和客户端之间的连接
 * 1.客户端连接断开时推送到游戏服务器
 * 2.管理已登陆用户+连接游客，对用户合法性进行验证
 * 3.接收客户端消息转发到指定的后端
 * 4.接收到后端消息转发给指定的前端
 * 5.收到后端指令强制关闭指定的前端
 * 6.UserId 对应 CommunicationId 的哈希

 */
#include "KXServer.h"
#include "LoginServer.h"

/* 选项:
	-g	组
	-si 服务器id
	-ip 绑定的ip地址
	-p 绑定端口
	-sg 服务器类型
	-sc 服务器个数
*/

int main(int argc, char ** argv) 
{
	LoginServer* server = LoginServer::getInstance();
	if (server->initServerInfo())
	{
		server->startServer();
	}
	LoginServer::destroy();

    return 0;
}
