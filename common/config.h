#ifndef __CONFIG_H
#define __CONFIG_H
enum SoftwareType { ROUTER, CONTROLLER };

/**
 * \struct Configuration
 * \brief Configuration object
 */
typedef struct Configuration
{
	enum SoftwareType type;
	char controllerAddress[20];
	int routerPort;
	int controllerPort;
	int pollTimeoutValue;
	int controllerUpdateInterval;
	int routerUpdateInterval;
	int defaultTTLValue;
	int defaultPingPacketCount;
	int defaultPacketTimeoutValue;
	int defaultDVTimeoutValue;
} Configuration;


Configuration* config__readController(void);
Configuration* config__readRouter(char* file);

//Private function
void config__readLine(Configuration*, char* line);

#endif
