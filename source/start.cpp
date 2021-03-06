/* debugging options */
//#define DEBUG         // allows quitting with 'q' to avoid the quit sequence
//#define MOUSECOORDS   // display current mouse posn in terminal
//#define INFINITEJUMPS // what it sounds like

#include <iostream>
#include <iomanip>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <sys/time.h>
#include <sys/stat.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "button.h"
#include "ColorPoint2.h"
#include "globaldefs.h"
#include "text.h"
#include "texture.h"
#include "start.h"
#include "InstructionLines.h"
#include "target.h"
#include "draw.h"
#include "koala.h"
#include "date.h"
#include "sorcerer.h"
#include "sound.h"
#include<vector>
#include<iomanip>
#include<sstream>

using namespace std;
//Sound Files
FMOD::Sound *valkyrie, *splash;

// root target
Target **rootTarget;
const char *defaultMakefile = "Makefile.level";

// general state of program
char programName[] = "Makefile Madness 2";
enum screenType screen;
int backgroundTexture;
int keyboardTexture;
int mouseTexture;
int koalaTexture;
int swanTexture;
int flyingSquirrelTexture;
int jaguarTexture;
int seaTurtleTexture;
int waterTexture;
int brickTexture;
int gppTexture;

double lastTime;
int offset = 0;
bool dateIsGenerated = false;
bool hasFailedDate = false;
Date randomDate;

//button info
const int buttonHeight = 118;
const int bufferHeight = buttonHeight / 4;
const int buttonX = 256;//x position of where button starts
const char numButtons = 15;

//Start Screen Buttons//
Button startButton("Start Game", buttonX, (bufferHeight*5 + buttonHeight*4),500, 118, GAME,START, 464, false);
Button loadButton("Load Makefile", buttonX, (bufferHeight*4 + buttonHeight*3),500, 118, LOAD,START, 452,false);
Button instructionsButton("Instructions", buttonX, (bufferHeight*3 + buttonHeight*2),500,118, INSTRUCTIONS,START, 460,false);
Button customizeButton("Customize Character", buttonX, (bufferHeight*2 + buttonHeight),500,118, CUSTOMIZE,START, 420,false);
MovingButton quitButton("Quit", buttonX, (bufferHeight),500, 118, QUIT_MOVE,START, 235,false);

//Instruction Screen Buttons //
Button backButton("Go Back", 0,0,80,768, START, INSTRUCTIONS, 4,false);
Button backButton2("Go Back", 0,0,80,768, START, LOAD,4,false);
Button backButton3("Go Back", 0,0,80,768, START, CUSTOMIZE, 4,false);
//Load Screen Button
Button loadMakefileButton("Load", 360,250,200,40, LOAD, LOAD, 442,false);
//Quit Screen Button
Button submitDateButton("Submit", 380,290,200,40, CHECK_QUIT_DATE, QUIT_DATE, 450,false);

// Customize Screen Buttons //
Button koalaButton("Click Here", buttonX-100, 574,100, 100,CUSTOMIZE,CUSTOMIZE, buttonX-92,false);
Button jaguarButton("Click Here", buttonX-100, 454,100, 100,CUSTOMIZE,CUSTOMIZE, buttonX-92,false);
Button flyingSquirrelButton("Click Here", buttonX-100, 334,100, 100,CUSTOMIZE,CUSTOMIZE, buttonX-92,false);
Button swanButton("Click Here", buttonX-100, 214,100, 100,CUSTOMIZE,CUSTOMIZE, buttonX-92,false);
Button seaTurtleButton("Click Here", buttonX-100, 94,100, 100,CUSTOMIZE,CUSTOMIZE, buttonX-92,false);

bool pressed=false;


// Main Button Array//
Button* Buttons[numButtons];

// game screen global variables
int mouseposx;
int mouseposy;

Animal *animal;
Koala koala{};
SeaTurtle seaTurtle{};
FlyingSquirrel flyingSquirrel{};
Jaguar jaguar{};
Swan swan{};

//QUIT_DATE TextBox
int quitBoxText1[] = {182, 350, 600, 40};
int quitBoxText2[] = {188, 355, 590, 30};
textBox dateBox{false, quitBoxText1, quitBoxText2};

//LoadPage TextBox
int loadBoxText1[] = {182, 350, 600,40};
int loadBoxText2[] = {188, 355, 590, 30 };
textBox loadBox{false, loadBoxText1, loadBoxText2};

Sorcerer *sorcerer;

bool innitted = false;

void quitProgram()
{
  cout << "You're good to go back to the real world! Quitting." << endl;
  
  delete sorcerer;

  int win = glutGetWindow();
  glutDestroyWindow(win);
  exit(0);
}

