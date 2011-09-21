#ifndef CORTEXSETTINGS_H
#define CORTEXSETTINGS_H

//(*Headers(CortexSettings)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class CortexSettings: public wxDialog
{
	public:

		CortexSettings(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CortexSettings();
        void PullSettingsFromCortex();
        void PushSettingsToCortex();

		//(*Declarations(CortexSettings)
		wxTextCtrl* ComparisonMinScore;
		wxStaticText* StaticText24;
		wxButton* CancelButton;
		wxStaticText* StaticText22;
		wxTextCtrl* MinG;
		wxTextCtrl* ClosestDepth;
		wxTextCtrl* MinB;
		wxTextCtrl* MaxG;
		wxStaticText* StaticText21;
		wxStaticText* StaticText13;
		wxTextCtrl* PatchMinR;
		wxStaticText* StaticText14;
		wxCheckBox* DepthMapGuesses;
		wxStaticText* StaticText15;
		wxTextCtrl* ComparisonThreshold;
		wxStaticText* StaticText35;
		wxStaticText* StaticText37;
		wxTextCtrl* ComparisonThresholdLarge;
		wxStaticText* StaticText32;
		wxStaticText* StaticText17;
		wxStaticText* StaticText28;
		wxTextCtrl* ComparisonThresholdMedium;
		wxButton* SaveButton;
		wxCheckBox* DoNotProcessCloserThanPreviousSizeResult;
		wxTextCtrl* PatchMinB;
		wxTextCtrl* ComparisonExpectedThreshold;
		wxTextCtrl* PatchMinG;
		wxStaticText* StaticText20;
		wxTextCtrl* PatchSizeYMedium;
		wxStaticText* StaticText18;
		wxStaticText* StaticText1;
		wxTextCtrl* SobelMultiplier;
		wxStaticText* StaticText10;
		wxTextCtrl* PatchSizeXLarge;
		wxStaticText* StaticText16;
		wxTextCtrl* EdgeStrictness;
		wxCheckBox* DoNotProcessFar;
		wxStaticBox* StaticBox2;
		wxTextCtrl* PatchesEdgesPerCent;
		wxTextCtrl* MinR;
		wxStaticText* StaticText3;
		wxTextCtrl* InstantQuality;
		wxTextCtrl* EdgeStrictnessHigh;
		wxStaticText* StaticText23;
		wxTextCtrl* PatchSizeYLarge;
		wxStaticLine* StaticLine1;
		wxTextCtrl* PatchesEdgesPercentMedium;
		wxTextCtrl* OffsetUp;
		wxButton* DefaultButton;
		wxTextCtrl* RGBMultiplier;
		wxCheckBox* ImproveUsingHistogram;
		wxStaticText* StaticText8;
		wxTextCtrl* Quality;
		wxStaticText* StaticText34;
		wxStaticText* StaticText12;
		wxStaticText* StaticText38;
		wxCheckBox* ImproveEdges;
		wxTextCtrl* MoveMultiplier;
		wxTextCtrl* PatchesEdgesPercentLarge;
		wxStaticBox* StaticBox1;
		wxStaticText* StaticText7;
		wxTextCtrl* MaxR;
		wxTextCtrl* ShiftDown;
		wxStaticText* StaticText4;
		wxTextCtrl* SecondDerMultiplier;
		wxCheckBox* FillHoles;
		wxStaticText* StaticText5;
		wxStaticText* StaticText36;
		wxStaticText* StaticText30;
		wxStaticText* StaticText2;
		wxTextCtrl* ShiftUp;
		wxStaticText* StaticText27;
		wxTextCtrl* FeatureDetectionThreshold;
		wxStaticText* StaticText26;
		wxStaticText* StaticText6;
		wxTextCtrl* StartLeft;
		wxTextCtrl* PatchSensitivity;
		wxStaticText* StaticText19;
		wxStaticText* StaticText29;
		wxTextCtrl* PatchSizeXMedium;
		wxStaticText* StaticText9;
		wxTextCtrl* OffsetDown;
		wxStaticText* StaticText33;
		wxStaticText* StaticText31;
		wxTextCtrl* PatchSizeXSmall;
		wxStaticText* StaticText11;
		wxStaticText* StaticText25;
		wxTextCtrl* MaxB;
		wxCheckBox* DepthMapReverseCheck;
		wxTextCtrl* PatchSizeYSmall;
		//*)

	protected:

		//(*Identifiers(CortexSettings)
		static const long ID_STATICBOX1;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_TEXTCTRL6;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT7;
		static const long ID_TEXTCTRL8;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL9;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRL10;
		static const long ID_STATICTEXT10;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_STATICBOX2;
		static const long ID_STATICTEXT11;
		static const long ID_TEXTCTRL11;
		static const long ID_STATICTEXT12;
		static const long ID_TEXTCTRL12;
		static const long ID_TEXTCTRL13;
		static const long ID_TEXTCTRL14;
		static const long ID_STATICTEXT13;
		static const long ID_STATICTEXT14;
		static const long ID_TEXTCTRL15;
		static const long ID_TEXTCTRL16;
		static const long ID_TEXTCTRL17;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_CHECKBOX3;
		static const long ID_STATICTEXT19;
		static const long ID_STATICTEXT20;
		static const long ID_TEXTCTRL18;
		static const long ID_STATICTEXT21;
		static const long ID_TEXTCTRL19;
		static const long ID_STATICTEXT22;
		static const long ID_TEXTCTRL20;
		static const long ID_CHECKBOX4;
		static const long ID_CHECKBOX5;
		static const long ID_TEXTCTRL21;
		static const long ID_STATICTEXT23;
		static const long ID_TEXTCTRL22;
		static const long ID_STATICTEXT24;
		static const long ID_STATICTEXT25;
		static const long ID_STATICTEXT26;
		static const long ID_TEXTCTRL23;
		static const long ID_TEXTCTRL24;
		static const long ID_TEXTCTRL25;
		static const long ID_TEXTCTRL26;
		static const long ID_TEXTCTRL27;
		static const long ID_TEXTCTRL28;
		static const long ID_TEXTCTRL29;
		static const long ID_TEXTCTRL30;
		static const long ID_STATICTEXT27;
		static const long ID_STATICTEXT28;
		static const long ID_STATICTEXT29;
		static const long ID_TEXTCTRL31;
		static const long ID_TEXTCTRL32;
		static const long ID_STATICTEXT30;
		static const long ID_STATICTEXT31;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT32;
		static const long ID_TEXTCTRL33;
		static const long ID_TEXTCTRL34;
		static const long ID_CHECKBOX6;
		static const long ID_STATICTEXT33;
		static const long ID_TEXTCTRL35;
		static const long ID_TEXTCTRL36;
		static const long ID_TEXTCTRL37;
		static const long ID_TEXTCTRL38;
		static const long ID_STATICTEXT34;
		static const long ID_STATICTEXT35;
		static const long ID_STATICTEXT36;
		static const long ID_STATICTEXT37;
		static const long ID_CHECKBOX7;
		static const long ID_STATICTEXT38;
		static const long ID_TEXTCTRL39;
		//*)

	private:

		//(*Handlers(CortexSettings)
		void OnSaveButtonClick(wxCommandEvent& event);
		void OnDefaultButtonClick(wxCommandEvent& event);
		void OnCancelButtonClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
