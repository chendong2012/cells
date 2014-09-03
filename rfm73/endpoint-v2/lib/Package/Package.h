#ifndef __PACKAGE_H__
#define __PACKAGE_H__
class Package {
public:
	Package(void);
	static unsigned char attach_pkg_ptr(unsigned char *ptr);
	static unsigned char get_rmt_addr(void);
	static unsigned char get_rmt_port(void);
	static unsigned char get_local_addr(void);
	static unsigned char get_local_port(void);
	static unsigned char *get_pkg_datas(void);
};
#endif