void drawInstructions()
{
  drawTexture(backgroundTexture,0., 768., 1024., -768.);
  drawTexture(keyboardTexture,170,548,768,-200);
  //drawTexture(mouseTexture,200,250, 200, -200);
  Text spacet1(300, 275, "Spacebar allows the user to");
  Text spacet2(300, 255, "launch the koala");
  LegendItem spaceKey(360,396,300,372,2.5, spacet1, spacet2);

  Text darrowt1(600, 275, "Pressing the down, left or right arrow");
  Text darrowt2(600, 255, "allows user to set");
  Text darrowt3(600, 235, "the trajectory of the launch");
  LegendItem darrowKey(732,732,300,368,2.5, darrowt1, darrowt2, darrowt3);
  Line darrowl1(630,696,300,368,2.5);
  Line darrowl2(840,765,300,368,2.5);

  Text uarrowt1(760, 640, "Pressing the up arrow");
  Text uarrowt2(760, 620, "allows user to set");
  Text uarrowt3(760, 600, "the trajectory of the launch");
  LegendItem uarrowKey(760,728,600,392,2.5, uarrowt1, uarrowt2, uarrowt3);

  Text f1t1(230, 663, "Pressing F1 returns you");
  Text f1t2(230, 643, "to the start screen");
  LegendItem f1Key(250,250,632,525,2.5, f1t1, f1t2, uarrowt3);

  spaceKey.draw();
  darrowKey.draw();
    darrowl1.draw();
    darrowl2.draw();
  uarrowKey.draw();
  f1Key.draw();
}

