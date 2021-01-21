#include "KRecordPlot.h"
#include <QtGui>

#include <QPainter>

#include "verdigris/wobjectimpl.h"
#include <iostream>
#include "libsamplerate/samplerate.h"
#include "KWidget.h"

W_OBJECT_IMPL(KRecordPlot)

//! [0]
KRecordPlot::KRecordPlot(QWidget* parent)
	: QOpenGLWidget(parent),
    buffer_float(nullptr),
    len_buffer(0)
{
	spectrWidth = 450;
	//elapsed = 0;
	//setFixedSize(spectrWidth, spectrHeight);
	setMinimumWidth(spectrWidth);
	setFixedHeight(spectrHeight);
	setAutoFillBackground(false);
	buffer.resize(1);
	buffer.push_back(0);
	peak_max_buffer.resize(s["CHANNEL"], std::vector<short>(0));
	peak_min_buffer.resize(s["CHANNEL"], std::vector<short>(0));
}


KRecordPlot::~KRecordPlot(){
  if(len_buffer!=0)
    delete[] buffer_float;

}

void KRecordPlot::DrawPlot(short* temp,int len)
{	
	//todo
	//split with channel function
  
  if(len_buffer ==0){
	  buffer_float = (float*)calloc(len, sizeof(float));
    len_buffer = len;
  }
  else if(len_buffer != len){
    delete[] buffer_float;
	  buffer_float = (float*)calloc(len, sizeof(float));
    len_buffer = len;
  
  }

	src_short_to_float_array(temp, buffer_float, len);
	/*
	for (int i = 0; i < len; i++) {
		std::cout << temp[i] << std::endl;
	}
	*/
	
	//code with draw all data
	/*
	//elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval() / 50) % 1000;
	buffer.resize(writtensize + len);
	//memcpy(&(buffer[writtensize]), temp, len * sizeof(short));
	for (int i = 0; i < len; i++) {
		buffer.push_back(getDataForCoordinates(temp[i]));
		//std::cout << getDataForCoordinates(0) << std::endl;
	}
	writtensize += len;
	*/

	//code with rms
	//writtensize += 1;
	//buffer.resize(writtensize);
	//buffer.push_back(getDataForCoordinatesWithRMS(temp_float, len));
	//delete[] temp_float;
	drawPeakPlot(temp, len);
	update();
}

void KRecordPlot::DrawPlot(double** raw)
{
  //input raw demension
  //raw[ch][frame_size]
  if(cnt++>8){
  drawPeakPlot(raw);
  update();
  cnt=0;
  }
}

void KRecordPlot::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);
	painter.fillRect(event->rect(), QBrush(QColor(255, 255, 255)));
  for (int i = 0; i < buffer.size()-1; i++)
	{
      //double data = getDataForCoordinates(ic, ir); // your function
			//QColor color = getColorForData(data);  // your function
      painter.setPen(QColor(0,0,255));
			//painter.drawPoint(i, buffer[i]);
			//painter.drawLine(int(i/1000.0), buffer.at(i), int((i+1)/1000.0), buffer.at(i+1));
			painter.drawLine((i-1)/2, buffer.at(i), i/2, buffer.at(i + 1));
			//std::cout << int(i / 10.0) << "," << int((i+1) / 10.0) << std::endl;
	}
	painter.setRenderHint(QPainter::Antialiasing);
	painter.end();
}

void KRecordPlot::resizeEvent(QResizeEvent* e) {
	//spectrWidth = this->width;
}

short KRecordPlot::getDataForCoordinates(short x) {
	return short((32767 - x) / 65535.0 * spectrHeight*10 - (spectrHeight * 5 - spectrHeight/2));
}


float KRecordPlot::rms(float* data, int len) {
	float RMS = 0;
	for (int a = 0; a < len; a++) {
		RMS += data[a] * data[a];
	}
	return sqrt(RMS / len);
}

