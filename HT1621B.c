#include "gpio.h"
#include "HT1621B.h"
#include "stm32f1xx_hal.h"
#include"stdio.h"

char dispnum[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char error[] = { 0xE5, 0x02, 0xE7 }; //n,-,A for n-A print

//HT1621B() {
// dispnum[0] = 0x00;
//dispnum[1] = 0x00;
//dispnum[2] = 0x00;
//dispnum[3] = 0x00;
//dispnum[4] = 0x00;
//dispnum[5] = 0x00;
//char dispnum[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//const char num[] =     {0xf5, 0x05, 0xB6, 0x97, 0x47, 0xD3, 0xF3, 0x85, 0xF7, 0xD7};
//const char channel[] = {0x5F, 0x50, 0x3D, 0x79, 0x72, 0x6B, 0x6F, 0x51, 0x7F, 0x7B};
//const char num_bat[] = {0xFD, 0x0D, 0xBE, 0x9F, 0x4F, 0xDB, 0xFB, 0x8D, 0xFF, 0xDF};

//}

void SendBit_1621(uchar sdata, uchar cnt) {
	uchar i;
	for (i = 0; i < cnt; i++) {
		WR0;
		if (sdata & 0x80)
			DATA1;
		else
			DATA0;
		WR1;
		sdata <<= 1;
	}
}

void SendCmd_1621(uchar command) {
	CS0;
	SendBit_1621(0x80, 4);
	SendBit_1621(command, 8);
	CS1;
}

void Write_1621(uchar addr, uchar sdata) {
	addr <<= 2;
	CS0;
	SendBit_1621(0xa0, 3);
	SendBit_1621(addr, 6);
	SendBit_1621(sdata, 8);
	CS1;
}

void HT1621_all_off(uchar num) {
	uchar i;
	uchar addr = 0;
	for (i = 0; i < num; i++) {
		Write_1621(addr, 0x00);
		addr += 2;
	}
}

void HT1621_all_on(uchar num) {
	uchar i;
	uchar addr = 0;
	for (i = 0; i < num; i++) {
		Write_1621(addr, 0xff);
		addr += 2;
	}
}

//void HT1621B_begin(int cs, int wr, int data) {
void HT1621B_begin() {
	//CS = cs;
	//WR = wr;
	//DATA = data;
	HAL_GPIO_WritePin(GPIOA, CS_Pin | WR_Pin | DATA_Pin, GPIO_PIN_SET);
	//pinMode(CS, OUTPUT);
	//pinMode(WR, OUTPUT);
	//pinMode(DATA, OUTPUT);
	//digitalWrite(CS, HIGH);
	//digitalWrite(WR, HIGH);
	//digitalWrite(DATA, HIGH);
}

void Init_1621(void) {
	SendCmd_1621(Sys_en);
	SendCmd_1621(RCosc);
	SendCmd_1621(ComMode);
	SendCmd_1621(LCD_on);
}

void num_print(int num, short int dp) {
	///Serial1.print("num is:");
	//Serial1.println(num);
	char localbuffer[5]; //buffer to work with in the function
	if (num >= 0) {
		sprintf(localbuffer, "  %03d", num); // convert the decimal into string
	} else if (num < 0) {
		sprintf(localbuffer, " %04d", num); // convert the decimal into string
	}
	//Serial1.println("localbuffer:");
	//Serial1.println(localbuffer);
	for (int i = 0; i < 5; i++) {
		//dispnum[i] &= 0x80; // mask the first bit, used by batter and decimal point
		switch (localbuffer[i]) {
		case '0':
			dispnum[i] = 0xF5;
			break;
		case '1':
			dispnum[i] = 0x05;
			break;
		case '2':
			dispnum[i] = 0xB6;
			break;
		case '3':
			dispnum[i] = 0x97;
			break;
		case '4':
			dispnum[i] = 0x47;
			break;
		case '5':
			dispnum[i] = 0xD3;
			break;
		case '6':
			dispnum[i] = 0xF3;
			break;
		case '7':
			dispnum[i] = 0x85;
			break;
		case '8':
			dispnum[i] = 0xF7;
			break;
		case '9':
			dispnum[i] = 0xD7;
			break;
		case '-':
			dispnum[i] = 0x02;
			break;
		case ' ':
			dispnum[i] = 0x00;
			break;
		}
		if (dp == 1) {
			Write_1621(8, 0x40);
		}

		//Serial1.println("dispnum:");
		//Serial1.println(dispnum);
		Write_1621(0, dispnum[0]); //1
		Write_1621(2, dispnum[1]); //2
		Write_1621(4, dispnum[2]); //3
		Write_1621(6, dispnum[3]); //4
		Write_1621(9, dispnum[4]); //5

		//strcpy(localbuffer,"\0");

	}
	// strcpy(localbuffer,"\0");
}
void Err_print() {
	dispnum[0] = 0x02;
	Write_1621(0, 0x02); //1
	dispnum[1] = 0x02;
	Write_1621(2, 0x02); //2
	dispnum[2] = 0x02;
	Write_1621(4, 0x02); //3
	dispnum[3] = 0x02;
	Write_1621(6, 0x02); //4
	dispnum[4] = 0x00;
	Write_1621(9, 0x02); //5
	dispnum[4] = 0x02;

}
void minus_print() {
	dispnum[0] = 0;
	//Write_1621(2, 0x02); //1
	//Write_1621(2, 0x02); //2
	Write_1621(4, 0xE5); //3/n
	Write_1621(6, 0x02); //4//-
	// Write_1621(6, 0x60); //4//i
	dispnum[4] = 0x00;
	//Write_1621(9, 0x70); //5//L
	Write_1621(9, 0xE7); //5//A

}
void clear(int len) {
	unsigned char addr = 0;
	unsigned char i;
	for (i = 0; i < len; i++) {
		Write_1621(addr, 0x00);
		addr = addr + 2;
	}
}

void battey_level(short unsigned int level) {
	if (dispnum[4] != 0)
		Write_1621(9, dispnum[4] | 0x08);
	else
		Write_1621(9, error[2] | 0x08);
	//Write_1621(10,  0x80);
	if (level == 1) {
//		Write_1621(2, dispnum[1] | 0x08);
		if (dispnum[1] != 0) {
			Write_1621(2, dispnum[1] | 0x08);
			dispnum[1] = 0x00;
		} else
			Write_1621(2, 0x08);
	} else if (level == 2) {
		if (dispnum[1] != 0) {
			Write_1621(2, dispnum[1] | 0x08);
			dispnum[1] = 0x00;
		} else
			Write_1621(2, 0x08);
		if (dispnum[2] != 0) {
			Write_1621(4, dispnum[2] | 0x08);
			dispnum[2] = 0x00;
		} else
			Write_1621(4, error[0] | 0x08);
		//Write_1621(2, dispnum[1] | 0x08);
		//Write_1621(4, dispnum[2] | 0x08);
	} else if (level == 3) {
		if (dispnum[1] != 0) {
			Write_1621(2, dispnum[1] | 0x08);
			dispnum[1] = 0x00;
		} else
			Write_1621(2, 0x08);
		if (dispnum[2] != 0) {
			Write_1621(4, dispnum[2] | 0x08);
			dispnum[2] = 0x00;
		} else
			Write_1621(4, error[0] | 0x08);
		if (dispnum[3] != 0) {
			Write_1621(6, dispnum[3] | 0x08);
			dispnum[3] = 0x00;
		} else
			Write_1621(6, error[1] | 0x08);
//		Write_1621(2, dispnum[1] | 0x08);
//		Write_1621(4, error[0] | 0x08);
//		Write_1621(6, error[1] | 0x08);
	} else if (level == 4) {
		if (dispnum[1] != 0) {
			Write_1621(2, dispnum[1] | 0x08);
			dispnum[1] = 0x00;
		} else
			Write_1621(2, 0x08);
		if (dispnum[2] != 0) {
			Write_1621(4, dispnum[2] | 0x08);
			dispnum[2] = 0x00;
		} else
			Write_1621(4, error[0] | 0x08);
		if (dispnum[3] != 0) {
			Write_1621(6, dispnum[3] | 0x08);
			dispnum[3] = 0x00;
		} else
			Write_1621(6, error[1] | 0x08);
		if (dispnum[0] != 0) {
			Write_1621(0, dispnum[0] | 0x08);
			dispnum[4] = 0x00;
		} else
			Write_1621(0, 0x08);
	}

}
void signal_method(short unsigned int method, short unsigned int strength) {
	//******** for wifi method=0,strength=0********//
	if (method == 0) // WIFI =0, strength=0
			{
		Write_1621(11, 0x01);
	}

	//******** for tower method=1,strength=level of strength in integer********//

	if (method == 1) // if need the wifi or -ed the 0x01 with addr
			{
		if (strength == 1) {
			addr = 0x02;
		} else if (strength == 2) {
			addr = 0x06;

		} else if (strength == 3) {
			addr = 0x86;

		} else if (strength == 4) {
			addr = 0xC6;
		}
		Write_1621(11, addr);
	}
}

void units(short unsigned int unit) // temp=0, humi=1
{
	// if need with tower means add the addr with address
	if (unit == 0) {
		Write_1621(11, addr | 0x10);
	}
	if (unit == 1) {
		Write_1621(11, addr | 0x20);
	}
}
void channel_count(int channel) {
	//Serial.println(channel);
	unsigned char address;
	switch (channel) {

	case 0:
		address = 0x5F;
		break;
	case 1:
		address = 0x50;
		break;
	case 2:
		address = 0x3D;
		break;
	case 3:
		address = 0x79;
		break;
	case 4:
		address = 0x72;
		break;
	case 5:
		address = 0x6B;
		break;
	case 6:
		address = 0x6F;
		break;
	case 7:
		address = 0x51;
		break;
	case 8:
		address = 0x7F;
		break;
	case 9:
		address = 0x7B;
		break;

	}
	// if refersh need means Write_1621(13, 0x10);
	Write_1621(13, address);
}
void symbol_HIGH() {
	if (dispnum[4] != 0x00)
		Write_1621(8, 0x60); // with decimal point
	else
		Write_1621(8, 0x20);
	Write_1621(9, dispnum[4]);
}
void symbol_LOW() {
	if (dispnum[4] != 0x00)
		Write_1621(8, 0x50); // with decimal point
	else
		Write_1621(8, 0x10);
	Write_1621(9, dispnum[4]);
}

void symbol_REFRESH() {
	Write_1621(13, 0x10);
}
void set_print() {

	//Write_1621(0, 0x02); //1
	//Write_1621(2, 0x02); //2
	Write_1621(4, 0xD3); //S
	Write_1621(6, 0xF6); //E
	dispnum[4] = 0x00;
	Write_1621(9, 0x72); //t

}
void ON_print() {
	Write_1621(6, 0xF5); //O
	dispnum[4] = 0x00;
	Write_1621(9, 0xE5); //n
}
