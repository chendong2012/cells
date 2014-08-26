#ifndef __PUBLIC_H__
#define __PUBLIC_H__
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//#define DEBUG

/*comm dir*/
#define I_AM_CLIENT	0
#define I_AM_SERVER	1


/*src dir*/
#define TICK_LED 6



#define LED_REMOTE_CONTROL
#define FAN_REMOTE_CONTROL

/*================LED_REMOTE_CONTROL====================*/
#ifdef LED_REMOTE_CONTROL
#define LED_REMOTE_KEYWORD "led"
#define LED_REMOTE_ON "on"
#define LED_REMOTE_OFF "off"
#define LED_SWITCH_GPIO 7

#define LED_REMOTE_ACK_OK "led:ok"
#define LED_REMOTE_ACK_FAIL "led:fail"
#endif

/*================FAN_REMOTE_CONTROL====================*/
#ifdef FAN_REMOTE_CONTROL
#define FAN_REMOTE_KEYWORD "fan"
#define FAN_REMOTE_SPEED "speed"
#define FAN_REMOTE_OFF "off"

#define FAN_REMOTE_ACK_SPEED_OK "fanspeed:ok"
#define FAN_REMOTE_ACK_SPEED_FAIL "fanspeed:fail"

#define FAN_REMOTE_ACK_OFF_OK "fanoff:ok"
#define FAN_REMOTE_ACK_OFF_FAIL "fanoff:fail"

#define FAN_SPEED_KEY 5
#define FAN_STOP_KEY 6
#endif






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
/*sendors*/
#define GET_SERVER_TIME		0





#endif





























