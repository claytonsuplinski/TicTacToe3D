/*
Name: Clayton Suplinski
Project: InsertProjectNameHere
*/

#include <iostream>
#include <time.h>       
#include <assert.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <conio.h>
//#include <AL/al.h>
//#include <AL/alc.h>
//#include <AL/alut.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader_utils.h"

#include "disc.h"
#include "sphere.h"
#include "cylinder.h"
#include "squareStadium.h"
#include "gengar.h"
#include "cube2.h"

#include "snow.h"

#include "vertexattributes2.h"
#include "object2.h"
#include "shader2.h"
#include "globals.h"
#include "fbo.h"
#include "team.h"
#include "menu.h"
#include "precip.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "board.h"

using namespace std;using namespace glm;

//The projection and modelview matrices. 
mat4 projection, modelview;

//x (0=Normal, 1=night, 2=twilight), y(0=normal, 1=underwater, 2=fog)
ivec2 tod = ivec2(0, 0);

//Used for adjusting the y value for the lookAt function for the first and third person views.
float perspectiveOffset = 0.0f;
//Used for adjusting the z value for the lookAt function for the first and third person views.
float perspectiveStepBack = 50.0f;

//Creates a class for maintaining the window information for easy access.
class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 20;this->interval = 1000 / 120;this->window_handle = -1;
	}
	float time_last_pause_began;float total_time_paused;bool paused , wireframe, normals;
	int window_handle;int interval;int slices;ivec2 size;float window_aspect;
} window;

/////////////////////////////////////VARIABLE DECLARATIONS/////////////////////////////
int viewPerspective = 0; //Select amongst 1st and 3rd person views
int weatherType = 0; //Select amongst the weather types

//Time control variables.
float current_timeDisplay;

Sphere usr;Disc disc;Cylinder healthBar;
SquareStadium stadium;
FrameBufferObject fbo;Square4 framebufferScreen;Precip precip;
Cube2 skybox;Menu menu;Team userTeam, cpuTeam;
Board board;

float velocity2 = 0;float maxUserVelocity = 1.0;

bool gameOver;int winningTeam = 0;string winningTeamName = "";vec4 winningTeamFontColor;
int endMenuPage = 0;int totYellow=0;int totGreen=0;int totBlue=0;int totRed=0;

static const int X_TURN = 1;
static const int O_TURN = 2;

//Keyboard flags
bool ePressed, wPressed, rPressed, zPressed, qPressed, aPressed, sPressed, dPressed, xPressed;

//Values defining the position/rotation of the camera.
double RotatedX = 0;double RotatedY = 0;double RotatedTan = 0;
double RotateEarthX = 0; double RotateEarthY = 0;
float transX = 0;float transY = 0;float transZ = 0;float movingWRTSnow = 0;
float lastShot = 0;float lastGrenade = 0;

