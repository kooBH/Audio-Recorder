
#include <QtGui>
#include <QPainter>
#include <iostream>
#include "KRecordPlot.h"

KRecordPlotRec::KRecordPlotRec(QWidget* parent):QOpenGLWidget(parent){

  //elapsed = 0;
	//setFixedSize(spectrWidth, spectrHeight);
  spectrWidth = 400;
	setFixedWidth(spectrWidth);

	setFixedHeight(spectrHeight);
	setAutoFillBackground(false);
	middleLine = QPen(QColor("BLUE"));
  ResetShiftSize();
	bytes.resize(shift_size*8);//frame_size * cnt
	std::fill(bytes.begin(), bytes.end(), 0);

	nPoints = (int)(spectrWidth/5);
	if (nPoints < BAR_MIN_POINTS)
		nPoints = BAR_MIN_POINTS;

	mBarWidth = -1;
	nBatchCount = 0;
	//setAnimationSpeed(mAnimSpeed);

	// = new Rect();

	mSrcY.resize(nPoints);
	mDestY.resize(nPoints);
	buffer.resize(mSrcY.size());

  middleLine.setColor(QColor("lightblue"));
}

void KRecordPlotRec::ResetShiftSize(){
	ConfigParam s("../config.json");
  shift_size = s["SHIFT_SIZE"];
  bytes.clear();
	bytes.resize(shift_size*8);//frame_size * cnt
	std::fill(bytes.begin(), bytes.end(), 0);

}


void KRecordPlotRec::setDensity(float _density) {
	if (_density > 180) {
		middleLine.setWidth(1);
		gap = 1;
	}
	else {
		gap = 4;
	}
	density = _density;
	if (_density > 256) {
		density = 250;
		gap = 0;
	}
	else if (_density <= 10) {
		density = 10;
	}
}

KRecordPlotRec::~KRecordPlotRec(){
   
}



void KRecordPlotRec::DrawPlot(short* raw)
{

	//input raw demension
	//raw[ch][frame_size]
	if (cnt++ > 6) {
	bytes.insert(bytes.end(), raw, raw + shift_size);
  
	update();
	cnt = 0;
	}
}


void KRecordPlotRec::paintEvent(QPaintEvent* event){
	QPainter painter;
	painter.begin(this);
  painter.setPen(middleLine);

	int w = width();

		if (bytes.size() != 0 ) {
			
			painter.fillRect(event->rect(), QBrush(QColor(0, 0, 0)));
			float barWidth = float(w) / density;
			float div = bytes.size() / density;
			middleLine.setWidth(1);

      //draw middle line
			middleLine.setWidth(barWidth - gap);

			for (int i = 0; i < density; i++) {
				int bytePosition = (int)ceil(i * div);
        int top = height() - 3.5*(abs(bytes[bytePosition]))* (height()) / scale_plot;

				float barX = (i * barWidth) + (barWidth / 2);
			painter.drawLine(barX, top, barX, height());
			}
			bytes.resize(0);
		}

	//painter.drawLine(i*5, 0, i*5, buffer.at(i));

	painter.setRenderHint(QPainter::Antialiasing);
	painter.end();
}


void KRecordPlotRec::resizeEvent(QResizeEvent* e) {
	//spectrWidth = this->width;
}

