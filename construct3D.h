#ifndef construct_H_
#define construct_H_


GLuint programID;
float camera_rotation_angle = 90;

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

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data);
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue);
void draw3DObject (struct VAO* vao);
void construct(VAO x);

#include "construct3D.c"
#endif