void display()
{
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  switch(screen) {
    case START:
      drawTexture(backgroundTexture, 0.0, 768.0, 1024.0, -768.0);
      glDisable(GL_LINE_STIPPLE);
      for (short int i=0; i<numButtons; ++i) {
        if (Buttons[i]->active == screen)
          Buttons[i]->draw();
      }
      break;

    case GAME:
    {
      drawTexture(backgroundTexture, 0.0, 768.0, 1024., -768.);
      for (short int i=0; i<numButtons; ++i) {
        if(Buttons[i]->active == screen)
          Buttons[i]->draw();
      }
      // draw the target boxes
      rootTarget[0]->drawTargetBoxes(offset);
      rootTarget[0]->drawDependLines(offset); // this doesn't work yet


      if(Buttons[10]->getkeepPressed())
	{
	  animal = &koala;
	  koala.drawKoala(mouseposx);
	  koala.drawTrajectory(mouseposx, mouseposy);
	}
      else if(Buttons[11]->getkeepPressed())
	{
	  animal = &jaguar;
	  jaguar.drawTrajectory(mouseposx, mouseposy);
	  jaguar.drawJaguar(mouseposx);
	}
      else if(Buttons[12]->getkeepPressed())
	{
	  animal = &flyingSquirrel;
	  flyingSquirrel.drawTrajectory(mouseposx, mouseposy);
	  flyingSquirrel.drawflyingSquirrel(mouseposx);
	}
      else if(Buttons[13]->getkeepPressed())
	{
	  animal = &swan;
	  swan.drawTrajectory(mouseposx, mouseposy);
	  swan.drawSwan(mouseposx);
	}
      else if(Buttons[14]->getkeepPressed())
	{
	  animal = &seaTurtle;
	  seaTurtle.drawTrajectory(mouseposx, mouseposy);
	  seaTurtle.drawseaTurtle(mouseposx);
	}
      else
	{
	  animal = &koala;
	  koala.drawKoala(mouseposx);
	  koala.drawTrajectory(mouseposx, mouseposy);
	}
      //koala.drawTrajectory(mouseposx, mouseposy);
      //koala.drawKoala(mouseposx);
      //animal ->drawTrajectory(mouseposx, mouseposy);

      sorcerer->draw(offset);

      // draw base box
      if(animal->isAtBottom())
	drawTexture(brickTexture,0,100,1024,-100);
      // drawBox(0,0 + offset,1024,100,1,1,1);
      else {
        animal->vel.toggleGravity(true);
        drawTexture(waterTexture,0,100 + offset,1024,-500);
      }

      break;
    }

    case LOAD:
      drawTexture(backgroundTexture, 0., 768.,1024., -768.);
      glColor3f(1,1,1);
      drawWhiteText(200,400,(string)"Please type the name of the Makefile you want to use below!");
      for (short int i=0; i<numButtons; ++i) {
        if(Buttons[i]->active == screen)
          Buttons[i] -> draw();
      }

      // draw the textbox
      loadBox.drawTextBox();
      loadBox.writeTextinBox();
      glutSwapBuffers();

      break;

    case INSTRUCTIONS:
      drawInstructions();

      for (short int i=0; i<numButtons; ++i) {
        if(Buttons[i]->active == screen)
          Buttons[i]->draw();
      }
      break;

    case CUSTOMIZE:
      drawTexture(backgroundTexture, 0.0, 768.0,1024., -768.);
      for (short int i=0; i<numButtons; ++i) {
        if(Buttons[i]->active == screen)
          Buttons[i]->draw();
      }
      drawTexture(koalaTexture,buttonX+50,768-94,100,-100);
      drawTexture(jaguarTexture, buttonX+50, 768-214, 268, -100);
      drawTexture(flyingSquirrelTexture,buttonX+50, 768-334, 147, -100); 
      drawTexture(swanTexture, buttonX+50, 768-454, 178-1,-100);
      drawTexture(seaTurtleTexture, buttonX+50, 768-574+20, 165,-140);
      glColor3f(1,1,1);
      drawWhiteText(300,700,(string) "Please Choose An Animal!");
      glColor3f(0,0,0);
      
      break;

    case QUIT_MOVE:
      /* The first stage of quitting: Catch the quit button! */
      drawTexture(backgroundTexture, 0.0, 768.0,1024., -768.);
      quitButton.move();
      quitButton.active = QUIT_MOVE; // move quit button to the moving
      quitButton.screen = QUIT_DATE; // screen
      for (short int i=0; i<numButtons; ++i)
        if(Buttons[i]->active == screen)
          Buttons[i]->draw();
      break;

    case QUIT_DATE:
    {
      /* The second stage of quitting: Get the right date! */
      quitButton.active = START; // get it out of the way

      if (!dateIsGenerated) {
        dateBox.erase();
        randomDate.randomize();
        dateIsGenerated = true;
      }

      drawTexture(backgroundTexture, 0.0, 768.0,1024., -768.);
      glColor3f(1,1,1);

      string dateMessage = ( hasFailedDate ? "Nope! " : "" );
      dateMessage += "Please enter the day of the week that " +
          randomDate.getStringRepr() + " falls on:";
      drawWhiteText( (hasFailedDate ? 240 : 257),400,dateMessage );

      for (short int i=0; i<numButtons; ++i) {
        if(Buttons[i]->active == screen)
          Buttons[i]->draw();
      }

      dateBox.drawTextBox();
      dateBox.writeTextinBox();
      glutSwapBuffers();
      break;
    }

    case CHECK_QUIT_DATE:
    {
      /* Run when the user presses "submit" on the quit screen; either
       * quit or return control to the normal QUIT_DATE screen. */

      string userDateName = dateBox.getTextInBox();
      unsigned userDateNum = convertDayToNumber(userDateName);
      if ( randomDate.compareDay(userDateNum) )
        quitProgram();
      else {
        dateIsGenerated = false;
        hasFailedDate = true;
        screen = QUIT_DATE;
        break;
      }
    }

    default:
      cerr << "This screen not defined (yet?)." << endl;
      break;
  }
  glutSwapBuffers();
}

void launch()
{
  if ( screen == GAME ) {
    if ( animal->isAtBottom() )
      animal->leaveBottom();
    Point2d tmppos(mouseposx - 100, mouseposy);
#ifdef INFINITEJUMPS
    if (true) {
#else
    if (animal->jumps) {
#endif
      animal->setTarget(tmppos);//, (double).5);
      animal->jumps = false;
    }
  }
}

// process keyboard events
void keyboard(unsigned char c, int x, int y)
{
  // if we're in a text box, type as normal there
  loadBox.keyboardfunction(c);
  dateBox.keyboardfunction(c);
  switch(c) {
    case ' ':
      launch();
      break;

#ifdef DEBUG
    case 'q':
    case 'Q':
    case 27:
      quitProgram();
#endif

    case '\b':
    default:
      break;
  }
  glutPostRedisplay();
}

// process "special" keyboard events
void special_keyboard(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_F1:
      offset=0;
      screen = START;
      break;
    case GLUT_KEY_UP:
      mouseposy++;
      break;
    case GLUT_KEY_DOWN:
      mouseposy--;
      break;
    case GLUT_KEY_LEFT:
      mouseposx--;
      break;
    case GLUT_KEY_RIGHT:
      mouseposx++;
      break;
  }
  glutPostRedisplay();
}

