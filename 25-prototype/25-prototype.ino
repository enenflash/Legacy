/*
Teamname: Legacy
Authors: Isabelle Tang, Ryan Beikrasouli & Shing Feng 
Date of Creation: 11/1/25
*/

/* ##### LIBRARIES ##### */
#include <iostream>
#include <array>
#include <map>
#include <algorithm>
#include <time.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SoftwareSerial.h>
#include "SparkFun_Qwiic_OTOS_Arduino_Library.h"
#include "Grove_Multi_Switch.h"

using namespace std;

/* ##### PINS ##### */
// Motor Pins
#define TL_PWM 5
#define TL_DIR 4
#define TR_PWM 6
#define TR_DIR 7
#define BL_PWM 3
#define BL_DIR 2
#define BR_PWM 9
#define BR_DIR 8

// Ultrasonic Pins
#define USR_TRIG 34
#define USR_ECHO 33
#define USL_TRIG 22
#define USL_ECHO 23
#define USB_TRIG 28
#define USB_ECHO 27

// IDLE SWITCH
#define IDLE_PIN 40

/* ##### CONSTANTS ##### */

// Field Mesurements
#define FIELD_WIDTH 183
#define FIELD_LENGTH 244

// Robot Settings
const bool USING_OTOS = true;

// History lengths
const int DIR_HIST_LENGTH = 5;
const int STR_HIST_LENGTH = 5;
const int ULT_HIST_LENGTH = 5;

// Strength distances
const int MIN_CLOSE_STRENGTH = 35;
const int ULTRASONIC_TO_ROBOT = 10;
const int MINIMUM_STRENGTH_DIFFERANCE = 10;

// Positions
const float GOAL_POS_X = FIELD_WIDTH/2.0;
const float GOAL_POS_Y = 30.0;
const int LINE_LIMIT = 30;

// Speeds 
// Note: Slowest Possible Speed is 10%
const float MAX_SPEED = 100.0;
const int BALL_BEHIND_CLOSE_SPEED = MAX_SPEED * 0.7;
const int BALL_BEHIND_SPEED = MAX_SPEED * 0.8;
const int LINE_SPEED = 10;
const float MINIMUM_PID_SPEED = 20.0;


// IR Directions To Radians
std::map<int, float> irAngles = {
  { 0,  0        },
  { 11, PI/3     },
  { 10, PI/6     },
  { 9,  0        },
  { 8,  11*PI/6  },
  { 7,  5*PI/3   },
  { 6,  3*PI/2   },
  { 5,  4*PI/3   },
  { 4,  7*PI/6   },
  { 3,  PI       },
  { 2,  5*PI/6   },
  { 1,  2*PI/3   },
  { 12, PI/2     }
};

// PID constants
const float PROPORTIONAL_CONSTANT = 4.0;
const float INTEGRAL_CONSTANT = 0.005;
const float DERIVETIVE_CONSTANT = 0.1;