//////////////////////////////////////TEXT RENDERING/////////////////////////////////////////////
////////Font variables//////////
GLuint program;GLint attribute_coord;GLint uniform_tex;GLint uniform_color;
struct point {GLfloat x;GLfloat y;GLfloat s;GLfloat t;};
GLuint vbo;FT_Library ft;FT_Face face;const char *fontfilename;
int init_resources()
{        /* Initialize the FreeType2 library */
        if(FT_Init_FreeType(&ft)){fprintf(stderr, "Could not init freetype library\n");return 0;}
        /* Load a font */
        if(FT_New_Face(ft, fontfilename, 0, &face)){fprintf(stderr, "Could not open font %s\n", fontfilename);return 0;}
        /* Compile and link the shader program */
        GLint link_ok = GL_FALSE;GLuint vs, fs;
        if ((vs = create_shader("text.vert", GL_VERTEX_SHADER))        == 0) return 0;
        if ((fs = create_shader("text.frag", GL_FRAGMENT_SHADER)) == 0) return 0;

        program = glCreateProgram();glAttachShader(program, vs);glAttachShader(program, fs);
        glLinkProgram(program);glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
        if (!link_ok) {fprintf(stderr, "glLinkProgram:");print_log(program);return 0;}

        const char* attribute_name;attribute_name = "coord";
        attribute_coord = glGetAttribLocation(program, attribute_name);
        if (attribute_coord == -1) {fprintf(stderr, "Could not bind attribute %s\n", attribute_name);return 0;}

        const char* uniform_name;uniform_name = "tex";
        uniform_tex = glGetUniformLocation(program, uniform_name);
        if (uniform_tex == -1){fprintf(stderr, "Could not bind uniform %s\n", uniform_name);return 0;}

        uniform_name = "color";uniform_color = glGetUniformLocation(program, uniform_name);
        if (uniform_color == -1){fprintf(stderr, "Could not bind uniform %s\n", uniform_name);return 0;}

        // Create the vertex buffer object
        glGenBuffers(1, &vbo);
        return 1;
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy) {
        const char *p;FT_GlyphSlot g = face->glyph;

        /* Create a texture that will be used to hold one "glyph" */
        GLuint tex;glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &tex);glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(uniform_tex, 0);
        /* We require 1 byte alignment when uploading texture data */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /* Set up the VBO for our vertex data */
        glEnableVertexAttribArray(attribute_coord);glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
        /* Loop through all characters */
        for(p = text; *p; p++) {
                /* Try to load and render the character */
                if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
                        continue;

                /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
                /* Calculate the vertex and texture coordinates */
                float x2 = x + g->bitmap_left * sx;float y2 = -y - g->bitmap_top * sy;
                float w = g->bitmap.width * sx;float h = g->bitmap.rows * sy;

                point box[4] = {
                        {x2,     -y2    , 0, 0}, {x2 + w, -y2    , 1, 0},
                        {x2,     -y2 - h, 0, 1}, {x2 + w, -y2 - h, 1, 1},
                };

                /* Draw the character on the screen */
                glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                /* Advance the cursor to the start of the next character */
                x += (g->advance.x >> 6) * sx;y += (g->advance.y >> 6) * sy;
        }
        glDisableVertexAttribArray(attribute_coord);glDeleteTextures(1, &tex);
}

void activeTextDisplay(const char* words, float x, float y, FT_UInt size, vec4 color)
{
		glDisable(GL_CULL_FACE);
        float sx = (float)(2.0/glutGet(GLUT_WINDOW_WIDTH));
        float sy = (float)(2.0/glutGet(GLUT_WINDOW_HEIGHT));

		/* Enable blending, necessary for our alpha texture */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(program);

        
		/* Set font size to 48 pixels, color to black */
        FT_Set_Pixel_Sizes(face, 0, size);

		GLfloat black[4] = {color.x,color.y,color.z,color.w};glUniform4fv(uniform_color, 1, black);
		
		render_text(words,x,y,sx,sy); 
		
		//glDisable(GL_BLEND);
		glUseProgram(0);
        //glFinish();
		glEnable(GL_CULL_FACE);
}

/*
/////////////////////////////////////AUDIO CONTROLS/////////////////////////////////////////////////////
 // Buffers to hold sound data.
ALuint Buffer;ALuint origBuffer;
// Sources are points of emitting sound.
ALuint Source;ALuint origSource;
/*
 * These are 3D cartesian vector coordinates. A structure or class would be
 * a more flexible of handling these, but for the sake of simplicity we will
 * just leave it as is.
 */
/*
// Position of the source sound.
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
// Velocity of the source sound.
ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
// Position of the Listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
// Velocity of the Listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
// Also note that these should be units of '1'.
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

/*
 * ALboolean LoadALData()
 *
 *         This function will load our sample data from the disk using the Alut
 *         utility and send the data into OpenAL as a buffer. A source is then
 *         also created to play that buffer.
 */
/*
ALboolean LoadALData(ALbyte *song, bool looping){
            // Variables to load into.
	        ALenum format;ALsizei size;ALvoid* data;ALsizei freq;
            ALboolean loop = true; 

            // Load wav data into a buffer.
            alGenBuffers(1, &Buffer);
			if(alGetError() != AL_NO_ERROR)
                        return AL_FALSE;

    // Load any of your favourite wav song here
            alutLoadWAVFile(song, &format, &data, &size, &freq, &loop);
            alBufferData(Buffer, format, data, size, freq);
            alutUnloadWAV(format, data, size, freq);
			
            // Bind the buffer with the source.
            alGenSources(1, &Source);
            if(alGetError() != AL_NO_ERROR)
                        return AL_FALSE;
			
            alSourcei (Source, AL_BUFFER,   Buffer   );
            alSourcef (Source, AL_PITCH,    1.0      );
            alSourcef (Source, AL_GAIN,     1.0      );
            alSourcefv(Source, AL_POSITION, SourcePos);
            alSourcefv(Source, AL_VELOCITY, SourceVel);
			if(looping){
            alSourcei (Source, AL_LOOPING,  AL_TRUE     );
			}
			else{
			alSourcei (Source, AL_LOOPING,  AL_FALSE     );
			}

            // Do another error check and return.
            if(alGetError() == AL_NO_ERROR)
                        return AL_TRUE;
			
            return AL_FALSE;
}

/*
 * void SetListenerValues()
 *
 *         We already defined certain values for the Listener, but we need
 *         to tell OpenAL to use that data. This function does just that.
 */
