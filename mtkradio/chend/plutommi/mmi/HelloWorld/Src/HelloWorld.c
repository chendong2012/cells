#include "stdC.h"
#include "MMI_Features.h"	/* 编译开关会出现在这个由make update生成的文件里面 */
#include "L4Dr.h"
#include "L4Dr1.h"

#include "AllAppGprot.h"
#include "FrameworkStruct.h"
#include "GlobalConstants.h"
#include "EventsGprot.h"
#include "mmiappfnptrs.h"
#include "HistoryGprot.h"

#include "HelloWorldProt.h"
#include "HelloWorldTypes.h"
#include "HelloWorldDefs.h"

#include "MainMenuDef.h"

#include "wgui_categories.h"

#include "Unicodexdcl.h"

/*模块入口*/
void mmi_HelloWorld_entry(void)
{
#ifdef	__MMI_HELLOWORLD_ENABLED__
	/* 强制退出当前屏幕，之后进入到我们的模块了 */
	/* 上电缺省是idle屏幕，现进入MAIN_MENU_SCREENID屏 */
	/* 注意看第二个参数，这个是当我们模块被强制退出时执行的一些操作 */
	EntryNewScreen(MAIN_MENU_SCREENID, mmi_HelloWorld_exit, NULL, NULL);

	/* 关掉屏幕顶部的状态条，我们要用整个屏幕 */
	entry_full_screen();

	/* 擦除当前背景 */
	clear_screen();

	/* 移动文本输出光标 */
	gui_move_text_cursor(50, 100);

	/* 设置字体颜色 */
	gui_set_text_color(UI_COLOR_RED);

	/* 输出文本到显示缓冲, 注意是Unicode编码 */
	gui_print_text(L"Hello, World");

	/* 刷新屏幕显示，MMI用的是双缓冲绘图方式，而且需要显式刷新 */
	gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

	/* 注册一个按键处理，右软键弹起时返回到之前被我们强制退出的模块 */
	SetKeyHandler(GoBackHistory, KEY_RSK, KEY_EVENT_UP);
#endif
}

/* 模块出口 
 * 当我们的模块被其他模块强制退出时会执行这个函数，
 * 这个函数的常见写法，包括：
 *   1、模块已申请资源的释放(如果需要的话)，这一步可选
 *   2、手动把自己压栈到窗口(实际是整个屏)堆栈里面，
 *      便于强制我们退出的模块执行完后重新把我们叫出来
 *      不像Window的窗口管理是自动压栈的，Pluto MMI需要手动压栈
 *   3、其他一些清理动作
 */
void mmi_HelloWorld_exit(void)
{
#ifdef	__MMI_HELLOWORLD_ENABLED__
	history currHistory;
	S16 nHistory = 0;

	currHistory.scrnID = MAIN_MENU_SCREENID;
	currHistory.entryFuncPtr = mmi_HelloWorld_entry;
	pfnUnicodeStrcpy( (S8*)currHistory.inputBuffer, (S8*)&nHistory);

	AddHistory(currHistory);
#endif
}
