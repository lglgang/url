//gcc -Wall -o ls7366A ls7366A.c -lwiringPi
//gcc ls7366M0.c -shared -o ls7366M0.so -lwiringPi

#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
//encoder 1
#define EX01_CNT_EN 24
#define EX01_SS 23

//encoder 2
#define EX01_CNT_EN1 2
#define EX01_SS1 0

//STR 110000 0x30
#define _STR_LS7366R_B543 0x30
//OTR 101000 0x28
#define _OTR_LS7366R_B543 0x28
//CNTR 100000 0x20
#define _CNTR_LS7366R_B543 0x20
//DTR 011000 0x18
#define _DTR_LS7366R_B543 0x18
//MDR1 010000 0x10
#define _MDR1_LS7366R_B543 0x10
//MDR0 001000 0x08
#define _MDR0_LS7366R_B543 0X08

//CLR 00000000 0x00
#define _CLR_LS7366R_B76 0x00
//RD 01000000 0x40
#define _RD_LS7366R_B76 0x40
//WR 10000000 0x80
#define _WR_LS7366R_B76 0x80
//LOAD 11000000 0xc0
#define _LOAD_LS7366R_B76 0xc0

unsigned char rxtx[8];

//CLR
void CLR_MDR0(void)
{
	rxtx[0]=_CLR_LS7366R_B76+_MDR0_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,1);
}
void CLR_MDR1(void)
{
	rxtx[0]=_CLR_LS7366R_B76+_MDR1_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,1);
}
void CLR_CNTR(void)
{
	rxtx[0]=_CLR_LS7366R_B76+_CNTR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,1);
}
void CLR_STR(void)
{
	rxtx[0]=_CLR_LS7366R_B76+_STR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,1);
}
//RD  4BYTES
unsigned char RD_MDR0(void)
{
	rxtx[0]=_RD_LS7366R_B76+_MDR0_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,2);
	return (rxtx[1]);
}
unsigned char RD_MDR1(void)
{
	rxtx[0]=_RD_LS7366R_B76+_MDR1_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,2);
	return (rxtx[1]);
}
unsigned int RD_CNTR(void)
{
	unsigned int data01,data02,data03,data04;
	rxtx[0]=_RD_LS7366R_B76+_CNTR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,5);
	data01=rxtx[1];
	data02=rxtx[2];
	data03=rxtx[3];
	data04=rxtx[4];
	return (data01<<24)+(data02<<16)+(data03<<8)+(data04<<0);
}
unsigned int RD_OTR(void)
{
	unsigned int data01,data02,data03,data04;
	rxtx[0]=_RD_LS7366R_B76+_OTR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,5);
	data01=rxtx[1];
	data02=rxtx[2];
	data03=rxtx[3];
	data04=rxtx[4];
	return (data01<<24)+(data02<<16)+(data03<<8)+(data04<<0);
}
unsigned int RD_STR(void)
{
	unsigned int data01,data02,data03,data04;
	rxtx[0]=_RD_LS7366R_B76+_STR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,5);
	data01=rxtx[1];
	data02=rxtx[2];
	data03=rxtx[3];
	data04=rxtx[4];
	return (data01<<24)+(data02<<16)+(data03<<8)+(data04<<0);
}
//WR 4 BYTES
void WR_MDR0(unsigned char datax)
{
	rxtx[1]=datax;
	rxtx[0]=_WR_LS7366R_B76+_MDR0_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,2);
}
void WR_MDR1(unsigned char datax)
{
	rxtx[1]=datax;
	rxtx[0]=_WR_LS7366R_B76+_MDR1_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,2);
}
void WR_DTR(unsigned int datax)
{
	rxtx[4]=datax;
	rxtx[3]=datax>>8;
	rxtx[2]=datax>>16;
	rxtx[1]=datax>>24;
	rxtx[0]=_WR_LS7366R_B76+_DTR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,5);
}
//LOAD
void LOAD_CNTR(void)
{
	rxtx[0]=_LOAD_LS7366R_B76+_CNTR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,1);
}
void LOAD_OTR(void)
{
	rxtx[0]=_LOAD_LS7366R_B76+_OTR_LS7366R_B543;
	wiringPiSPIDataRW(0,rxtx,1);
}
void init_Pi(void)
{
	wiringPiSetup();
	pinMode(EX01_CNT_EN,OUTPUT);
	pinMode(EX01_SS,OUTPUT);
	pinMode(EX01_CNT_EN1,OUTPUT);
	pinMode(EX01_SS1,OUTPUT);
	wiringPiSPISetup(0,500000);
}
void init_Pi1(void)
{
	wiringPiSetup();
	pinMode(EX01_CNT_EN,OUTPUT);
	pinMode(EX01_SS,OUTPUT);
	wiringPiSPISetup(0,500000);
}
void init_Pi2(void)
{
	wiringPiSetup();
	pinMode(EX01_CNT_EN1,OUTPUT);
	pinMode(EX01_SS1,OUTPUT);
	wiringPiSPISetup(0,500000);
}


void encoder1_enable(void)
{
	digitalWrite(EX01_CNT_EN ,HIGH);
}

