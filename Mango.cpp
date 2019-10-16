#include "Mango.h"

IplImage *CMango::Automatic;
IplImage *CMango::Blacknwhite;
IplImage *CMango::Grayscale;
IplImage *CMango::Buf;
std::list<IplImage *> CMango::ImageList;
std::list<std::string> CMango::PropertyList;
int CMango::ColorSpace;

CMango::CMango(void)
{
	ColorSpace = IM_RGB;
}

CMango::~CMango(void)
{
}

void CMango::ShowActiveImage(char *WindowName)
{
	//Create window  of image
	if (!ImageList.empty()){
 		cvNamedWindow(WindowName, 1);
		cvShowImage(WindowName, ImageList.front());
	}
}

int CMango::LoadImageFromFile(char* FileName)
{
	//Init!
	//Releasing NULL pointer is OK, so no need to pre-check!
	cvReleaseImage(&Automatic);
	cvReleaseImage(&Grayscale);
	cvReleaseImage(&Blacknwhite);
	ImageList.clear();
	PropertyList.clear();
	cvDestroyAllWindows();

	//Normal image
	Automatic = cvLoadImage(FileName, -1);
	if(!Automatic)
		return -1;

	//Grayscale image
	Grayscale = cvLoadImage(FileName, 0);
	if(!Grayscale)
		return -1;

	//Black and white image
	Blacknwhite = cvCreateImage(cvSize(Automatic->width, Automatic->height), 8, 1);
	cvAdaptiveThreshold(Grayscale, Blacknwhite, 0xFF);

	//Always set buf equals to active image
	Buf = cvCloneImage(Automatic);

	LoadPrestoredImage(IM_AUTOMATIC);
	ShowActiveImage(FileName);
	return 0;
}

void CMango::LoadPrestoredImage(int type)
{
	if (type == IM_AUTOMATIC)
		ImageList.push_front(Automatic);
	else if (type == IM_BLACKNWHITE)
		ImageList.push_front(Blacknwhite);
	else if (type == IM_GRAYSCALE)
		ImageList.push_front(Grayscale);

	//Remeber to set buf equals to active image
	Buf = cvCloneImage(ImageList.front());
}

int CMango::SaveToFile(char *FileName)
{
		if (ImageList.empty())
			return -1;
		return cvSaveImage(FileName, ImageList.front());
}

int CMango::SetProperty()
{
	//Put property data into a list
	if (ImageList.empty())
		return -1;

	char buf[64];
	sprintf(buf, "Size: [%i]\n", ImageList.front()->imageSize);
	PropertyList.push_back(buf);

	sprintf(buf, "Size (hxw): [%ix%i]\n", ImageList.front()->height, ImageList.front()->width);
	PropertyList.push_back(buf);

	sprintf(buf, "Alphachannel: [%i]\n", ImageList.front()->alphaChannel);
	PropertyList.push_back(buf);

	sprintf(buf, "Number of channel: [%i]\n", ImageList.front()->nChannels);
	PropertyList.push_back(buf);

	sprintf(buf, "Width step: [%i]\n", ImageList.front()->widthStep);
	PropertyList.push_back(buf);

	sprintf(buf, "Channel sequence: [%c|%c|%c|%c]\n", ImageList.front()->channelSeq[0],
		ImageList.front()->channelSeq[1], ImageList.front()->channelSeq[2], ImageList.front()->channelSeq[3]);
	PropertyList.push_back(buf);

	sprintf(buf, "Color model: [%c|%c|%c|%c]\n", ImageList.front()->colorModel[0], ImageList.front()->colorModel[1],
		ImageList.front()->colorModel[2], ImageList.front()->colorModel[3]);
	PropertyList.push_back(buf);

	sprintf(buf, "Depth: [%i]\n", ImageList.front()->depth);
	PropertyList.push_back(buf);

	return 0;

}

std::string CMango::GetProperty()
{
	//Take property data from the list
	std::string retval = "";
	if (!PropertyList.empty()){
		retval = PropertyList.front();
		PropertyList.pop_front();
	}
	return retval;
 }

