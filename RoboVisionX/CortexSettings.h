#ifndef CORTEXSETTINGS_H
#define CORTEXSETTINGS_H

//(*Headers(CortexSettings)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbox.h>
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
		wxStaticText* StaticText17;
		wxButton* SaveButton;
		wxTextCtrl* PatchMinB;
		wxTextCtrl* PatchMinG;
		wxStaticText* StaticText20;
		wxStaticText* StaticText18;
		wxStaticText* StaticText1;
		wxStaticText* StaticText10;
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
		wxTextCtrl* OffsetUp;
		wxButton* DefaultButton;
		wxCheckBox* ImproveUsingHistogram;
		wxStaticText* StaticText8;
		wxTextCtrl* Quality;
		wxStaticText* StaticText12;
		wxCheckBox* ImproveEdges;
		wxStaticBox* StaticBox1;
		wxStaticText* StaticText7;
		wxTextCtrl* MaxR;
		wxStaticText* StaticText4;
		wxCheckBox* FillHoles;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxTextCtrl* StartLeft;
		wxTextCtrl* PatchSensitivity;
		wxStaticText* StaticText19;
		wxStaticText* StaticText9;
		wxTextCtrl* OffsetDown;
		wxStaticText* StaticText11;
		wxTextCtrl* MaxB;
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
