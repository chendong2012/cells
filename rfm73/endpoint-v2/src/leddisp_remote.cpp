#include "public.h"
#include <IReceive.h>
#include <user_activity.h>
#include "comm.h"
#include <DelayRun.h>
#include <Task.h>
#include "public.h"
extern user_activity *myu2;

/*
 *命令：
 *１、<ra rp la lp><index><f><b><fg><bg><addr><count>
 *	f：指的是关键字
 *	b:指的是类型：开始，帧的起始码
 *	fg:表示前景色
 *	bg:表示背景色
 *	addr:地址
 *	count:数据总长度,表示多少个8x16
 * */

/*
 *命令：
 *2、<ra rp la lp><index><f><d><contents>
 *	f：指的是关键字
 *	m:指的是类型：数据，帧内容
 *	contents:数据16 固定字节
 * */

/*
 *命令：
 *3、<ra rp la lp><index><f><e>
 *	f：指的是关键字
 *	e:指的是类型：帧结束
 * */
#define LEDDISP_FONT_KEYWORD	"f"
#define LEDDISP_FONT_BEGIN "b"
#define LEDDISP_FONT_BEGIN_OK	"b:ok"

#define LEDDISP_FONT_DATAS "d"
#define LEDDISP_FONT_DATAS_OK	"d:ok"

#define LEDDISP_FONT_END "e"
#define LEDDISP_FONT_END_OK	"e:ok"

static void cb_leddisp(unsigned char *dat, unsigned char len);
IReceive irec_leddisp(LEDDISP_FONT_KEYWORD, cb_leddisp);

static void cb_leddisp(unsigned char *dat, unsigned char len)
{
	unsigned char l;
	unsigned char *ack_data;
	if(irec_leddisp.isNewPackage(dat)) {
		/*接收到开始*/
		if(irec_leddisp.is_contain_userdata(dat, len, (unsigned char *)LEDDISP_FONT_BEGIN)) {

			l = strlen(LEDDISP_FONT_BEGIN_OK);
			irec_leddisp.saveUserBufToAckBuf((unsigned char *)LEDDISP_FONT_BEGIN_OK, l);

			l = irec_leddisp.getAckBufLen();
			ack_data = irec_leddisp.getAckBuf();
			myu2->m_comm->send((const char *)ack_data, l);
			//myu2->m_comm->send("fanspeed:ok", 11);

			dispinfo((const char *)Package::get_pkg_datas(INDEX_EXIST, dat));

		/*接收到数据*/
		} else if (irec_leddisp.is_contain_userdata(dat, len , (unsigned char *)LEDDISP_FONT_DATAS)) {

			l = strlen(LEDDISP_FONT_DATAS_OK);
			irec_leddisp.saveUserBufToAckBuf((unsigned char *)LEDDISP_FONT_DATAS_OK, l);

			l =irec_leddisp.getAckBufLen();
			ack_data = irec_leddisp.getAckBuf();
			myu2->m_comm->send((const char *)ack_data, l);
			//myu2->m_comm->send("fanoff:ok", 9);

			dispinfo((const char *)Package::get_pkg_datas(INDEX_EXIST, dat));

		/*接收到结束*/
		} else if (irec_leddisp.is_contain_userdata(dat, len , (unsigned char *)LEDDISP_FONT_END)) {

			l = strlen(LEDDISP_FONT_END_OK);
			irec_leddisp.saveUserBufToAckBuf((unsigned char *)LEDDISP_FONT_END_OK, l);

			l =irec_leddisp.getAckBufLen();
			ack_data =irec_leddisp.getAckBuf();
			myu2->m_comm->send((const char *)ack_data, l);
			//myu2->m_comm->send("fanoff:ok", 9);

			dispinfo((const char *)Package::get_pkg_datas(INDEX_EXIST, dat));

		} else {
				
		}
	} else {
		l = irec_leddisp.getAckBufLen();
		ack_data = irec_leddisp.getAckBuf();
		//myu2->m_comm->send("fanspeed:ok", 11);
		myu2->m_comm->send((const char *)ack_data, l);	
	}
}
