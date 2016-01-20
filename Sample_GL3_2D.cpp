#include <bits/stdc++.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define loop(i,a,b) for(int i=a;i<b;i++)
#define lower_limit -300

using namespace std;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;
    float trans[3];
    float rotate_angle;
    float sizex;
    float sizey;
    float vx;
    float vy;
    float starttime;
    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;


struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
} Matrices;

VAO* create_circle(float x,float y,float a,float b,float angle,float red,float green , float blue);

vector<VAO> balls;
vector<VAO> stand;
vector<VAO> canon;
vector<VAO> power_arr;
vector<VAO> obstructions;
int power;
bool power_state;
bool keys[26];
GLuint programID;

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


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void releaseball()
{
  if(balls.size()==0)
  {
    balls.push_back(*create_circle(canon[1].trans[0],canon[1].trans[1],10,10,0,1,0,1));
    int t = balls.size();
    balls[t-1].vx=55*cos(canon[0].rotate_angle*M_PI/180)*((float)power/66);
    balls[t-1].vy=40*sin(canon[0].rotate_angle*M_PI/180)*((float)power/66);
  }
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
float camera_rotation_angle = 90;
VAO* create_rectangle(float x,float y,float a,float b,float angle,float red,float green , float blue,float vx=0,float vy=0 )
{
  GLfloat vertex_buffer_data[100];
  int i=0,j=0;
  vertex_buffer_data[i++]=a;vertex_buffer_data[i++]=b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=-1*a;vertex_buffer_data[i++]=-1*b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=a;vertex_buffer_data[i++]=-1*b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=-1*a;vertex_buffer_data[i++]=b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=-1*a;vertex_buffer_data[i++]=-1*b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=a;vertex_buffer_data[i++]=b;vertex_buffer_data[i++]=0;
  GLfloat color_buffer_data [100];j=0;
  for(int i=1;i<=6;i++)
  {
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
  }
  struct VAO* vao = new struct VAO;
  vao = create3DObject(GL_TRIANGLES, 6 , vertex_buffer_data, color_buffer_data, GL_FILL);
  vao->trans[0]=x; vao->trans[1]=y; vao->trans[2]=0; vao->sizex=a; vao->sizey=b;
  vao->rotate_angle=angle; vao->starttime=glfwGetTime(); vao->vx=vx; vao->vy=vy; vao->starttime=glfwGetTime();
  return vao;

}
/* Render the scene with openGL */
/* Edit this function according to your assignment */
VAO* create_circle(float x,float y,float a,float b,float angle,float red,float green , float blue)
{
  GLfloat vertex_buffer_data[650];
  int i=0,j=0;
  for (i=1;i<=72;i++)
  {
      int angle = i*5;
      vertex_buffer_data[j++]=a*cos((angle*(3.14159265))/180);
      vertex_buffer_data[j++]=b*sin((angle*(3.14159265))/180);
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=a*cos(((angle-5)*(3.14159265))/180);
      vertex_buffer_data[j++]=b*sin(((angle-5)*(3.14159265))/180);
      vertex_buffer_data[j++]=0;
  }
  GLfloat color_buffer_data [650];j=0;
  for (int r=1;r<=72;r++)
  {
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
  }
  struct VAO* vao = new struct VAO;
  vao = create3DObject(GL_TRIANGLES, 216 , vertex_buffer_data, color_buffer_data, GL_FILL);
  vao->trans[0]=x; vao->trans[1]=y; vao->trans[2]=0; vao->sizex=a; vao->sizey=b;
  vao->rotate_angle=angle; vao->starttime=glfwGetTime();vao->vx=0; vao->vy=0;
  return vao;
}

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
    stand[2]=*create_rectangle(stand[1].trans[0],(stand[1].trans[1]-400)/2,10,(stand[1].trans[1]+400)/2,0,0,0,0);
  }
  if(keys['S'-'A']==1 && canon[0].trans[1]>-250 && canon[0].trans[0]>=-570 && canon[0].trans[0]<-530)
  {
    canon[0].trans[1]--; canon[1].trans[1]--; stand[0].trans[1]--; stand[1].trans[1]--;
    stand[2]=*create_rectangle(stand[1].trans[0],(stand[1].trans[1]-400)/2,10,(stand[1].trans[1]+400)/2,0,0,0,0);
  }
}
void construct(VAO x)
{
  glUseProgram (programID);
  glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  glm::vec3 target (0, 0, 0);
  glm::vec3 up (0, 1, 0);
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;	// MVP = Projection * View * Model
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateobject = glm::translate (glm::vec3(x.trans[0],x.trans[1],x.trans[2])); // glTranslatef
  glm::mat4 rotateobject = glm::rotate((float)(x.rotate_angle*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
  glm::mat4 objectsTransform = translateobject * rotateobject;
  Matrices.model *= objectsTransform;
  MVP = VP * Matrices.model; // MVP = p * V * M
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(&x);
}

void checkpower()
{
  int i=power_arr.size()-1;
  if(power_state==1)
  power_arr.push_back(*create_rectangle(620,-392+i*8,10,4,0,(float)i/100,(100-(float)i)/100,0));
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
  /* Render your scene */

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
  stand.push_back(*create_rectangle(-570,-300,30,10,90,0,0,0));
  stand.push_back(*create_rectangle(-570,-340,50,10,0,0,0,0));
  stand.push_back(*create_rectangle(-570,-370,10,60,0,0,0,0));
  canon.push_back(*create_rectangle(-570,-250,80,20,45,0,1,0));
  canon.push_back(*create_circle(-570,-250,40,40,0,1,0,0));
  power = 0;
  power_state=1;
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