/*
void SetListenerValues(){
            alListenerfv(AL_POSITION,    ListenerPos);alListenerfv(AL_VELOCITY,    ListenerVel);
            alListenerfv(AL_ORIENTATION, ListenerOri);
}

/*
 * void KillALData()
 *
 *         We have allocated memory for our buffers and sources which needs
 *         to be returned to the system. This function frees that memory.
 */
/*
void KillALData(){/*alDeleteBuffers(1, &Buffer);alDeleteSources(1, &Source);alDeleteBuffers(1, &origBuffer);alDeleteSources(1, &origSource);*///alutExit();}
/*
void init(ALbyte *song, bool loop){
            // Initialize OpenAL and clear the error bit. 
            alutInit(NULL, 0);alGetError();
			
            // Load the wav data. 
            if(LoadALData(song, loop) == AL_FALSE) //method 1: LoadALData()
            {
                printf("Error loading data.");
				return;
            }
			
            SetListenerValues(); //method 2: void SetListenerValues()
			
            // Setup an exit procedure. 
            atexit(KillALData); //method 3: void KillALData()
			if(loop){origSource = Source;}
			alSourcePlay(Source);

}*/

////////////////////////////////////MOUSE DETECTION & MANAGEMENT/////////////////////////
//Mouse value/identification variables.
float mouseX = 0;float mouseY = 0;
float prevMouseY = (float)(window.size.y/2);

void mouseMovement(int x, int y) {
    mouseX=(float)x; //set lastx to the current x position
    mouseY=(float)y; //set lasty to the current y position
}

void mouseRotations(int stadium, int person){
	if(person == 1 || person == 3){
		//Turning controls
		if(mouseX < window.size.x/2){RotatedY -= abs((mouseX - window.size.x/2)/75);}
		else if(mouseX > window.size.x/2){RotatedY += abs((mouseX - window.size.x/2)/75);}
	}
	prevMouseY = mouseY;
}

/////////////////////////////////////CAMERA CONTROLS//////////////////////////////////////////
//Moves the camera according to user inputs -Draws things fitted to the environment
glm::mat4 render(glm::mat4 Model){		
	Model = glm::rotate(Model, (GLfloat) RotatedX, glm::vec3(1,0,0));
	Model = glm::rotate(Model, (GLfloat) RotatedY, glm::vec3(0,1,0));
	Model = glm::translate(Model, glm::vec3(transX,0,0));    
	Model = glm::translate(Model, glm::vec3(0,0,transZ));    
	Model = glm::translate(Model, glm::vec3(0,-transY,0));	
	glLoadMatrixf(glm::value_ptr(Model));
    return Model;
}

//Undoes the render function. -Use to draw independent of the environment (e.g. the user) 
glm::mat4 endRender(glm::mat4 Model){	
	Model = glm::translate(Model, glm::vec3(0,transY,0));	
	Model = glm::translate(Model, glm::vec3(0,0,-transZ));
	Model = glm::translate(Model, glm::vec3(-transX,0,0));
	Model = glm::rotate(Model, (GLfloat) -RotatedY, glm::vec3(0,1,0));
	Model = glm::rotate(Model, (GLfloat) -RotatedX, glm::vec3(1,0,0));			
	glLoadMatrixf(glm::value_ptr(Model));
    return Model;
}

//Changes the value of the angle in which the camera should be rotated vertically.
void rotateX(double angle){
    if(!(RotatedX>=89 && angle>0) && !(RotatedX<=-89 && angle<0)){RotatedX += angle;}
}

//Changes the value of the angle in which the camera should be rotated horizontally.
void rotateY(double angle){RotatedY += angle;}

