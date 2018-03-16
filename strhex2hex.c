

#include <stdio.h>

int strhex2hex_byte(char *str, int size, unsigned char *hex)
{
	if (size != 2) {
		return -1;
	}

	int idx;
	unsigned char halfHex[2];

	for(idx = 0;idx < size;idx++)
	{
		if ('0' <= str[idx] && str[idx] <='9')
		{
			halfHex[idx] = str[idx] - '0';
		}else if ('a' <= str[idx] && str[idx] <= 'f'){
			halfHex[idx] = str[idx] - 'a' + 10;
		}else if ('A' <= str[idx] && str[idx] <='F') {
			halfHex[idx] = str[idx] - 'A' + 10;
		}
		else{
			*hex = 0;
			return -1;
		}
	}
	*hex = (halfHex[0] << 4) + halfHex[1];
	return 0;

}


int main (void)
{
	char hex[8];
	unsigned char idx, value;
	for (idx=0;idx < 0xff;idx++)
	{
		sprintf(hex, "%02X",idx);
		strhex2hex_byte(hex, 2, &value);
		printf("%s:[%02x]\n", hex, value);
	}
		sprintf(hex, "%02X",idx);
		strhex2hex_byte(hex, 2, &value);
		printf("%s:[%02x]\n", hex, value);
	return 0;
}
