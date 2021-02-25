
#include "hardwarecommunication/pci.h"

PCIController::PCIController() :
	dataPort(0xCFC),
	commandPort(0xCF8)
{

}

PCIController::~PCIController()
{

}

uint32_t PCIController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
	uint32_t id = 
		0x1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| ((registeroffset & 0xFC));
	commandPort.Write(id);
	uint32_t result = dataPort.Read();

	return result >> (8 * (registeroffset % 4));
}

void PCIController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
	uint32_t id = 
		0x1 << 31
		| ((bus &0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| ((registeroffset & 0xFC));
	commandPort.Write(id);
	dataPort.Write(value);
}

bool PCIController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
	return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void PCIController::SelectDrivers(DriverManager *driverManager)
{
	for (int bus = 0; bus < 8; bus++) {

		for (int device = 0; device < 32; device++) {

			int numFunctions = DeviceHasFunctions(bus, device);			
			for (int function = 0; function < numFunctions; function++) {

			}
		}
	}
}

PCIDeviceDescriptor PCIController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
	
}