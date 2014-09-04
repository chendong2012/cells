#ifndef __PACKAGE_H__
#define __PACKAGE_H__
class Package {
public:
	Package(void);
	static unsigned char get_rmt_addr(unsigned char *pkg_dat);
	static unsigned char get_rmt_port(unsigned char *pkg_dat);
	static unsigned char get_local_addr(unsigned char *pkg_dat);
	static unsigned char get_local_port(unsigned char *pkg_dat);
	static unsigned char *get_pkg_datas(unsigned char index_flag, unsigned char *pkg_dat);
	static unsigned char get_addr_head_len();
	static unsigned char *get_user_datas(unsigned char index_flag,
				unsigned char *dat, 
				unsigned char keyword_len);
	static unsigned char get_user_datas_offset(unsigned char index_flag, unsigned char keyword_len);
	static unsigned char get_user_datas_len(unsigned char index_flag,
				unsigned char package_len,
				unsigned char keyword_len);
};
#endif
