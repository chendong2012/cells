#include "public.h"
#include <IReceive.h>
#include <user_activity.h>
#include "comm.h"

extern user_activity *myuff;

static void cb_remote_control_brd(unsigned char *dat, unsigned char len);
IReceive irec_brd(BRD_REMOTE_KEYWORD, cb_remote_control_brd);


static void cb_remote_control_brd(unsigned char *dat, unsigned char len)
{
		sprintf(g_debug, "%s:", (const char *)&dat[5]);
		Serial.println((const char *)g_debug);
	if(irec_brd.isNewPackage(dat)) {
		sprintf(g_debug, "%s:", (const char *)&dat[5]);
		Serial.println((const char *)g_debug);
/*
		if(irec_brd.cmpAction(dat, len , (unsigned char *)LED_REMOTE_ON)) {
			;
		} else if (irec_led.cmpAction(dat, len , (unsigned char *)LED_REMOTE_OFF)) {
			;
		} else {
			;
		}

		myuff->m_comm->send(LED_REMOTE_ACK_OK, strlen(LED_REMOTE_ACK_OK));
		irec_led.saveAckBuf((unsigned char *)LED_REMOTE_ACK_OK, strlen(LED_REMOTE_ACK_OK));
*/

	} else {
		;
//		myu2->m_comm->send((const char *)irec_led.getAckBuf(), irec_led.getAckBufLen());	
	}
}
