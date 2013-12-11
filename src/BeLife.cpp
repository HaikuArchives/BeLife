/*
	BeLife.cpp
	Copyright 2006 Studio-33, All Rights Reserved.
*/

#include "BeLife.h"

int32 	speed 	= 500000;
int 	zoom  	= 15;
bool 	grid  	= true;
BRect 	ScreenResolution;
sem_id life_sem;
sem_id compute_sem;

int32 lifeWork(void * incoming);
std::map<LPoint, cell> world;

bool LPoint::operator< (const LPoint a) const  
{	
	return (a.y * 10000 + a.x) < (y * 10000 + x);	
}

LPoint::LPoint() : BPoint(){}
LPoint::LPoint(float X, float Y) : BPoint(X,Y){}
LPoint::LPoint(const LPoint& pt) : BPoint(pt) {}
LPoint::LPoint(const BPoint& pt) : BPoint(pt) {}

cell::cell()			{	alive = false;	aliveNs = 0;	}
void cell::increment()	{	++aliveNs;	}
void cell::makeAlive()	{	alive = true;  cout << "MAKING ALIVE" << endl;}

dotterApp::dotterApp() : BApplication("application/x-vnd.BeLife")
{	
	if ((life_sem = create_sem(1, "belife block copy")) < B_OK)
		be_app->Quit();
	if ((compute_sem = create_sem(1, "belife computing")) < B_OK)
		be_app->Quit();
		
	rgb_color grey = {170,170,170,0};
	rgb_color purple = {115,120,184,0};
	
	lifeWindow = new LWindow(BRect(100,100,700,700), "BeLife", B_DOCUMENT_WINDOW , B_WILL_ACCEPT_FIRST_CLICK | B_QUIT_ON_WINDOW_CLOSE);
	controlWindow = new LWindow(BRect(750,100,950,700), "BeLife Controls", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_WILL_ACCEPT_FIRST_CLICK);
	lifeView   = new LView(BRect(0,0,585,585), "lifeView");
	lifeScroller = new BScrollView("Big Billay", lifeView, B_WILL_DRAW, B_FOLLOW_ALL_SIDES, true, true, B_NO_BORDER);
	controlView = new CView(controlWindow->Bounds(), "controlView");
	zoomSlider = new BSlider(BRect(15,40,185,60), "zoom", "Zoom:", new BMessage(SET_ZOOM_LEVEL), 5, 25);
	speedSlider = new BSlider(BRect(15,85,185,105), "speed", "Speed:", new BMessage(SET_SPEED_LEVEL), 1, 9);
	gridCheckbox = new BCheckBox(BRect(15,10,165,40), "grid", " Show grid ", new BMessage(SET_GRID));
	backString = new BStringView(BRect(15,140,185,160), "backString", "Background color:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	frontString = new BStringView(BRect(15,285,185,305), "frontString", "Foreground color:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	backColor = new BColorControl(BPoint(15,170), B_CELLS_16x16, 1, "backColor", new BMessage(SET_BACK_COL), false);
	frontColor = new BColorControl(BPoint(15,315), B_CELLS_16x16, 1, "frontColor", new BMessage(SET_FRONT_COL), false);
	
	aboutButton = new BButton(BRect(175,5,195,25), "about", " ? ", new BMessage(B_ABOUT_REQUESTED));
	startButton = new BButton(BRect(15,560,185,590), "start", "Start", new BMessage(START_GAME));
	clearButton = new BButton(BRect(15,520,185,550), "clear", "Kill All", new BMessage(KILL_ALL));
	randomButton = new BButton(BRect(15,480,185,510), "random", "Random Pattern", new BMessage(GIVE_BIRTH));
	specialButton = new BButton(BRect(15,440,185,470), "special", "Special Patterns", new BMessage(SPEC_BIRTH));
	
    zoomSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	zoomSlider->SetHashMarkCount(5);
    speedSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	speedSlider->SetHashMarkCount(5);
	gridCheckbox->SetValue(true);
	zoomSlider->SetValue(15);
	speedSlider->SetValue(5);
	lifeView->SetHighColor(115,120,184,0);
	lifeView->SetViewColor(170,170,170,0);
	controlView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	backColor->SetValue(grey);
	frontColor->SetValue(purple);

	lifeWindow->AddChild(lifeScroller);
	controlWindow->AddChild(controlView);
	controlView->AddChild(gridCheckbox);
	controlView->AddChild(zoomSlider);
	controlView->AddChild(speedSlider);
	controlView->AddChild(startButton);
	controlView->AddChild(backString);
	controlView->AddChild(frontString);
	controlView->AddChild(backColor);
	controlView->AddChild(frontColor);
	controlView->AddChild(clearButton);
	controlView->AddChild(randomButton);
	controlView->AddChild(specialButton);
	controlView->AddChild(aboutButton);
	
}

LWindow::LWindow(BRect rect, char * name, window_type f, long int s) 
				:BWindow(rect, name, f, s)
{	
	int test = 0;
	tid = spawn_thread(lifeWork, "life work", 10, (void *)test );
}

LView::LView(BRect frame, char* name) 
				:BView(frame, name, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW)
{	
}

CView::CView(BRect frame, char* name)
				:BView(frame, name, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW)
{
}


void LWindow::MessageReceived(BMessage * message)
{		
	switch (message->what)
	{
		case START_GAME:
			resume_thread( tid);
			((dotterApp *)be_app)->StartButtonOn(true);
			break;	
		case PAUSE_GAME:
			acquire_sem(compute_sem);
			suspend_thread( tid);
			release_sem(compute_sem);
			((dotterApp *)be_app)->StartButtonOn(false);
			break;
		case SET_ZOOM_LEVEL:
			((dotterApp *)be_app)->SetZoom();
			break;
		case SET_SPEED_LEVEL:
			((dotterApp *)be_app)->SetSpeed();
			break;
		case SET_GRID:
			((dotterApp *)be_app)->SetGrid();
			break;
		case SET_BACK_COL:
			((dotterApp *)be_app)->SetBack();
			break;
		case SET_FRONT_COL:
			((dotterApp *)be_app)->SetFront();
			break;
		case B_ABOUT_REQUESTED:
			((dotterApp *)be_app)->AboutRequested();
			break;
		case KILL_ALL:
			((dotterApp *)be_app)->KillAll();
			break;
		case GIVE_BIRTH:
			((dotterApp *)be_app)->GiveBirth();
			break;
		case SPEC_BIRTH:
			((dotterApp *)be_app)->SpecBirth();
			break;

		default:
			BWindow::MessageReceived(message);
			break;
	}	
}


void dotterApp::SpecBirth()
{
	BScreen mScreen(B_MAIN_SCREEN_ID);
	ScreenResolution.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));

	BWindow* patternwindow = new PatternWindow(BRect(((ScreenResolution.right / 2) - 150), ((ScreenResolution.bottom / 2) - 200), ((ScreenResolution.right / 2) + 150), ((ScreenResolution.bottom / 2) + 200)));
	patternwindow->Show();

}


