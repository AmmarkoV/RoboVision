#include "CortexSettings.h"
#include "../VisualCortex/VisualCortex.h"

//(*InternalHeaders(CortexSettings)
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/font.h>
//*)

//(*IdInit(CortexSettings)
const long CortexSettings::ID_STATICBOX1 = wxNewId();
const long CortexSettings::ID_STATICTEXT1 = wxNewId();
const long CortexSettings::ID_TEXTCTRL1 = wxNewId();
const long CortexSettings::ID_BUTTON1 = wxNewId();
const long CortexSettings::ID_BUTTON2 = wxNewId();
const long CortexSettings::ID_STATICTEXT2 = wxNewId();
const long CortexSettings::ID_TEXTCTRL2 = wxNewId();
const long CortexSettings::ID_STATICTEXT3 = wxNewId();
const long CortexSettings::ID_TEXTCTRL3 = wxNewId();
const long CortexSettings::ID_BUTTON3 = wxNewId();
const long CortexSettings::ID_STATICTEXT4 = wxNewId();
const long CortexSettings::ID_TEXTCTRL4 = wxNewId();
const long CortexSettings::ID_STATICTEXT5 = wxNewId();
const long CortexSettings::ID_TEXTCTRL5 = wxNewId();
const long CortexSettings::ID_TEXTCTRL6 = wxNewId();
const long CortexSettings::ID_STATICTEXT6 = wxNewId();
const long CortexSettings::ID_TEXTCTRL7 = wxNewId();
const long CortexSettings::ID_STATICTEXT7 = wxNewId();
const long CortexSettings::ID_TEXTCTRL8 = wxNewId();
const long CortexSettings::ID_STATICTEXT8 = wxNewId();
const long CortexSettings::ID_TEXTCTRL9 = wxNewId();
const long CortexSettings::ID_STATICTEXT9 = wxNewId();
const long CortexSettings::ID_TEXTCTRL10 = wxNewId();
const long CortexSettings::ID_STATICTEXT10 = wxNewId();
const long CortexSettings::ID_CHECKBOX1 = wxNewId();
const long CortexSettings::ID_CHECKBOX2 = wxNewId();
const long CortexSettings::ID_STATICBOX2 = wxNewId();
const long CortexSettings::ID_STATICTEXT11 = wxNewId();
const long CortexSettings::ID_TEXTCTRL11 = wxNewId();
const long CortexSettings::ID_STATICTEXT12 = wxNewId();
const long CortexSettings::ID_TEXTCTRL12 = wxNewId();
const long CortexSettings::ID_TEXTCTRL13 = wxNewId();
const long CortexSettings::ID_TEXTCTRL14 = wxNewId();
const long CortexSettings::ID_STATICTEXT13 = wxNewId();
const long CortexSettings::ID_STATICTEXT14 = wxNewId();
const long CortexSettings::ID_TEXTCTRL15 = wxNewId();
const long CortexSettings::ID_TEXTCTRL16 = wxNewId();
const long CortexSettings::ID_TEXTCTRL17 = wxNewId();
const long CortexSettings::ID_STATICTEXT15 = wxNewId();
const long CortexSettings::ID_STATICTEXT16 = wxNewId();
const long CortexSettings::ID_STATICTEXT17 = wxNewId();
const long CortexSettings::ID_STATICTEXT18 = wxNewId();
const long CortexSettings::ID_CHECKBOX3 = wxNewId();
const long CortexSettings::ID_STATICTEXT19 = wxNewId();
const long CortexSettings::ID_STATICTEXT20 = wxNewId();
const long CortexSettings::ID_TEXTCTRL18 = wxNewId();
const long CortexSettings::ID_STATICTEXT21 = wxNewId();
const long CortexSettings::ID_TEXTCTRL19 = wxNewId();
const long CortexSettings::ID_STATICTEXT22 = wxNewId();
const long CortexSettings::ID_TEXTCTRL20 = wxNewId();
const long CortexSettings::ID_CHECKBOX4 = wxNewId();
const long CortexSettings::ID_CHECKBOX5 = wxNewId();
const long CortexSettings::ID_TEXTCTRL21 = wxNewId();
const long CortexSettings::ID_STATICTEXT23 = wxNewId();
const long CortexSettings::ID_TEXTCTRL22 = wxNewId();
const long CortexSettings::ID_STATICTEXT24 = wxNewId();
const long CortexSettings::ID_STATICTEXT25 = wxNewId();
const long CortexSettings::ID_STATICTEXT26 = wxNewId();
const long CortexSettings::ID_TEXTCTRL23 = wxNewId();
const long CortexSettings::ID_TEXTCTRL24 = wxNewId();
const long CortexSettings::ID_TEXTCTRL25 = wxNewId();
const long CortexSettings::ID_TEXTCTRL26 = wxNewId();
const long CortexSettings::ID_TEXTCTRL27 = wxNewId();
const long CortexSettings::ID_TEXTCTRL28 = wxNewId();
const long CortexSettings::ID_TEXTCTRL29 = wxNewId();
const long CortexSettings::ID_TEXTCTRL30 = wxNewId();
const long CortexSettings::ID_STATICTEXT27 = wxNewId();
const long CortexSettings::ID_STATICTEXT28 = wxNewId();
const long CortexSettings::ID_STATICTEXT29 = wxNewId();
const long CortexSettings::ID_TEXTCTRL31 = wxNewId();
const long CortexSettings::ID_TEXTCTRL32 = wxNewId();
const long CortexSettings::ID_STATICTEXT30 = wxNewId();
const long CortexSettings::ID_STATICTEXT31 = wxNewId();
const long CortexSettings::ID_STATICLINE1 = wxNewId();
const long CortexSettings::ID_STATICTEXT32 = wxNewId();
const long CortexSettings::ID_TEXTCTRL33 = wxNewId();
const long CortexSettings::ID_TEXTCTRL34 = wxNewId();
const long CortexSettings::ID_CHECKBOX6 = wxNewId();
const long CortexSettings::ID_STATICTEXT33 = wxNewId();
const long CortexSettings::ID_TEXTCTRL35 = wxNewId();
const long CortexSettings::ID_TEXTCTRL36 = wxNewId();
const long CortexSettings::ID_TEXTCTRL37 = wxNewId();
const long CortexSettings::ID_TEXTCTRL38 = wxNewId();
const long CortexSettings::ID_STATICTEXT34 = wxNewId();
const long CortexSettings::ID_STATICTEXT35 = wxNewId();
const long CortexSettings::ID_STATICTEXT36 = wxNewId();
const long CortexSettings::ID_STATICTEXT37 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CortexSettings,wxDialog)
	//(*EventTable(CortexSettings)
	//*)
