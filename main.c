#include <kipr/wombat.h>
//Function Declarations
void grabBall();
void attachTransporter();
void followTheLine(int m1, int m2);
void trackTB();
void knockOver();

int main()
{
    enable_servos();
    //Sets servo positions at the start of the run
    set_servo_position(3,1000);
    set_servo_position(2,1200);
    set_servo_position(1,2047);
    msleep(500);

    while(digital(1) != 1){
        mav(0,1400);
        mav(3,1500);
    }
    ao();

    mav(0,-1500);
    mav(3,1000);
    msleep(850);
    set_servo_position(3, 2000);
    //Drives robot until it reaches the black line in front of the tennis ball door
    while(analog(5)<4000){
        mav(0,1430);
        mav(3,1500);
    }
    mav(0,-1500);
    mav(3,-1500);
    msleep(800);
    mav(0,1500);
    msleep(600);
    mav(3,1500);
    msleep(300);
    ao();
   //Uses servo arm to open and close the tennis ball door 4 times 
    int count1 = 0;
    while(count1 < 4){
        set_servo_position(3,1000);
        msleep(3000);
        set_servo_position(3,2000);
        msleep(1000);
        count1++;
    }

    mav(3,1500);
    mav(0,750);
    msleep(2000);
    //Drives to the transporter using the black line on the game board
    while(analog(5)<4000){
        mav(0,750);
        mav(3,1500);
    }

    set_servo_position(3,1000);
    msleep(500);
    while(digital(1) == 0){
        followTheLine(3,0);
        if(digital(1) == 1){
            break;
        }
    }
    ao();

    attachTransporter();
    set_servo_position(1,1024);
    msleep(500);
    set_servo_position(3,2047);
    msleep(500);
    mav(3,1500);
    mav(0,1500);
    msleep(1000);
    ao();

    //Tracks tennis balls and places them in the transporter
    cmpc(0);
    while(gmpc(0)<10000){
        camera_open();
        mav(0,1500);
        mav(3,1500);
        if(analog(0)<2900){
            trackTB();
        }
        if(analog(0)>2800){
        	grabBall();
        }
    }
    //Return to starting box
    set_servo_position(3,1350);
    mav(3,1500);
    mav(0,1500);
    if(digital(1) == 1){
        mav(0,1500);
        mav(3,-1500);
        msleep(800);
    }
    ao();
    set_servo_position(1,2047);
    disable_servos();
    return 0;
}

//Function to grab tennis/pickle balls and place them one by one in the transporter attached to the back of the robot
void grabBall(){
    ao();
    //Picks up ball
    set_servo_position(1,1024);
    msleep(500);
    set_servo_position(3,2047);
    msleep(500);
    set_servo_position(1,1900);
    msleep(500);
    //Slowly moves servo arm back
    int pos = 2000;
    while(pos>0){
        set_servo_position(3,pos);
        msleep(2);
        pos-=2;
    }
    //Drops ball
    set_servo_position(1,1024);
    msleep(500);
    set_servo_position(3,2047);
    msleep(500);
    ao();
}

//Function that attaches the transporter to the back of the robot
void attachTransporter(){
    int check = 0;
    //Waits for lever button press at the front of the robot and turns around to reverse back towards the transporter
    if(digital(1) == 1){
        mav(0,-1500);
        mav(3,-1500);
        msleep(1000);
        mav(0,1750);
        mav(3,-1500);
        msleep(1550);
        //Backs up until button press then drops back servo to secure the transporter
        while(digital(0) == 0 && check == 0){
            mav(0,-500);
            mav(3,-500);
        }
        if(digital(0) == 1){
            ao();
            check=1;
            set_servo_position(2,0);
            msleep(500);
        }
    }
    set_servo_position(3,2047);
}

//Function to follow the black line on the game board
void followTheLine(int m1, int m2){
    while(digital(1)==0){
        if(analog(5) > 4000){
            mav(m1,1500);
            mav(m2,0);
        }
        if(analog(5) < 4000){
            mav(m1,0);
            mav(m2,1500);
        }
    }
    ao();
}

//Function to track the color of the tennis ball
void trackTB(){
    camera_update();
    if(get_object_count(0) == 0){
        mav(0,750);
        mav(3,-750);
    }
    else{
        if(get_object_center_x(0,0) > 85){
            mav(3,750);
        }
        else if(get_object_center_x(0,0) < 75){
            mav(0,750);
        }
        else{
            ao();
        }
    }
}

//Function to knock over the rock-a-stack
void knockOver(){
    set_servo_position(1,800);
    set_servo_position(3,1700);
    mav(0,1500);
    mav(3,1500);
    msleep(3000);
    ao();
}

