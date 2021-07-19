// * LIBRARIES
#include <AFMotor.h>
#include <Servo.h>

// * INPUTS
const int sensorOutPin = 10;

// * OUTPUTS
// ** locomotion
const int leftChannel = 1;
const int rightChannel = 4;

AF_DCMotor leftMotor(leftChannel, MOTOR12_64KHZ);
AF_DCMotor rightMotor(rightChannel, MOTOR12_64KHZ);

// ** movement
// *** hand
const int handServoPin = A0;

Servo handServo;

// *** arm
const int yawServoPin = A1;
const int elbowServoPin = A2;
const int shoulderServoPin = A3;

Servo yawServo, elbowServo, shoulderServo;

// ** vision
const int S0 = 2;
const int S1 = 3;
const int S2 = 8;
const int S3 = 9;

// * VARIABLES
// ** locomotion
const int motorSpeed = 250;

// ** movement
// *** hand
const int handGraspValue = 1;
const int handReleaseValue = 180;

// *** arm
// **** elbow
const int elbowReachValue = 180;
const int elbowRetractValue = 1;

// **** shoulder
const int shoulderReachValue = 180;
const int shoulderRetractValue = 1;

// ** vision
// *** object
int objectColor = 0;

// *** colors
const int red = 1;
const int yellow = 4;
const int blue = 6;

int color = 0;

// ** commands
String string = "";
int command;

// ** debug
const bool debug = true;
const bool verbose = true;

void setup()
{
    Serial.begin(9600);

    // motors should be fully stopped first
    leftMotor.run(RELEASE);
    rightMotor.run(RELEASE);

    leftMotor.setSpeed(motorSpeed);
    rightMotor.setSpeed(motorSpeed);

    yawServo.attach(yawServoPin);
    elbowServo.attach(elbowServoPin);
    shoulderServo.attach(shoulderServoPin);
    handServo.attach(handServoPin);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    retract();
    release();

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
}

void loop()
{
    if (Serial.available())
    {
        if (debug)
        {
            Serial.println("available");
        }

        doTheLocomotion();
    }
}

void doTheLocomotion()
{
    getCommand();

    if (shouldGoForward())
    {
        goForward();
    }
    else if (shouldGoBackward())
    {
        goBackward();
    }
    else if (shouldTurnLeft())
    {
        turnLeft();
    }
    else if (shouldTurnRight())
    {
        turnRight();
    }
    else if (shouldBrake())
    {
        brake();
    }
    // else if (shouldPickup())
    // {
    //     pickup();
    // }
    // else if (shouldDrop())
    // {
    //     drop();
    // }
    else if (shouldHandleBox())
    {
        handleBox();
    }
}

void getCommand()
{
    string = Serial.readStringUntil("\n");
    if (debug && verbose)
    {
        Serial.print("string is: \"");
        Serial.println(string);
        Serial.println("\"");
    }

    command = string.toInt();
    if (debug)
    {
        Serial.print("command is: \"");
        Serial.print(command);
        Serial.println("\"");
    }
}

/*
|--------------------------------------------------------------------------
| Checks
|--------------------------------------------------------------------------
*/
bool shouldGoForward()
{
    return command == 1;
}

bool shouldGoBackward()
{
    return command == 2;
}

bool shouldTurnLeft()
{
    return command == 3;
}

bool shouldTurnRight()
{
    return command == 4;
}

bool shouldBrake()
{
    return command == 5;
}

bool shouldPickup()
{
    return command == 6;
}

bool shouldDrop()
{
    return command == 7;
}

bool shouldHandleBox() {
    return command == 8;
}

/*
|--------------------------------------------------------------------------
| Locomotion
|--------------------------------------------------------------------------
*/
void goForward()
{
    if (debug)
    {
        Serial.println("goForward");
    }
    leftForward();
    rightForward();
}

void goBackward()
{
    if (debug)
    {
        Serial.println("goBackward");
    }
    leftBackward();
    rightBackward();
}

/*
|--------------------------------------------------------------------------
| Turns
|--------------------------------------------------------------------------
*/
void turnLeft()
{
    if (debug)
    {
        Serial.println("turnLeft");
    }
    leftBackward();
    rightForward();
}

void turnRight()
{
    if (debug)
    {
        Serial.println("turnRight");
    }
    leftForward();
    rightBackward();
}

/*
|--------------------------------------------------------------------------
| Wheels
|--------------------------------------------------------------------------
*/
void leftForward()
{
    if (debug && verbose)
    {
        Serial.println("leftForward");
    }
    // leftMotor.setSpeed(motorSpeed);
    leftMotor.run(FORWARD);
}