void CMango::Rotate(int angle)
{
	//E-mail: qchen@discover.uottawa.ca
	//DiscoverLab
	//School of Information Technology & Engineering
	//University of Ottawa

	IplImage *src = ImageList.front();
	IplImage *dst = cvCloneImage(src);

	float m[6];
    CvMat M = cvMat(2, 3, CV_32F, m);
    int w = src->width;
    int h = src->height;

	m[0] = (float)(cos(-angle*2*CV_PI/180.));
    m[1] = (float)(sin(-angle*2*CV_PI/180.));
    m[3] = -m[1];
    m[4] = m[0];
    m[2] = w*0.5f;
    m[5] = h*0.5f;

    cvGetQuadrangleSubPix(src, dst, &M);
	ImageList.push_front(dst);
}

void CMango::ZoomImage(int flag)
{
	// Zooming is done by scaling the image
	if (flag == IM_ZOOMIN){
		IplImage *src = ImageList.front();
		IplImage *dst = cvCreateImage(cvSize(src->width+20, src->height+20), src->depth, src->nChannels);

		cvResize(src, dst, CV_INTER_CUBIC);
		ImageList.push_front(dst);
	}

	else if (flag == IM_ZOOMOUT){
		IplImage *src = ImageList.front();
		IplImage *dst = cvCreateImage(cvSize(src->width-20, src->height-20), src->depth, src->nChannels);

		cvResize(src, dst, CV_INTER_CUBIC);
		ImageList.push_front(dst);
	}
}

int CMango::GetImageElement( int channel, int brightness, int contrast)
{

	IplImage *src = ImageList.front();
	IplImage *src_ = cvCloneImage(src);
	if (channel < src->nChannels){

		// First, convert to target color space
		if (ColorSpace == IM_HSV){
			cvCvtColor(src, src_, CV_BGR2HSV);
		}
		else if (ColorSpace == IM_HLS){
			cvCvtColor(src, src_, CV_BGR2HLS);
		}
		else if (ColorSpace == IM_YCrCb){
			cvCvtColor(src, src_, CV_BGR2YCrCb);
		}

		// Second, do manipulation
		IplImage *dst = cvCreateImage(cvSize(src->width, src->height), src->depth, 1);
		cvZero(dst);

		// Extract single channel, you can also use cvSplit built-in function!
		for (int i = 0; i < src_->height; i++){
			for (int j = 0; j < src_->width;j++)
				(dst->imageData + i*dst->widthStep)[j*dst->nChannels + 0] =
					((uchar *)(Buf->imageData + i*Buf->widthStep))[j*Buf->nChannels + channel]; // B
		}

		// Brigthness and contrast adjustment
		/*
		 * The algorithm is by Werner D. Streidt
		 * (http://visca.com/ffactory/archives/5-99/msg00021.html)
		 */
		uchar lut[256];
		CvMat* lut_mat;
		IplImage *dst_ = cvCloneImage(dst);
		IplImage *dst__ = cvCloneImage(src_);

		lut_mat = cvCreateMatHeader(1, 256, CV_8UC1);
		cvSetData( lut_mat, lut, 0 );
		brightness = brightness - 100;
		contrast = contrast - 100;

   		if (contrast > 0)
		{
			double delta = 127.*contrast/100;
			double a = 255./(255. - delta*2);
			double b = a*(brightness - delta);
			for (int i = 0; i < 256; i++)
			{
				int v = cvRound(a*i + b);
				if (v < 0)
					v = 0;
				if (v > 255)
					v = 255;
				lut[i] = (uchar)v;
			}
		}
		else
		{
			double delta = -128.*contrast/100;
			double a = (256.-delta*2)/255.;
			double b = a*brightness + delta;
			for (int i = 0; i < 256; i++)
			{
				int v = cvRound(a*i + b);
				if (v < 0)
					v = 0;
				if (v > 255)
					v = 255;
				lut[i] = (uchar)v;
			}
		}

		cvLUT(dst, dst_, lut_mat);

		//Write back the modified channel
		for (int i = 0; i < src->height; i++){
			for (int j = 0; j < src->width;j++)
				(dst__->imageData + i*dst__->widthStep)[j*dst__->nChannels + channel] =
					((uchar *)(dst_->imageData + i*dst_->widthStep))[j*dst_->nChannels];
		}

		// Third, convert to back RGB to be showed
		IplImage *dst___ = cvCloneImage(dst__);
		if (ColorSpace == IM_HSV)
		cvCvtColor(dst__, dst___, CV_HSV2BGR);
		else if (ColorSpace == IM_HLS)
			cvCvtColor(dst__, dst___, CV_HLS2BGR);
		else if (ColorSpace == IM_YCrCb)
			cvCvtColor(dst__, dst___, CV_YCrCb2BGR);

		ImageList.pop_back(); //This part doesn't need to be saved in list
		ImageList.push_front(dst___);

		// No need for these two anymore, avoiding leaking memory!
		cvReleaseImage(&src_);
		cvReleaseImage(&dst);
		cvReleaseImage(&dst_);
		cvReleaseImage(&dst__);
	}
	return 0;
}

