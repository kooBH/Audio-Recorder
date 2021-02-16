
#ifndef _H_KRECORDPLOTREC_
#define _H_KRECORDPLOTREC_


#include <QOpenGLWidget>
#include <QPen>
#include "jsonConfig.h"
#include <deque>
//#include "KRecordPlot.h"
class KRecordPlotRec : public QOpenGLWidget
{
	Q_OBJECT

  //default 32768
  const int scale_plot = 2000;

	int BAR_MAX_POINTS = 120;
	int BAR_MIN_POINTS = 3;
	int nPoints;
	int mBarWidth;
	int nBatchCount;
	int mRandom;
	int density = 100;
	int gap = 2;
	int cnt = 0;
	std::vector<float> mSrcY;
	std::vector<float> mDestY;
	//float* mSrcY;
	//float* mDestY;
	int mMaxBatchCount = 10;

  int shift_size;

public:
	QPen middleLine;
	KRecordPlotRec(QWidget* parent=nullptr);
	~KRecordPlotRec();
	void DrawPlot(short* raw);

	int spectrWidth = 400;
	const int spectrHeight = 100;
	long writtensize = 0;
	void setDensity(float _density);
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* e) override;
	std::deque<short> buffer;
	std::vector<short> bytes;
	std::vector<std::vector<short> > peak_min_buffer;

  void ResetShiftSize();
};

#endif
