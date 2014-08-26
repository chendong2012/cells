#include <CallMe.h>
#include <ISend.h>

extern char g_buf1[32];
static void cb_sendors(unsigned char *dat, unsigned char len);

/*主动发送的命令集合*/
const char * send_cmds[] = { 
	"getsvrtm",/*获取服务器的时间*/
	"cmd4"
};

static boolean timer_func(void);
ISend sendors(send_cmds[0], cb_sendors);
CallMe cmrf(500, timer_func);

static boolean timer_func(void)
{
	sendors.send_cb(&sendors);
	return true;
}

static void cb_sendors(unsigned char *dat, unsigned char len)
{
//	sprintf(g_buf, "%s:", sendors.getItemData());
//	Serial.println(g_buf);
//	sprintf(g_buf, "%s", sendors.getAckData());
//	Serial.println(g_buf);
}
