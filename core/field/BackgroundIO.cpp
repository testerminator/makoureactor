#include "BackgroundIO.h"
#include "PaletteIO.h"
#include "BackgroundTilesIO.h"
#include "BackgroundTexturesIO.h"

BackgroundIO::BackgroundIO(QIODevice *device) :
	_device(device)
{
}

BackgroundIO::~BackgroundIO()
{
}

void BackgroundIO::setDevice(QIODevice *device)
{
	_device = device;
}

QIODevice *BackgroundIO::device() const
{
	return _device;
}

bool BackgroundIO::canRead() const
{
	if(_device) {
		if(!_device->isOpen()) {
			return _device->open(QIODevice::ReadOnly);
		}
		return _device->isReadable();
	}
	return false;
}

bool BackgroundIO::canWrite() const
{
	if(_device) {
		if(!_device->isOpen()) {
			return _device->open(QIODevice::WriteOnly);
		}
		return _device->isWritable();
	}
	return false;
}

BackgroundIOPC::BackgroundIOPC(QIODevice *device, QIODevice *devicePal) :
	BackgroundIO(device), _devicePal(devicePal)
{
}

void BackgroundIOPC::setDevicePal(QIODevice *device)
{
	_devicePal = device;
}

QIODevice *BackgroundIOPC::devicePal() const
{
	return _devicePal;
}

bool BackgroundIOPC::canReadPal() const
{
	if(_devicePal) {
		if(!_devicePal->isOpen()) {
			return _devicePal->open(QIODevice::ReadOnly);
		}
		return _devicePal->isReadable();
	}
	return false;
}

bool BackgroundIOPC::canWritePal() const
{
	if(_devicePal) {
		if(!_devicePal->isOpen()) {
			return _devicePal->open(QIODevice::WriteOnly);
		}
		return _devicePal->isWritable();
	}
	return false;
}

bool BackgroundIOPC::openPalettes(PalettesPC &palettes) const
{
	if(!_devicePal->reset() ||
			!device()->seek(12)) {
		return false;
	}

	PaletteIOPC io(_devicePal, device());
	if(!io.read(palettes)) {
		return false;
	}

	return true;
}

bool BackgroundIOPC::openTiles(BackgroundTiles &tiles) const
{
	if(!device()->reset()) {
		return false;
	}

	BackgroundTilesIOPC io(device());
	if(!io.read(tiles)) {
		return false;
	}

	return true;
}

bool BackgroundIOPC::openTextures(BackgroundTexturesPC &textures) const
{
	if(!device()->seek(device()->pos() + 7)) {
		return false;
	}

	BackgroundTexturesIOPC io(device());
	if(!io.read(&textures)) {
		return false;
	}

	return true;
}

bool BackgroundIOPC::read(BackgroundFile &background) const
{
	if(!canRead()
			|| !canReadPal()) {
		return false;
	}

	background.clear();

	PalettesPC palettes;
	BackgroundTiles tiles;
	BackgroundTexturesPC textures;

	if(!openPalettes(palettes)
			|| !openTiles(tiles)
			|| !openTextures(textures)) {
		foreach(Palette *palette, palettes) {
			delete palette;
		}
		return false;
	}

	background.setPalettes(palettes);
	background.setTiles(tiles);
	background.setTextures(new BackgroundTexturesPC(textures));

	return true;
}

bool BackgroundIOPC::write(const BackgroundFile &background) const
{
	if(!canWrite()
			|| !canWritePal()) {
		return false;
	}

	quint16 unknown1 = 0, depth = 1;
	quint8 unknown2 = 1;

	if(device()->write((char *)&unknown1, 2) != 2
			|| device()->write((char *)&depth, 2) != 2
			|| device()->write((char *)&unknown2, 1) != 1
			|| device()->write("PALETTE", 7) != 7) {
		return false;
	}

	PaletteIOPC paletteIO(devicePal(), device());
	if(!paletteIO.write(background.palettes())) {
		return false;
	}

	if(device()->write("BACK", 4) != 4) {
		return false;
	}

	BackgroundTilesIOPC backgroundTiles(device());

	if(!backgroundTiles.write(background.tiles())) {
		return false;
	}

	if(device()->write("TEXTURE", 7) != 7) {
		return false;
	}

	BackgroundTexturesIOPC backgroundTextures(device());

	if(!backgroundTextures.write((BackgroundTexturesPC *)background.textures())) {
		return false;
	}

	if(device()->write("END", 3) != 3) {
		return false;
	}

	if(device()->write("FINAL FANTASY7", 14) != 14) {
		return false;
	}

	return true;
}

BackgroundIOPS::BackgroundIOPS(QIODevice *device, QIODevice *deviceTiles) :
	BackgroundIO(device), _deviceTiles(deviceTiles)
{
}

void BackgroundIOPS::setDeviceTiles(QIODevice *device)
{
	_deviceTiles = device;
}

QIODevice *BackgroundIOPS::deviceTiles() const
{
	return _deviceTiles;
}

bool BackgroundIOPS::canReadTiles() const
{
	if(_deviceTiles) {
		if(!_deviceTiles->isOpen()) {
			return _deviceTiles->open(QIODevice::ReadOnly);
		}
		return _deviceTiles->isReadable();
	}
	return false;
}

bool BackgroundIOPS::canWriteTiles() const
{
	if(_deviceTiles) {
		if(!_deviceTiles->isOpen()) {
			return _deviceTiles->open(QIODevice::WriteOnly);
		}
		return _deviceTiles->isWritable();
	}
	return false;
}

bool BackgroundIOPS::openPalettes(PalettesPS &palettes) const
{
	if(!device()->reset()) {
		return false;
	}

	PaletteIOPS io(device());
	if(!io.read(palettes)) {
		return false;
	}

	return true;
}

bool BackgroundIOPS::openTiles(BackgroundTiles &tiles) const
{
	if(!deviceTiles()->reset()) {
		return false;
	}

	BackgroundTilesIOPS io(deviceTiles());
	if(!io.read(tiles)) {
		return false;
	}

	return true;
}

bool BackgroundIOPS::openTextures(BackgroundTexturesPS &textures) const
{
	if(!device()->reset()) {
		return false;
	}

	BackgroundTexturesIOPS io(device());
	if(!io.read(&textures)) {
		return false;
	}

	return true;
}

bool BackgroundIOPS::read(BackgroundFile &background) const
{
	if(!canRead()
			|| !canReadTiles()) {
		return false;
	}

	background.clear();

	PalettesPS palettes;
	BackgroundTiles tiles;
	BackgroundTexturesPS textures;

	if(!openPalettes(palettes)
			|| !openTiles(tiles)
			|| !openTextures(textures)) {
		foreach(Palette *palette, palettes) {
			delete palette;
		}
		return false;
	}

	background.setPalettes(palettes);
	background.setTiles(tiles);
	BackgroundTexturesPS *texPtr = new BackgroundTexturesPS(textures);
	background.setTextures(texPtr);

	return true;
}

bool BackgroundIOPS::write(const BackgroundFile &background) const
{
	if(!canWrite()
			|| !canWriteTiles()) {
		return false;
	}

	return false;
}