#include "Package.h"
Package::Package(void)
{
}

unsigned char Package::get_rmt_addr(unsigned char *pkg_dat)
{
	return pkg_dat[0];
}

unsigned char Package::get_rmt_port(unsigned char *pkg_dat)
{
	return pkg_dat[1];
}

unsigned char Package::get_local_addr(unsigned char *pkg_dat)
{
	return pkg_dat[2];
}

unsigned char Package::get_local_port(unsigned char *pkg_dat)
{
	return pkg_dat[3];
}

unsigned char *Package::get_pkg_datas(unsigned char *pkg_dat)
{
	return &pkg_dat[4];
}

unsigned char Package::get_addr_head_len()
{
	return 4;
}

unsigned char *Package::get_user_datas(unsigned char *dat, 
					unsigned char keyword_len)
{
	unsigned char len = Package::get_addr_head_len() + keyword_len;
	return &dat[len];
}

unsigned char Package::get_user_datas_len(unsigned char *dat, 
				unsigned char package_len,
				unsigned char keyword_len)
{
	package_len = package_len - Package::get_addr_head_len() - keyword_len;
	return package_len;
}
