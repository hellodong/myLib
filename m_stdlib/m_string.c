
char *m_strcpy(char *dest, const char *src)
{
	while (*src != '\0') {
		*dest++ = *src++;
	}
	return dest;
}

char *m_strcat(char *dest, const char *src)
{
	char *ptr = dest;
	while (*ptr != '\0'){
		ptr++;
	}
	m_strcpy(ptr, src);
	return dest;
}

int m_atoi(const char str[])
{
	int val = 0;
	int flag = 1;
	int idx = 0;

	if (str[idx] == '-') {
		flag = -1;
		idx++;
	}
	for (; idx < strlen(str); idx++) {
		if (str[idx] >= '0' && str[idx] <= '9') {
			val += str[idx] - '0';
		}
		val *= 10;
	}
	val = val / 10;
	val *= flag;
	return val;
}

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

void int_to_str(_U32 pass, _U8 * buf, int str_len) 
{
	int i;
	_U32 d = 1;
	for (i = (str_len - 1); i >= 0; i--) {
		buf[i] = ((pass / d) % 10) + '0';
		d = d * 10;
	}
	buf[str_len] = 0;
}

int int2str(_U32 val, _U8 *valStr)
{
    int oLen = 0;
	int len;
    _U32 ori = val;
    while(val){
        oLen++;
        val /= 10; 
    }   
    val = ori;
    if (!val) {
        oLen = 1;
    }   
    valStr[oLen] = 0;
	len = oLen;
    for(;oLen;oLen--) {
        valStr[oLen - 1] = val % 10 + '0';
        val /= 10; 
    }   
	return len;
}