void dotterApp::GiveBirth()
{
			int s = 1;
			acquire_sem(compute_sem);// pauses the game
			BPoint point;
			for(int x = 1; x < 500; x++)
			{
			srand(s);
			point.x = ((int)( rand()%35) + 2 );
			s++;
			srand(s);
			point.y = ((int)( rand()%35) + 2 );
			s++;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			}
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::KillAll()
{
			
			acquire_sem(compute_sem);// pauses the game
			BPoint point;
			point.x = 0;
			point.y = 0;

			for(point.x = 0; point.x <= 500; point.x++)
			{
			
			for(point.y = 0; point.y <= 499; point.y++)
			{
			world.erase(point);
			lifeView->worldCopy.erase(point);
			}
			world.erase(point);
			lifeView->worldCopy.erase(point);
			
			}
			
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
			snooze(10000);
}

void dotterApp::SetGrid()  
{	
	lifeView->Window()->Lock();
	grid = (gridCheckbox->Value());
	lifeView->Invalidate();
	lifeView->Window()->Unlock();
}

void dotterApp::SetZoom()  
{
	lifeView->Window()->Lock();
	zoom = (zoomSlider->Value());
	lifeView->Invalidate();
	lifeView->Window()->Unlock();	
}

void dotterApp::SetBack() 
{
	lifeView->Window()->Lock();
	lifeView->SetViewColor(backColor->ValueAsColor());
	lifeView->Invalidate();
	lifeView->Window()->Unlock();
}
void dotterApp::SetFront() 
{
	lifeView->Window()->Lock();
	lifeView->SetHighColor(frontColor->ValueAsColor());
	lifeView->Invalidate();
	lifeView->Window()->Unlock();	
}

void dotterApp::SetSpeed() 
{	
	
	switch (speedSlider->Value())
		{
		case 1:
		speed = 900000;
		break;

		case 2:
		speed = 800000;
		break;

		case 3:
		speed = 700000;
		break;

		case 4:
		speed = 600000;
		break;

		case 5:
		speed = 500000;
		break;
		
		case 6:
		speed = 400000;
		break;

		case 7:
		speed = 300000;
		break;

		case 8:
		speed = 200000;
		break;

		case 9:
		speed = 100000;
		break;

		default:
		speed = 500000;
		}
}



/////////////////////

void dotterApp::StartButtonOn(bool isOn)
{
	if (isOn)
	{
		startButton->SetLabel("Stop");
		startButton->SetMessage(new	BMessage(PAUSE_GAME));
	}
	else
	{
		startButton->SetLabel("Start");
		startButton->SetMessage(new BMessage(START_GAME));
	}
}
	

void LView::MouseDown(BPoint point)
{	
	uint32 buttons = 0;
	acquire_sem(compute_sem);// pauses the game 
	GetMouse(&point, &buttons, true);
	while (buttons == B_PRIMARY_MOUSE_BUTTON)
	{	
		GetMouse(&point, &buttons, true);
		point.x = (int (point.x-1)/zoom);
		point.y = (int (point.y-1)/zoom);
		world[point].makeAlive();
		worldCopy[point].makeAlive();
		Window()->Lock();
		Draw(BRect(point.x * zoom, point.y * zoom, (point.x +1) * zoom, (point.y + 1) * zoom));
		Window()->Unlock();
		snooze(10000);
	}
	if (buttons == B_SECONDARY_MOUSE_BUTTON)
	{
		GetMouse(&point, &buttons, true);
		point.x = (int (point.x-1)/zoom);
		point.y = (int (point.y-1)/zoom);
		world.erase(point);
		worldCopy.erase(point);
		Window()->Lock();
		Invalidate(BRect(point.x * zoom, point.y * zoom, (point.x +1) * zoom, (point.y + 1) * zoom));
		Window()->Unlock();
	}
	release_sem(compute_sem);
}

void LView::Draw(BRect rect)  
{
	float x, y;	
	rgb_color gridColor = {100,100,100,0};

	if (grid)
	{
		BPoint LeftTop, RightBottom;
		LeftTop = rect.LeftTop();
		rect.PrintToStream();
		RightBottom = rect.RightBottom();
		BeginLineArray(rect.IntegerWidth() / zoom + rect.IntegerHeight() / zoom + 4);
 		
 		int i;
 		for (i = (int) LeftTop.x - (int (LeftTop.x) % zoom); i <= RightBottom.x ; i += zoom ) 
 		{
			AddLine(BPoint(i, LeftTop.y), BPoint(i, RightBottom.y), gridColor);
		}
		for (i = (int) LeftTop.y - (int (LeftTop.y) % zoom) ; i <= RightBottom.y ; i += zoom )
		{							
			AddLine(BPoint(LeftTop.x, i), BPoint(RightBottom.x, i), gridColor); 
		}
		EndLineArray();
	}
	std::map<LPoint, cell>::iterator pos;
	acquire_sem(life_sem); 
	for (pos = worldCopy.begin(); pos != worldCopy.end(); ++pos)
	{
		x = pos->first.x;
		y = pos->first.y;
		if (grid)
			FillRect(BRect(x*zoom+1,y*zoom+1,x*zoom+zoom-1,y*zoom+zoom-1), B_SOLID_HIGH); 
		else
			FillRect(BRect(x*zoom,y*zoom,x*zoom+zoom,y*zoom+zoom), B_SOLID_HIGH);
	}
	release_sem(life_sem);
}

void CView::Draw( BRect )
{
	BRect bounds = Bounds();
	
	SetHighColor(239,239,239);
	MovePenTo( bounds.LeftTop() );
	StrokeLine((BPoint(0,134)), (BPoint(200,134)));
	MovePenTo( bounds.LeftTop() );
	StrokeLine((BPoint(0,429)), (BPoint(200,429)));
	
	SetHighColor(162,162,162);
	MovePenTo( bounds.LeftTop() );
	StrokeLine((BPoint(0,135)), (BPoint(200,135)));
	MovePenTo( bounds.LeftTop() );
	StrokeLine((BPoint(0,430)), (BPoint(200,430)));

}


int32 lifeWork(void * incoming) 
								 
{
	int xConst = 10000;
	int yConst = 10000;
	std::map<LPoint, cell>::iterator pos;
	float y, x;
	
	((dotterApp *)be_app)->display(world);

	while (true)
	{	
		acquire_sem(compute_sem);
		for (pos = world.begin(); pos != world.end(); ++pos)  
			{		
			x = pos->first.x;
			y = pos->first.y;
			if (pos->second.alive)
			{
				if  (y == 0) // top
 				{
 					if (x == xConst)  // and right
					{	//set all left and lower points (3)
						world[BPoint(x-1, y)].increment();	// left
						world[BPoint(x, y + 1)].increment();	// bottom
						world[BPoint(x-1, y + 1)].increment();// bottom left
					}
					else if (x == 0) // top and left
					{	//set all right and lower points (3)
						world[BPoint(x+1, y)].increment();	// right
						world[BPoint(x, y + 1)].increment();	// bottom
						world[BPoint(x +1, y + 1)].increment();// bottom right
					}
					else 
					{	//set all lower points
						world[BPoint(x-1, y)].increment();	// left
						world[BPoint(x+1, y)].increment();	// right
						world[BPoint(x, y + 1)].increment();	// bottom
						world[BPoint(x-1, y + 1)].increment();// bottom left
						world[BPoint(x +1, y + 1)].increment();// bottom right
					}
				}
				else if (y == yConst) // bottom
				{
					if (x == xConst)  // and right
					{	//set all left and upper points (3)
						world[BPoint(x-1, y)].increment();	// left	
						world[BPoint(x, y - 1)].increment();	// top
						world[BPoint(x-1, y - 1)].increment();// top left
					}
					else if (x == 0) // bottom and left
					{	//set all right and upper points (3)
						world[BPoint(x+1, y)].increment();	// right
						world[BPoint(x, y - 1)].increment();	// top
						world[BPoint(x+1, y - 1)].increment();// top right
					}
					else  
					{	//set all upper points (5)
						world[BPoint(x-1, y)].increment();	// left
						world[BPoint(x+1, y)].increment();	// right
						world[BPoint(x, y - 1)].increment();	// top
						world[BPoint(x-1, y - 1)].increment();// top left
						world[BPoint(x+1, y - 1)].increment();// top right
					}
				}
				else 
				{
					if (x == xConst) // right (5)
					{	//set all left points
						world[BPoint(x-1, y)].increment();	// left
						world[BPoint(x, y + 1)].increment();	// bottom
						world[BPoint(x-1, y + 1)].increment();// bottom left
						world[BPoint(x, y - 1)].increment();	// top
						world[BPoint(x-1, y - 1)].increment();// top left
					}
					else if (x == 0) // left (5)
					{	//set all right points
						world[BPoint(x+1, y)].increment();	// right
						world[BPoint(x, y + 1)].increment();	// bottom
						world[BPoint(x +1, y + 1)].increment();// bottom right
						world[BPoint(x, y - 1)].increment();	// top
						world[BPoint(x+1, y - 1)].increment();// top right
					}
					else 
					{	//set ALL POINTS (8)
						world[BPoint(x-1, y)].increment();	// left
						world[BPoint(x+1, y)].increment();	// right
						world[BPoint(x, y - 1)].increment();	// top
						world[BPoint(x-1, y - 1)].increment();// top left
						world[BPoint(x+1, y - 1)].increment();// top right
						world[BPoint(x, y + 1)].increment();	// bottom
						world[BPoint(x-1, y + 1)].increment();// bottom left
						world[BPoint(x +1, y + 1)].increment();// bottom right
					}
				}		
			}
		}
		
		for (pos = world.begin(); pos != world.end(); )
		{
			if (pos->second.alive)
			{
				if (pos->second.aliveNs != 2 && pos->second.aliveNs != 3)
				{	
					world.erase(pos++);
				}
				else
				{
				pos->second.aliveNs = 0;
				pos++;
				}
			}
			else
			{
				if (pos->second.aliveNs == 3)
				{
					pos->second.alive = true;
					pos->second.aliveNs = 0;
					pos++;
				}
				else 
				{	
					world.erase(pos++);
				}
			}
		}
		
		((dotterApp *)be_app)->display(world);
		release_sem(compute_sem);
		snooze(speed);
	}
}

int main()
{
	new dotterApp;
	be_app->Run();
	delete be_app;
	return B_OK;
}

void dotterApp::ReadyToRun()
{
	controlWindow->Show();
	lifeWindow->Show();
}

void dotterApp::display(std::map<LPoint, cell> world)
{
	acquire_sem(life_sem); 
	lifeView->worldCopy = world;
	release_sem(life_sem);
	
	lifeWindow->Lock();
	lifeView->Invalidate();
	lifeWindow->Unlock();
}

void dotterApp::AboutRequested()
{
	BScreen mScreen(B_MAIN_SCREEN_ID);
	ScreenResolution.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));

	BWindow* Aboutwindow = new AboutWindow(BRect(((ScreenResolution.right / 2) - 305), ((ScreenResolution.bottom / 2) - 148), ((ScreenResolution.right / 2) + 260), ((ScreenResolution.bottom / 2) + 146)));
	Aboutwindow->Show();
}