//Deletes all of the created objects here.
void CloseFunc(){
	window.window_handle = -1;
	disc.TakeDown();stadium.TakeDown();healthBar.TakeDown();
	fbo.TakeDown();skybox.TakeDown();
	usr.TakeDown();userTeam.TakeDown();framebufferScreen.TakeDown();
	cpuTeam.TakeDown();board.TakeDown();
}

//Maintains aspect ratio when window is resized.
void ReshapeFunc(int w, int h){
	if (h > 0){window.size = ivec2(w, h);window.window_aspect = float(w) / float(h);}
}

float fboTrans = 0;
float fboRot = 0;

void KeyboardFunc(unsigned char c, int x, int y){
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c){
		case 'e':ePressed = true;break;
		case 'r':rPressed = true;break;
		case 'z':zPressed = true;break;
		case 'q':qPressed = true;break;
		case 'w':wPressed = true;break;
		case 's':sPressed = true;break;
		case 'a':aPressed = true;break;
		case 'd':dPressed = true;break;
		case 'x':xPressed = true;break;

		case 'p': // Pause/unpause
			if(!menu.menuOn){
				if (window.paused == true){
					window.total_time_paused += (current_time - window.time_last_pause_began);
				}
				else{
					window.time_last_pause_began = current_time;
				}
				window.paused = !window.paused;
			}
			break;

		case 'i':  //Toggles wireframe mode
			window.wireframe = !window.wireframe;
			break;
		
		case '1': 	
			if(menu.menuOn){menu.Update(1);}
			return;
		case '2':  
			if(menu.menuOn){menu.Update(2);}
			return;
		case '3':
			if(menu.menuOn){menu.Update(3);}
			return;
		case '4':
			if(menu.menuOn){menu.Update(4);}
			return;
		case '5':
			cout << "user pos = (" << transX << ", " << transY << ", " << transZ << ")" << endl;
			return;

		case '6':
			board.x.editX-=0.1;
			board.o.editX-=0.1;
			cout << "editX = " << board.x.editX << endl;
			return;
		case '7':
			board.x.editX+=0.1;
			board.o.editX+=0.1;
			cout << "editX = " << board.x.editX << endl;
			return;
		case '8':
			board.x.editY-=0.1;
			board.o.editY-=0.1;
			cout << "editY = " << board.x.editY << endl;
			return;
		case '9':
			board.x.editY+=0.1;
			board.o.editY+=0.1;
			cout << "editY = " << board.x.editY << endl;
			return;
		
		//Enter key
		case 13:
			if(board.userTurn){
				board.userMove();
			}
			return;

		case '\'':
			board.moveMarker(1, vec3(0,1,0));
			return;

		case '/':
			board.moveMarker(-1, vec3(0,1,0));
			return;

		//Exit the program
		case 27:
			glutLeaveMainLoop();
			return;
	}	
}

void KeyboardUpFunc(unsigned char c, int x, int y){
	switch (c){
		case 'e':ePressed = false;break;
		case 'r':rPressed = false;break;
		case 'z':zPressed = false;break;
		case 'q':qPressed = false;break;
		case 'w':wPressed = false;break;
		case 's':sPressed = false;break;
		case 'a':aPressed = false;break;
		case 'd':dPressed = false;break;
		case 'x':xPressed = false;break;
	}	
}

void SpecialFunc(int c, int x, int y)
{
	switch(c){
        case GLUT_KEY_UP:
			board.moveMarker(1, vec3(1,0,0));
			//rotateX(1);
			return;
        case GLUT_KEY_DOWN:
			board.moveMarker(-1, vec3(1,0,0));
			return;
        case GLUT_KEY_RIGHT:board.moveMarker(1, vec3(0,0,1));return;
        case GLUT_KEY_LEFT:board.moveMarker(-1, vec3(0,0,1));return;
		case GLUT_KEY_F1:	
			weatherType = (weatherType + 1)%4;
			if(weatherType == 1){tod.y = 2;}
			else{tod.y=0;}
			return;
		case GLUT_KEY_F2:
			tod.x  = (tod.x + 1)%3;
			return;
    }
}

