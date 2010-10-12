/***************************************************************
 * Name:      RoboVisionXApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-01
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "RoboVisionXApp.h"

//(*AppHeaders
#include "RoboVisionXMain.h"
#include <wx/image.h>
//*)

// ROBOVISION X 0.0

IMPLEMENT_APP(RoboVisionXApp);

bool RoboVisionXApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	RoboVisionXFrame* Frame = new RoboVisionXFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