void CMango::FlipImage(int flag)
{
	IplImage *src = ImageList.front();
	IplImage *dst = cvCloneImage(src);

	if (flag == IM_HORIZONTAL)
		cvFlip(src, dst, 0);
	else if (flag == IM_VERTICAL)
		cvFlip(src, dst, 1);

	ImageList.push_front(dst);
}

void CMango::ApplyEffects(int flag)
{
	IplImage *src = ImageList.front();
	IplImage *dst = cvCloneImage(src);

	if (flag == IM_DILATE)
		cvDilate(src, dst, NULL, 2);
	else if (flag == IM_ERODE)
		cvErode(src, dst, NULL, 2);

	ImageList.push_front(dst);
}

void CMango::FaceDetect(int flag)
{
	if (flag == IM_FACE1){

	   /*
		* Using Haar Classifier Cascade
		* http://opencv.willowgarage.com/wiki/FaceDetection
		*/

		static CvMemStorage* storage = cvCreateMemStorage(0);
		static CvHaarClassifierCascade* cascade = 0;
		const char* cascade_name = "haarcascade_frontalface_alt.xml";
		int scale = 1;
		IplImage *img = ImageList.front();
		IplImage *temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );
		CvPoint pt1, pt2; // Create two points to represent the face locations
		int i;

		if (storage)
			cvClearMemStorage( storage );

		// Find whether the cascade is loaded, to find the faces. If yes, then:
		cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
		if( cascade )
		{

			// There can be more than one face in an image. So create a growable sequence of faces.
			// Detect the objects and store them in the sequence
			CvSeq* faces = cvHaarDetectObjects( img, cascade, storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40) );

			// Loop the number of faces found.
			for( i = 0; i < (faces ? faces->total : 0); i++ )
			{
			   // Create a new rectangle for drawing the face
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

				// Find the dimensions of the face,and scale it if necessary
				pt1.x = r->x*scale;
				pt2.x = (r->x+r->width)*scale;
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;

				// Draw the rectangle in the input image
				cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
			}
		}

		cvNamedWindow("detected face");
		cvShowImage( "detected face", img ); // Show the image in the window named "result"
		cvReleaseImage( &temp ); // Release the temp image created.

		if (storage)
			cvClearMemStorage( storage );

	}
}