bool dotterApp::QuitRequested() 
{
	acquire_sem(life_sem); 
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

/// SPECIAL PATTERN FROM HERE
void dotterApp::Pat1()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 11;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 13;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat2()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 11;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat3()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 11;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 13;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat4()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 11;
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat5()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;

			point.y = 10;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 11;
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 13;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
						
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat6()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 11;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat7()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 11;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 13;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 14;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat8()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 0;
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 1;
			point.x = 22;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 2;
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 20;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 21;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 34;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 35;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 3;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 15;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 20;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 21;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 34;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 35;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 4;
			point.x = 0;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 1;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 16;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 20;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 21;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 5;
			point.x = 0;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 1;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 16;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 17;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 22;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 6;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 16;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 7;
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 15;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 8;
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 13;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}

void dotterApp::Pat9()
{
			acquire_sem(compute_sem);// pauses the game
		
			BPoint point;
			
			point.y = 10;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 15;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 16;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 18;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 22;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 25;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 26;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 28;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 29;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 30;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			
			point.y = 11;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 18;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 28;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 12;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 15;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 18;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 22;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 25;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 28;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 29;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
 
			point.y = 13;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 18;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 22;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 28;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			point.y = 14;
			point.x = 10;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 11;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 12;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 14;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 15;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 16;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 18;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 19;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 20;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 22;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 24;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 28;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 29;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();
			point.x = 30;
			world[point].makeAlive();
			lifeView->worldCopy[point].makeAlive();

			
			lifeView->Window()->Lock();
			lifeView->Invalidate();
			lifeView->Window()->Unlock();
			release_sem(compute_sem);//resumes
}




