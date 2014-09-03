#include "Package.h"
Package::Package(void)
{
	pkg_dat = NULL;
}

static unsigned char Package::attach_pkg_ptr(unsigned char *ptr)
{
	pkg_dat = ptr;
}

static unsigned char Package::get_rmt_addr(void)
{
	return pkg_dat[0];
}

static unsigned char Package::get_rmt_port(void)
{
	return pkg_dat[1];
}

static unsigned char Package::get_local_addr(void)
{
	return pkg_dat[2];
}

static unsigned char Package::get_local_port(void)
{
	return pkg_dat[3];
}

static unsigned char *Package::get_pkg_datas(void)
{
	return &pkg_dat[4];
}

static unsigned char Package::get_addr_head_len(void)
{
	return 4;
}

static unsigned char *Package::get_user_datas(unsigned char *dat, unsigned char keyword_len)
{
	unsigned char len = Package::get_addr_head_len()+keyword_len;
	return dat[len];
}



Package g_pkg;
