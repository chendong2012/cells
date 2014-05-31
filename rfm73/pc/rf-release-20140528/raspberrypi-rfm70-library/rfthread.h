#ifndef __RFTHREAD__
#define __RFTHREAD__

class user_activity;
class rfthread {
public:
	rfthread(user_activity *users[], int count);
	int init_ok();
	user_activity *m_users[10];
	int m_usercount;
};

#endif
