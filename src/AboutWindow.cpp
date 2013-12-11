/*
	AboutWindow.cpp
	Copyright 2006 Studio-33, All Rights Reserved.
*/


#include "AboutWindow.h"

AboutWindow::AboutWindow(BRect frame)
	: BWindow(frame, "About BeLife", B_MODAL_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_WILL_ACCEPT_FIRST_CLICK | B_NOT_RESIZABLE | B_NOT_ZOOMABLE )
{
	
	AboutView = new BView(BRect(0,0,611,296), "AboutView", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	AboutButton = new BButton(BRect(395,265,560,285), "AboutButton", "Thanks for using BeLife", new BMessage(ABOUT_CLOSE_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	AboutBitmap = BTranslationUtils::GetBitmap("about_bmap");

	AboutView->SetViewColor(170,170,170,0);
		
	AddChild(AboutView);
	
	AboutView->SetViewBitmap(AboutBitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	
	AboutView->AddChild(AboutButton);

	}

void
AboutWindow::MessageReceived(BMessage* message)
{

switch(message->what)
	{
	case ABOUT_CLOSE_MSG:
	
	AboutWindow::Close();
	
	break;
	}
	
}