// The mouse function is called when a mouse button is pressed down or released
void mouse(int mouseButton, int state, int x, int y)
{
  if ( GLUT_LEFT_BUTTON == mouseButton )
  {
    if ( GLUT_DOWN == state ) // mouse press
    {
      for (short int i=0; i<numButtons; ++i)
      {
        if(Buttons[i]->active == screen && Buttons[i]->onButton(x,y))
        {
          //Buttons[i]->IsPressed = true;
          if (Buttons[i]->getLabel() == "Load")
          {
	    Buttons[i]->IsPressed = true;
            if((!innitted))
            {
              init_targets(loadBox.getTextInBox().c_str());
              loadBox.erase();
            }
            else
            {
              cout<<"Makefile is Already Loaded!!!!!"<<endl;
            }
            screen = Buttons[i]->screen;
          }
          else if (screen == QUIT_MOVE && Buttons[i]->active == QUIT_MOVE)
          {
	    Buttons[i]->IsPressed = true;
            // quit button, and already pressed once; move to date part
            screen = QUIT_DATE;
            Buttons[i]->active = QUIT_DATE;
          }
	  else if (Buttons[i]->getLabel() == "Click Here")
	    {
	      if((Buttons[i]->IsPressed = true) && (pressed))
		{
		  pressed = false;
		  for(short i=0; i<numButtons; ++i)
		    {
		      Buttons[i]->changekeepPressed(pressed);
		    }
		  Buttons[i]->changekeepPressed(true);
		  pressed=true;
		}
	      else if((Buttons[i]->IsPressed = true) && (!pressed))
		{
		  pressed = true;
		  Buttons[i]->changekeepPressed(pressed);
		 
		}
	    }
          else
          {
            screen = Buttons[i]->screen;
          }
        }
      }
    }
    else
    { // mouse release
      for (short int i=0; i<numButtons; ++i)
      {
        if ( Buttons[i]->onButton(x,y) && Buttons[i]->IsPressed )
	  {
	   if(Buttons[i]->active != CUSTOMIZE)
	     Buttons[i]->IsPressed = false;
	    if(Buttons[i]->active == CUSTOMIZE)
	      Buttons[i]->IsPressed = false;
	  }
	
      }
    }
    glutPostRedisplay();
  }
  else if ( GLUT_RIGHT_BUTTON == mouseButton )
    launch();
}

//mouse_motion function...called from init function
//called when mouse is being dragged, and gives the current location
// of the mouse
void mouse_motion(int x, int y)
{
#ifdef MOUSECOORDS
  cerr <<"Mouse position: ("<<x<<","<<y<<")"<<endl;
#endif
  mouseposx = x+54;
  mouseposy=768-y;

  // is the mouse button currently depressed over any screen button?
  bool aButtonIsPressed = false;
  for (short int i=0; i<numButtons; ++i) {
    if (Buttons[i]->IsPressed) {
      aButtonIsPressed = true;
      break;
    }
  }

  // if not, for each button, determine if it's moused over
  // this is used both to set and unset this status
  if ( !aButtonIsPressed ) {
    for (short int i=0; i<numButtons; ++i) {
      if ( Buttons[i]->onButton(x,y) )
        Buttons[i]->overButton = true;
      else
        Buttons[i]->overButton = false;
    }
  }

  // focus on any text boxes
  if ( loadBox.onTextBox(x,768-y) ) loadBox.changeoverTextBox(true);
  else loadBox.changeoverTextBox(false);

  if ( dateBox.onTextBox(x,768-y) ) dateBox.changeoverTextBox(true);
  else dateBox.changeoverTextBox(false);
  glutPostRedisplay();
}

// the init function sets up the graphics card to draw properly
void init(void)
{
  // clear the window to black
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // set up the coordinate system:  number of pixels along x and y
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, 0., HEIGHT-1, -1.0, 1.0);

  // set up how points and lines will be drawn.  The following
  // commands make points and lines look nice and smooth.
  glPointSize(3);
  glLineWidth(1.5);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

  // welcome message
  cout << "Welcome to " << programName << "." << endl;
  cout << "To quit the entire program press the 'q' key" <<endl;
  cout << "To return to start screen press the F1 key" <<endl;
}

