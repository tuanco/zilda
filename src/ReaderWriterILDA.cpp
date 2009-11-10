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

#include <list>
#include <algorithm>

//=======================================================================================

ReaderWriterILDA::ReaderWriterILDA()
{
	_currentPalette.clear();									  
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

			quint32 formatType;
			stream >> formatType;

			if (!_ildaVersion.contains(formatType))
				_ildaVersion.append(formatType);

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
					// Color Table
					eof = readColorTable(stream);
					break;

				case 3:
					// True Color
					eof = readTrueColorSection(stream);
					break;
					
				case 4:
					eof = readFormat5(stream, true);
					break;

				case 5:
					eof = readFormat5(stream, false);
					break;
					
				default:
					// Unknow section type
					{
						qint32 dataLength;
						qint32 numberOfPoints;
						
						stream >> dataLength >> numberOfPoints;
						stream.skipRawData(dataLength);
					}
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
	QSharedPointer<Frame> frame = QSharedPointer<Frame>(new Frame(_sequence, is3DFrame));
	frame->setFrameNr(_sequence->frameCount());

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
		
		Point p(QPointF(x, y), z, QColor(), (stateByte & 64) == 64, colorIndex);
	
		if (_currentPalette.count() > 0)
			p.setColor(_currentPalette.at(i));

		frame->addPoint(p);
	}

	if (entryCount > 0)
		_sequence->addFrame(frame);

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

//=======================================================================================

bool ReaderWriterILDA::readFormat5(QDataStream& stream, bool is3DFrame)
{
	QString name, companyName;
	quint16 entryCount, objectNumber, objectCount;
	QSharedPointer<Frame> frame = QSharedPointer<Frame>(new Frame(_sequence, is3DFrame));
	frame->setFrameNr(_sequence->frameCount());
	
	readHeader(stream, name, companyName, entryCount, objectNumber, objectCount);
	
	for (quint16 i=0; i<entryCount; ++i)
	{
		qint16 x, y, z;
		quint8 r, g, b, state;

		stream >> x >> y;

		if (is3DFrame)
			stream >> z;
		else
			z = 0;

		stream >> state >> b >> g >> r;
		
		Point p(QPointF(x, y), z, QColor(r, g, b), (state & 64) == 64);
		frame->addPoint(p);
	}
	
	if (entryCount > 0)
		_sequence->addFrame(frame);
	
	return entryCount == 0;
}

//=======================================================================================

bool ReaderWriterILDA::readColorTable(QDataStream& stream)
{
	QVector<QColor> pal;
	char name_[9], companyName_[9];
	quint16 totalColors, paletteNumber;

	name_[8] = '\0';
	companyName_[8] = '\0';

	stream.readRawData(name_, 8);
	stream.readRawData(companyName_, 8);
	stream >> totalColors;
	stream >> paletteNumber;
	stream.skipRawData(4);

	pal.reserve(totalColors);

	for (quint16 i=0; i<totalColors; ++i)
	{
		quint8 r, g, b;

		stream >> r >> g >> b;

		pal.append(QColor(r, g, b));
	}

	_currentPalette = pal;

	return false;
}

//=======================================================================================

QString ReaderWriterILDA::version() const
{
	QList<int> tmp(_ildaVersion);
	QString retValue("");

	qSort(tmp.begin(), tmp.end());

	foreach (int v, tmp)
	{
		retValue.append(QString::number(v) + " ");
	}

	return retValue;
}
