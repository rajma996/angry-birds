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

#include "construct3D.h"
#include "rectangle.h"
#include "circle.h"
#include "score.h"




vector<VAO> balls;
vector<VAO> stand;
vector<VAO> canon;
vector<VAO> power_arr;
vector<VAO> obstructions;
int power;
bool power_state;
bool keys[26];

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


void releaseball()
{
  // if(balls.size()==0)
  // {
    balls.push_back(*create_circle(canon[1].trans[0],canon[1].trans[1],10,10,0,1,0,1));
    int t = balls.size();
    balls[t-1].vx=55*cos(canon[0].rotate_angle*M_PI/180)*((float)power/66);
    balls[t-1].vy=40*sin(canon[0].rotate_angle*M_PI/180)*((float)power/66);
  // }
}
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
          case GLFW_KEY_X:
                keys['X'-'A']=0;
                break;
          case GLFW_KEY_Z:
                  keys['Z'-'A']=0;
                  break;
          case GLFW_KEY_R:
                keys['R'-'A']=0;
                  break;
          case GLFW_KEY_D:
                keys['D'-'A']=0;
                break;
          case GLFW_KEY_A:
                keys['A'-'A']=0;
                break;
          case GLFW_KEY_W:
                keys['W'-'A']=0;
                break;
          case GLFW_KEY_S:
                keys['S'-'A']=0;
                break;
          default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
          case GLFW_KEY_X:
                keys['X'-'A']=1;
                break;
          case GLFW_KEY_Z:
                  keys['Z'-'A']=1;
                  break;
          case GLFW_KEY_R:
                // if(balls.size()==0)
                  keys['R'-'A']=1;
                  break;
          case GLFW_KEY_D:
                keys['D'-'A']=1;
                break;
          case GLFW_KEY_A:
                keys['A'-'A']=1;
                break;
          case GLFW_KEY_W:
                keys['W'-'A']=1;
                break;
          case GLFW_KEY_S:
                keys['S'-'A']=1;
                break;
          case GLFW_KEY_T:
                if(balls.size()!=0)
                  balls.erase(balls.begin());
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

	// set the projection matrix as perspective
	/* glMatrixMode (GL_PROJECTION);
	   glLoadIdentity ();
	   gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
	// Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    Matrices.projection = glm::ortho(-650.0f, 650.0f, -400.0f, 400.0f, 0.1f, 500.0f);
}

// Creates the triangle object used in this sample code

/* Render the scene with openGL */
/* Edit this function according to your assignment */

void checkkeys()
{
  if(keys['X'-'A']==1) canon[0].rotate_angle++;
  if(keys['Z'-'A']==1) canon[0].rotate_angle--;
  if(keys['R'-'A']==1) releaseball();
  // if(keys['D'-'A']==1 && canon[0].trans[0]<570 && canon[0].trans[1]==-250) {canon[0].trans[0]++; canon[1].trans[0]++;stand[0].trans[0]++;}
  // if(keys['A'-'A']==1 && canon[0].trans[0]>-570 && canon[0].trans[1]==-250) {canon[0].trans[0]--; canon[1].trans[0]--;stand[0].trans[0]--;}
  if(keys['W'-'A']==1 && canon[0].trans[1]<250 && canon[0].trans[0]>=-570 && canon[0].trans[0]<-530)
  {
    canon[0].trans[1]++; canon[1].trans[1]++; stand[0].trans[1]++; stand[1].trans[1]++;
    stand[2]=*create_rectangle(stand[1].trans[0],(stand[1].trans[1]-400)/2,10,(stand[1].trans[1]+400)/2,0,0,0,0,0,0);
  }
  if(keys['S'-'A']==1 && canon[0].trans[1]>-250 && canon[0].trans[0]>=-570 && canon[0].trans[0]<-530)
  {
    canon[0].trans[1]--; canon[1].trans[1]--; stand[0].trans[1]--; stand[1].trans[1]--;
    stand[2]=*create_rectangle(stand[1].trans[0],(stand[1].trans[1]-400)/2,10,(stand[1].trans[1]+400)/2,0,0,0,0,0,0);
  }
}