void drawRadar(){
		//Set up orthographic projection for radar
		mat4 projection1 = perspective(45.0f, window.window_aspect, 0.01f, 10.0f);
		glm::mat4 Projection2 = glm::ortho( -10.0f, 10.0f, -10.0f, 00.0f,1.0f, 10.0f);
		glm::mat4 View       = glm::lookAt(
			glm::vec3(0,0,5), // Camera is at (0,0,5), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);  
			
		mat4 another = View;
		mat4 healthBarMatrix = View;
		another = translate(another, vec3(8.5,-8.5,0));
		
		another = rotate(another, 90.0f, vec3(0,1,0));
		another = rotate(another, 90.0f, vec3(0,0,1));

		healthBarMatrix = translate(healthBarMatrix, vec3(-9.5,-0.5,0));
		/*
		//healthBarMatrix = rotate(healthBarMatrix, 90.0f, vec3(0,1,0));
		healthBarMatrix = rotate(healthBarMatrix, -90.0f, vec3(0,0,1));
		healthBarMatrix = scale(healthBarMatrix, vec3(0.05f, userTeam.soldiers[0].health/10.f, 0.05f));

		if(userTeam.soldiers[0].health < 50){
			healthBar.color = vec3(1.f, userTeam.soldiers[0].health/50.f, 0.f);
		}
		else{
			healthBar.color = vec3(1.f - (userTeam.soldiers[0].health-50.f)/50.f, 1.f, 0.f);
		}
		healthBar.Draw(Projection2, healthBarMatrix, tod, 0);

		mat4 radar = another;

		//Radar display
		
		int radarBlipTime = (int)(current_timeDisplay) * 1000;
		float radarBlip = (float)(0.0015 * (radarBlipTime%1000));
		radar = scale(radar, vec3(radarBlip, radarBlip, radarBlip));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		disc.color = vec3(0,1,0);
		disc.Draw(Projection2, radar, tod, 0);

		another = scale(another, vec3(1.5, 1.5, 1.5));
		disc.Draw(Projection2, another, tod, 0);

		

		glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);
		disc.color = vec3(0,0.25,0);
		disc.Draw(Projection2, another, tod, 0);

		another = translate(View, vec3(8.5 - ((3/104.6)*transX),-8.6 + ((3/104.6)*transZ),0));
		usr.Draw(Projection2, another, tod, 0);*/
}

void makeSkybox(){
	modelview = endRender(modelview);
	modelview = rotate(modelview, (GLfloat) RotatedX, vec3(1,0,0));
	modelview = rotate(modelview, (GLfloat) RotatedY, vec3(0,1,0));
	modelview = scale(modelview, vec3(2,2,2));
	skybox.Draw(projection, modelview, tod, current_timeDisplay/2);	
	modelview = scale(modelview, vec3(0.5,0.5,0.5));
	modelview = rotate(modelview, (GLfloat) -RotatedY, vec3(0,1,0));
	modelview = rotate(modelview, (GLfloat) -RotatedX, vec3(1,0,0));
	modelview = render(modelview);
}

void drawPrecipitationWithRadar(){
		modelview = endRender(modelview);

		modelview = rotate(modelview, (GLfloat) RotatedX, vec3(1,0,0));
		modelview = rotate(modelview, (GLfloat) RotatedY, vec3(0,1,0));
		//skybox.Draw(projection, modelview, tod, 0);
		
		precip.RotatedY = (float)(RotatedY);
		precip.movingWRTSnow = movingWRTSnow;
		precip.weatherType = weatherType;

		precip.Draw(projection, modelview, tod, 0);	
			
		modelview = rotate(modelview, (GLfloat) -RotatedY, vec3(0,1,0));
		modelview = rotate(modelview, (GLfloat) -RotatedX, vec3(1,0,0));

		drawRadar();

		modelview = render(modelview);
}

void drawStadium(){
	//Change the stadium to match the weather
	modelview = translate(modelview, vec3(0.f,-1,0.f));
	stadium.Draw(projection, modelview, tod, current_timeDisplay);
}

//Orchestrates all the objects and variables into a playable game
void GameDisplay(){
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection = perspective(25.0f, window.window_aspect, 0.01f, 2000.0f);
	modelview = lookAt(vec3(0.0f, perspectiveOffset, perspectiveStepBack), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	modelview = render(modelview);
	glLoadMatrixf(value_ptr(modelview));
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);	
	/////////////////////////////Menu Display////////////////////////////////
	if(menu.menuOn){
		viewPerspective = 3;
		perspectiveOffset = 0.0f;
		perspectiveStepBack = 10.0f;

		activeTextDisplay(menu.title1.c_str(), -0.5, 0.75, 32, vec4(1,1,1,1));
		activeTextDisplay(menu.title2.c_str(), -0.5, 0.5, 32, vec4(1,1,1,1));
		activeTextDisplay(menu.line1.c_str(), -0.5, 0.25, 32, vec4(1,1,1,1));
		activeTextDisplay(menu.line2.c_str(), -0.5, 0.0, 32, vec4(1,1,1,1));
		activeTextDisplay(menu.line3.c_str(), -0.5, -0.25, 32, vec4(1,1,1,1));
		activeTextDisplay(menu.line4.c_str(), -0.5, -0.5, 32, vec4(1,1,1,1));
		menu.menuOn = false;

	}

	///////////////////////Game Display///////////////////////////
	else if(!gameOver){
		current_timeDisplay = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
		current_timeDisplay = (window.paused ? window.time_last_pause_began : current_timeDisplay) - window.total_time_paused;

		if(wPressed){
			if(velocity2 < maxUserVelocity){velocity2 += 0.1;}
			if(movingWRTSnow < 1){movingWRTSnow = movingWRTSnow + 0.2f;}
			transZ = (float)(transZ + velocity2*cos(-RotatedY*3.14/180));
			transX = (float)(transX + velocity2*sin(-RotatedY*3.14/180));
			if(abs(transZ) > 500.0f){
				transZ = (float)(transZ - velocity2*cos(-RotatedY*3.14/180));
			}
			if(abs(transX) > 500.0f){
				transX = (float)(transX - velocity2*sin(-RotatedY*3.14/180));
			}
		}
		else{
			if(velocity2 > 0){velocity2 -= 0.1;}
			if(velocity2 < 0){velocity2 = 0;}
		}
		
		if(ePressed){rotateX(0.2);}
		if(xPressed){rotateX(-0.2);}
		if(zPressed){transY = transY - 1;}
		if(qPressed){transY = transY + 1;}
		if(sPressed){transZ = (float)(transZ - 1*cos(-RotatedY*3.14/180));transX = (float)(transX - 1*sin(-RotatedY*3.14/180));}
		if(aPressed){transX = (float)(transX + 1*cos(RotatedY*3.14/180));transZ = (float)(transZ + 1*sin(RotatedY*3.14/180));}
		if(dPressed){transX = (float)(transX - 1*cos(RotatedY*3.14/180));transZ = (float)(transZ - 1*sin(RotatedY*3.14/180));}

		
		
		//Mouse movement
		mouseRotations(1, viewPerspective);

		drawPrecipitationWithRadar();

		makeSkybox();

		modelview = endRender(modelview);

			
		//Draw the user

		modelview = render(modelview);

		//testingOnly.Draw(projection, modelview, window.size, 0);

		//Draw cpu players (for both teams)

		board.Draw(projection, modelview, tod, 0);

		//Draw bullets
		mat4 scaler;
		scaler = translate(modelview, vec3(0.0f, 0.5f, 0.0f));
		scaler = rotate(scaler, 90.0f, vec3(0,1,0));

		drawStadium();

		/*
		stringstream yS (stringstream::in | stringstream::out);stringstream rS (stringstream::in | stringstream::out);
		stringstream bS (stringstream::in | stringstream::out);stringstream gS (stringstream::in | stringstream::out);
		stringstream ammo (stringstream::in | stringstream::out);stringstream grenades (stringstream::in | stringstream::out);
		
		ammo << userTeam.soldiers[0].ammo;yS << userTeam.numRemaining;gS << greenTeam.numRemaining;
		bS << blueTeam.numRemaining;rS << redTeam.numRemaining;grenades << userTeam.soldiers[0].grenades;

		activeTextDisplay((yS.str()).c_str(), 0.3, 0.85, 32, vec4(1,1,0,1));
		activeTextDisplay((gS.str()).c_str(), 0.45, 0.85, 32, vec4(0,1,0,1));
		activeTextDisplay((bS.str()).c_str(), 0.6, 0.85, 32, vec4(0,0,1,1));
		activeTextDisplay((rS.str()).c_str(), 0.75, 0.85, 32, vec4(1,0,0,1));
		activeTextDisplay(("Ammo: " + ammo.str()).c_str(), -0.95f, -0.80f, 16, vec4(1,1,1,1));
		activeTextDisplay(("Grenades: " + grenades.str()).c_str(), -0.95f, -0.9f, 16, vec4(1,1,1,1));
		*/

		/*if(board.oWins || board.xWins){
			//gameOver = true;
		}*/
		if(board.oWins){
			activeTextDisplay("O's Win!", -0.85f, 0.75f, 64, vec4(1,0,0,1));
		}
		else if(board.xWins){
			activeTextDisplay("X's Win!", -0.85f, 0.75f, 64, vec4(0.25,0.25,1,1));
		}

	}
	else{
		
		
	}

	if(movingWRTSnow > 0){movingWRTSnow = movingWRTSnow - 0.1f;}
	if(movingWRTSnow < 0){movingWRTSnow = 0.0;}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFlush();
	
}

//Put objects into the fbo
void RenderIntoFrameBuffer(){fbo.Bind();GameDisplay();fbo.Unbind();}

//Give fbo texture id to an object
void UseFramebufferToDrawSomething(){	
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	glEnable(GL_TEXTURE_2D);
	framebufferScreen.fboID = fbo.texture_handles[0];
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

//Manage pausing and when to display the game / jumbotrons
void DisplayFunc(){
	glViewport(0, 0, window.size.x, window.size.y);
	if(!window.paused){
		GameDisplay();	
		glViewport(0, 0, (GLsizei)(512.0), (GLsizei)(512.0));
		RenderIntoFrameBuffer();
		UseFramebufferToDrawSomething();

		if(!gameOver && !menu.menuOn){
			glm::mat4 Projection2 = glm::ortho( -10.0f, 10.0f, -10.0f, 10.0f,1.0f, 10.0f);
			glm::mat4 View       = glm::lookAt(
				glm::vec3(0,0,5), // Camera is at (0,0,5), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);  

			mat4 fboMatrix = View;
			fboMatrix = translate(fboMatrix, vec3(0.0,10.0,0));
			fboMatrix = rotate(fboMatrix, 90.f, vec3(0,1,0));
			fboMatrix = rotate(fboMatrix, -90.f, vec3(0,0,1));
			fboMatrix = rotate(fboMatrix, -90.f, vec3(0,1,0));
			fboMatrix = scale(fboMatrix, vec3(5, 1, 2));
			//framebufferScreen.Draw(Projection2, fboMatrix, window.size, 0);
		}
		glutSwapBuffers();
	}	
}

void TimerFunc(int value){
	if (window.window_handle != -1){glutTimerFunc(window.interval, TimerFunc, value);glutPostRedisplay();}
}

int main(int argc, char * argv[])
{
	/* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewInit();
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	srand (time(NULL));

	//Initialize object's colors / values
	disc.color = vec3(0.1f, 0.5f, 1.0f);
	fontfilename = "Pokemon.ttf";

	window.window_handle = glutCreateWindow("Base Project");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	
	glutMotionFunc(mouseMovement);
	glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	glutKeyboardFunc(KeyboardFunc);
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	
	//Initialize everything

	menu.Initialize();

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	init_resources();
	glutDisplayFunc(DisplayFunc);

	if (!fbo.Initialize(glm::ivec2(512, 512), 1, true)){
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}

	usr.color = vec3(0.0f, 0.0f, 0.5f);
	if (!usr.Initialize(2,0.05f, "phong.vert", "phong.frag")){return 0;}
	if (!stadium.Initialize(0)){return 0;}
	if (!skybox.Initialize(0, 5000, "basic_skybox_shader.vert", "basic_skybox_shader.frag")){return 0;}
	
	if (!disc.Initialize(8, 1.0f, "phong3.vert", "phong3.frag")){return 0;}
	if (!healthBar.Initialize(2, 1.0f, 1.f, 1.f, "phong3.vert", "phong3.frag")) {return 0;}
	if (!precip.Initialize()){return 0;}

	userTeam.userTeam = true;
	if (!userTeam.Initialize(1)) {return 0;}
	cpuTeam.userTeam = false;
	if (!cpuTeam.Initialize(2)) {return 0;}

	if(!board.Initialize(vec3(1,0,0))){return 0;}

	glutMainLoop();
}