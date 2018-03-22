#include <iostream>
#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/ModelState.h>
#include "gazebo_msgs/ContactsState.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include <cstddef>


#define STATE_STOP 			0
#define STATE_FORWARD 		1
#define STATE_REVERSE		2
#define STATE_LEFT_TURN		3
#define STATE_RIGHT_TURN	4

using namespace std;
void bump_event();

bool bump_happened;
int state;

void bump_event(const gazebo_msgs::ContactsState::ConstPtr& msg)
{
	if(msg->states.size() == 0) {
		bump_happened = false;
	} else {
		bump_happened = true;
		ROS_INFO("Collision detected: %s with %s", msg->states[0].collision1_name.c_str(), msg->states[0].collision2_name.c_str());
	}
}
int main(int argc, char **argv)
{
	
	greetings(); // useless function just for checking multi fil in ROS
	bump_happened = false;
	state = STATE_STOP;
	
	ros::init(argc,argv,"red1");
	ros::NodeHandle n;
	ros::Subscriber bumper_sub = n.subscribe("bumper_contact_sensor_state", 1000,bump_event);	
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
	//ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	ros::Rate loop_rate(10);
	ros::Time ros_time;
	geometry_msgs::Twist model_twist;


	//-- The FSM (the brain)
	//   Decision making process	
	while(ros::ok()) {
		switch(state) {
			case STATE_STOP:
				ROS_INFO("Robot stopped, going forward.");
				state = STATE_FORWARD;
				break;
			case STATE_FORWARD:
				if(bump_happened) {
					ROS_INFO("Bump detected");
					state = STATE_REVERSE;
					ros_time = ros::Time::now();
				}
				break;
			case STATE_REVERSE:
				if(ros::Time::now() > ros_time + ros::Duration(0.5)) {
					ROS_INFO("Turning Right.");
					state = STATE_RIGHT_TURN;
					ros_time = ros::Time::now();
				}
				break;
			case STATE_RIGHT_TURN:
				if(ros::Time::now() > ros_time + ros::Duration(0.5)){
					ROS_INFO("Going forward again.");
					state = STATE_FORWARD;
				}
				break;	
		}
		
		//calling function move_robot in module MoneRobotGaz.cpp
		move_robot(vel_pub); 
		//vel_pub.publish(model_twist);
		ros::spinOnce();
		loop_rate.sleep();
	}
		return 0;		

}
