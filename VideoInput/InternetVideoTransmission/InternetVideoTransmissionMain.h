/***************************************************************
 * Name:      InternetVideoTransmissionMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2011-10-14
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#ifndef INTERNETVIDEOTRANSMISSIONMAIN_H
#define INTERNETVIDEOTRANSMISSIONMAIN_H

//(*Headers(InternetVideoTransmissionFrame)
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/timer.h>
//*)

class InternetVideoTransmissionFrame: public wxFrame
{
    public:

        InternetVideoTransmissionFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~InternetVideoTransmissionFrame();

    private:

        //(*Handlers(InternetVideoTransmissionFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        //*)

        void OnPaint(wxPaintEvent& event);

        //(*Identifiers(InternetVideoTransmissionFrame)
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(InternetVideoTransmissionFrame)
        wxStatusBar* StatusBar1;
        wxTimer Timer1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // INTERNETVIDEOTRANSMISSIONMAIN_H
