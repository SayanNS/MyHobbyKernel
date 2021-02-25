#include "drivers/ata.h"
#include "common/print.h"

ATA::ATA(uint16_t portBase, bool master) :
	dataPort(portBase),
	errorPort(portBase + 1),
	sectorCountPort(portBase + 2),
	lbaLowPort(portBase + 3),
	lbaMidPort(portBase + 4),
	lbaHighPort(portBase + 5),
	devicePort(portBase + 6),
	commandPort(portBase + 7),
	controlPort(portBase + 0x206)
{
	bytesPerSector = 512;
	this->master = master;
}

ATA::~ATA()
{

}

void ATA::Identify()
{
	devicePort.Write(master ? 0xA0 : 0xB0);
	controlPort.Write(0);

	devicePort.Write(0xA0);
	uint8_t status = commandPort.Read();
	if (status == 0xFF)
		return;
	
	devicePort.Write(master ? 0xA0 : 0xB0);
	sectorCountPort.Write(0);
	lbaLowPort.Write(0);
	lbaMidPort.Write(0);
	lbaHighPort.Write(0);
	commandPort.Write(0xEC); // identify command

	status = commandPort.Read();
	if (status == 0x00) // no device
		return;

	while (((status & 0x80) == 0x80)
		&& ((status & 0x01) != 0x01))
		status = commandPort.Read();
	
	if (status & 0x01) {
		printf("ERROR");
		return;
	}
}

void ATA::Read28(uint32_t sector, uint8_t *data, uint32_t count)
{
	if (sector > 0x0FFFFFFF)
		return;

	devicePort.Write((master ? 0xE0 : 0xF) | ((sector & 0x0F000000) >> 24));
	errorPort.Write(0);
	sectorCountPort.Write(1);
	lbaLowPort.Write(sector & 0x000000FF);
	lbaMidPort.Write((sector & 0x0000FF00) >> 8);
	lbaHighPort.Write((sector & 0x00FF0000) >> 16);
	commandPort.Write(0x20);

	uint8_t status = commandPort.Read();
	while (((status & 0x80) == 0x80)
		&& ((status & 0x01) != 0x01))
		status = commandPort.Read();

	if (status & 0x01) {
		printf("ERROR");
		return;
	}

	for (int i = 0; i < count; i += 2) {
		uint16_t wdata = dataPort.Read();
		// Proceed data
	}

	for (int i = count + (count % 2); i < 512; i += 2) {
		dataPort.Read();
	}
}

void ATA::Write28(uint32_t sector, uint8_t *data, uint32_t count)
{
	if (sector > 0x0FFFFFFF)
		return;

	devicePort.Write((master ? 0xE0 : 0xF) | ((sector & 0x0F000000) >> 24));
	errorPort.Write(0);
	sectorCountPort.Write(1);
	lbaLowPort.Write(sector & 0x000000FF);
	lbaMidPort.Write((sector & 0x0000FF00) >> 8);
	lbaHighPort.Write((sector & 0x00FF0000) >> 16);
	commandPort.Write(0x30);

	for (int i = 0; i < count; i += 2) {
		uint16_t wdata = data[i];
		if (i + 1 < count)
			wdata |= ((uint16_t)data[i + 1]) << 8;
		dataPort.Write(wdata);
	}

	for (int i = count + (count % 2); i < 512; i += 2) {
		dataPort.Write(0);
	}
}

void ATA::Flush()
{
	devicePort.Write(master ? 0xE0 : 0xF0);
	commandPort.Write(0xE7);

	uint8_t status = commandPort.Read();
	if (status == 0x00)
		return;

	while (((status & 0x80) == 0x80)
		&& ((status & 0x01) != 0x01))
		status = commandPort.Read();

	if (status & 0x01) {
		printf("ERROR");
		return;
	}
}