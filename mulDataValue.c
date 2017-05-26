

#include <stdio.h>
#include <stdint.h>


struct _u8Data
{
	uint8_t HH;
	uint8_t H;
	uint8_t L;
	uint8_t LL;

};


struct _u16Data
{
	uint16_t H;
	uint16_t L;

};


typedef union _mulDataValue
{
	uint32_t u32Data;
	struct _u16Data u16Data;
	struct _u8Data u8Data;

}mulDataValue_t;


int main (int argc, char *argv[])
{
	mulDataValue_t myData;	

	myData.u32Data = 0x12345678;
	printf ("u32:0x%08x, \r\nu16_h:0x%04x, u16_l:0x%04x\r\n", myData.u32Data, myData.u16Data.H, myData.u16Data.L);
	printf ("u8_hh:0x%x,u8_h:0x%x,u8_l:0x%x,u8_l:0x%x\r\n",myData.u8Data.HH,myData.u8Data.H,myData.u8Data.L,myData.u8Data.LL);

	return 0;
}
