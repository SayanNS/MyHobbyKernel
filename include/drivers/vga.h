
#ifndef __VGA_H
#define __VGA_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

class VideoGraphicsArray
{
protected:
	Port8Bit miscPort;
	Port8Bit crtcIndexPort;
	Port8Bit crtcDataPort;
	Port8Bit sequencerIndexPort;
	Port8Bit sequencerDataPort;
	Port8Bit graphicsControllerIndexPort;
	Port8Bit graphicsControllerDataPort;
	Port8Bit attributeControllerIndexPort;
	Port8Bit attributeControllerReadPort;
	Port8Bit attributeControllerWritePort;
	Port8Bit attributeControllerResetPort;

	void WriteRegisters(uint8_t *registers);
	uint8_t *GetFrameBufferSegment();
	
	virtual uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);

public:
	VideoGraphicsArray();
	~VideoGraphicsArray();

	virtual bool SupportsMode(uint32_t width, uint32_t height, uint32_t colorDepth);
	virtual bool SetMode(uint32_t width, uint32_t height, uint32_t colorDepth);
	virtual void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
	virtual void PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex);
};

#endif