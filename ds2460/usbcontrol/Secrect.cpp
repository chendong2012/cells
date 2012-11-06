static const char cmd_read_romid[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xbb};
static const char cmd_read_mac[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xbb};
static const char cmd_read_eeprom[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xbb};
Secrect::Secrect()
{
}

Secrect::~Secrect()
{
}


int Secrect::read_rom_id(unsigned char *romid, int romid_len)
{
	return uway_get_command_package(cmd_read_romid, sizeof(cmd_read_romid),
			romid, romid_len);
}

int Secrect::read_mac_code(unsigned char *mac, int mac_len)
{
	return uway_get_command_package(cmd_read_mac, sizeof(cmd_read_mac),
			mac, mac_len);
}

int Secrect::read_eeprom(int addr, unsigned char *data, int data_len)
{
	cmd_read_eeprom[5] = addr;
	cmd_read_eeprom[4] = data_len;
	return uway_get_command_package(cmd_read_eeprom, sizeof(cmd_read_eeprom),
			data, data_len);
}

int Secrect::write_input_data(unsigned char *data, int data_len);
int Secrect::write_s_secrect(unsigned char *secrect, int secrect_len);
int Secrect::write_e_secrect(int index, unsigned char *secrect, int secrect_len)

int Secrect::write_compute_cmd(unsigned char compute_cmd);
int Secrect::write_trans_cmd(unsigned char trans_cmd);
int Secrect::write_eeprom(int addr, unsigned char *data, int len);

check_mac_device();

