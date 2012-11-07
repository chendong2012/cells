static unsigned char cmd_read_romid[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xbb};
static unsigned char cmd_read_mac[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xbb};
static unsigned char cmd_read_eeprom[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xbb};

static unsigned char cmd_write_input_data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xbd};
static unsigned char cmd_write_s_secrect[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xbd};
static unsigned char cmd_write_e_secrect1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xbd};
static unsigned char cmd_write_e_secrect2[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xbd};
static unsigned char cmd_write_e_secrect3[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x05,0xbd};
static unsigned char cmd_write_compute[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x06,0xbd};
static unsigned char cmd_write_trans[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xbd};
static unsigned char cmd_write_check_mac[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xbd};
static unsigned char cmd_write_eeprom[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xbd};

#define FLAG_OK 1
Secrect::Secrect()
{
}

Secrect::~Secrect()
{
}

/*
 *返回实际读回来的字节数,错误返回-1
 * */
int Secrect::read_rom_id(unsigned char *romid, int romid_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);

	rc = uway_get_command_package(cmd_read_romid, sizeof(cmd_read_romid),
			buf, 64);

	if (rc >= 0 && buf[1] == FLAG_OK )  {
		memcpy(romid, &buf[2], romid_len);
		return buf[0];
	}
	return -1;
}

int Secrect::read_mac_code(unsigned char *mac, int mac_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);

	rc = uway_get_command_package(cmd_read_mac, sizeof(cmd_read_mac),
			buf, 64);

	if (rc >= 0 && buf[1] == FLAG_OK)  {
		memcpy(mac, &buf[2], mac_len);
		return buf[0];
	}
	return -1;
}

int Secrect::read_eeprom(int addr, unsigned char *data, int data_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);

	cmd_read_eeprom[5] = addr;
	cmd_read_eeprom[4] = data_len;
	rc = uway_get_command_package(cmd_read_eeprom, sizeof(cmd_read_eeprom),
			buf, 64);

	if (rc >= 0 && buf[1] == FLAG_OK)  {
		memcpy(data, &buf[2], data_len);
		return buf[0];
	}
	return -1;
}

int Secrect::write_input_data(unsigned char *data, int data_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	buf[0] = data_len;
	memcpy(&buf[1], data, data_len);

	rc = uway_put_command_package(cmd_write_input_data, sizeof(cmd_write_input_data),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_s_secrect(unsigned char *secrect, int secrect_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, secrect, secrect_len);

	rc = uway_put_command_package(cmd_write_s_secrect, sizeof(cmd_write_s_secrect),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_e_secrect1(unsigned char *secrect, int secrect_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, secrect, secrect_len);

	return uway_put_command_package(cmd_write_e_secrect1, sizeof(cmd_write_e_secrect1),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_e_secrect2(unsigned char *secrect, int secrect_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, secrect, secrect_len);

	rc = uway_put_command_package(cmd_write_e_secrect2, sizeof(cmd_write_e_secrect2),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_e_secrect3(unsigned char *secrect, int secrect_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, secrect, secrect_len);

	rc = uway_put_command_package(cmd_write_e_secrect3, sizeof(cmd_write_e_secrect3),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_compute(unsigned char *compute_cmd, int compute_cmd_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, compute_cmd, compute_cmd_len);

	rc = uway_put_command_package(cmd_write_compute, sizeof(cmd_write_compute),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_trans(unsigned char *trans_cmd, int trans_cmd_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, trans_cmd, trans_cmd_len);

	rc = uway_put_command_package(cmd_write_trans, sizeof(cmd_write_trans),
			trans_cmd, trans_cmd_len);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;

}

int Secrect::check_mac_device(unsigned char *data, int data_len)
{
	int rc;
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(buf, data, data_len);

	rc = uway_put_command_package(cmd_write_check_mac, sizeof(cmd_write_check_mac),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}

int Secrect::write_eeprom(int addr, unsigned char *data, int data_len)
{
	unsigned char buf[64];
	memset(buf, 0x00, 64);
	memcpy(&buf[2], data, data_len);
	buf[0] = addr;
	buf[1] = data_len;

	rc = uway_put_command_package(cmd_write_eeprom, sizeof(cmd_write_eeprom),
			buf, 64);

	if (rc >= 0 && buf[0] == FLAG_OK)  {
		return FLAG_OK;
	}
	return -1;
}
