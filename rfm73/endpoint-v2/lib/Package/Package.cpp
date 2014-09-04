#include "Package.h"
/*下面的数据是针对本地主动发送，从远程返回来的应答包的分析情况
 *比如：
 *本地主动发送[3][90][1][81][index][keyword][userdatas]
 *本地接收回来[1][81][3][90][keyword][userdatas]
 *
 * */
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

unsigned char *Package::get_pkg_datas(unsigned char index_flag, unsigned char *pkg_dat)
{
	unsigned char offset = Package::get_addr_head_len() + index_flag;
	return &pkg_dat[offset];
}

unsigned char Package::get_addr_head_len()
{
	return 4;
}

unsigned char *Package::get_user_datas(unsigned char index_flag, unsigned char *dat, 
					unsigned char keyword_len)
{
	unsigned char offset;
	offset = Package::get_user_datas_offset(index_flag, keyword_len);
	return &dat[offset];
}

unsigned char Package::get_user_datas_offset(unsigned char index_flag, unsigned char keyword_len)
{
	unsigned char offset = Package::get_addr_head_len() + index_flag + keyword_len;
	return offset;
}

unsigned char Package::get_user_datas_len(unsigned char index_flag,
				unsigned char package_len,
				unsigned char keyword_len)
{
	unsigned char len = package_len - get_user_datas_offset(index_flag, keyword_len);
	return len;
}
