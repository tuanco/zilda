/*
 *  ReadWriterILDA.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 06.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Project
#include "ReaderWriterILDA.h"
#include "Frame.h"
#include "Point.h"

//=======================================================================================

ReaderWriterILDA::ReaderWriterILDA()
{
	_currentPalette.clear();
	
	_defaultIldaPalette.reserve(64);
	_defaultIldaPalette.append(QColor(255, 0,   0));
	_defaultIldaPalette.append(QColor(255, 16,  0));
	_defaultIldaPalette.append(QColor(255, 32,  0));
	_defaultIldaPalette.append(QColor(255, 48,  0));
	_defaultIldaPalette.append(QColor(255, 64,  0));
	_defaultIldaPalette.append(QColor(255, 80,  0));
	_defaultIldaPalette.append(QColor(255, 96,  0));
	_defaultIldaPalette.append(QColor(255, 112, 0));
	_defaultIldaPalette.append(QColor(255, 128, 0));
	_defaultIldaPalette.append(QColor(255, 144, 0));
	_defaultIldaPalette.append(QColor(255, 160, 0));
	_defaultIldaPalette.append(QColor(255, 176, 0));
	_defaultIldaPalette.append(QColor(255, 192, 0));
	_defaultIldaPalette.append(QColor(255, 208, 0));
	_defaultIldaPalette.append(QColor(255, 224, 0));
	_defaultIldaPalette.append(QColor(255, 240, 0));
	_defaultIldaPalette.append(QColor(255, 255, 0));
	_defaultIldaPalette.append(QColor(224, 255, 0));
	_defaultIldaPalette.append(QColor(192, 255, 0));
	_defaultIldaPalette.append(QColor(160, 255, 0));
	_defaultIldaPalette.append(QColor(128, 255, 0));
	_defaultIldaPalette.append(QColor(96,  255, 0));
	_defaultIldaPalette.append(QColor(64,  255, 0));
	_defaultIldaPalette.append(QColor(32,  255, 0));
	_defaultIldaPalette.append(QColor(0,   255, 0));
	_defaultIldaPalette.append(QColor(0,   255, 32));
	_defaultIldaPalette.append(QColor(0,   255, 64));
	_defaultIldaPalette.append(QColor(0,   255, 96));
	_defaultIldaPalette.append(QColor(0,   255, 128));
	_defaultIldaPalette.append(QColor(0,   255, 160));
	_defaultIldaPalette.append(QColor(0,   255, 192));
	_defaultIldaPalette.append(QColor(0,   255, 224));
	_defaultIldaPalette.append(QColor(0,   130, 255));
	_defaultIldaPalette.append(QColor(0,   114, 255));
	_defaultIldaPalette.append(QColor(0,   104, 255));
	_defaultIldaPalette.append(QColor(10,  96,  255));
	_defaultIldaPalette.append(QColor(0,   82,  255));
	_defaultIldaPalette.append(QColor(0,   74,  255));
	_defaultIldaPalette.append(QColor(0,   64,  255));
	_defaultIldaPalette.append(QColor(0,   32,  255));
	_defaultIldaPalette.append(QColor(0,   0,   255));
	_defaultIldaPalette.append(QColor(32,  0,   255));
	_defaultIldaPalette.append(QColor(64,  0,   255));
	_defaultIldaPalette.append(QColor(96,  0,   255));
	_defaultIldaPalette.append(QColor(128, 0,   255));
	_defaultIldaPalette.append(QColor(160, 0,   255));
	_defaultIldaPalette.append(QColor(192, 0,   255));
	_defaultIldaPalette.append(QColor(224, 0,   255));
	_defaultIldaPalette.append(QColor(255, 0,   255));
	_defaultIldaPalette.append(QColor(255, 32,  255));
	_defaultIldaPalette.append(QColor(255, 64,  255));
	_defaultIldaPalette.append(QColor(255, 96,  255));
	_defaultIldaPalette.append(QColor(255, 128, 255));
	_defaultIldaPalette.append(QColor(255, 160, 255));
	_defaultIldaPalette.append(QColor(255, 192, 255));
	_defaultIldaPalette.append(QColor(255, 224, 255));
	_defaultIldaPalette.append(QColor(255, 255, 255));
	_defaultIldaPalette.append(QColor(255, 224, 224));
	_defaultIldaPalette.append(QColor(255, 255, 255));
	_defaultIldaPalette.append(QColor(255, 160, 160));
	_defaultIldaPalette.append(QColor(255, 128, 128));
	_defaultIldaPalette.append(QColor(255, 96,  96));
	_defaultIldaPalette.append(QColor(255, 64,  64));
	_defaultIldaPalette.append(QColor(255, 32,  329));
									  
}

//=======================================================================================

ReaderWriterILDA::~ReaderWriterILDA()
{
}

//=======================================================================================

Sequence* ReaderWriterILDA::readFile(const QString& fileName)
{
	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly))
	{
		bool eof = false;
		QDataStream stream(&file);
		stream.setByteOrder(QDataStream::BigEndian);

		_sequence = new Sequence();

		while (!stream.atEnd() && !eof)
		{
			char signature[4];

			stream.readRawData(signature, 4);

			if (memcmp(signature, "ILDA", 4))
			{
				file.close();
				return 0L;
			}

			stream.skipRawData(3);

			quint8 formatType;
			stream >> formatType;

			switch (formatType)
			{
				case 0:
					// 3D Frame
					eof = readFrameSection(stream, true);
					break;
					
				case 1:
					// 2D Frame
					eof = readFrameSection(stream, false);
					break;

				case 2:
					// Palette
					break;

				case 3:
					// True Color
					eof = readTrueColorSection(stream);
					break;
			}

		}

		file.close();
	}

	return _sequence;
}

//=======================================================================================

void ReaderWriterILDA::readHeader(QDataStream& stream, QString& name, 
								  QString& companyName, quint16& entryCount, 
								  quint16& objectNumber, quint16& objectCount)
{
	char name_[9], companyName_[9];

	name_[8] = '\0';
	companyName_[8] = '\0';

	stream.readRawData(name_, 8);
	stream.readRawData(companyName_, 8);
	stream >> entryCount;
	stream >> objectNumber;
	stream >> objectCount;

	stream.skipRawData(2);

	name = name_;
	companyName = companyName_;
}

//=======================================================================================

bool ReaderWriterILDA::readFrameSection(QDataStream& stream, bool is3DFrame)
{
	QString name, companyName;
	quint16 entryCount, objectNumber, objectCount;
	Frame *frame = new Frame();

	readHeader(stream, name, companyName, entryCount, objectNumber, objectCount);

	for (quint16 i=0; i<entryCount; ++i)
	{
		qint16 x, y, z;
		quint8 stateByte, colorIndex;

		stream >> x >> y;

		if (is3DFrame)
			stream >> z;
		else
			z = 0;

		stream >> stateByte >> colorIndex;
		
		Point p(QPointF(x, y), z, Qt::white, (stateByte & 64) == 64);
	
		if (_currentPalette.count() > 0)
			p.setColor(_currentPalette.at(i));
		else if (colorIndex < _defaultIldaPalette.count())
			p.setColor(_defaultIldaPalette.at(colorIndex));

		frame->addPoint(p);
	}

	if (entryCount > 0)
		_sequence->addFrame(frame);
	else
		delete frame;

	return entryCount == 0;
}

//=======================================================================================

bool ReaderWriterILDA::readTrueColorSection(QDataStream& stream)
{
	quint32 dataLength, numberOfPoints;
	
	stream >> dataLength >> numberOfPoints;
	
	_currentPalette.clear();
	_currentPalette.reserve(numberOfPoints);
	
	for (quint32 i=0; i<numberOfPoints; ++i)
	{
		quint8 r, g, b;
		
		stream >> r >> g >> b;
		
		_currentPalette.append(QColor(r, g, b));
	}
	
	return numberOfPoints == 0;
}

