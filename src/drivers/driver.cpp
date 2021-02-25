
#include "drivers/driver.h"

Driver::Driver()
{

}

Driver::~Driver()
{

}

void Driver::Activate()
{

}

void Driver::Deactivate()
{

}

int Driver::Reset()
{

}

DriverManager::DriverManager()
{

}

void DriverManager::AddDriver(Driver *driver)
{
	drivers[numDrivers] = driver;
	numDrivers++;
}