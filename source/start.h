#ifndef _START_
#define _START_

// other functions
void quitProgram();
int main();

// openGL functions
void display();
void keyboard(unsigned char c, int x, int y);
void special_keyboard(int key, int x, int y);
void mouse(int mouseButton, int state, int x, int y);
void mouse_motion(int x, int y);
void reshape(int w, int h);
void init(void);
void init_gl_window();
void init_buttons();

#endif
