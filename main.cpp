#include "main.h"
#include "sample.xpm"

#define APPNAME "Mango"
#define VERSION "0.01"
#define AUTHOR  "Auralius (mr_manurung@yahoo.com)"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool PhotoApp::OnInit()
{
	TheFrame *frame = new TheFrame(_T(APPNAME), 130, 650, 0);
	frame->Show(true);

	return true;
}

TheFrame::TheFrame(const wxChar *title, int xx, int yy, int windowtype)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(xx, yy),
	wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX)
{
	if (windowtype == 0){
		SetIcon(wxIcon(sample_xpm));
		SetMenuBar(CreateMenuBar());
		CreateFoldBar();
	}
	else if (windowtype == 1){
		SetIcon(wxIcon(sample_xpm));
		CreatePropertyList();
	}
}

TheFrame::~TheFrame()
{

}

//----------------------------------------------------------------------------
// This part will create fold bar
//----------------------------------------------------------------------------

void TheFrame::CreateFoldBar()
{
	foldbar = new wxFoldPanelBar(this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxFPB_DEFAULT_STYLE, wxFPB_COLLAPSE_TO_BOTTOM);
	//////////////////////////////////////////////////////////////////////////

	wxFoldPanel item = foldbar->AddFoldPanel(_T("BW/Grayscale"), false);
	sliderBrightness1 = new wxSlider(item.GetParent(), ID_BRIGHTNESS1, 100, 0, 200);
	sliderBrightness2 = new wxSlider(item.GetParent(), ID_BRIGHTNESS2, 100, 0, 200);
	sliderBrightness3 = new wxSlider(item.GetParent(), ID_BRIGHTNESS3, 100, 0, 200);
	sliderContrast1 = new wxSlider(item.GetParent(), ID_CONTRAST1, 100, 0, 200);
	sliderContrast2 = new wxSlider(item.GetParent(), ID_CONTRAST2, 100, 0, 200);
	sliderContrast3 = new wxSlider(item.GetParent(), ID_CONTRAST3, 100, 0, 200);

	radioDefaultColorSpace= new wxRadioButton(item.GetParent(), ID_RGB, _T("RGB/BGR"));

	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_AUTOMATIC, _T("Automatic")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_BLACKNWHITE, _T("Black/white")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_GRAYSCALE, _T("Grayscale")));
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Color Spaces:")));
	foldbar->AddFoldPanelWindow(item, radioDefaultColorSpace);
	foldbar->AddFoldPanelWindow(item, new wxRadioButton(item.GetParent(), ID_HSV, _T("HSV")));
	foldbar->AddFoldPanelWindow(item, new wxRadioButton(item.GetParent(), ID_HLS, _T("HLS")));
	foldbar->AddFoldPanelWindow(item, new wxRadioButton(item.GetParent(), ID_YCrCb, _T("YCrCb")));

	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Channels:")));
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Channel 1:")));
	foldbar->AddFoldPanelWindow(item, sliderBrightness1);
	foldbar->AddFoldPanelWindow(item, sliderContrast1);
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Channel 2:")));
	foldbar->AddFoldPanelWindow(item, sliderBrightness2);
	foldbar->AddFoldPanelWindow(item, sliderContrast2);
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Channel2:")));
	foldbar->AddFoldPanelWindow(item, sliderBrightness3);
	foldbar->AddFoldPanelWindow(item, sliderContrast3);
	//////////////////////////////////////////////////////////////////////////

	item = foldbar->AddFoldPanel(_T("Zoom/Rotate/Flip"), true);
	spinAngle = new wxSpinCtrl(item.GetParent(), wxID_ANY, _T("90"), wxDefaultPosition, wxDefaultSize, 4096, -180, 180);

	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Zoom:")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_ZOOMIN, _T("++")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_ZOOMOUT, _T("--")));
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Rotate:")));
	foldbar->AddFoldPanelWindow(item, spinAngle);
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_ROTATE, _T("Rotate")));
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Flip:")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_FLIPHORIZONTAL, _T("Horizontal")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_FLIPVERTICAL, _T("Vertical")));
	//////////////////////////////////////////////////////////////////////////

	item = foldbar->AddFoldPanel(_T("Effects"), true);
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_DILATE, _T("Dilate")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_ERODE, _T("Erode")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_FACE1, _T("Face Detect")));
	foldbar->AddFoldPanelWindow(item, new wxStaticText(item.GetParent(), wxID_ANY, _T("Histogram:")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_HISTOGRAM, _T("Show Histogram")));
	foldbar->AddFoldPanelWindow(item, new wxButton(item.GetParent(), ID_HIST_EQUALIZATION, _T("Hist Equalization")));

	radioDefaultColorSpace->SetValue(true);
	foldbar->Disable();
}

