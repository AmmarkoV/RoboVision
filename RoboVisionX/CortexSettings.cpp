#include "CortexSettings.h"
#include "../VisualCortex/VisualCortex.h"

//(*InternalHeaders(CortexSettings)
#include <wx/string.h>
#include <wx/intl.h>
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
//*)

BEGIN_EVENT_TABLE(CortexSettings,wxDialog)
	//(*EventTable(CortexSettings)
	//*)
END_EVENT_TABLE()

CortexSettings::CortexSettings(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(CortexSettings)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(719,542));
	Move(wxDefaultPosition);
	StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Disparity Map"), wxPoint(24,16), wxSize(456,312), 0, _T("ID_STATICBOX1"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Quality ( 1..15)"), wxPoint(32,48), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	Quality = new wxTextCtrl(this, ID_TEXTCTRL1, _("6"), wxPoint(136,40), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	SaveButton = new wxButton(this, ID_BUTTON1, _("Save"), wxPoint(312,496), wxSize(157,29), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	SaveButton->SetDefault();
	CancelButton = new wxButton(this, ID_BUTTON2, _("Cancel"), wxPoint(600,496), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Start Left Px"), wxPoint(32,112), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StartLeft = new wxTextCtrl(this, ID_TEXTCTRL2, _("15"), wxPoint(128,104), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Edge Strictness"), wxPoint(240,48), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	EdgeStrictness = new wxTextCtrl(this, ID_TEXTCTRL3, _("25"), wxPoint(360,40), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	DefaultButton = new wxButton(this, ID_BUTTON3, _("Default"), wxPoint(512,496), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Instant Quality (1..15)"), wxPoint(32,80), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	InstantQuality = new wxTextCtrl(this, ID_TEXTCTRL4, _("2"), wxPoint(184,72), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Camera Offsets Up/Down"), wxPoint(240,80), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	OffsetUp = new wxTextCtrl(this, ID_TEXTCTRL5, _("1"), wxPoint(416,72), wxSize(24,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	OffsetDown = new wxTextCtrl(this, ID_TEXTCTRL6, _("1"), wxPoint(448,72), wxSize(24,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Comparison Threshold"), wxPoint(240,112), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	ComparisonThreshold = new wxTextCtrl(this, ID_TEXTCTRL7, _("15000"), wxPoint(392,104), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Comparison Min Score"), wxPoint(240,144), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	ComparisonMinScore = new wxTextCtrl(this, ID_TEXTCTRL8, _("30000"), wxPoint(392,136), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Closest Depth Plane"), wxPoint(32,144), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	ClosestDepth = new wxTextCtrl(this, ID_TEXTCTRL9, _("90"), wxPoint(168,136), wxSize(56,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Comparison EdgesPerCent Required"), wxPoint(152,192), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	PatchesEdgesPerCent = new wxTextCtrl(this, ID_TEXTCTRL10, _("15"), wxPoint(392,184), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("%"), wxPoint(440,186), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FillHoles = new wxCheckBox(this, ID_CHECKBOX1, _("Fill Holes"), wxPoint(208,224), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	FillHoles->SetValue(false);
	ImproveEdges = new wxCheckBox(this, ID_CHECKBOX2, _("Improve Using Edges"), wxPoint(304,224), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	ImproveEdges->SetValue(false);
	StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Movement"), wxPoint(488,16), wxSize(208,224), 0, _T("ID_STATICBOX2"));
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Patch Sensitivity"), wxPoint(496,40), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	PatchSensitivity = new wxTextCtrl(this, ID_TEXTCTRL11, _("7"), wxPoint(616,32), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("MinRGB Threshold"), wxPoint(496,72), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	MinR = new wxTextCtrl(this, ID_TEXTCTRL12, _("25"), wxPoint(512,104), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL12"));
	MinG = new wxTextCtrl(this, ID_TEXTCTRL13, _("25"), wxPoint(568,104), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL13"));
	MinB = new wxTextCtrl(this, ID_TEXTCTRL14, _("25"), wxPoint(632,104), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("R"), wxPoint(496,108), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("G"), wxPoint(552,108), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	MaxR = new wxTextCtrl(this, ID_TEXTCTRL15, _("45"), wxPoint(512,144), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
	MaxG = new wxTextCtrl(this, ID_TEXTCTRL16, _("45"), wxPoint(568,144), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
	MaxB = new wxTextCtrl(this, ID_TEXTCTRL17, _("45"), wxPoint(632,144), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL17"));
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("B"), wxPoint(608,108), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("R"), wxPoint(496,148), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("G"), wxPoint(552,148), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("B"), wxPoint(608,148), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	DepthMapGuesses = new wxCheckBox(this, ID_CHECKBOX3, _("Depth Map Guessing"), wxPoint(32,224), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	DepthMapGuesses->SetValue(false);
	StaticText19 = new wxStaticText(this, ID_STATICTEXT19, _("Patch Histogram Threshold"), wxPoint(504,192), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	StaticText20 = new wxStaticText(this, ID_STATICTEXT20, _("R"), wxPoint(496,230), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	PatchMinR = new wxTextCtrl(this, ID_TEXTCTRL18, _("18"), wxPoint(512,224), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL18"));
	StaticText21 = new wxStaticText(this, ID_STATICTEXT21, _("G"), wxPoint(552,230), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	PatchMinG = new wxTextCtrl(this, ID_TEXTCTRL19, _("18"), wxPoint(568,224), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL19"));
	StaticText22 = new wxStaticText(this, ID_STATICTEXT22, _("B"), wxPoint(608,230), wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	PatchMinB = new wxTextCtrl(this, ID_TEXTCTRL20, _("18"), wxPoint(632,224), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL20"));
	ImproveUsingHistogram = new wxCheckBox(this, ID_CHECKBOX4, _("Depth Map Improve Using Histogram"), wxPoint(32,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	ImproveUsingHistogram->SetValue(true);
	DoNotProcessFar = new wxCheckBox(this, ID_CHECKBOX5, _("Do not process further than this"), wxPoint(32,160), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	DoNotProcessFar->SetValue(false);

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

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_EDGE_STRICTNESS);
  EdgeStrictness->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_INSTANT_DETAIL);
  InstantQuality->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_VERT_OFFSET_UP);
  OffsetUp->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_VERT_OFFSET_DOWN);
  OffsetDown->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_COMPARISON_THRESHOLD);
  ComparisonThreshold->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(DEPTHMAP_CLOSEST_DEPTH);
  ClosestDepth->SetValue(val);

  if (VisCortx_GetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH)==1) { DoNotProcessFar->SetValue(1); } else
                                                                                             { DoNotProcessFar->SetValue(0); }

 if (VisCortx_GetSetting(DEPTHMAP_GUESSES)==1) { DepthMapGuesses->SetValue(1); } else
                                               { DepthMapGuesses->SetValue(0); }

 if (VisCortx_GetSetting(DEPTHMAP_IMPROVE_FILLING_HOLES)==1) { FillHoles->SetValue(1); } else
                                                             { FillHoles->SetValue(0); }

 if (VisCortx_GetSetting(DEPTHMAP_IMPROVE_USING_EDGES)==1) { ImproveEdges->SetValue(1); } else
                                                           { ImproveEdges->SetValue(0); }


 if (VisCortx_GetSetting(DEPTHMAP_IMPROVE_USING_HISTOGRAM)==1) { ImproveUsingHistogram->SetValue(1); } else
                                                               { ImproveUsingHistogram->SetValue(0); }
 // PATCHES

  val.Clear(); val<<VisCortx_GetSetting(PATCH_COMPARISON_SCORE_MIN);
  ComparisonMinScore->SetValue(val);

  val.Clear(); val<<VisCortx_GetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED);
  PatchesEdgesPerCent->SetValue(val);


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

  if(EdgeStrictness->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_EDGE_STRICTNESS,(unsigned int) value); }

  if(InstantQuality->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_INSTANT_DETAIL,(unsigned int) value); }

  if(OffsetUp->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_UP,(unsigned int) value); }

  if(OffsetDown->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_DOWN,(unsigned int) value); }

  if(ComparisonThreshold->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD,(unsigned int) value); }

  if(ClosestDepth->GetValue().ToLong(&value)) { VisCortx_SetSetting(DEPTHMAP_CLOSEST_DEPTH,(unsigned int) value); }

  if ( DoNotProcessFar->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH,1); } else
                                      { VisCortx_SetSetting(DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH,0); }

  if ( DepthMapGuesses->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_GUESSES,1); } else
                                      { VisCortx_SetSetting(DEPTHMAP_GUESSES,0); }

  if ( FillHoles->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_IMPROVE_FILLING_HOLES,1); } else
                                { VisCortx_SetSetting(DEPTHMAP_IMPROVE_FILLING_HOLES,0); }

  if ( ImproveEdges->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_EDGES,1); } else
                                   { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_EDGES,0); }

  if ( ImproveUsingHistogram->IsChecked() ) { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_HISTOGRAM,1); } else
                                            { VisCortx_SetSetting(DEPTHMAP_IMPROVE_USING_HISTOGRAM,0); }


 // PATCHES
  if(ComparisonMinScore->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_COMPARISON_SCORE_MIN,(unsigned int) value); }

  if(PatchesEdgesPerCent->GetValue().ToLong(&value)) { VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED,(unsigned int) value); }

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
