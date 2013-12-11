/*
	PatternWindow.h
	Copyright 2006 Studio-33, All Rights Reserved.
*/

#ifndef PATTERN_WINDOW_H
#define PATTERN_WINDOW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <TranslationUtils.h>
#include <Bitmap.h>

#define PAT_0	'0'
#define PAT_1	'1'
#define PAT_2	'2'
#define PAT_3	'3'
#define PAT_4	'4'
#define PAT_5	'5'
#define PAT_6	'6'
#define PAT_7	'7'
#define PAT_8	'8'
#define PAT_9	'9'

class PatternWindow : public BWindow 
{
public:
				PatternWindow(BRect frame); 
				
	virtual void	MessageReceived(BMessage* message);
	
private:
	
	BBitmap* 		Icon1Bitmap;
	BBitmap* 		Icon2Bitmap;

	BView*			Icon1View;
	BView*			Icon2View;
	BView*			Icon3View;
	BView*			Icon4View;
	BView*			Icon5View;
	BView*			Icon6View;
	BView*			Icon7View;
	BView*			Icon8View;
	BView*			Icon9View;

	BView*			PatternView;

	BView*			Pat0View;
	BView*			Pat1View;
	BView*			Pat2View;
	BView*			Pat3View;
	BView*			Pat4View;
	BView*			Pat5View;
	BView*			Pat6View;
	BView*			Pat7View;
	BView*			Pat8View;
	BView*			Pat9View;

	BButton* 		Pat0Button;
	BButton* 		Pat1Button;
	BButton* 		Pat2Button;
	BButton* 		Pat3Button;
	BButton* 		Pat4Button;
	BButton* 		Pat5Button;
	BButton* 		Pat6Button;
	BButton* 		Pat7Button;
	BButton* 		Pat8Button;
	BButton* 		Pat9Button;

	BStringView*	Pat1String;
	BStringView*	Pat2String;
	BStringView*	Pat3String;
	BStringView*	Pat4String;
	BStringView*	Pat5String;
	BStringView*	Pat6String;
	BStringView*	Pat7String;
	BStringView*	Pat8String;
	BStringView*	Pat9String;
	
};

#endif
