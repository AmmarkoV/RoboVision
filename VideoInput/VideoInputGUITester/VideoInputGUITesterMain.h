/***************************************************************
 * Name:      VideoInputGUITesterMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2011-04-28
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#ifndef VIDEOINPUTGUITESTERMAIN_H
#define VIDEOINPUTGUITESTERMAIN_H

//(*Headers(VideoInputGUITesterDialog)
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/timer.h>
//*)

class VideoInputGUITesterDialog: public wxDialog
{
    public:

        VideoInputGUITesterDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~VideoInputGUITesterDialog();

    private:
        void OnPaint(wxPaintEvent& event);

        //(*Handlers(VideoInputGUITesterDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnDrawTimerTrigger(wxTimerEvent& event);
        void OnButtonQuitClick(wxCommandEvent& event);
        void OnButtonSaveSnapshotClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(VideoInputGUITesterDialog)
        static const long ID_BUTTON1;
        static const long ID_STATICBOX1;
        static const long ID_BUTTON2;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(VideoInputGUITesterDialog)
        wxTimer DrawTimer;
        wxButton* ButtonSaveSnapshot;
        wxStaticBox* StaticBox1;
        wxButton* ButtonQuit;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // VIDEOINPUTGUITESTERMAIN_H