void TheFrame::ResetSliderBar()
{
	sliderContrast1->SetValue(100);
	sliderBrightness1->SetValue(100);
	sliderContrast2->SetValue(100);
	sliderBrightness2->SetValue(100);
	sliderContrast3->SetValue(100);
	sliderBrightness3->SetValue(100);
}

void TheFrame::CreatePropertyList()
{
	wxArrayString items;
	//wxListBox *listProperties = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, NULL);
	wxListBox *listProperties = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	if (mango.SetProperty()==0){
		while(1)
		{
			std::string itemProperties = mango.GetProperty();
			if (itemProperties == "")
				break;
			//items.Add(_T(itemProperties.c_str()));
			items.Add(itemProperties.c_str());
			listProperties->Set(items);

		}
	}
}

//----------------------------------------------------------------------------
// This part will create menu bar
//----------------------------------------------------------------------------

wxMenuBar *TheFrame::CreateMenuBar()
{
	wxMenu *menuFile = new wxMenu;
    menuFile->Append(menuLoadPicture, _T("&Load picture..."));
	menuFile->Append(menuShowProperties, _T("Picture p&roperties..."));
	menuFile->Append(menuPrint, _T("&Print..."));
	menuFile->Append(menuUndo, _T("&Undo change"));
	menuFile->Append(menuSave, _T("&Save changes..."));
	menuFile->AppendSeparator();
    menuFile->Append(menuExit, _T("E&xit"));

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(menuAbout, _T("&About..."));

	wxMenuBar *value = new wxMenuBar();
    value->Append(menuFile, _T("&File"));
    value->Append(menuHelp, _T("&Help"));

	return value;
}

//----------------------------------------------------------------------------
// Event handler
//----------------------------------------------------------------------------

void TheFrame::OnFileOpen(wxCommandEvent &WXUNUSED(event))
{
	wxFileDialog dialog(this, _T("Open an image file"), wxEmptyString,
		wxEmptyString, _T("Image files (*.jpg;*.bmp)|*.jpg;*.bmp"));

	dialog.SetDirectory(wxGetHomeDir());

	if (dialog.ShowModal() == wxID_OK){
		Path = dialog.GetPath();
		strcpy(WorkingFile, (const char *)Path.c_str());
		mango.LoadImageFromFile(WorkingFile);
		foldbar->Enable();
	}
}

void TheFrame::OnUndo(wxCommandEvent &)
{
	mango.PopImage();
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnSave(wxCommandEvent &)
{
	wxFileDialog dialog(this, _T("Save an image file"), wxEmptyString,
		wxEmptyString, _T("Image files (*.jpg;*.bmp)|*.jpg;*.bmp"), wxFD_SAVE);

	dialog.SetDirectory(wxGetHomeDir());

	if (dialog.ShowModal() == wxID_OK){
		Path = dialog.GetPath();
		strcpy(WorkingFile, (const char*)Path.c_str());
		if (mango.SaveToFile(WorkingFile) == -1)
			wxMessageBox(_T("No file to save!"), _T("Attention"));
	}
}

static void InitAboutInfoMinimal(wxAboutDialogInfo& info)
{
    info.SetName(_T("About Mango"));
    info.SetVersion(_T(VERSION));
    info.SetDescription(_T("Simple Image Editor with OpenCV and wxWidgets"));
    info.AddDeveloper(_T(AUTHOR));
}

static void InitAboutInfoWebsite(wxAboutDialogInfo& info)
{
    InitAboutInfoMinimal(info);
    info.SetWebSite(_T("http://sites.google.com/site/auraliusproject"), _T("Visit my web site"));
}

void TheFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
	wxAboutDialogInfo info;
    InitAboutInfoWebsite(info);
    wxAboutBox(info);
}

void TheFrame::OnShowProperties(wxCommandEvent &WXUNUSED(event))
{
	TheFrame *frame = new TheFrame(_T("Picture Properties"), 300, 300, 1);
	frame->Show(true);
}