void encoder1_disable(void)
{
	digitalWrite(EX01_CNT_EN ,LOW);
}

void encoder2_enable(void)
{
	digitalWrite(EX01_CNT_EN1 ,HIGH);
}

void encoder2_disable(void)
{
	digitalWrite(EX01_CNT_EN1 ,LOW);
}


void encoder1_spi_start(void)
{
	digitalWrite(EX01_SS1 ,HIGH);
	digitalWrite(EX01_SS ,HIGH);
	digitalWrite(EX01_SS ,LOW);
}

void encoder1_spi_stop(void)
{
	digitalWrite(EX01_SS1 ,HIGH);	
	digitalWrite(EX01_SS ,LOW);
	digitalWrite(EX01_SS ,HIGH);
}


void encoder2_spi_start(void)
{
	digitalWrite(EX01_SS ,HIGH);
	digitalWrite(EX01_SS1 ,HIGH);
	digitalWrite(EX01_SS1 ,LOW);
}

void encoder2_spi_stop(void)
{
	digitalWrite(EX01_SS ,HIGH);	
	digitalWrite(EX01_SS1 ,LOW);
	digitalWrite(EX01_SS1 ,HIGH);
}

void encoder1_clr(void)
{
	//encoder1_clr
	encoder1_spi_start();
	CLR_MDR0();	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	CLR_MDR1();	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	CLR_CNTR();	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	CLR_STR();	
	encoder1_spi_stop();
}

void encoder2_clr(void)
{
	//encoder2_clr
	encoder2_spi_start();
	CLR_MDR0();	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	CLR_MDR1();	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	CLR_CNTR();	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	CLR_STR();	
	encoder2_spi_stop();
}

//set middle is 50000    
//arrange is from 0 to 100000

void encoder1_write(void)
{
	//encoder1_write
	encoder1_spi_start();
	WR_MDR0(0x4b);	//0x4b  0xd2
	encoder1_spi_stop();
	
	encoder1_spi_start();
	WR_MDR1(0x00);	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	WR_DTR(50000);	//this is the middle value.
	encoder1_spi_stop();
	
	encoder1_spi_start();
	LOAD_CNTR();	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	WR_DTR(100000);	//this is the max value.
	encoder1_spi_stop();
	
}
//set middle is 50000    
//arrange is from 0 to 100000
void encoder2_write(void)
{
	//encoder1_write
	encoder2_spi_start();
	WR_MDR0(0x4b);	//0x4b  0xd2
	encoder2_spi_stop();
	
	encoder2_spi_start();
	WR_MDR1(0x00);	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	WR_DTR(50000);	//this is the middle value.
	encoder2_spi_stop();
	
	encoder2_spi_start();
	LOAD_CNTR();	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	WR_DTR(100000);	//this is the max value.
	encoder2_spi_stop();
	
}

void encoder1_load(void)
{
	//encoder1_spi_start();
	//LOAD_CNTR();	
	//encoder1_spi_stop();
	
	encoder1_spi_start();
	LOAD_OTR();	
	encoder1_spi_stop();
}

void encoder2_load(void)
{
	//encoder2_spi_start();
	//LOAD_CNTR();	
	//encoder2_spi_stop();
	
	encoder2_spi_start();
	LOAD_OTR();	
	encoder2_spi_stop();
}

void encoder1_read_print(void)
{
	unsigned int countx;
	encoder1_spi_start();
	LOAD_OTR();	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	countx=RD_OTR();	
	encoder1_spi_stop();
	printf("counterx1= %d\n",countx);

}

void encoder2_read_print(void)
{
	unsigned int countx;
	encoder2_spi_start();
	LOAD_OTR();	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	countx=RD_OTR();	
	encoder2_spi_stop();
	printf("counterx2= %d\n",countx);

}

void init_encoder1(void)
{
	init_Pi1();
	encoder1_enable();
	encoder1_clr();
	encoder1_write();
	encoder1_load();
	encoder1_enable();
}

void init_encoder2(void)
{
	init_Pi2();
	encoder2_enable();
	encoder2_clr();
	encoder2_write();
	encoder2_load();
	encoder2_enable();
}

unsigned int get_counter1(void)
{
	unsigned int countx;
	encoder1_spi_start();
	LOAD_OTR();	
	encoder1_spi_stop();
	
	encoder1_spi_start();
	countx=RD_OTR();	
	encoder1_spi_stop();
	return countx;
}

unsigned int get_counter2(void)
{
	unsigned int countx;
	encoder2_spi_start();
	LOAD_OTR();	
	encoder2_spi_stop();
	
	encoder2_spi_start();
	countx=RD_OTR();	
	encoder2_spi_stop();
	return countx;
}

/*	
unsigned int xxx;

int main(void)
{
	init_Pi();
	encoder1_enable();
	encoder1_clr();
	encoder1_write();
	encoder1_load();
	encoder1_enable();
	
	encoder2_enable();
	encoder2_clr();
	encoder2_write();
	encoder2_load();
	encoder2_enable();
	xxx=1;
	while(1)
	{
		encoder1_read_print();
		encoder2_read_print();
		xxx++;
		printf("xxx=%d\n",xxx);

	}
	

return 0;
}
*/