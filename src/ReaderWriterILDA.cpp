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

//=======================================================================================

ReaderWriterILDA::ReaderWriterILDA()
{
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

			if (qstrcmp(signature, "ILDA"))
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

		frame->addPoint(x, y, z);
	}

	if (entryCount > 0)
		_sequence->addFrame(frame);
	else
		delete frame;

	return entryCount == 0;
}

//=======================================================================================