void CMango::ShowHistogram()
{
	int i, j, bin_w;
	int hist_size = 256;
	int sch = 0, ch_width = 260;
	float max_value = 0;
	float range_0 [] = {0, 256};
	float *ranges [] = {range_0};
	IplImage *src_img = ImageList.front();
	IplImage *dst_img [4] = {0, 0, 0, 0};
	IplImage *hist_img;
	CvHistogram *hist;

	sch = src_img-> nChannels;
	for (i = 0; i < sch; i++)
		dst_img [i] = cvCreateImage (cvSize (src_img-> width, src_img-> height), src_img-> depth, 1);

	hist = cvCreateHist (1, & hist_size, CV_HIST_ARRAY, ranges, 1);
	hist_img = cvCreateImage (cvSize (ch_width * sch, 200), 8, src_img->nChannels);

	if (sch == 1)
		cvCopy (src_img, dst_img [0], NULL);
	else
		cvSplit (src_img, dst_img [0], dst_img [1], dst_img [2], dst_img [3]);

	// Histogram clear picture
	cvSet (hist_img, cvScalarAll (255), 0);
	bin_w = cvRound((double)hist_img->width/hist_size);

	for (i = 0; i < sch; i++) {
		// calculate the histogram
		cvCalcHist (& dst_img [i], hist, 0, NULL);
		cvGetMinMaxHistValue (hist, 0, & max_value, 0, 0);
		cvScale (hist-> bins, hist-> bins, ((double) hist_img-> height) / max_value, 0);

		// histogram drawing bin_w
		cvRound ((double) ch_width / hist_size);
		for (j = 0; j <hist_size; j++)
			cvRectangle (hist_img, cvPoint (j * bin_w / src_img->nChannels + (i * ch_width), hist_img-> height),
				cvPoint ((j + 1) * bin_w / src_img->nChannels + (i * ch_width), hist_img-> height - cvRound (cvGetReal1D (hist-> bins, j))),
				src_img->nChannels == 1 ? cv::Scalar(0) : (i == 0 ? CV_RGB(0,0,255):(i == 1 ? CV_RGB(0,255,0) : CV_RGB(255,0,0))),
				-1, 8, 0);
	}
	// See the image histogram
	cvNamedWindow ( "Histogram", CV_WINDOW_AUTOSIZE);
	cvShowImage ( "Histogram", hist_img);
	cvReleaseImage (& hist_img);
	for (i = 0; i <sch; i++)
		cvReleaseImage (& dst_img [i]);
	cvReleaseHist (& hist);
}


int CMango::HistogramEqualization()
{

   /*
	* Actually OpenCV has this already: cvEqualizeHist(&src, &dst)
	* but I'd rather use this one:
	* http://www.codepedia.com/1/CppOpenCvHistogramEqualization
	*/

	IplImage *src = ImageList.front();
	if (src->nChannels > 1)
		return -1;

	CvHistogram *hist;
	uchar lut[256];
	double lut1[256];
	CvMat* lut_mat;
	int hist_size = 256;
	float range_0[]={0,255};
	float* ranges[] = { range_0 };

	IplImage *dst = cvCloneImage(src);

	//int high=0;
	//int low =0;
	float hist_value = 0.0;

	hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	lut_mat = cvCreateMatHeader( 1, 256, CV_8UC1 );
	cvSetData( lut_mat, lut, 0 );
	//cvCalcHist( &pSource, hist, 0, NULL );
	cvCalcHist( const_cast<IplImage**>(&src), hist, 0, NULL );

	//CUMULATIVE ARRAY
	lut1[0] = 0;
	for(int index = 0; index != hist_size-1; ++index)
	{
        //hist_value = ((float*)(cvPtr1D( (hist)->bins, (index), 0 ));
        hist_value = cvGetReal1D(hist,index);
		//hist_value = cvQueryHistValue_1D(hist,index);
		lut1[index+1]= lut1[index] + hist_value;
	}

	//CALCULATE THE NEW LUT
	float scale_factor;
	scale_factor = 255.0f / (float) (src->width * src->height);
	for (int index=0; index<256; ++index)
	{
		lut[index]= (unsigned char)((float)lut1[index]*scale_factor);
	}
	//PERFORM IT ON THE CHANNEL
	cvLUT( src, dst, lut_mat );
	cvReleaseMat( &lut_mat);
	cvReleaseHist(&hist);

	ImageList.push_front(dst);
	return 0;
}

int CMango::ChangeColorSpace(int flag)
{
	IplImage *src = Automatic;
	if (src->nChannels < 3)
		return -1;

	if (flag == IM_HSV){
		cvCvtColor(src, Buf, CV_BGR2HSV);
	}
	else if (flag == IM_HLS){
		cvCvtColor(src, Buf, CV_BGR2HLS);
	}
	else if (flag == IM_YCrCb){
		cvCvtColor(src, Buf, CV_BGR2YCrCb);
	}
	ColorSpace = flag;
	return 0;
}
