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

class ReaderWriterILDA
{
public:

			ReaderWriterILDA();
	virtual ~ReaderWriterILDA();

	bool	readFile(const QString& fileName);


private:

	bool	readFrameSection(QDataStream& stream, bool is3DFrame);
	void	readHeader(QDataStream& stream, QString& name, 
					   QString& companyName, quint16& entryCount, 
					   quint16& objectNumber, quint16& objectCount);
};
