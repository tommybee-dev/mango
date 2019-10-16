#pragma once

#undef __GXX_ABI_VERSION
#define __GXX_ABI_VERSION 1008

#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include <wx/wx.h>
#include <list>
#include <string>

class CMango
{

public:
	enum type{
		IM_AUTOMATIC,
		IM_GRAYSCALE,
		IM_BLACKNWHITE,
		IM_RGB,
		IM_HSV,
		IM_HLS,
		IM_YCrCb,
		IM_ZOOMIN,
		IM_ZOOMOUT,
		IM_HORIZONTAL = 25,
		IM_VERTICAL,
		IM_DILATE = 40,
		IM_ERODE,
		IM_FACE1,
	};

	CMango(void);
	~CMango(void);

	int LoadImageFromFile(char *FileName);
	void ShowActiveImage(char *WindowName);
	int SetProperty();
	std::string GetProperty();
	void LoadPrestoredImage(int type);
	void PushImage(IplImage *Image){ImageList.push_front(Image);};
	void PopImage(){ if (!ImageList.empty()) ImageList.pop_front();};
	int SaveToFile(char *FileName);

	void Rotate(int angle);
	void ZoomImage(int flag);
	int GetImageElement(int channel, int brightness, int contrast);
	void FlipImage(int flag);
	void ApplyEffects(int flag);
	void FaceDetect(int flag);
	void ShowHistogram();
	int HistogramEqualization();
	int ChangeColorSpace(int flag);

private:
	static IplImage *Automatic;
	static IplImage *Blacknwhite;
	static IplImage *Grayscale;
	static IplImage *Buf;

	static std::list<IplImage *> ImageList;
	static std::list<std::string> PropertyList;

	static int ColorSpace;
};
