/*
	PatternWindow.cpp
	Copyright 2006 Studio-33, All Rights Reserved.
*/


#include "PatternWindow.h"
#include "BeLife.h"


PatternWindow::PatternWindow(BRect frame)
	: BWindow(frame, "BeLife Pattern Select", B_MODAL_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_WILL_ACCEPT_FIRST_CLICK | B_NOT_RESIZABLE | B_NOT_ZOOMABLE )
{
	
	Icon1Bitmap = BTranslationUtils::GetBitmap("icon1_bmap");
	Icon2Bitmap = BTranslationUtils::GetBitmap("icon2_bmap");
	
	PatternView = new BView(BRect(0,0,300,400), "PatternView", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat0View = new BView(BRect(0,0,300,40), "Pat0View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat1View = new BView(BRect(0,40,300,80), "Pat1View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat2View = new BView(BRect(0,80,300,120), "Pat2View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat3View = new BView(BRect(0,120,300,160), "Pat3View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat4View = new BView(BRect(0,160,300,200), "Pat4View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat5View = new BView(BRect(0,200,300,240), "Pat5View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat6View = new BView(BRect(0,240,300,280), "Pat6View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat7View = new BView(BRect(0,280,300,320), "Pat7View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat8View = new BView(BRect(0,320,300,360), "Pat8View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Pat9View = new BView(BRect(0,360,300,400), "Pat9View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	
	Icon1View = new BView(BRect(10,4,42,36), "Icon1View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon2View = new BView(BRect(10,4,42,36), "Icon2View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon3View = new BView(BRect(10,4,42,36), "Icon3View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon4View = new BView(BRect(10,4,42,36), "Icon4View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon5View = new BView(BRect(10,4,42,36), "Icon5View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon6View = new BView(BRect(10,4,42,36), "Icon6View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon7View = new BView(BRect(10,4,42,36), "Icon7View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon8View = new BView(BRect(10,4,42,36), "Icon8View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	Icon9View = new BView(BRect(10,4,42,36), "Icon9View", B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW);
	
	Pat1String = new BStringView(BRect(60,3,230,33), "Pat1String", "'DROP'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat2String = new BStringView(BRect(60,3,230,33), "Pat2String", "'SQUARE'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat3String = new BStringView(BRect(60,3,230,33), "Pat3String", "'EYE'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat4String = new BStringView(BRect(60,3,230,33), "Pat4String", "'BEETLE'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat5String = new BStringView(BRect(60,3,230,33), "Pat5String", "'TOAD'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat6String = new BStringView(BRect(60,3,230,33), "Pat6String", "'BOULDER'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat7String = new BStringView(BRect(60,3,230,33), "Pat7String", "'PULSAR'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat8String = new BStringView(BRect(60,3,230,33), "Pat8String", "'GOSPER'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	Pat9String = new BStringView(BRect(60,3,230,33), "Pat9String", "'BELIFE'", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	
	Pat0Button = new BButton(BRect(235,4,295,34), "Pat0Button", "Cancel", new BMessage(PAT_0), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat1Button = new BButton(BRect(235,4,295,34), "Pat1Button", "Choose", new BMessage(PAT_1), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat2Button = new BButton(BRect(235,4,295,34), "Pat2Button", "Choose", new BMessage(PAT_2), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat3Button = new BButton(BRect(235,4,295,34), "Pat3Button", "Choose", new BMessage(PAT_3), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat4Button = new BButton(BRect(235,4,295,34), "Pat4Button", "Choose", new BMessage(PAT_4), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat5Button = new BButton(BRect(235,4,295,34), "Pat5Button", "Choose", new BMessage(PAT_5), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat6Button = new BButton(BRect(235,4,295,34), "Pat6Button", "Choose", new BMessage(PAT_6), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat7Button = new BButton(BRect(235,4,295,34), "Pat7Button", "Choose", new BMessage(PAT_7), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat8Button = new BButton(BRect(235,4,295,34), "Pat8Button", "Choose", new BMessage(PAT_8), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	Pat9Button = new BButton(BRect(235,4,295,34), "Pat9Button", "Choose", new BMessage(PAT_9), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

	PatternView->SetViewColor(216,216,216,0);
	Pat0View->SetViewColor(226,226,226,0);
	Pat1View->SetViewColor(206,206,206,0);
	Pat2View->SetViewColor(226,226,226,0);
	Pat3View->SetViewColor(206,206,206,0);
	Pat4View->SetViewColor(226,226,226,0);
	Pat5View->SetViewColor(206,206,206,0);
	Pat6View->SetViewColor(226,226,226,0);
	Pat7View->SetViewColor(206,206,206,0);
	Pat8View->SetViewColor(226,226,226,0);
	Pat9View->SetViewColor(206,206,206,0);
	
	Pat1String->SetAlignment(B_ALIGN_LEFT);
	Pat2String->SetAlignment(B_ALIGN_LEFT);
	Pat3String->SetAlignment(B_ALIGN_LEFT);
	Pat4String->SetAlignment(B_ALIGN_LEFT);
	Pat5String->SetAlignment(B_ALIGN_LEFT);
	Pat6String->SetAlignment(B_ALIGN_LEFT);
	Pat7String->SetAlignment(B_ALIGN_LEFT);
	Pat8String->SetAlignment(B_ALIGN_LEFT);
	Pat9String->SetAlignment(B_ALIGN_LEFT);
	
	Pat1String->SetFontSize(18);
	Pat2String->SetFontSize(18);
	Pat3String->SetFontSize(18);
	Pat4String->SetFontSize(18);
	Pat5String->SetFontSize(18);
	Pat6String->SetFontSize(18);
	Pat7String->SetFontSize(18);
	Pat8String->SetFontSize(18);
	Pat9String->SetFontSize(18);
		
	AddChild(PatternView);
	PatternView->AddChild(Pat0View);	
	PatternView->AddChild(Pat1View);	
	PatternView->AddChild(Pat2View);	
	PatternView->AddChild(Pat3View);	
	PatternView->AddChild(Pat4View);	
	PatternView->AddChild(Pat5View);	
	PatternView->AddChild(Pat6View);	
	PatternView->AddChild(Pat7View);	
	PatternView->AddChild(Pat8View);	
	PatternView->AddChild(Pat9View);	

	Pat1View->AddChild(Icon1View);
	Pat2View->AddChild(Icon2View);
	Pat3View->AddChild(Icon3View);
	Pat4View->AddChild(Icon4View);
	Pat5View->AddChild(Icon5View);
	Pat6View->AddChild(Icon6View);
	Pat7View->AddChild(Icon7View);
	Pat8View->AddChild(Icon8View);
	Pat9View->AddChild(Icon9View);

	Pat0View->AddChild(Pat0Button);
	Pat1View->AddChild(Pat1Button);
	Pat2View->AddChild(Pat2Button);
	Pat3View->AddChild(Pat3Button);
	Pat4View->AddChild(Pat4Button);
	Pat5View->AddChild(Pat5Button);
	Pat6View->AddChild(Pat6Button);
	Pat7View->AddChild(Pat7Button);
	Pat8View->AddChild(Pat8Button);
	Pat9View->AddChild(Pat9Button);

	Pat1View->AddChild(Pat1String);
	Pat2View->AddChild(Pat2String);	
	Pat3View->AddChild(Pat3String);	
	Pat4View->AddChild(Pat4String);	
	Pat5View->AddChild(Pat5String);	
	Pat6View->AddChild(Pat6String);	
	Pat7View->AddChild(Pat7String);	
	Pat8View->AddChild(Pat8String);	
	Pat9View->AddChild(Pat9String);	

	Icon1View->SetViewBitmap(Icon1Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon2View->SetViewBitmap(Icon2Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon3View->SetViewBitmap(Icon1Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon4View->SetViewBitmap(Icon2Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon5View->SetViewBitmap(Icon1Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon6View->SetViewBitmap(Icon2Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon7View->SetViewBitmap(Icon1Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon8View->SetViewBitmap(Icon2Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);
	Icon9View->SetViewBitmap(Icon1Bitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_TILE_BITMAP);

	}

void
PatternWindow::MessageReceived(BMessage* message)
{

switch(message->what)
	{
	case PAT_0:
		PatternWindow::Close();
	break;

	case PAT_1:
		((dotterApp *)be_app)->Pat1();
		PatternWindow::Close();
	break;

	case PAT_2:
		((dotterApp *)be_app)->Pat2();
		PatternWindow::Close();
	break;

	case PAT_3:
		((dotterApp *)be_app)->Pat3();
		PatternWindow::Close();
	break;

	case PAT_4:
		((dotterApp *)be_app)->Pat4();
		PatternWindow::Close();
	break;

	case PAT_5:
		((dotterApp *)be_app)->Pat5();
		PatternWindow::Close();
	break;

	case PAT_6:
		((dotterApp *)be_app)->Pat6();
		PatternWindow::Close();
	break;

	case PAT_7:
		((dotterApp *)be_app)->Pat7();
		PatternWindow::Close();
	break;

	case PAT_8:
		((dotterApp *)be_app)->Pat8();
		PatternWindow::Close();
	break;

	case PAT_9:
		((dotterApp *)be_app)->Pat9();
		PatternWindow::Close();
	break;

	default:
		
	break;
	
	}
	
}

