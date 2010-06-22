/***************************************************************
 * Name:      WorldMappingApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-26
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "WorldMappingApp.h"

//(*AppHeaders
#include "WorldMappingMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(WorldMappingApp);

bool WorldMappingApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	WorldMappingFrame* Frame = new WorldMappingFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
