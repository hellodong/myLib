

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

int strdec2dec_uint32(char *str, int size, unsigned int *value)
{
    if (size < 2 || size > 32) {
        return -1;
    }
    
    int idx;
    unsigned int dec[32], decValue = 0;

    for (idx = 0; idx < size;idx++)
    {
        if ('0' <= str[idx] && str[idx] <= '9')
        {
           decValue = decValue * 10;
           decValue += str[idx] - '0';
        }else{
            return -2;
        }
    }
    *value = decValue;
    
    return 0;
}


int main (void)
{
    char str[] = "1";
    char str1[] = "12345679";
    unsigned int decValue;

    strdec2dec_uint32(str, strlen(str), decValue);
    printf("%s:%d\n", str, decValue);
    strdec2dec_uint32(str1, strlen(str1), decValue);
    printf("%s:%d\n", str1, decValue);
    
	return 0;
}
