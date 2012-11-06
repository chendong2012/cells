#ifndef __INTERFACE__
#define __INTERFACE__
int uway_init_device(void);
void uway_close_device(void);
int uway_open_device(int product);
int uway_reset(int devNo);
int yway_issue_control_command(unsigned char *cmd, int len);
int uway_recv_data(unsigned char *data, int len);
int uway_send_data(unsigned char *data,int len);
int uway_get_command_package(unsigned char *cmd, int cmd_len,
			unsigned char *data, int data_len);
int uway_put_command_package(unsigned char *cmd, int cmd_len,
			unsigned char *data, int data_len);
#endif