void init_gl_window()
{
  char *argv[] = {programName};
  int argc = sizeof(argv) / sizeof(argv[0]);
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInitWindowPosition(100,100);
  glutCreateWindow(programName);
  init();

  backgroundTexture = loadTexture("../resources/background.pam");
  keyboardTexture = loadTexture("../resources/keyboard.pam");
  koalaTexture = loadTexture("../resources/Koala.pam");
  jaguarTexture = loadTexture("../resources/jaguar.pam");
  flyingSquirrelTexture = loadTexture("../resources/flyingSquirrel.pam");
  seaTurtleTexture = loadTexture("../resources/seaTurtle.pam");
  swanTexture = loadTexture("../resources/swan.pam");
  koala.loadTexture(koalaTexture);
  jaguar.loadTexture(jaguarTexture);
  flyingSquirrel.loadTexture(flyingSquirrelTexture);
  seaTurtle.loadTexture(seaTurtleTexture);
  swan.loadTexture(swanTexture);
  brickTexture = loadTexture("../resources/brickwall.pam");
  waterTexture = loadTexture("../resources/waterTexture.pam");
  //mouseTexture = loadTexture("../resources/mouse.pam");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_motion);
  glutIdleFunc(idle);
  glutMainLoop();
}

double getCurrentTime()
{
  struct timeval tv = {0,0};
  struct timezone tz;
  gettimeofday(&tv, &tz);
  // cout << "tv is " << tv.tv_sec << " micro " << tv.tv_usec << endl;
  return tv.tv_sec + tv.tv_usec/(double)1000000.;
}

void resetGame()
{
  animal->makeAtBottom();
  animal->vel.toggleGravity(false);
  animal->setPosition(10, 200);
  animal->vel.set(animal ->posn, animal ->posn);
             animal ->jumps = true;
  offset = 0;
  screen = START;
}

void idle()
{
  // do things only if a certain amount of time has passed
  double now = getCurrentTime();
  double elapsedTime = now - lastTime;

  if ( elapsedTime > .05 ) {
    lastTime = now; // note that we've run this

    switch(screen) {
      case QUIT_MOVE:
        glutPostRedisplay();
        break;

      case GAME:
      {
        /* move sorcerer */
        sorcerer->move();
        if (sorcerer->isHit(koala.posn, 100, 100, offset)) {
          resetGame();
          cout << "You win!" << endl;
          screen = START;
        }

        /* move animal */
        animal ->move();
        if (rootTarget[0]->checkCollisions(animal->posn, 100, 100, offset)) {
          animal ->velocityZero();
          animal ->jumps = true;
        }
        if (animal ->getY() - offset <= 100 && !animal ->isAtBottom()) {

	  if(USE_SOUND) playSound(splash,1);
          resetGame();

          cout << "You lose!" << endl;
        }

        // scrolling
        if (animal ->getY() >= HEIGHT - 200) {
          offset += HEIGHT - 200 - animal ->getY();
          animal ->scrollAnimalUp();
        } else if (animal ->getY() <= 200) {
          offset += 200 - animal ->getY();
          animal ->scrollAnimalDown();
        }

        glutPostRedisplay();
        break;
      }

      default:
        break;
    }
  }
}


void init_buttons()
{
  // to change the number of buttons, adjust numButtons global variable
  Buttons[0] = &startButton;
  Buttons[1] = &loadButton;
  Buttons[2] = &instructionsButton;
  Buttons[3] = &customizeButton;
  Buttons[4] = &quitButton;
  Buttons[5] = &backButton;
  Buttons[6] = &loadMakefileButton;
  Buttons[7] = &backButton2;
  Buttons[8] = &backButton3;
  Buttons[9] = &submitDateButton;
  Buttons[10] = &koalaButton;
  Buttons[11] = &jaguarButton;
  Buttons[12] = & flyingSquirrelButton;
  Buttons[13] = &swanButton;
  Buttons[14] = &seaTurtleButton;
  for (short int i=0; i<numButtons; ++i) {
    Buttons[i]->IsPressed = false;
    Buttons[i]->overButton = false;
  }
}

bool file_exists (const char *name)
{
  /* from http://stackoverflow.com/questions/12774207/
   * fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c */
  struct stat buffer;   
  return (stat (name, &buffer) == 0); 
}

void init_targets(const char *filename)
{
  string name;
  if(*filename && file_exists(filename)) {
    delete [] rootTarget;
    rootTarget = parseTargets(filename);
    rootTarget[0]->initPositions();
    innitted=true;
  } else {
    cerr << "That file does not exist!" << endl;
  }
  cout << "just loaded a makefile" << endl;
}

int main()
{
  rootTarget = parseTargets(defaultMakefile);
  rootTarget[0]->initPositions();
  lastTime = getCurrentTime();

  sorcerer = new Sorcerer(rootTarget[0]->getNumTargets());
  init_sound_system();
  valkyrie = loadSoundFile("../resources/valkyrie.ogg");
  splash = loadSoundFile("../resources/splash.wav");
  if(USE_SOUND) playSound(valkyrie,0);
  screen = START;
  init_buttons();
  //init_targets();
  init_gl_window();
}
