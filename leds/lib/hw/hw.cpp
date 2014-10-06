void hw::hw_shift_1bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{
	unsigned char datas=0;
	digitalWrite(SHCP, LOW);

	datas|=((r1&0x80)>>5);
	datas|=((g1&0x80)>>4);
	datas|=((b1&0x80)>>3);
	datas|=((r2&0x80)>>2);
	datas|=((g2&0x80)>>1);
	datas|=((b2&0x80)>>0);
	
	PORTD=datas;

	digitalWrite(SHCP, HIGH);
}


void hw::hw_shift_8bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{
	unsigned char i;

	for(i=0; i<8; i++) {
		hw_shift_1bits(r1,g1,b1,r2,g2,b2);
		r1<<=1;
		g1<<=1;
		b1<<=1;

		r2<<=1;
		g2<<=1;
		b2<<=1;
	}
}


void hw::hw_shift_32bits(struct _rgb_line *rgb1, struct _rgb_line *rgb2)
{
	unsigned char i;
	for(i=0;i<W/8;i++) {
		hw_shift_8bits(rgb1->r[i],\
				rgb1->g[i],\
				rgb1->b[i],\
				rgb2->r[i],\
				rgb2->g[i],\
				rgb2->b[i]);
	}
}