void rightForward()
{
    if (debug && verbose)
    {
        Serial.println("rightForward");
    }
    // rightMotor.setSpeed(motorSpeed);
    rightMotor.run(FORWARD);
}

void leftBackward()
{
    if (debug && verbose)
    {
        Serial.println("leftBackward");
    }
    // leftMotor.setSpeed(motorSpeed);
    leftMotor.run(BACKWARD);
}

void rightBackward()
{
    if (debug && verbose)
    {
        Serial.println("rightBackward");
    }
    // rightMotor.setSpeed(motorSpeed);
    rightMotor.run(BACKWARD);
}

/*
|--------------------------------------------------------------------------
| Brakes
|--------------------------------------------------------------------------
*/
void brake()
{
    if (debug)
    {
        Serial.println("brake");
    }
    brakeLeft();
    brakeRight();
}

void brakeLeft()
{
    if (debug && verbose)
    {
        Serial.println("brakeLeft");
    }
    leftMotor.run(RELEASE);
}

void brakeRight()
{
    if (debug && verbose)
    {
        Serial.println("brakeRight");
    }
    rightMotor.run(RELEASE);
}

/*
|--------------------------------------------------------------------------
| Movement
|--------------------------------------------------------------------------
*/
void pickup(int color)
{
    if (debug)
    {
        Serial.println("pickup");
    }
    release();
    reach();
    grasp();

    objectColor = color;

    retract();
}

void drop()
{
    if (debug)
    {
        Serial.println("drop");
    }
    reach();
    release();

    objectColor = 0;

    retract();
}

/*
|--------------------------------------------------------------------------
| Hand
|--------------------------------------------------------------------------
*/
void grasp()
{
    if (debug)
    {
        Serial.println("grip");
    }
    handServo.write(handGraspValue);
}

void release()
{
    if (debug && verbose)
    {
        Serial.println("release");
    }
    handServo.write(handReleaseValue);
}

/*
|--------------------------------------------------------------------------
| Arm
|--------------------------------------------------------------------------
*/
void reach()
{
    if (debug && verbose)
    {
        Serial.println("reach");
    }
    elbowServo.write(elbowReachValue);
    shoulderServo.write(shoulderReachValue);
}

void retract()
{
    if (debug)
    {
        Serial.println("retract");
    }
    elbowServo.write(elbowRetractValue);
    shoulderServo.write(shoulderRetractValue);
}

void handleBox()
{
    color = detectColor();
    if (debug)
    {
        Serial.println(color);
    }
    if (debug && verbose) {
        switch (color)
        {
            case red:
                return "red";
                Serial.println("red");
                break;
            case yellow:
                Serial.println("yellow");
                break;
            case blue:
                Serial.println("blue");
                break;
            default:
                break;
        Serial.println("non interesting color");
        }
    }

    reactTo(color);
}

void reactTo(int color)
{
    if (!hasObject() && colorIsInteresting(color))
    {
        if (debug)
        {
            Serial.println("I have no object and the detected color is interesting.");
        }
        pickup(color);
    }

    if (hasObject() && colorIsInteresting(color))
    {
        if (debug)
        {
            Serial.println("I have an object and the detected color is interesting.");
        }
        if (objectColor == color)
        {
            drop();
        }
    }
}

int detectColor()
{
    int R = detectRed();
    int G = detectGreen();
    int B = detectBlue();

    if (debug)
    {
        Serial.print("R: ");
        Serial.print(R);
        Serial.print("\tG: ");
        Serial.print(G);
        Serial.print("\tB: ");
        Serial.println(B);
    }

    if (R<45 & R> 32 & G<65 & G> 55)
    {
        color = red; // Red
    }
    // if (G<55 & G> 43 & B<47 & B> 35)
    // {
    //     color = 2; // Orange
    // }
    // if (R<53 & R> 40 & G<53 & G> 40)
    // {
    //     color = 3; // Green
    // }
    if (R<38 & R> 24 & G<44 & G> 30)
    {
        color = yellow; // Yellow
    }
    // if (R<56 & R> 46 & G<65 & G> 55)
    // {
    //     color = 5; // Brown
    // }
    if (G<58 & G> 45 & B<40 & B> 26)
    {
        color = blue; // Blue
    }

    return color;
}

/*
|--------------------------------------------------------------------------
| Detect Color
|--------------------------------------------------------------------------
*/
int detectRed()
{
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    return pulseIn(sensorOut, LOW);
}

int detectGreen()
{
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    return pulseIn(sensorOut, LOW);
}

int detectBlue()
{
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    return pulseIn(sensorOut, LOW);
}

bool colorIsInteresting(int color)
{
    return color == red || color == yellow || color == blue;
}

bool hasObject()
{
    return objectColor != 0;
}

bool userIsControlling() {

}
