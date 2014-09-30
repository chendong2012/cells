
struct _raw_8points
{
	unsigned char dat;
}

void set_rgb_8points_value(struct _rgb_8points *rgb8points, unsigned char r, unsigned char g, unsigned b)
{
	rgb8points->r=r;
	rgb8points->g=g;
	rgb8points->b=b;
}

void set_fg_apoints(struct _rgb_8points *rgb, unsigned char fg, unsigned short focus)
{
	switch (fg) {
	case 0://000
		rgb->r&=~(1<<focus);
		rgb->g&=~(1<<focus);
		rgb->b&=~(1<<focus);
		break;
	case 1://001

		rgb->r&=~(1<<focus);
		rgb->g&=~(1<<focus);
		rgb->b|=(1<<focus);
		break;
	case 2://010
		rgb->r&=~(1<<focus);
		rgb->g|=(1<<focus);
		rgb->b&=~(1<<focus);
		break;
	case 3://011
		rgb->r&=~(1<<focus);
		rgb->g|=(1<<focus);
		rgb->b|=(1<<focus);
		break;
	case 4://100
		rgb->r|=(1<<focus);
		rgb->g&=~(1<<focus);
		rgb->b&=~(1<<focus);
		break;
	case 5://101
		rgb->r|=(1<<focus);
		rgb->g&=~(1<<focus);
		rgb->b|=(1<<focus);
		break;
	case 6://110
		rgb->r|=(1<<focus);
		rgb->g|=(1<<focus);
		rgb->b&=~(1<<focus);
		break;
	case 7://111
		rgb->r|=(1<<focus);
		rgb->g|=(1<<focus);
		rgb->b|=(1<<focus);
		break;
	}
}

void set_bg_8points(struct _rgb_8points *rgb, unsigned char bg)
{
	switch (bg) {
	case 0://000
		set_rgb_8points_value(rgb, 0x00, 0x00, 0x00);
		break;
	case 1://001
		set_rgb_8points_value(rgb, 0x00, 0x00, 0xff);
		break;
	case 2://010
		set_rgb_8points_value(rgb, 0x00, 0xff, 0x00);
		break;
	case 3://011
		set_rgb_8points_value(rgb, 0x00, 0xff, 0xff);
		break;
	case 4://100
		set_rgb_8points_value(rgb, 0xff, 0x00, 0x00);
		break;
	case 5://101
		set_rgb_8points_value(rgb, 0xff, 0x00, 0xff);
		break;
	case 6://110
		set_rgb_8points_value(rgb, 0xff, 0xff, 0x00);
		break;
	case 7://111
		set_rgb_8points_value(rgb, 0xff, 0xff, 0xff);
		break;
	default:
		break;
	}
}

void zkcvt::cvt_1rgb_from(const struct _raw_8points *raw, struct _rgb_8points *rgb, unsigned char fg, unsigned char bg)
{
	unsigned char j;
	unsigned char temp=0;
	set_bg_8points(rgb, bg);
	temp = raw->dat;

	for(j=0;j<8;j++) {
		if(((temp<<j)&0x80)==0x80)
			set_fg_apoints(rgb, fg, (7-j));
	}
}

void zkcvt::cvt_8x16rgb_from(const struct _raw_8points *raw, struct _rgb_8points *rgb, unsigned char fg, unsigned char bg)
{
	for(i=0;i<16;i++) {
		cvt_1rgb_from_f(raw++, rgb++, fg, bg);
	}
}
