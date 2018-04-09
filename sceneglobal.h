#ifndef SCENEGLOBAL_H
#define SCENEGLOBAL_H

#include <GL/glew1/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class SceneMode;

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1080
#endif // WINDOW_WIDTH

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 720
#endif // WINDOW_HEIGHT

extern int width;
extern int height;
extern bool firstSet;
extern bool viewerShouldCalculateDot;
extern bool viewerShouldFindHotSpot;
extern SceneMode scene_mode;
extern GLint camera[];
extern GLfloat yaw;
extern GLfloat pitch;
extern GLfloat m_roll;
extern GLfloat m_pitch;
extern GLfloat aspect;
extern GLfloat lastX;
extern GLfloat lastY;
extern bool rot_camera;
extern GLfloat cameraSpeedRate;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern GLfloat cameraSpeed;
extern double lastFrame;
extern double currentFrame;

extern int airPhotoWidth;
extern int airPhotoHeight;

extern float CoverBoxXmin;
extern float CoverBoxXmax;
extern float CoverBoxYmin;
extern float CoverBoxYmax;
extern float CoverBoxZmin;
extern float CoverBoxZmax;

extern glm::vec3 beam_near;
extern glm::vec3 beam_far;

extern glm::vec3 point1;
extern glm::vec3 point2;

#endif // SCENEGLOBAL_H
