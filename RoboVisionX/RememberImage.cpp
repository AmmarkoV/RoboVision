#include "RememberImage.h"


//(*InternalHeaders(RememberImage)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(RememberImage)
const long RememberImage::ID_BUTTON1 = wxNewId();
const long RememberImage::ID_BUTTON2 = wxNewId();
const long RememberImage::ID_TEXTCTRL1 = wxNewId();
const long RememberImage::ID_STATICTEXT1 = wxNewId();
const long RememberImage::ID_STATICTEXT2 = wxNewId();
const long RememberImage::ID_TEXTCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(RememberImage,wxDialog)
	//(*EventTable(RememberImage)
	//*)
END_EVENT_TABLE()

RememberImage::RememberImage(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(RememberImage)
	Create(parent, id, _("Add Patch to Image Memory"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(445,324));
	Move(wxDefaultPosition);
	ButtonAddToDatabase = new wxButton(this, ID_BUTTON1, _("Add To Database"), wxPoint(56,224), wxSize(328,45), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	ButtonBack = new wxButton(this, ID_BUTTON2, _("Back"), wxPoint(176,272), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	SignatureText = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(56,32), wxSize(328,72), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Signature :"), wxPoint(24,8), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Keywords ( Seperated by whitespace )  :"), wxPoint(24,112), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	KeywordsText = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(56,136), wxSize(328,80), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL2"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RememberImage::OnButtonAddToDatabaseClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RememberImage::OnButtonBackClick);
	//*)

	ready=0;
}

RememberImage::~RememberImage()
{
	//(*Destroy(RememberImage)
	//*)
}


void RememberImage::OnButtonBackClick(wxCommandEvent& event)
{
    Close();
}

void RememberImage::OnButtonAddToDatabaseClick(wxCommandEvent& event)
{
     char keywords[1024]={0};
     fprintf(stderr,"TODO: Get keywords from textbox\n");

     char command[1024]={0};
     sprintf(command,"REMEMBER IMAGE(%u,%u,%u,%u,%u,\"%s\")",camera,patch_x,patch_y,patch_width,patch_height,keywords);
     IssueCommand((char *) command,0,0,(char *)"GUI"); fprintf(stderr,"Requesting Image segment to be stored!\n");
     Close();
}
