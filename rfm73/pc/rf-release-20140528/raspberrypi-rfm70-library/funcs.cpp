#if 0
/********************建一个定时器**************************/
static void t_cb(union sigval si)
{       
#if 0   
        user_activity *this_act;
        rftimer *ptr = (rftimer *)si.sival_ptr;
        
        this_act=ptr->m_activity;
        printf("timer is over %d\n",this_act->test++);
        
#endif  
}

void user_activity::ack_timeout(void)
{       
}               
        
void user_activity::setup_timer(void)
{       
        m_rftimer = new rftimer(this);
        m_rftimer->init(t_cb);
        m_rftimer->begin((float)1, (float)(10));
}
#endif  

