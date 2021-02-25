
#ifndef __ATA_H
#define __ATA_H

#include "hardwarecommunication/port.h"
#include "common/types.h"

class ATA
{
protected:
	Port16Bit dataPort;
	Port8Bit errorPort;
	Port8Bit sectorCountPort;
	Port8Bit lbaLowPort;
	Port8Bit lbaMidPort;
	Port8Bit lbaHighPort;
	Port8Bit devicePort;
	Port8Bit commandPort;
	Port8Bit controlPort;

	bool master;
	uint16_t bytesPerSector;

public:
	ATA(uint16_t portBase, bool master);
	~ATA();

	void Identify();
	void Read28(uint32_t sector, uint8_t *data, uint32_t count);
	void Write28(uint32_t sector, uint8_t *data, uint32_t count);
	void Flush();
};

#endif