#pragma once

#include <wx/wx.h>
#include "wx/intl.h"
#include "wx/foldbar/foldpanelbar.h"
#include "wx/filedlg.h"
#include "wx/listbox.h"
#include "wx/spinctrl.h"
#include "wx/aboutdlg.h"
#include "mango.h"

//http://cool-emerald.blogspot.com/2017/11/opencv-with-wxwidgets.html
// ----------------------------------------------------------------------------
// TheFrame Class
// ----------------------------------------------------------------------------

class TheFrame : public wxFrame
{
public:
	TheFrame(const wxChar *title, int xx, int yy, int windowtype);

	~TheFrame();
	void CreateFoldBar();
	void CreatePropertyList();
	wxMenuBar *CreateMenuBar();
	void ResetSliderBar();

	//Event handler
	void OnFileOpen(wxCommandEvent &WXUNUSED(event));
	void OnUndo(wxCommandEvent &WXUNUSED(event));
	void OnSave(wxCommandEvent &WXUNUSED(event));
	void OnAbout(wxCommandEvent &WXUNUSED(event));
	void OnBlaknwhite(wxCommandEvent &WXUNUSED(event));
	void OnGrayscale(wxCommandEvent &WXUNUSED(event));
	void OnAutomatic(wxCommandEvent &WXUNUSED(event));
	void OnShowProperties(wxCommandEvent &WXUNUSED(event));
	void OnChangeColorSpace(wxCommandEvent &event);

	void OnRotate(wxCommandEvent &WXUNUSED(event));
	void OnZoom(wxCommandEvent &event);
	void OnAdjBrightness(wxCommandEvent &event);
	void OnFlip(wxCommandEvent &event);
	void OnApplyEffect(wxCommandEvent &event);
	void OnFaceDetect(wxCommandEvent &event);
	void OnShowHistogram(wxCommandEvent &WXUNUSED(event));
	void OnHistogramManipulation(wxCommandEvent &WXUNUSED(event));

private:
	CMango mango;
	wxString FileName, Path;
	char WorkingFile[256];

	wxFoldPanelBar *foldbar;
	wxSpinCtrl *spinAngle;
	wxSlider * sliderBrightness1;
	wxSlider * sliderBrightness2;
	wxSlider * sliderBrightness3;
	wxSlider * sliderContrast1;
	wxSlider * sliderContrast2;
	wxSlider * sliderContrast3;
	wxRadioButton *radioDefaultColorSpace;

	DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// PhotoApp Class
// ----------------------------------------------------------------------------

class PhotoApp : public wxApp
{
public:
	virtual bool OnInit();
};

// ----------------------------------------------------------------------------
// Constants and global variabels
// ----------------------------------------------------------------------------

enum
{
	ID_AUTOMATIC,
	ID_GRAYSCALE,
	ID_BLACKNWHITE,
	ID_RGB,
	ID_HSV,
	ID_HLS,
	ID_YCrCb,
	menuLoadPicture,
	menuExit,
	menuSave,
	menuAbout,
	menuShowProperties,
	menuPrint,
	menuUndo,
	ID_ZOOMIN,
	ID_ZOOMOUT,
	ID_ROTATE,
	ID_FLIPHORIZONTAL = 25,
	ID_FLIPVERTICAL,
	ID_COLORBRIGHTNESS,
	ID_COLORCONTRAST,
	ID_BRIGHTNESS1,
	ID_BRIGHTNESS2,
	ID_BRIGHTNESS3,
	ID_CONTRAST1,
	ID_CONTRAST2,
	ID_CONTRAST3,
	ID_ERODE = 40,
	ID_DILATE,
	ID_FACE1,
	ID_HISTOGRAM,
	ID_HIST_EQUALIZATION,
	ID_HIST_NORMALIZATION
};

// ----------------------------------------------------------------------------
// Event handler lists
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(TheFrame, wxFrame)
    EVT_MENU		(menuLoadPicture,       TheFrame::OnFileOpen)
	EVT_MENU		(menuShowProperties,    TheFrame::OnShowProperties)
	EVT_MENU		(menuUndo,			    TheFrame::OnUndo)
	EVT_MENU		(menuSave,			    TheFrame::OnSave)
	EVT_MENU		(menuAbout,				TheFrame::OnAbout)
	EVT_BUTTON		(ID_BLACKNWHITE,		TheFrame::OnBlaknwhite)
	EVT_BUTTON		(ID_GRAYSCALE,			TheFrame::OnGrayscale)
	EVT_BUTTON		(ID_AUTOMATIC,			TheFrame::OnAutomatic)
	EVT_RADIOBUTTON	(ID_RGB,				TheFrame::OnChangeColorSpace)
	EVT_RADIOBUTTON	(ID_HSV,				TheFrame::OnChangeColorSpace)
	EVT_RADIOBUTTON	(ID_HLS,				TheFrame::OnChangeColorSpace)
	EVT_RADIOBUTTON	(ID_YCrCb,				TheFrame::OnChangeColorSpace)
	EVT_BUTTON		(ID_ROTATE,				TheFrame::OnRotate)
	EVT_BUTTON		(ID_ZOOMIN,				TheFrame::OnZoom)
	EVT_BUTTON		(ID_ZOOMOUT,			TheFrame::OnZoom)
	EVT_BUTTON		(ID_FLIPHORIZONTAL,		TheFrame::OnFlip)
	EVT_BUTTON		(ID_FLIPVERTICAL,		TheFrame::OnFlip)
	EVT_SLIDER		(ID_BRIGHTNESS1,		TheFrame::OnAdjBrightness)
	EVT_SLIDER		(ID_BRIGHTNESS2,		TheFrame::OnAdjBrightness)
	EVT_SLIDER		(ID_BRIGHTNESS3,		TheFrame::OnAdjBrightness)
	EVT_SLIDER		(ID_CONTRAST1,			TheFrame::OnAdjBrightness)
	EVT_SLIDER		(ID_CONTRAST2,			TheFrame::OnAdjBrightness)
	EVT_SLIDER		(ID_CONTRAST3,			TheFrame::OnAdjBrightness)
	EVT_BUTTON		(ID_ERODE,				TheFrame::OnApplyEffect)
	EVT_BUTTON		(ID_DILATE,				TheFrame::OnApplyEffect)
	EVT_BUTTON		(ID_FACE1,				TheFrame::OnFaceDetect)
	EVT_BUTTON		(ID_HISTOGRAM,			TheFrame::OnShowHistogram)
	EVT_BUTTON		(ID_HIST_NORMALIZATION,	TheFrame::OnHistogramManipulation)
	EVT_BUTTON		(ID_HIST_EQUALIZATION,	TheFrame::OnHistogramManipulation)

END_EVENT_TABLE()

IMPLEMENT_APP(PhotoApp)
