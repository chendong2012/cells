#include "public.h"
#include <IReceive.h>
#include <user_activity.h>
#include "comm.h"

extern user_activity *myu2;

static void cb_remote_control_led(unsigned char *dat, unsigned char len);
IReceive irec_led(LED_REMOTE_KEYWORD, cb_remote_control_led);


static void led_off(void)
{
	digitalWrite(LED_SWITCH_GPIO, HIGH);
}

static void led_on(void)
{
	digitalWrite(LED_SWITCH_GPIO, LOW);
}
/*
 *接收到从远程过来的ＬＥＤ命令，然后根据变量判断，是开灯还是关灯，
  并执行相关动作。
**/
static void cb_remote_control_led(unsigned char *dat, unsigned char len)
{
	if(irec_led.isNewPackage(dat)) {
		if(irec_led.cmpAction(dat, len , (unsigned char *)LED_REMOTE_ON)) {
			led_on();

		} else if (irec_led.cmpAction(dat, len , (unsigned char *)LED_REMOTE_OFF)) {
			led_off();

		} else {
		}

		myu2->m_comm->send(LED_REMOTE_ACK_OK, strlen(LED_REMOTE_ACK_OK));
		irec_led.saveAckBuf((unsigned char *)LED_REMOTE_ACK_OK, strlen(LED_REMOTE_ACK_OK));

	} else {
		myu2->m_comm->send((const char *)irec_led.getAckBuf(), irec_led.getAckBufLen());	
	}
}
