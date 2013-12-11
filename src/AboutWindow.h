/*
	AboutWindow.h
	Copyright 2006 Studio-33, All Rights Reserved.
*/

#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <TranslationUtils.h>
#include <Bitmap.h>

#define ABOUT_CLOSE_MSG	'1'

class AboutWindow : public BWindow 
{
public:
				AboutWindow(BRect frame); 
				
	virtual void	MessageReceived(BMessage* message);
	
private:
	
	BView*			AboutView;
	BButton*		AboutButton;
	BBitmap* 		AboutBitmap;
	
};

#endif
