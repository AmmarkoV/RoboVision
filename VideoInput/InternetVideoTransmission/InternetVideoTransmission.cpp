/***************************************************************
 * Name:      VideoInputGUITesterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2011-04-28
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "VideoInputGUITesterApp.h"

//(*AppHeaders
#include "VideoInputGUITesterMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(VideoInputGUITesterApp);

bool VideoInputGUITesterApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	VideoInputGUITesterDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