short KRecordPlot::getDataForCoordinatesWithRMS(float* data, int len) {
	float RMS = rms(data, len);
	return getDataForCoordinates(short(RMS * 65535));
}

void KRecordPlot::drawPeakPlot(short* temp, int len) {
	//channel split
	for (int channel = 0; channel < s["CHANNEL"]; channel++) {
		peak_max_buffer[channel].push_back(-32768);
		peak_min_buffer[channel].push_back(32767);
		int cur = peak_max_buffer[channel].size()-1;
		
		for (int shift = 0; shift < s["SHIFT_SIZE"]; shift++) {
			if (temp[shift * s["CHANNEL"] + channel] > peak_max_buffer[channel][cur]) peak_max_buffer[channel][cur] = temp[shift * s["CHANNEL"] + channel];
			if (temp[shift * s["CHANNEL"] + channel] < peak_min_buffer[channel][cur]) peak_min_buffer[channel][cur] = temp[shift * s["CHANNEL"] + channel];
		}
	}
	//std::cout << peak_max_buffer[0].size() << std::endl;
	//get peak 

	//put into peakbuffer
	int blocksize = 3;
	if (peak_max_buffer[0].size()==blocksize-1) {
		short max = *max_element(peak_max_buffer[0].begin(), peak_max_buffer[0].end());
		short min = *min_element(peak_min_buffer[0].begin(), peak_min_buffer[0].end());
		buffer.push_back(getDataForCoordinates(max));
		buffer.push_back(getDataForCoordinates(min));
		for (int i = 0; i < blocksize; i++) {
			peak_max_buffer[0].pop_back();
			peak_min_buffer[0].pop_back();
		}
		peak_max_buffer.resize(s["CHANNEL"], std::vector<short>(0));
		peak_min_buffer.resize(s["CHANNEL"], std::vector<short>(0));
	}
	
}

void KRecordPlot::drawPeakPlot(double** raw){
  for (int channel = 0; channel < s["CHANNEL"]; channel++) {
		peak_max_buffer[channel].push_back(-32768);
		peak_min_buffer[channel].push_back(32767);
		int cur = peak_max_buffer[channel].size()-1;
		
    //read one by one and set peak_max and peak_min for shift
		for (int shift = 0; shift < s["SHIFT_SIZE"]; shift++) {
			if (readOneFromRaw(raw, channel, shift) > peak_max_buffer[channel][cur]) peak_max_buffer[channel][cur] = readOneFromRaw(raw, channel, shift);
			if (readOneFromRaw(raw, channel, shift) < peak_min_buffer[channel][cur]) peak_min_buffer[channel][cur] = readOneFromRaw(raw, channel, shift);
		}
	}
  
  int channeladdToDrawbuffer = 0;
  addToDrawBuffer(channeladdToDrawbuffer, peak_max_buffer[channeladdToDrawbuffer].back(), peak_min_buffer[channeladdToDrawbuffer].back());
  peak_max_buffer[channeladdToDrawbuffer].pop_back();
  peak_min_buffer[channeladdToDrawbuffer].pop_back();
}

double KRecordPlot::readOneFromRaw(double** raw, int channel, int idx){
  //readOneShortDataFromRaw
  //Raw data structure
  //0 0 0 0
  //0 0 0 1
  //0 0 1 2
  //0 1 2 3
  //1 2 3 4
  //Raw size has [channel][frame size]
  //this function return one short data from last shift by idx
  
  return raw[channel][((s["FRAME_SIZE"]/s["SHIFT_SIZE"])-1)*s["SHIFT_SIZE"]+idx];
}

void KRecordPlot::addToDrawBuffer(int channel, int max, int min){
 	if(buffer.size()>spectrWidth*2){
	//if(buffer.size()>width()*2){
		buffer.pop_front();
		buffer.pop_front();
	}
  
	buffer.push_back(getDataForCoordinates(max));
	buffer.push_back(getDataForCoordinates(min));	 
}


void KRecordPlot::Reload(){
 s = ConfigParam();
}
