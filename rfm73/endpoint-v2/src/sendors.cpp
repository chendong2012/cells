#include <CallMe.h>
#include <ISend.h>
#include <public.h>


static void cb_sendors(unsigned char *dat, unsigned char len);

static boolean timer_func(void);
ISend sendors(CMD_GET_SERVER_TIME, cb_sendors);
CallMe cmrf(500, timer_func);

static boolean timer_func(void)
{
	sendors.send_cb(&sendors);
	return true;
}

static void cb_sendors(unsigned char *dat, unsigned char len)
{
	sprintf(g_debug, "%s", Package::get_pkg_datas(dat));
	Serial.println(g_debug);
}
