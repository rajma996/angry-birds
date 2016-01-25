#include <bits/stdc++.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

#define loop(i,a,b) for(int i=a;i<b;i++)

// #define push_back pb;
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


float ortho_x;
float ortho_y;
float ortho_x_shift;
float ortho_y_shift;
#include "construct3D.h"
#include "rectangle.h"
#include "circle.h"
#include "score.h"
#include "targets.h"
vector<VAO> canon;
int power;
bool keys[500];
vector<VAO> obstructions;
#include "show_projectile.h"
#include "rocks.c"
#include "balls.h"


vector<VAO> stand;
vector<VAO> power_arr;
double mouse_x,mouse_y;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
          case GLFW_KEY_X:
                keys[GLFW_KEY_X]=0;
                break;
          case GLFW_KEY_Z:
                  keys[GLFW_KEY_Z]=0;
                  break;
          case GLFW_KEY_R:
                keys[GLFW_KEY_R]=0;
                  break;
          case GLFW_KEY_D:
                keys[GLFW_KEY_D]=0;
                break;
          case GLFW_KEY_A:
                keys[GLFW_KEY_A]=0;
                break;
          case GLFW_KEY_W:
                keys[GLFW_KEY_W]=0;
                break;
          case GLFW_KEY_S:
                keys[GLFW_KEY_S]=0;
                break;
					case GLFW_KEY_P:
								keys[GLFW_KEY_P]=0;
								break;
					case GLFW_KEY_O:
								keys[GLFW_KEY_O]=0;
								break;
					case GLFW_KEY_L:
								if(power_projectile>0 && projectile_state == 1)
										power_projectile --;
								keys[GLFW_KEY_L]=0;
								break;
					case GLFW_KEY_UP:
								keys[GLFW_KEY_UP]=0;
								break;
					case GLFW_KEY_DOWN:
								keys[GLFW_KEY_DOWN]=0;
								break;
					case GLFW_KEY_LEFT:
								keys[GLFW_KEY_LEFT]=0;
								break;
  				case GLFW_KEY_RIGHT:
	  						keys[GLFW_KEY_RIGHT]=0;
								break;

          default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
          case GLFW_KEY_X:
                keys[GLFW_KEY_A]=1;
                break;
          case GLFW_KEY_Z:
                  keys[GLFW_KEY_Z]=1;
                  break;
          case GLFW_KEY_R:
                  keys[GLFW_KEY_R]=1;
                  break;
          case GLFW_KEY_D:
                keys[GLFW_KEY_D]=1;
                break;
          case GLFW_KEY_A:
                keys[GLFW_KEY_A]=1;
                break;
          case GLFW_KEY_W:
                keys[GLFW_KEY_W]=1;
                break;
          case GLFW_KEY_S:
                keys[GLFW_KEY_S]=1;
                break;
          case GLFW_KEY_T:
                if(balls.size()!=0)
                  balls.erase(balls.begin());
                  break;
					case GLFW_KEY_P:
								keys[GLFW_KEY_P]=1;
								break;
					case GLFW_KEY_O:
								keys[GLFW_KEY_O]=1;
								break;
					case GLFW_KEY_L:
								if(power_projectile>0)
									{ keys[GLFW_KEY_L]=1; projectile_state = 1 ; }
								break;
					case GLFW_KEY_UP:
							keys[GLFW_KEY_UP]=1;
							break;
					case GLFW_KEY_DOWN:
							keys[GLFW_KEY_DOWN]=1;
							break;
  				case GLFW_KEY_LEFT:
							keys[GLFW_KEY_LEFT]=1;
							break;
					case GLFW_KEY_RIGHT:
							keys[GLFW_KEY_RIGHT]=1;
							break;
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            default:
                break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
            quit(window);
            break;
		default:
			break;
	}
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
        default:
            break;
    }
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);


    Matrices.projection = glm::ortho(-1300*ortho_x+ortho_x_shift, 1300*ortho_x+ortho_x_shift, -800*ortho_y+ortho_y_shift, 800*ortho_y+ortho_y_shift, 0.1f, 500.0f);
}

// Creates the triangle object used in this sample code

/* Render the scene with openGL */
/* Edit this function according to your assignment */

void checkkeys()
{
  // if(keys['X'-'A']==1) canon[0].rotate_angle++;
  // if(keys['Z'-'A']==1) canon[0].rotate_angle--;
  if(keys[GLFW_KEY_R]==1) releaseball();
  if(keys[GLFW_KEY_D]==1 ) canon[0].rotate_angle++;
  if(keys[GLFW_KEY_A]==1 ) canon[0].rotate_angle--;
  if(keys[GLFW_KEY_W]==1 && canon[0].trans[1]<500 && canon[0].trans[0]>=-1140 && canon[0].trans[0]<-1060)
  {
    canon[0].trans[1]++; canon[1].trans[1]++; stand[0].trans[1]++; stand[1].trans[1]++;
    stand[2]=*create_rectangle(stand[1].trans[0],(stand[1].trans[1]-800)/2,10,(stand[1].trans[1]+800)/2,0,1,1,1,0,0);
  }
  if(keys[GLFW_KEY_S]==1 && canon[0].trans[1]>-500 && canon[0].trans[0]>=-1140 && canon[0].trans[0]<-1060)
  {
    canon[0].trans[1]--; canon[1].trans[1]--; stand[0].trans[1]--; stand[1].trans[1]--;
    stand[2]=*create_rectangle(stand[1].trans[0],(stand[1].trans[1]-800)/2,10,(stand[1].trans[1]+800)/2,0,1,1,1,0,0);
  }
	if(keys[GLFW_KEY_UP]) { ortho_x*=1.01; ortho_y*=1.01; }
	if(keys[GLFW_KEY_DOWN] && ortho_x>1) { ortho_x/=1.01; ortho_y/=1.01; }
	if(keys[GLFW_KEY_LEFT]) { ortho_x_shift++;  }
	if(keys[GLFW_KEY_RIGHT]) { ortho_x_shift--;}
}

