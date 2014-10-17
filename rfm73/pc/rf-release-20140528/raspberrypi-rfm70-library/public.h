#ifndef __PUBLIC_H__
#define __PUBLIC_H__

/*================TIME_REMOTE_CONTROL====================*/
#define TIME_REMOTE_CONTROL
#ifdef TIME_REMOTE_CONTROL

#define TIME_REMOTE_KEYWORD "time"

#endif 

#define BRD_SEND_CONTROL
#ifdef BRD_SEND_CONTROL

#define BRD_SEND_KEYWORD "brd"

#endif 



#ifdef FAN_REMOTE_CONTROL
#define FAN_REMOTE_KEYWORD "fan"
#define FAN_REMOTE_SPEED "speed"
#define FAN_REMOTE_OFF "off"
#endif

#define LEDDISP_FONT
#ifdef LEDDISP_FONT
#define LEDDISP_FONT_KEYWORD    "f"
#define LEDDISP_FONT_BEGIN "b"
#define LEDDISP_FONT_BEGIN_OK   "b:ok"

#define LEDDISP_FONT_DATAS "d"
#define LEDDISP_FONT_DATAS_OK   "d:ok"

#define LEDDISP_FONT_END "e"
#define LEDDISP_FONT_END_OK     "e:ok"
#endif
/*******************************************************/
#endif
