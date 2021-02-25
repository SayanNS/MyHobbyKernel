
#ifndef __PCI_H
#define __PCI_H

#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "common/types.h"

class PCIDeviceDescriptor
{
public:
	uint32_t portBase;
	uint32_t interrupt;

	uint16_t bus;
	uint16_t device;
	uint16_t function;

	uint16_t vendorId;
	uint16_t deviceId;
	
	uint8_t classId;
	uint8_t subclassId;
	uint8_t interfaceId;

	uint8_t revision;

	PCIDeviceDescriptor();
	~PCIDeviceDescriptor();
};

class PCIController
{
	Port32Bit dataPort;
	Port32Bit commandPort;

public:
	PCIController();
	~PCIController();

	uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
	void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
	bool DeviceHasFunctions(uint16_t bus, uint16_t device);
	void SelectDrivers(DriverManager *driverManager);
	PCIDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
};

#endif