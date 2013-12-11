/*
	BeLife.h
	Copyright 2006 Studio-33, All Rights Reserved.
*/

#include <Point.h>
#include <Application.h>
#include <InterfaceKit.h>
#include <map.h>
#include <OS.h>
#include <Beep.h>
#include <Screen.h>
#include <Deskbar.h>
#include <stdio.h>
#include <stdlib.h>
#include "AboutWindow.h"
#include "PatternWindow.h"

#define SET_ZOOM_LEVEL 'szlv'
#define SET_SPEED_LEVEL 'sslv'
#define START_GAME 'stgm'
#define SET_GRID 'stgd'
#define PAUSE_GAME 'psgm'
#define SET_BACK_COL 'sbcl'
#define SET_FRONT_COL 'sfcl'
#define KILL_ALL 'kill'
#define GIVE_BIRTH 'give'
#define SPEC_BIRTH 'spec'
#define B_ABOUT_REQUESTED 'bout'


class cell
{
	public:
		cell();
		void alertNs();
		void increment();
		void makeAlive();
		bool alive;
		int aliveNs;	
};

class LPoint : public BPoint
{
	public:
		LPoint();
		LPoint(float X, float Y);
		LPoint(const LPoint& pt);
		LPoint(const BPoint& pt);
		bool operator< (const LPoint) const;
};

class LWindow : public BWindow
{
	public: 
		LWindow(BRect, char *, window_type, long int);
		void MessageReceived(BMessage * message);
		thread_id tid;
};


class LView : public BView
{
	public: 
					 LView(BRect frame, char* name);
		virtual void Draw(BRect updateRect);
		std::map<LPoint, cell> worldCopy;
		void MouseDown(BPoint point);
};

class CView : public BView
{
	public:
					 CView(BRect frame, char* name);
		virtual void Draw( BRect );
		
};

		
class dotterApp : public BApplication
{
	public:
		dotterApp();
		void ReadyToRun();
		virtual bool QuitRequested();
		virtual	void AboutRequested();
		void SetZoom();
		void SetSpeed();
		void SetGrid();
		void SetBack();
		void SetFront();
		void KillAll();
		void GiveBirth();
		void SpecBirth();
		void Pat1();
		void Pat2();
		void Pat3();
		void Pat4();
		void Pat5();
		void Pat6();
		void Pat7();
		void Pat8();
		void Pat9();
		void StartButtonOn(bool);
		void display(std::map<LPoint, cell>);
		BColorControl*	backColor;
		BColorControl*	frontColor;

		
	private:
		BRect 	  		frame;
		LWindow* 		lifeWindow;
		LView* 			lifeView;
		CView*			controlView;
		BScrollView* 	lifeScroller;
		BWindow* 		controlWindow;
		BSlider* 		zoomSlider;
		BSlider* 		speedSlider;
		BCheckBox* 		gridCheckbox;
		BStringView*	backString;
		BStringView*	frontString;
		BButton*		startButton;
		BButton*		aboutButton;
		BButton*		clearButton;
		BButton*		randomButton;
		BButton*		specialButton;
};

