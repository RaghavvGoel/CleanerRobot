#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <cstdlib>
#include "yo_description/creator.h"
#include "test.h"
#include <cmath>
//#include <conio>
using namespace std;


//we'll have to take input from user all the time so cin in while loop
//assign speed acordingly and hence subscribe to cmd_vel or whatever the name is
//record the points : whether there is an obstacle there or not

//assign 1 for free and -1 for obstacle, initially all 0


int main(int argc, char **argv)
{
	ros::init(argc,argv,"control");
	ros::NodeHandle n;

	//publisher for velocity
	ros::Publisher vel_pub   = n.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);

	ros::Rate loop_rate(10);
	geometry_msgs::Twist  vel;
	while(ros::ok()){
		char a = ' ';
		std::cin>>a;
		//a = _getch();
		std::cout<<a<<" yoy \n";

		if(a == 'w'){ //FORWARD
			vel.linear.x  =  1.0;
			vel.angular.z =  0.0;
			ros::Duration(0.1);
		}else if(a == 'a'){ // LEFT TURN
			vel.linear.x  =   0.0;
			vel.angular.z =  -0.5;
			ros::Duration(0.1);			
		}else if(a == 'd'){           // RIGHT
			vel.linear.x  =  0.0;
			vel.angular.z =  0.5;
			ros::Duration(0.1);			
		}else if(a == 's'){				//BACK
			vel.linear.x  =  -1.0;
			vel.angular.z =   0.0;
			ros::Duration(0.1);			
		}else{  			// STOP
			vel.linear.x  =   0.0;
			vel.angular.z =   0.0;
			ros::Duration(0.1);						
		}

		vel_pub.publish(vel);

		ros::spinOnce();
		loop_rate.sleep();

	}

	return 0;
}
