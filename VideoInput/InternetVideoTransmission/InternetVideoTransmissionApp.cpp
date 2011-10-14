/***************************************************************
 * Name:      InternetVideoTransmissionApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2011-10-14
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "InternetVideoTransmissionApp.h"

//(*AppHeaders
#include "InternetVideoTransmissionMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(InternetVideoTransmissionApp);

bool InternetVideoTransmissionApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	InternetVideoTransmissionFrame* Frame = new InternetVideoTransmissionFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
