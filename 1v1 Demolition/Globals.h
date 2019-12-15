#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "3D Physics Playground"

// 3D Context variables
#define SKYBOX_ALPHA 1.f					//Defines the alpha/transparency of the skybox in the 3D context.

// ---------------------- GENERAL VARIABLES ---------------------- 
// Misc Variables
#define HALF 0.5f
#define CAM_OFFSET 1

// Arena Elements Variables
#define ROTOR_TORQUE 60000.0f 

// ----------------- Player Variables -----------------
// Vehicle Movement Variables
#define MAX_ACCELERATION 5200.0f			//Fast: 5500.0f, Slow but steady: 3650.0f 
#define TURN_DEGREES 40.0f * DEGTORAD		//Slow Turn: 25.0f
#define BRAKE_POWER 1250.0f

// Gameplay Variables
#define MAX_BODIES 30
#define MAX_LIVES 3
#define MAX_AMMO 4
#define Y_OFFSET 1
#define PROJECTILE_FORCE 5000.0f

class Application;

extern Application* App;