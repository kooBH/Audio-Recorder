#ifndef _H_KRECORDPLOT_
#define _H_KRECORDPLOT_

#include <QOpenGLWidget>
#include "verdigris/wobjectdefs.h"

#include <deque>
#include "util/Config.h"

class KRecordPlot : public QOpenGLWidget
{
	//Q_OBJECT
	W_OBJECT(KRecordPlot)

	ConfigParam s;
public:
	KRecordPlot(QWidget* parent=nullptr);
	~KRecordPlot();
	int spectrWidth = 500;
	const int spectrHeight = 100;
	long writtensize = 0;
	//short* buffer = (short*)calloc(1,sizeof(short));
	std::deque<short> buffer;
	std::vector<std::vector<short> > peak_max_buffer;
	std::vector<std::vector<short> > peak_min_buffer;
	void DrawPlot(short* temp,int len);
  void DrawPlot(double** raw);

  int cnt=0;

public slots:
	//void DrawPlot(short* temp,int len);
  //W_SLOT(DrawPlot,(short*,int))
protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* e) override;
	short getDataForCoordinates(short x);
	float rms(float* data, int len);
	short getDataForCoordinatesWithRMS(float* data, int len);
	void drawPeakPlot(short* temp, int len);
  void drawPeakPlot(double** raw);
  double readOneFromRaw(double** raw, int channel, int idx);
  void addToDrawBuffer(int channel, int max, int min);
  void Reload();

  float* buffer_float;
  int len_buffer;
};

#endif
