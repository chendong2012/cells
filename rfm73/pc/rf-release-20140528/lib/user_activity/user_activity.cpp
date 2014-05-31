#include <stdio.h>
#include <stdlib.h>
#include "user_activity.h"
#include "comm.h"

/*作为服务器端
如有消息到达，
1、广播给系统
2、把消息传出去

*/
user_activity::user_activity()
{
}

int user_activity::init_ok()
{
	return 0;
}

void user_activity::receive_listener(unsigned char *data, unsigned char len)
{
}

void user_activity::set_comm(COMM *comm)
{
	m_comm=comm;
}
