
#include <QtGui>
#include <QPainter>
#include <iostream>
#include "KRecordPlot.h"

KRecordPlotRec::KRecordPlotRec(QWidget* parent) :QOpenGLWidget(parent) {

  //elapsed = 0;
  //setFixedSize(spectrWidth, spectrHeight);
  spectrWidth = 400;
  setFixedWidth(spectrWidth);

  setFixedHeight(spectrHeight);
  setAutoFillBackground(false);
  ResetShiftSize();
  bytes.resize(shift_size * 8);//frame_size * cnt
  std::fill(bytes.begin(), bytes.end(), 0);

  nPoints = (int)(spectrWidth / 5);
  if (nPoints < BAR_MIN_POINTS)
    nPoints = BAR_MIN_POINTS;

  mBarWidth = -1;
  nBatchCount = 0;
  //setAnimationSpeed(mAnimSpeed);

  // = new Rect();

  mSrcY.resize(nPoints);
  mDestY.resize(nPoints);
  buffer.resize(mSrcY.size());

  clippingLine.setColor(QColor("red"));
  plotLine.setColor(QColor("lightblue"));

  plotLine.setWidth(1);
  clippingLine.setWidth(1);
}

void KRecordPlotRec::ResetShiftSize() {
  jsonConfig s("../config.json", "param");
  shift_size = s["shift_size"];
  bytes.clear();
  bytes.resize(shift_size * 8);//frame_size * cnt
  std::fill(bytes.begin(), bytes.end(), 0);

}


void KRecordPlotRec::setDensity(float _density) {
  if (_density > 180) {
    plotLine.setWidth(1);
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

KRecordPlotRec::~KRecordPlotRec() {

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


void KRecordPlotRec::paintEvent(QPaintEvent* event) {
  QPainter painter;
  painter.begin(this);
  painter.setPen(plotLine);

  int w = width();
  double tmp = 0;

  if (bytes.size() != 0) {

    painter.fillRect(event->rect(), QBrush(QColor(0, 0, 0)));
    float barWidth = float(w) / density;
    float div = bytes.size() / density;

    //draw middle line
    plotLine.setWidth(barWidth - gap);

    for (int i = 0; i < bytes.size(); i++) {
      if (abs(bytes[i]) == SHRT_MAX) {
        painter.setPen(clippingLine);
        signal_update_status("WARNING::Clipping detected!");
      }
    }


    for (int i = 0; i < density; i++) {
      int bytePosition = (int)ceil(i * div);

      //int top = height() - 3.5*(abs(bytes[bytePosition]))* (height()) / scale_plot;
      tmp = std::log(alpha*abs(bytes[bytePosition])+1)/std::log(alpha*scale_plot+1);
      int top = static_cast<int>((1.0-tmp)*height());

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

