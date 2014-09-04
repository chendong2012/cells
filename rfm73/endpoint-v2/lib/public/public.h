#ifndef __PUBLIC_H__
#define __PUBLIC_H__
#include "Package.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define PACKAGE_LEN	32
#define PACKAGE_LEN_INDEX	31

#define INDEX_EXIST	1
#define INDEX_NONE	0

//#define DEBUG

extern char g_debug[PACKAGE_LEN];
/*comm dir*/
#define I_AM_CLIENT	0
#define I_AM_SERVER	1


/*src dir*/
#define TICK_LED 6


/*===============================*/
#define LOCAL_ADDR      3
#define LOCAL_PORT      90
#define REMOTE_ADDR     1 
#define REMOTE_PORT     81

#define BRD_LOCAL_ADDR 3
#define BRD_LOCAL_PORT 15
#define BRD_REMOTE_ADDR 1
#define BRD_REMOTE_PORT 15



/*===============================*/


#define LED_REMOTE_CONTROL
#define FAN_REMOTE_CONTROL

/*本地被动接收部分*/
/*================LED_REMOTE_CONTROL====================*/
#ifdef LED_REMOTE_CONTROL
#define LED_REMOTE_KEYWORD "led"
#define LED_REMOTE_ON "on"
#define LED_REMOTE_OFF "off"

#define LED_REMOTE_ACK_OK "ok"
#define LED_REMOTE_ACK_FAIL "fail"


#define LED_SWITCH_GPIO 7
#endif
/*******************************************************/

/*================FAN_REMOTE_CONTROL====================*/
#ifdef FAN_REMOTE_CONTROL
#define FAN_REMOTE_KEYWORD "fan"
#define FAN_REMOTE_SPEED "speed"
#define FAN_REMOTE_OFF "off"

#define FAN_REMOTE_ACK_SPEED_OK "speed:ok"
#define FAN_REMOTE_ACK_SPEED_FAIL "speed:fail"

#define FAN_REMOTE_ACK_OFF_OK "off:ok"
#define FAN_REMOTE_ACK_OFF_FAIL "off:fail"

#define FAN_SPEED_KEY 5
#define FAN_STOP_KEY 6
#endif
/*******************************************************/

/*================BROADCAST_REMOTE_CONTROL====================*/
#define BRD_REMOTE_CONTROL
#ifdef BRD_REMOTE_CONTROL
#define BRD_REMOTE_KEYWORD "brd"
#define BRD_REMOTE_ON "on"
#define BRD_REMOTE_OFF "off"

#define BRD_REMOTE_ACK_OK "brd:ok"
#define BRD_REMOTE_ACK_FAIL "brd:fail"
#endif
/*******************************************************/




/********************键盘功能**************************/
#define GPIO3_KEY 3
#define  GPIO3_KEY_IRQ 1

/*对于中断管脚配置注意事项
 *１、要配成输入带上拉功能
 *第3 gpio端口对应第１的中断口
 *处理方法：
 １、检测到中断
 ２、进入中断函数，启动一个按键线程，关中断
 ３、再过０。５秒，是否还是低电平，如果是的话，
 ４、认为有键按下。触发动作。这时不开中断
 ５、每等０。５秒，检查是否为高电平，如是，表示用户已松开键，开中断
 * */

/*本地主动发送命令部分*/
/*====================sendors==========================*/
#define CMD_GET_SERVER_TIME		"time"




extern void dispinfo(const char *dat);
#endif