END_EVENT_TABLE()

CortexSettings::CortexSettings(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(CortexSettings)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(792,622));
	Move(wxDefaultPosition);
	StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Disparity Map"), wxPoint(24,16), wxSize(456,464), 0, _T("ID_STATICBOX1"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Quality ( 1..15)"), wxPoint(32,48), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	Quality = new wxTextCtrl(this, ID_TEXTCTRL1, _("6"), wxPoint(136,40), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	SaveButton = new wxButton(this, ID_BUTTON1, _("Save"), wxPoint(416,544), wxSize(157,29), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	SaveButton->SetDefault();
	CancelButton = new wxButton(this, ID_BUTTON2, _("Cancel"), wxPoint(696,544), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Start Left Px"), wxPoint(32,112), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StartLeft = new wxTextCtrl(this, ID_TEXTCTRL2, _("15"), wxPoint(128,104), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Edge Strictness"), wxPoint(240,40), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	EdgeStrictness = new wxTextCtrl(this, ID_TEXTCTRL3, _("25"), wxPoint(360,34), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	DefaultButton = new wxButton(this, ID_BUTTON3, _("Default"), wxPoint(608,544), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Instant Quality (1..15)"), wxPoint(32,80), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	InstantQuality = new wxTextCtrl(this, ID_TEXTCTRL4, _("2"), wxPoint(184,72), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Camera Search Up/Down"), wxPoint(240,72), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	OffsetUp = new wxTextCtrl(this, ID_TEXTCTRL5, _("1"), wxPoint(416,68), wxSize(24,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	OffsetDown = new wxTextCtrl(this, ID_TEXTCTRL6, _("1"), wxPoint(448,68), wxSize(24,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _(" Good Enough Match"), wxPoint(32,320), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	ComparisonThreshold = new wxTextCtrl(this, ID_TEXTCTRL7, _("15000"), wxPoint(288,232), wxSize(72,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Comparison Min Score"), wxPoint(560,344), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	ComparisonMinScore = new wxTextCtrl(this, ID_TEXTCTRL8, _("30000"), wxPoint(592,368), wxSize(72,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Closest Depth Plane"), wxPoint(40,392), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	ClosestDepth = new wxTextCtrl(this, ID_TEXTCTRL9, _("90"), wxPoint(192,388), wxSize(48,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Comparison EdgesPerCent Required"), wxPoint(360,448), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	PatchesEdgesPerCent = new wxTextCtrl(this, ID_TEXTCTRL10, _("15"), wxPoint(392,232), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("%"), wxPoint(440,240), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FillHoles = new wxCheckBox(this, ID_CHECKBOX1, _("Fill Holes"), wxPoint(224,528), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	FillHoles->SetValue(false);
	ImproveEdges = new wxCheckBox(this, ID_CHECKBOX2, _("Improve Using Edges"), wxPoint(40,504), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	ImproveEdges->SetValue(false);
	StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Movement"), wxPoint(552,16), wxSize(208,224), 0, _T("ID_STATICBOX2"));
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Patch Sensitivity"), wxPoint(576,40), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	PatchSensitivity = new wxTextCtrl(this, ID_TEXTCTRL11, _("7"), wxPoint(704,32), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("RGB Threshold"), wxPoint(576,64), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	MinR = new wxTextCtrl(this, ID_TEXTCTRL12, _("25"), wxPoint(600,88), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL12"));
	MinG = new wxTextCtrl(this, ID_TEXTCTRL13, _("25"), wxPoint(664,88), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL13"));
	MinB = new wxTextCtrl(this, ID_TEXTCTRL14, _("25"), wxPoint(720,88), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("R"), wxPoint(584,88), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("G"), wxPoint(648,88), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	MaxR = new wxTextCtrl(this, ID_TEXTCTRL15, _("45"), wxPoint(600,120), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
	MaxG = new wxTextCtrl(this, ID_TEXTCTRL16, _("45"), wxPoint(664,120), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
	MaxB = new wxTextCtrl(this, ID_TEXTCTRL17, _("45"), wxPoint(720,120), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL17"));
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("B"), wxPoint(704,88), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("R"), wxPoint(584,120), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("G"), wxPoint(648,120), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("B"), wxPoint(704,120), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	DepthMapGuesses = new wxCheckBox(this, ID_CHECKBOX3, _("Depth Map Guessing"), wxPoint(40,528), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	DepthMapGuesses->SetValue(false);
	StaticText19 = new wxStaticText(this, ID_STATICTEXT19, _("Patch Histogram Threshold"), wxPoint(504,192), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	StaticText20 = new wxStaticText(this, ID_STATICTEXT20, _("R"), wxPoint(496,230), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	PatchMinR = new wxTextCtrl(this, ID_TEXTCTRL18, _("18"), wxPoint(512,224), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL18"));
	StaticText21 = new wxStaticText(this, ID_STATICTEXT21, _("G"), wxPoint(552,230), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	PatchMinG = new wxTextCtrl(this, ID_TEXTCTRL19, _("18"), wxPoint(568,224), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL19"));
	StaticText22 = new wxStaticText(this, ID_STATICTEXT22, _("B"), wxPoint(608,230), wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	PatchMinB = new wxTextCtrl(this, ID_TEXTCTRL20, _("18"), wxPoint(632,224), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL20"));
	ImproveUsingHistogram = new wxCheckBox(this, ID_CHECKBOX4, _("Depth Map Improve Using Histogram"), wxPoint(40,552), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	ImproveUsingHistogram->SetValue(true);
	DoNotProcessFar = new wxCheckBox(this, ID_CHECKBOX5, _("Do not process closer"), wxPoint(40,416), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	DoNotProcessFar->SetValue(false);
	EdgeStrictnessHigh = new wxTextCtrl(this, ID_TEXTCTRL21, _("150"), wxPoint(424,34), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL21"));
	StaticText23 = new wxStaticText(this, ID_STATICTEXT23, _("to"), wxPoint(400,40), wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	ComparisonExpectedThreshold = new wxTextCtrl(this, ID_TEXTCTRL22, _("5000"), wxPoint(192,318), wxSize(72,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL22"));
	StaticText24 = new wxStaticText(this, ID_STATICTEXT24, _("Small Patches"), wxPoint(32,240), wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	StaticText25 = new wxStaticText(this, ID_STATICTEXT25, _("Medium Patches"), wxPoint(32,205), wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	StaticText26 = new wxStaticText(this, ID_STATICTEXT26, _("Large Patches"), wxPoint(32,168), wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	PatchSizeXSmall = new wxTextCtrl(this, ID_TEXTCTRL23, _("30"), wxPoint(176,232), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL23"));
	PatchSizeYSmall = new wxTextCtrl(this, ID_TEXTCTRL24, _("30"), wxPoint(232,232), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL24"));
	PatchSizeXMedium = new wxTextCtrl(this, ID_TEXTCTRL25, _("50"), wxPoint(176,200), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL25"));
	PatchSizeYMedium = new wxTextCtrl(this, ID_TEXTCTRL26, _("50"), wxPoint(232,200), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL26"));
	PatchSizeXLarge = new wxTextCtrl(this, ID_TEXTCTRL27, _("100"), wxPoint(176,168), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL27"));
	PatchSizeYLarge = new wxTextCtrl(this, ID_TEXTCTRL28, _("100"), wxPoint(232,168), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL28"));
	ComparisonThresholdLarge = new wxTextCtrl(this, ID_TEXTCTRL29, _("200000"), wxPoint(288,168), wxSize(72,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL29"));
	ComparisonThresholdMedium = new wxTextCtrl(this, ID_TEXTCTRL30, _("100000"), wxPoint(288,200), wxSize(72,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL30"));
	StaticText27 = new wxStaticText(this, ID_STATICTEXT27, _("Size X/Y"), wxPoint(200,136), wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	StaticText28 = new wxStaticText(this, ID_STATICTEXT28, _("Threshold"), wxPoint(288,136), wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	StaticText29 = new wxStaticText(this, ID_STATICTEXT29, _("Edge Quota"), wxPoint(376,136), wxDefaultSize, 0, _T("ID_STATICTEXT29"));
	PatchesEdgesPercentMedium = new wxTextCtrl(this, ID_TEXTCTRL31, _("5"), wxPoint(392,200), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL31"));
	PatchesEdgesPercentLarge = new wxTextCtrl(this, ID_TEXTCTRL32, _("1"), wxPoint(392,168), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL32"));
	StaticText30 = new wxStaticText(this, ID_STATICTEXT30, _("%"), wxPoint(440,208), wxDefaultSize, 0, _T("ID_STATICTEXT30"));
	StaticText31 = new wxStaticText(this, ID_STATICTEXT31, _("%"), wxPoint(440,176), wxDefaultSize, 0, _T("ID_STATICTEXT31"));
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(168,160), wxSize(296,0), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	StaticText32 = new wxStaticText(this, ID_STATICTEXT32, _("Camera Shift      Up/Down"), wxPoint(240,104), wxDefaultSize, 0, _T("ID_STATICTEXT32"));
	ShiftUp = new wxTextCtrl(this, ID_TEXTCTRL33, _("0"), wxPoint(416,96), wxSize(24,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL33"));
	ShiftDown = new wxTextCtrl(this, ID_TEXTCTRL34, _("0"), wxPoint(448,96), wxSize(24,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL34"));
	DoNotProcessCloserThanPreviousSizeResult = new wxCheckBox(this, ID_CHECKBOX6, _("Do not process closer than previous size results"), wxPoint(32,344), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
	DoNotProcessCloserThanPreviousSizeResult->SetValue(false);
	StaticText33 = new wxStaticText(this, ID_STATICTEXT33, _("Comparison Weights"), wxPoint(32,284), wxDefaultSize, 0, _T("ID_STATICTEXT33"));
	RGBMultiplier = new wxTextCtrl(this, ID_TEXTCTRL35, _("1"), wxPoint(192,280), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL35"));
	MoveMultiplier = new wxTextCtrl(this, ID_TEXTCTRL36, _("1"), wxPoint(240,280), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL36"));
	SobelMultiplier = new wxTextCtrl(this, ID_TEXTCTRL37, _("2"), wxPoint(288,280), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL37"));
	SecondDerMultiplier = new wxTextCtrl(this, ID_TEXTCTRL38, _("35"), wxPoint(336,280), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL38"));
	StaticText34 = new wxStaticText(this, ID_STATICTEXT34, _("RGB"), wxPoint(192,264), wxDefaultSize, 0, _T("ID_STATICTEXT34"));
	wxFont StaticText34Font(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText34->SetFont(StaticText34Font);
	StaticText35 = new wxStaticText(this, ID_STATICTEXT35, _("Move"), wxPoint(240,264), wxDefaultSize, 0, _T("ID_STATICTEXT35"));
	wxFont StaticText35Font(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText35->SetFont(StaticText35Font);
	StaticText36 = new wxStaticText(this, ID_STATICTEXT36, _("1st Der"), wxPoint(288,264), wxDefaultSize, 0, _T("ID_STATICTEXT36"));
	wxFont StaticText36Font(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText36->SetFont(StaticText36Font);
	StaticText37 = new wxStaticText(this, ID_STATICTEXT37, _("2nd Der"), wxPoint(336,264), wxDefaultSize, 0, _T("ID_STATICTEXT37"));
	wxFont StaticText37Font(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText37->SetFont(StaticText37Font);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CortexSettings::OnSaveButtonClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CortexSettings::OnCancelButtonClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CortexSettings::OnDefaultButtonClick);
	//*)
    PullSettingsFromCortex();
	SetTitle(wxT("Visual Cortex Configuration"));
}

CortexSettings::~CortexSettings()
{
	//(*Destroy(CortexSettings)
	//*)
}

void CortexSettings::PullSettingsFromCortex()
{
  wxString val;

 // DEPTH MAPS
  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_STARTLEFT_X);
  StartLeft->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_DETAIL);
  Quality->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_EDGE_LOW_STRICTNESS);
  EdgeStrictness->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_EDGE_HIGH_STRICTNESS);
  EdgeStrictnessHigh->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_INSTANT_DETAIL);
  InstantQuality->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_VERT_OFFSET_UP);
  OffsetUp->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_VERT_OFFSET_DOWN);
  OffsetDown->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_VERT_SHIFT_UP);
  ShiftUp->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_VERT_SHIFT_DOWN);
  ShiftDown->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_COMPARISON_THRESHOLD);
  ComparisonThreshold->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH);
  ComparisonThresholdMedium->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH);
  ComparisonThresholdLarge->SetValue(val);

 // PATCHES

  val.Clear(); val<<VisCortx_GetSetting(PATCH_COMPARISON_SCORE_MIN);
  ComparisonMinScore->SetValue(val);

  val.Clear(); val<<VisCortx_GetMetric(HORIZONTAL_BUFFER);
  PatchSizeXSmall->SetValue(val);
  val.Clear(); val<<VisCortx_GetMetric(VERTICAL_BUFFER);
  PatchSizeYSmall->SetValue(val);

  val.Clear(); val<<VisCortx_GetMetric(HORIZONTAL_BUFFER_LARGE);
  PatchSizeXMedium->SetValue(val);
  val.Clear(); val<<VisCortx_GetMetric(VERTICAL_BUFFER_LARGE);
  PatchSizeYMedium->SetValue(val);

  val.Clear(); val<<VisCortx_GetMetric(HORIZONTAL_BUFFER_EXTRALARGE);
  PatchSizeXLarge->SetValue(val);
  val.Clear(); val<<VisCortx_GetMetric(VERTICAL_BUFFER_EXTRALARGE);
  PatchSizeYLarge->SetValue(val);


  val.Clear(); val<<VisCortx_GetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED);
  PatchesEdgesPerCent->SetValue(val);
  val.Clear(); val<<VisCortx_GetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH);
  PatchesEdgesPercentMedium->SetValue(val);
  val.Clear(); val<<VisCortx_GetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH);
  PatchesEdgesPercentLarge->SetValue(val);


  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_RGB_MULTIPLIER);
  RGBMultiplier->SetValue(val);
  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_MOVEMENT_MULTIPLIER);
  MoveMultiplier->SetValue(val);
  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_SOBEL_MULTIPLIER);
  SobelMultiplier->SetValue(val);
  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_SECOND_DERIVATIVE_MULTIPLIER);
  SecondDerMultiplier->SetValue(val);


  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_COMPARISON_TOO_GOOD_THRESHOLD);
  ComparisonExpectedThreshold->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_CLOSEST_DEPTH);
  ClosestDepth->SetValue(val);

  if (VisCortx_GetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH)==1) { DoNotProcessFar->SetValue(1); } else
                                                                                             { DoNotProcessFar->SetValue(0); }

  if (VisCortx_GetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_PREVIOUS_PATCH_SIZE_DEPTH)==1) { DoNotProcessCloserThanPreviousSizeResult->SetValue(1); } else
                                                                                                         { DoNotProcessCloserThanPreviousSizeResult->SetValue(0); }


 if (VisCortx_GetSetting(DEPTHMAP_GUESSES)==1) { DepthMapGuesses->SetValue(1); } else
                                               { DepthMapGuesses->SetValue(0); }

 if (VisCortx_GetSetting(DEPTHMAP_IMPROVE_FILLING_HOLES)==1) { FillHoles->SetValue(1); } else
                                                             { FillHoles->SetValue(0); }

 if (VisCortx_GetSetting(DEPTHMAP_IMPROVE_USING_EDGES)==1) { ImproveEdges->SetValue(1); } else
                                                           { ImproveEdges->SetValue(0); }


 if (VisCortx_GetSetting(DEPTHMAP_IMPROVE_USING_HISTOGRAM)==1) { ImproveUsingHistogram->SetValue(1); } else
                                                               { ImproveUsingHistogram->SetValue(0); }


// MOVEMENT

  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_PATCH_SENSITIVITY);
  PatchSensitivity->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_MIN_R_THRESHOLD);
  MinR->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_MIN_G_THRESHOLD);
  MinG->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_MIN_B_THRESHOLD);
  MinB->SetValue(val);


  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_R_THRESHOLD);
  MaxR->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_G_THRESHOLD);
  MaxG->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(MOVEMENT_B_THRESHOLD);
  MaxB->SetValue(val);



  val.Clear(); val<<VisCortx_GetSetting(PATCH_HIST_THRESHOLD_R);
  PatchMinR->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(PATCH_HIST_THRESHOLD_G);
  PatchMinG->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(PATCH_HIST_THRESHOLD_B);
  PatchMinB->SetValue(val);



}



void CortexSettings::PushSettingsToCortex()
{
  long value;
  // DEPTH MAP
  if(StartLeft->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_STARTLEFT_X,(unsigned int) value); }

  if(Quality->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_DETAIL,(unsigned int) value); }

  if(EdgeStrictness->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_EDGE_LOW_STRICTNESS,(unsigned int) value); }

  if(EdgeStrictnessHigh->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_EDGE_HIGH_STRICTNESS,(unsigned int) value); }

  if(InstantQuality->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_INSTANT_DETAIL,(unsigned int) value); }

  if(OffsetUp->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_UP,(unsigned int) value); }
  if(OffsetDown->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_DOWN,(unsigned int) value); }

  if(ShiftUp->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_VERT_SHIFT_UP,(unsigned int) value); }
  if(ShiftDown->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_VERT_SHIFT_DOWN,(unsigned int) value); }

  if(ComparisonThreshold->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD,(unsigned int) value); }

  if(ComparisonThresholdMedium->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH,(unsigned int) value); }

  if(ComparisonThresholdLarge->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH,(unsigned int) value); }

 // PATCHES
  if(ComparisonMinScore->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_COMPARISON_SCORE_MIN,(unsigned int) value); }

  if(PatchSizeXSmall->GetValue().ToLong(&value)) { VisCortx_SetMetric(HORIZONTAL_BUFFER,(unsigned int) value); }
  if(PatchSizeYSmall->GetValue().ToLong(&value)) { VisCortx_SetMetric(VERTICAL_BUFFER,(unsigned int) value); }

  if(PatchSizeXMedium->GetValue().ToLong(&value)) { VisCortx_SetMetric(HORIZONTAL_BUFFER_LARGE,(unsigned int) value); }
  if(PatchSizeYMedium->GetValue().ToLong(&value)) { VisCortx_SetMetric(VERTICAL_BUFFER_LARGE,(unsigned int) value); }

  if(PatchSizeXLarge->GetValue().ToLong(&value)) { VisCortx_SetMetric(HORIZONTAL_BUFFER_EXTRALARGE,(unsigned int) value); }
  if(PatchSizeYLarge->GetValue().ToLong(&value)) { VisCortx_SetMetric(VERTICAL_BUFFER_EXTRALARGE,(unsigned int) value); }

  if(PatchesEdgesPerCent->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED,(unsigned int) value); }
  if(PatchesEdgesPercentMedium->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH,(unsigned int) value); }
  if(PatchesEdgesPercentLarge->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH,(unsigned int) value); }



  if(RGBMultiplier->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_RGB_MULTIPLIER,(unsigned int) value); }
  if(MoveMultiplier->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_MOVEMENT_MULTIPLIER,(unsigned int) value); }
  if(SobelMultiplier->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_SOBEL_MULTIPLIER,(unsigned int) value); }
  if(SecondDerMultiplier->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_SECOND_DERIVATIVE_MULTIPLIER,(unsigned int) value); }




  if(ComparisonExpectedThreshold->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_TOO_GOOD_THRESHOLD,(unsigned int) value); }

  if(ClosestDepth->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_CLOSEST_DEPTH,(unsigned int) value); }

  if ( DoNotProcessFar->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH,1); } else
                                      { VisCortx_SetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH,0); }

  if ( DoNotProcessCloserThanPreviousSizeResult->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_PREVIOUS_PATCH_SIZE_DEPTH,1); } else
                                                               { VisCortx_SetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_PREVIOUS_PATCH_SIZE_DEPTH,0); }

  if ( DepthMapGuesses->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_GUESSES,1); } else
                                      { VisCortx_SetSetting(DEPTHMAP_GUESSES,0); }

  if ( FillHoles->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_IMPROVE_FILLING_HOLES,1); } else
                                { VisCortx_SetSetting(DEPTHMAP_IMPROVE_FILLING_HOLES,0); }

  if ( ImproveEdges->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_EDGES,1); } else
                                   { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_EDGES,0); }

  if ( ImproveUsingHistogram->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_HISTOGRAM,1); } else
                                            { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_HISTOGRAM,0); }


  // MOVEMENT
  if(PatchSensitivity->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_PATCH_SENSITIVITY,(unsigned int) value); }


  if(MinR->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_MIN_R_THRESHOLD,(unsigned int) value); }
  if(MinG->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_MIN_G_THRESHOLD,(unsigned int) value); }
  if(MinB->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_MIN_B_THRESHOLD,(unsigned int) value); }

  if(MaxR->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_R_THRESHOLD,(unsigned int) value); }
  if(MaxG->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_G_THRESHOLD,(unsigned int) value); }
  if(MaxB->GetValue().ToLong(&value)) { VisCortx_SetSetting(MOVEMENT_B_THRESHOLD,(unsigned int) value); }


  if(PatchMinR->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_HIST_THRESHOLD_R,(unsigned int) value); }
  if(PatchMinG->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_HIST_THRESHOLD_G,(unsigned int) value); }
  if(PatchMinB->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_HIST_THRESHOLD_B,(unsigned int) value); }


}


void CortexSettings::OnSaveButtonClick(wxCommandEvent& event)
{
  PushSettingsToCortex();
  Destroy();
}

void CortexSettings::OnDefaultButtonClick(wxCommandEvent& event)
{
   VisCortx_SetDefaultSettings();
   PullSettingsFromCortex();
   Refresh();
}

void CortexSettings::OnCancelButtonClick(wxCommandEvent& event)
{
    Destroy();
}
