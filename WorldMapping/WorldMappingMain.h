/***************************************************************
 * Name:      WorldMappingMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-26
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#ifndef WORLDMAPPINGMAIN_H
#define WORLDMAPPINGMAIN_H

#include "MasterpathPlanning/MasterpathPlanning.h"
#include "../InputParser/InputParser_C.h"

#include <wx/joystick.h>
#include "../MotorFoundation/MotorHAL.h"

//(*Headers(WorldMappingFrame)
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class WorldMappingFrame: public wxFrame
{
    public:

        WorldMappingFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~WorldMappingFrame();

    private:
        wxBitmap *draw_area;
        //(*Handlers(WorldMappingFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnSetEndPointClick(wxCommandEvent& event);
        void OnSetStartPointClick(wxCommandEvent& event);
        void OnButtonCalculateClick(wxCommandEvent& event);
        void OnClearButtonClick(wxCommandEvent& event);
        void OnPrintButtonClick(wxCommandEvent& event);
        void OnButtonSimulateUltrasonicClick(wxCommandEvent& event);
        void OnButtonSetRotationClick(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        //*)
        void OnJoystickEvent(wxJoystickEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnMotion(wxMouseEvent& event);
        //(*Identifiers(WorldMappingFrame)
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL3;
        static const long ID_TEXTCTRL4;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL5;
        static const long ID_TEXTCTRL6;
        static const long ID_STATICBOX1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_SPINCTRL1;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL7;
        static const long ID_STATICTEXT6;
        static const long ID_BUTTON7;
        static const long ID_BUTTON8;
        static const long ID_BUTTON9;
        static const long ID_SPINCTRL2;
        static const long ID_BUTTON10;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(WorldMappingFrame)
        wxTextCtrl* obsx;
        wxButton* ButtonExecute;
        wxTextCtrl* ptx1;
        wxStatusBar* StatusBar1;
        wxButton* ButtonSimulateUltrasonic;
        wxTextCtrl* ptx2;
        wxSpinCtrl* TimeMS;
        wxTextCtrl* pty2;
        wxButton* Button1;
        wxButton* Button2;
        wxTextCtrl* scale_cm;
        wxStaticText* StaticText1;
        wxTextCtrl* obsy;
        wxButton* ClearButton;
        wxStaticText* StaticText3;
        wxStaticBox* StaticBox1;
        wxButton* PrintButton;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText2;
        wxButton* SetStartPoint;
        wxSpinCtrl* Rotation;
        wxStaticText* StaticText6;
        wxTextCtrl* pty1;
        wxButton* ButtonCalculate;
        wxButton* SetEndPoint;
        wxButton* ButtonSetRotation;
        //*)
        wxMouseState mouse;
 //       RobotHAL *guarddog;
        wxJoystick *joy_stick;
        //Map *floor_plan;
        struct Map * floor;
        int set_point_flag;

        DECLARE_EVENT_TABLE()
};

#endif // WORLDMAPPINGMAIN_H
