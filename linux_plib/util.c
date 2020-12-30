

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dlog.h"

int system_output(const char *cmd, char *output, uint16_t size)
{
	char buff[512];	
	FILE *fp;

	if (cmd == NULL || output == NULL || size < 1){
		return -2;
	}

	/* Open the command for reading. */
	fp = popen(cmd, "r");
	if (fp == NULL) {
		return -1;
	}

	int len = 0;
	/* Read the output a line at a time - output it. */
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		dlog_dbg("%s", buff);
		strncpy(output + len, buff, size - len);
		len += strlen(buff);
		if (size <= len){
			break;
		}
	}
	output[size - 1] = '\0';
	/* close */
	pclose(fp);
	return strlen(output);
}

int getInterfacIp(const char interfaceStr[])
{
	struct ifaddrs *ifaddr = NULL, *ifa = NULL;
	int retFunc, ret = 0;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return -1; 
	}   

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL){
			continue;  
		}   
		retFunc = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		if((strcmp(ifa->ifa_name, interfaceStr) == 0) && (ifa->ifa_addr->sa_family==AF_INET)) {
			if (retFunc != 0) {
				dlog_err("getnameinfo() failed: %s\n", gai_strerror(retFunc));
				freeifaddrs(ifaddr);
				return -1; 
			}   
			dlog_dbg("Interface(%s): %s\n", ifa->ifa_name, host);
			ret = 1;
		}   
	}   

	freeifaddrs(ifaddr);
	return ret;
}

int getstrline(int fd, char buff[], uint16_t size)
{
	char ch;
    int idx = 0;
    
    while (read (fd, &ch, 1) > 0 && idx < size && ch != '\n'){
		if (ch == 'n' && idx > 1 && buff[idx -1] == '\\'){
			buff[idx -1] = '\0';
			break;
		}
        buff[idx++] = ch;
    }
	if (idx < size){
		buff[idx] = 0;
	}else{
		buff[size - 1] = 0;
	}
    return idx;
}

uint32_t gettimestamp(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (uint32_t)tv.tv_sec;
}

int macstrtoh(const char _buff[], uint8_t mac[])
{
	char *p = NULL, *ptr, *lptr;
	int macLen = 0;

	ptr = strtok_r(_buff, ":", &p);
	while ( NULL != ptr && macLen < 6){
		mac[macLen] = strtol(ptr, &lptr, 16);
		ptr = strtok_r(NULL, ":", &p);
		macLen++;
	}
	return macLen;
}

int ipstrtoh(const char _buff[], uint32_t *ip)
{
	struct in_addr myaddr;
	int ret;
	ret = inet_pton(AF_INET, _buff, &myaddr);
	if (ret == 1){
		*ip = myaddr.s_addr;
	}
	return ret;
}

char *iphtostr(uint32_t ip)
{
	struct in_addr myaddr;

	myaddr.s_addr = ip;
	return inet_ntoa(myaddr);
}

