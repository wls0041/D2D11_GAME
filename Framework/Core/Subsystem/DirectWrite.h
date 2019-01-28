#pragma once
#include "ISubsystem.h"

class DirectWrite : public ISubsystem
{
public:
	DirectWrite(class Context *context);
	virtual ~DirectWrite();

	void Initialize() override;

	void CreateSurface();
	void DeleteSurface();

	void BeginTextDraw();
	void EndTextDraw();

private:
	ID2D1Factory1 *factory;
	IDWriteFactory *writeFactory;
	ID2D1Device *writeDevice;
	ID2D1DeviceContext *writeDeviceContext;
	ID2D1Bitmap1 *targetBitmap;
};