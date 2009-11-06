/*
 *  DarkStyle.h
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

#include <QPlastiqueStyle>
#include <QPalette>

class DarkStyle : public QPlastiqueStyle
{ 
public:
	
			DarkStyle();
	virtual ~DarkStyle();
	
	virtual void polish(QApplication *application);
	virtual void unpolish(QApplication *application);
	virtual void drawControl(ControlElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0) const;
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
	virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = 0) const;
	
private:
	
	QPalette _oldPalette;
};