void checkpower()
{
  int i=power_arr.size()-1;
  if(power_state==1)
  power_arr.push_back(*create_rectangle(620,-392+i*8,10,4,0,(float)i/100,0,(100-(float)i)/100,0,0));
  else power_arr.pop_back();
}
void draw ()
{
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
  loop(i,0,obstructions.size())
  {
    // if(obstruction[i].trans)
    if(obstructions[i].vy>0 && obstructions[i].trans[1]+obstructions[i].sizey>=300) {obstructions[i].vy*=-1; obstructions[i].starttime=glfwGetTime();}
    else if(obstructions[i].vy<0 && obstructions[i].trans[1]-obstructions[i].sizey<=-300) {obstructions[i].vy*=-1; obstructions[i].starttime=glfwGetTime();}
    obstructions[i].trans[1]=obstructions[i].trans[1]+(obstructions[i].vy)*(glfwGetTime()-obstructions[i].starttime);
     construct(obstructions[i]);
    //  sleep(10);
   }
  for (int i=0;i<balls.size();i++)
  {
    // printf("%f %f\n",balls[i].vx,balls[i].vy);
    if(balls[i].trans[0]<-570 || balls[i].trans[0]>570 || balls[i].trans[1]<-300 ) {  balls.erase(balls.begin()+i); }
    float etime=glfwGetTime()-balls[i].starttime;
    balls[i].trans[0]=balls[i].trans[0]+balls[i].vx*(etime)/2;
    balls[i].trans[1]=balls[i].trans[1]+balls[i].vy*(etime)-(etime)*(etime);
    balls[i].vy=balls[i].vy-etime;
    if(balls[i].trans[1]-balls[i].sizey<=-300)
    {
       balls[i].vy=-0.7*balls[i].vy; balls[i].starttime=glfwGetTime(); balls[i].trans[1]=-290;
       if(balls[i].vy<1.3) balls.erase(balls.begin()+i);
    };
    for(int j=0;j<obstructions.size();j++)
    {
      float temp = obstructions[j].trans[0]-obstructions[j].sizex;
      float u = obstructions[j].trans[1]+obstructions[j].sizey; float l = obstructions[j].trans[1]-obstructions[j].sizey;
      if(balls[i].trans[0]+balls[i].sizex>=temp-10 && balls[i].trans[0]+balls[i].sizex<=temp+10 && balls[i].trans[1]<=u+5 && balls[i].trans[1]>=l-5) balls[i].vx*=-1;
    }
    construct(balls[i]);
  }
  loop(i,0,power_arr.size()) construct(power_arr[i]);
	score_value++;
	if(score_value==0) {  display_digit(0,600,200); display_score(); }
	else
	{
		int score_temp = score_value;
		int temp_x = 570;
		while(score_temp!=0)
		{
				score.clear();
				display_digit(score_temp%10,temp_x,350);
				score_temp/=10; temp_x-=(2*lengthx+10);
				display_score();
		}
	}
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
  obstructions.push_back(*create_rectangle(-300,rand()%300,10,50,0,0,0,0,0,rand()%35*(rand()%2&2?1:-1)));
  obstructions.push_back(*create_rectangle(-250,rand()%300-300,10,50,0,0,1,0,0,rand()%35*(rand()%2&2?1:-1)));
  obstructions.push_back(*create_rectangle(-200,rand()%300-150,10,50,0,0,0,1,0,rand()%35*(rand()%2&2?1:-1)));
  loop(i,0,obstructions.size())
  {
    int temp=rand()%35;
    if(rand()%2==0) obstructions[i].vy=temp;
    else obstructions[i].vy=-1*temp;
  }
}
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
	// Create the models
  for (int i=0;i<26;i++) keys[i]=0;
  stand.push_back(*create_rectangle(-570,-300,30,10,90,0,0,0,0,0));
  stand.push_back(*create_rectangle(-570,-340,50,10,0,0,0,0,0,0));
  stand.push_back(*create_rectangle(-570,-370,10,60,0,0,0,0,0,0));
  canon.push_back(*create_rectangle(-570,-250,80,20,45,0,1,0,0,0));
  canon.push_back(*create_circle(-570,-250,40,40,0,1,0,0));
  power = 0;
  power_state=1;
	score_value = 0;
  createobstructions();
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
        if(power_state==1)
        {
          if(power==100) {power_state=0;}
          else power++;
        }
        else
        {
          if(power==2) {power_state=1;}
          else power--;
        }
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
