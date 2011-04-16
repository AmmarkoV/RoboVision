#ifndef REMEMBERIMAGE_H
#define REMEMBERIMAGE_H

//(*Headers(RememberImage)
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class RememberImage: public wxDialog
{
	public:

		RememberImage(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~RememberImage();

		//(*Declarations(RememberImage)
		wxButton* ButtonBack;
		wxTextCtrl* SignatureText;
		wxButton* ButtonAddToDatabase;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* KeywordsText;
		//*)

	protected:

		//(*Identifiers(RememberImage)
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		//*)

	private:

		//(*Handlers(RememberImage)
		void OnButtonBackClick(wxCommandEvent& event);
		void OnButtonAddToDatabaseClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