void TheFrame::OnBlaknwhite(wxCommandEvent &WXUNUSED(event))
{
	ResetSliderBar();
	mango.LoadPrestoredImage(CMango::IM_BLACKNWHITE);
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnGrayscale(wxCommandEvent &WXUNUSED(event))
{
	ResetSliderBar();
	mango.LoadPrestoredImage(CMango::IM_GRAYSCALE);
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnAutomatic(wxCommandEvent &WXUNUSED(event))
{
	ResetSliderBar();
	mango.LoadPrestoredImage(CMango::IM_AUTOMATIC);
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnChangeColorSpace(wxCommandEvent &event)
{
	if (mango.ChangeColorSpace(event.GetId()) == 0){
		// Prepare Automatic image first
		ResetSliderBar();
		mango.LoadPrestoredImage(CMango::IM_AUTOMATIC);
		mango.ShowActiveImage(WorkingFile);

		// What is the available adjustment?
		if (event.GetId() == ID_YCrCb){
			sliderContrast1->Enable();
			sliderBrightness1->Enable();
			sliderContrast2->Disable();
			sliderBrightness2->Disable();
			sliderContrast3->Disable();
			sliderBrightness3->Disable();
		}

		if (event.GetId() == ID_HSV || event.GetId() == ID_HLS){
			sliderContrast1->Disable();
			sliderBrightness1->Disable();
			sliderContrast2->Enable();
			sliderBrightness2->Enable();
			sliderContrast3->Enable();
			sliderBrightness3->Enable();
		}

		if (event.GetId() == ID_RGB){
			sliderContrast1->Enable();
			sliderBrightness1->Enable();
			sliderContrast2->Enable();
			sliderBrightness2->Enable();
			sliderContrast3->Enable();
			sliderBrightness3->Enable();
		}

		if (event.GetId() == ID_YCrCb){
			sliderContrast2->Disable();
			sliderBrightness2->Disable();
			sliderContrast3->Disable();
			sliderBrightness3->Disable();
		}
	}
	else
		wxMessageBox(_T("Can't change color space!\r\nThis seems to be 1 channel image."), _T("Attention"));
}

void TheFrame::OnRotate(wxCommandEvent &WXUNUSED(event))
{
	mango.Rotate(spinAngle->GetValue());
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnZoom(wxCommandEvent &event)
{
	if (event.GetId() == ID_ZOOMIN)
		mango.ZoomImage(CMango::IM_ZOOMIN);
	else if (event.GetId() == ID_ZOOMOUT)
		mango.ZoomImage(CMango::IM_ZOOMOUT);

	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnAdjBrightness(wxCommandEvent &event)
{
	int nChannel = 0;
	int nBrightness = 50;
	int nContrast = 50;

	if ((event.GetId() == ID_BRIGHTNESS1)||(event.GetId() == ID_CONTRAST1)){
		nChannel = 0;
		nBrightness = sliderBrightness1->GetValue();
		nContrast = sliderContrast1->GetValue();
	}
	else if ((event.GetId() == ID_BRIGHTNESS2)||(event.GetId() == ID_CONTRAST2)){
		nChannel = 1;
		nBrightness = sliderBrightness2->GetValue();
		nContrast = sliderContrast2->GetValue();
	}
	else if ((event.GetId() == ID_BRIGHTNESS3)||(event.GetId() == ID_CONTRAST3)){
		nChannel = 2;
		nBrightness = sliderBrightness3->GetValue();
		nContrast = sliderContrast3->GetValue();
	}

	mango.GetImageElement(nChannel, nBrightness, nContrast);
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnFlip(wxCommandEvent &event)
{
	mango.FlipImage(event.GetId());
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnApplyEffect(wxCommandEvent &event)
{
	mango.ApplyEffects(event.GetId());
	mango.ShowActiveImage(WorkingFile);
}

void TheFrame::OnFaceDetect(wxCommandEvent &event)
{
	mango.FaceDetect(ID_FACE1);
}

void TheFrame::OnShowHistogram(wxCommandEvent &WXUNUSED(event))
{
	mango.ShowHistogram();
}

void TheFrame::OnHistogramManipulation(wxCommandEvent &WXUNUSED(event))
{
	int nRetVal = mango.HistogramEqualization();
	if (nRetVal == -1)
		wxMessageBox(_T("This is only for 1 channel image (grayscale)!"), _T("Attention"));
	else
		mango.ShowActiveImage(WorkingFile);
}
