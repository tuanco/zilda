/*
 *  ReadWriterILDA.h
 *  zILDA
 *
 *  Created by Andre Normann on 06.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

// Qt
#include <QtCore>

// Project
#include "Sequence.h"


class ReaderWriterILDA
{
public:

			ReaderWriterILDA();
	virtual ~ReaderWriterILDA();

	SequenceRef readFile(const QString& fileName);
	bool		writeFile(const QString& fileName, SequenceRef sequence);

	QString		version() const;


private:
	
	bool	readFormat5(QDataStream& stream, bool is3DFrame);
	bool	readTrueColorSection(QDataStream& stream);
	bool	readFrameSection(QDataStream& stream, bool is3DFrame);
	bool	readColorTable(QDataStream& stream);
	void	readHeader(QDataStream& stream, QString& name, 
					   QString& companyName, quint16& entryCount, 
					   quint16& objectNumber, quint16& objectCount);

	SequenceRef		_sequence;
	QVector<QColor>	_currentPalette;
	QList<int>		_ildaVersion;
};