void checkpower()
{
  int i=power_arr.size()-1;
	if(keys[GLFW_KEY_P]==1 && power<100)
	{
		power++;
		power_arr.push_back(*create_rectangle(1240,-784+i*16,10,8,0,1-(float)i/100,1-(float)i/100,1-(float)i/100,0,0));
	}
	else if(keys[GLFW_KEY_O] && power>0)
	{
		power--;
		power_arr.pop_back();
	}
}
void draw ()
{
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	canon[0].rotate_angle=atan(mouse_y/mouse_x)*180/M_PI;
  checkkeys();
  checkpower();
  for (int i=0;i<stand.size();i++)
  {
    construct(stand[i]);
  }
  for (int i=0;i<canon.size();i++)
  {
    construct(canon[i]);
  }
	projectile.clear();
	if(keys['L'-'A']==1 && power_projectile>0 && projectile_state==1)
	{
		set_projectile();
	}
	loop(i,0,projectile.size()) construct(projectile[i]);
  loop(i,0,obstructions.size())
  {
    // if(obstruction[i].trans)
    if(obstructions[i].vy>0 && obstructions[i].trans[1]+obstructions[i].sizey>=780) {obstructions[i].vy*=-1; obstructions[i].starttime=glfwGetTime();}
    else if(obstructions[i].vy<0 && obstructions[i].trans[1]-obstructions[i].sizey<=-780) {obstructions[i].vy*=-1; obstructions[i].starttime=glfwGetTime();}
    obstructions[i].trans[1]=obstructions[i].trans[1]+(obstructions[i].vy)*(glfwGetTime()-obstructions[i].starttime);
     construct(obstructions[i]);
    //  sleep(10);
   }
	 move_rocks();
	 draw_rocks();
	 move_balls();
	 draw_balls();
	//  score_value++;
  loop(i,0,power_arr.size()) construct(power_arr[i]);
	if(score_value==0) {  display_digit(0,1140,700); display_score(); }
	else
	{
		int score_temp = score_value,neg_flag=0;
		if(score_value<0) { neg_flag = 1 ; score_temp*=-1; }
		int temp_x = 1140;
		while(score_temp!=0)
		{
				score.clear();
				display_digit(score_temp%10,temp_x,700);
				score_temp/=10; temp_x-=(2*lengthx+10);
				display_score();
		}
		if(neg_flag==1)
		{
			score.clear();
			rectangle4(temp_x,700);
			display_score();
		}
	}
	set_targets();
	move_targets();
	draw_targets();
}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void createobstructions()
{
  obstructions.push_back(*create_rectangle(-600,rand()%600,10,50,0,1,1,1,0,3));
  obstructions.push_back(*create_rectangle(-500,rand()%600-600,10,50,0,1,1,1,0,-3));
  // obstructions.push_back(*create_rectangle(-200,rand()%300-150,10,50,0,0,0,1,0,0));
}
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
	// Create the models
  for (int i=0;i<26;i++) keys[i]=0;
	set_rocks();
	ortho_x = 1;
	ortho_y = 1;
	ortho_x_shift=0; ortho_y_shift=0;
  stand.push_back(*create_rectangle(-1140,-600,10,60,0,0.8,0.8,0.8,0,0));
  stand.push_back(*create_rectangle(-1140,-680,50,10,0,1,1,1,0,0));
  stand.push_back(*create_rectangle(-1140,-740,10,60,0,1,1,1,0,0));
  canon.push_back(*create_rectangle(-1140,-500,80,20,45,1,1,1,0,0));
  canon.push_back(*create_circle(-1140,-500,40,40,0,1,1,1,0,0));
  power = 50;
	loop(i,0,50) power_arr.push_back(*create_rectangle(1240,-784+i*16,10,8,0,1-(float)i/100,1-(float)i/100,1-(float)i/100,0,0));
	score_value = 0;
  createobstructions();
	set_power_projectile(3);
	projectile_state = 0;
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
	int width = 1300;
	int height = 800;

    GLFWwindow* window = initGLFW(width, height);

	initGL (window, width, height);

    double last_update_time = glfwGetTime(), current_time;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        // OpenGL Draw commands
        draw();
        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();
				glfwGetCursorPos(window,&mouse_x,&mouse_y);
				mouse_x-=160; mouse_y-=1160; mouse_y*=-1;
        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 0.5) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds ..
            last_update_time = current_time;
        }
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
