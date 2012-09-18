/****************************************************************************
 ** Makou Reactor Final Fantasy VII Field Script Editor
 ** Copyright (C) 2009-2012 Arzel J�r�me <myst6re@gmail.com>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "InfFile.h"

InfFile::InfFile()
	: _isOpen(false), _isModified(false)
{
}

InfFile::InfFile(const QByteArray &data)
	: _isOpen(false), _isModified(false)
{
	open(data);
}

bool InfFile::isOpen()
{
	return _isOpen;
}

bool InfFile::isModified()
{
	return _isModified;
}

void InfFile::setModified(bool modified)
{
	_isModified = modified;
}

bool InfFile::open(const QByteArray &data, bool importMode)
{
	quint32 oldSize = size;
	size = data.size();

	if(sizeof(InfData) != 740) {
		qWarning() << "Error InfData" << sizeof(InfData) << "must be 740";
		return false;
	}

	if(size != 740 && size != 536) {
		qWarning() << "Error inf size" << size;
//		QFile debinf("debugInf.inf");
//		debinf.open(QIODevice::WriteOnly);
//		debinf.write(data);
//		debinf.close();
		return false;
	}

	this->data = InfData();
	memcpy(&this->data, data.constData(), size);
	this->data.name[8] = '\x00';

	if(importMode && _isOpen)		size = oldSize;

	_isOpen = true;

	return true;
}

QByteArray InfFile::save()
{
	return QByteArray((char *)&data, size);
}

bool InfFile::isJap()
{
	return size != sizeof(InfData);
}

QString InfFile::mapName()
{
	return QString(data.name);
}

void InfFile::setMapName(const QString &name)
{
	strcpy(data.name, name.toLatin1().constData());
	data.name[8] = '\x00';
	_isModified = true;
}

quint8 InfFile::control()
{
	return data.control;
}

void InfFile::setControl(quint8 control)
{
	data.control = control;
	_isModified = true;
}

qint16 InfFile::cameraFocusHeight() const
{
	return data.cameraFocusHeight;
}

void InfFile::setCameraFocusHeight(qint16 cameraFocusHeight)
{
	data.cameraFocusHeight = cameraFocusHeight;
	_isModified = true;
}

const Range &InfFile::cameraRange() const
{
	return data.camera_range;
}

void InfFile::setCameraRange(const Range &range)
{
	data.camera_range = range;
	_isModified = true;
}

qint16 InfFile::bgLayer3Width() const
{
	return data.bg_layer3_width;
}

void InfFile::setBgLayer3Width(qint16 width)
{
	data.bg_layer3_width = width;
	_isModified = true;
}

qint16 InfFile::bgLayer3Height() const
{
	return data.bg_layer3_height;
}

void InfFile::setBgLayer3Height(qint16 height)
{
	data.bg_layer3_height = height;
	_isModified = true;
}

qint16 InfFile::bgLayer4Width() const
{
	return data.bg_layer4_width;
}

void InfFile::setBgLayer4Width(qint16 width)
{
	data.bg_layer4_width = width;
	_isModified = true;
}

qint16 InfFile::bgLayer4Height() const
{
	return data.bg_layer4_height;
}

void InfFile::setBgLayer4Height(qint16 height)
{
	data.bg_layer4_height = height;
	_isModified = true;
}

QList<Exit> InfFile::exitLines() const
{
	QList<Exit> exit;
	for(int i=0 ; i<12 ; ++i) {
		exit.append(data.doors[i]);
	}
	return exit;
}

Exit InfFile::exitLine(quint8 id)
{
	return data.doors[id];
}

void InfFile::setExitLine(quint8 id, const Exit &line)
{
	data.doors[id] = line;
	_isModified = true;
}

QList<Trigger> InfFile::triggers() const
{
	QList<Trigger> trigger;
	for(int i=0 ; i<12 ; ++i) {
		trigger.append(data.triggers[i]);
	}
	return trigger;
}

const Trigger &InfFile::trigger(quint8 id) const
{
	return data.triggers[id];
}

void InfFile::setTrigger(quint8 id, const Trigger &trigger)
{
	data.triggers[id] = trigger;
	_isModified = true;
}

bool InfFile::arrowIsDisplayed(quint8 id)
{
	return data.display_arrow[id] & 1;
}

void InfFile::setArrowDiplay(quint8 id, bool display)
{
	data.display_arrow[id] = (data.display_arrow[id] & 0xFE) | display;
	_isModified = true;
}

QList<Arrow> InfFile::arrows() const
{
	QList<Arrow> arrowList;
	for(int i=0 ; i<12 ; ++i) {
		arrowList.append(data.arrows[i]);
	}
	return arrowList;
}

const Arrow &InfFile::arrow(quint8 id) const
{
	return data.arrows[id];
}

void InfFile::setArrow(quint8 id, const Arrow &arrow)
{
	data.arrows[id] = arrow;
	_isModified = true;
}

QByteArray InfFile::unknown1() const
{
	return QByteArray((char *)&data.u1, 4);
}

void InfFile::setUnknown1(const QByteArray &u)
{
	memcpy(&data.u1, u.leftJustified(4, '\0', true).constData(), 4);
	_isModified = true;
}

QByteArray InfFile::unknown2() const
{
	return QByteArray((char *)data.u2, 24);
}

void InfFile::setUnknown2(const QByteArray &u)
{
	memcpy(data.u2, u.leftJustified(24, '\0', true).constData(), 24);
	_isModified = true;
}
