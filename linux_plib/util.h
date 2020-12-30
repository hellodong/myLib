

#ifndef _UTIL_H_
#define _UTIL_H_

int system_output(const char *cmd, char *output, unsigned short size);

int getInterfacIp(const char interfaceStr[]); 

int getstrline(int fd, char buff[], uint16_t size);

unsigned int gettimestamp(void);

int macstrtoh(const char _buff[], uint8_t mac[]);

int ipstrtoh(const char _buff[], uint32_t *ip);

char *iphtostr(uint32_t ip);

#endif
