/***************************************************************
 * Name:      RoboVisionXMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-01
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#ifndef ROBOVISIONXMAIN_H
#define ROBOVISIONXMAIN_H

//(*Headers(RoboVisionXFrame)
#include <wx/gauge.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/slider.h>
#include <wx/statusbr.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/timer.h>
//*)
#include <wx/sound.h>

class RoboVisionXFrame: public wxFrame
{
    public:

        RoboVisionXFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~RoboVisionXFrame();
        void RedrawWindow();
        void DrawPatchComp(unsigned int basedon,unsigned int x,unsigned int y);
        void PrintStats();

        int feed_0_x,feed_0_y,feed_1_x,feed_1_y,feed_2_x,feed_2_y,feed_3_x,feed_3_y;

    private:

        //(*Handlers(RoboVisionXFrame)
        void OnTimerEvent(wxTimerEvent &event);
        void OnQuit(wxCommandEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnButtonDepthMapClick(wxCommandEvent& event);
        void OnMotionAlarmButtonClick(wxCommandEvent& event);
        void OnSwapFeedsClick(wxCommandEvent& event);
        void OnRecordButtonClick(wxCommandEvent& event);
        void OnPlayButtonClick(wxCommandEvent& event);
        void OnLiveButtonClick(wxCommandEvent& event);
        void OnAddTrackPointClick(wxCommandEvent& event);
        void OnConfigurationButtonClick(wxCommandEvent& event);
        void OnOkClick(wxCommandEvent& event);
        void OnUpButtonClick(wxCommandEvent& event);
        void OnDownButtonClick(wxCommandEvent& event);
        void OnStopButtonClick(wxCommandEvent& event);
        void OnLeftButtonClick(wxCommandEvent& event);
        void OnRightButtonClick(wxCommandEvent& event);
        void OnMovementVerticalCmdScroll(wxScrollEvent& event);
        void OnMovementHorizontalCmdScroll(wxScrollEvent& event);
        void OnSaveSnapshotsClick(wxCommandEvent& event);
        void OnAutonomousClick(wxCommandEvent& event);
        void OnAutonomousClick1(wxCommandEvent& event);
        //*)
        void OnMotion(wxMouseEvent& event);

        //(*Identifiers(RoboVisionXFrame)
        static const long ID_STATICBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_BUTTON2;
        static const long ID_STATICBOX2;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT8;
        static const long ID_GAUGE1;
        static const long ID_STATICTEXT9;
        static const long ID_STATICTEXT10;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_BUTTON7;
        static const long ID_BUTTON8;
        static const long ID_CHECKBOX1;
        static const long ID_CHOICE1;
        static const long ID_CHOICE2;
        static const long ID_BUTTON9;
        static const long ID_BUTTON10;
        static const long ID_BUTTON11;
        static const long ID_BUTTON12;
        static const long ID_BUTTON13;
        static const long ID_BUTTON14;
        static const long ID_CHECKBOX2;
        static const long ID_SLIDER1;
        static const long ID_SLIDER2;
        static const long ID_CHECKBOX3;
        static const long ID_GAUGE2;
        static const long ID_GAUGE3;
        static const long ID_STATICTEXT11;
        static const long ID_STATICTEXT12;
        static const long ID_STATICTEXT13;
        static const long ID_STATICTEXT14;
        static const long ID_STATICTEXT15;
        static const long ID_CHECKBOX4;
        static const long ID_STATICTEXT16;
        static const long idMenuOpenSnapshots;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(RoboVisionXFrame)
        wxButton* PlayButton;
        wxButton* StopButton;
        wxButton* UpButton;
        wxStatusBar* StatusBar1;
        wxButton* LiveButton;
        wxButton* Ok;
        wxStaticText* MeterComment;
        wxButton* AddTrackPoint;
        wxCheckBox* DrawFeeds;
        wxChoice* Choice2;
        wxSlider* MovementHorizontal;
        wxTextCtrl* Status;
        wxButton* MotionAlarmButton;
        wxButton* ButtonDepthMap;
        wxStaticText* StaticText1;
        wxTimer Timer1;
        wxButton* LeftButton;
        wxStaticBox* StaticBox2;
        wxTextCtrl* CommandBox;
        wxStaticText* StaticText3;
        wxGauge* Ultrasonic1;
        wxStaticText* FrameRate;
        wxChoice* Choice1;
        wxStaticText* RightUltrasonicLabel;
        wxGauge* Ultrasonic2;
        wxStaticText* UptimeLabel;
        wxButton* RecordButton;
        wxCheckBox* SaveLoadStreamOfSnapshots;
        wxStaticBox* StaticBox1;
        wxStaticText* StaticText7;
        wxStaticText* Flow;
        wxButton* DownButton;
        wxStaticText* PoseString;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText2;
        wxButton* SwapFeeds;
        wxCheckBox* lowcpuusage;
        wxStaticText* StaticText6;
        wxButton* ConfigurationButton;
        wxGauge* Battery;
        wxSlider* MovementVertical;
        wxStaticText* Uptime;
        wxButton* RightButton;
        wxStaticText* LeftUltrasonicLabel;
        wxCheckBox* Autonomous;
        wxStaticText* DistanceTraveled;
        //*)

        wxStopWatch *uptimer;

        unsigned int tick_count;
        unsigned int add_new_track_point;
        unsigned int last_draw;
        unsigned int mouse_x,mouse_y;
        unsigned int GUI_Shutdown,dpth_x,dpth_y,dpth_on;

        DECLARE_EVENT_TABLE()
};

void ActivateDepthMapButton();

#endif // ROBOVISIONXMAIN_H
