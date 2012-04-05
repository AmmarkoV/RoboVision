#ifndef MAPOVERVIEW_H
#define MAPOVERVIEW_H

//(*Headers(MapOverview)
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class MapOverview: public wxFrame
{
	public:

		MapOverview(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MapOverview();

		//(*Declarations(MapOverview)
		wxButton* ButtonExecute;
		wxTextCtrl* TextTargetPosX;
		wxCheckBox* CheckBoxAutoUpdate;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCurPosX;
		wxStaticText* StaticText3;
		wxStaticBox* StaticBox1;
		wxTextCtrl* TextTargetPosY;
		wxTextCtrl* TextCurPosY;
		//*)

	protected:

		//(*Identifiers(MapOverview)
		static const long ID_STATICBOX1;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_CHECKBOX1;
		static const long ID_BUTTON1;
		//*)

	private:

		//(*Handlers(MapOverview)
		void OnButtonExecuteClick(wxCommandEvent& event);
		//*)

        void OnPaint(wxPaintEvent& event);
        void OnMotion(wxMouseEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
