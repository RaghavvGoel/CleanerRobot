#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/ModelState.h>
#include "gazebo_msgs/ContactsState.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include <cstddef>
#include "nav_msgs/Odometry.h"

#define STATE_STOP 			0
#define STATE_FORWARD 		1
#define STATE_REVERSE		2
#define STATE_LEFT_TURN		3
#define STATE_RIGHT_TURN	4

void odom();

float POS_X=0.0,POS_Y=0.0,POS_Z=0.0,ANGLE_MADE=0.0;
float IniX=0.0,IniY,IniZ,IniAngleMade;
int state,count=0;

void odom(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Seq: [%d]", msg->header.seq);
  /*ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
  ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);
  */
  /*POS_X = static_cast<int>(std::round(msg->pose.pose.position.x));
  POS_Y = static_cast<int>(std::round(msg->pose.pose.position.y));
  POS_Z = static_cast<int>(std::round(msg->pose.pose.position.z));
  ANGLE_MADE = static_cast<int>(std::round(10*msg->pose.pose.orientation.z));
  */
  POS_X =      floor(msg->pose.pose.position.x * 10.)/10.;
  POS_Y =      floor(msg->pose.pose.position.y * 10.)/10.;
  POS_Z = 	   floor(msg->pose.pose.position.z * 10.)/10.;
  ANGLE_MADE = floor(10*msg->pose.pose.orientation.z * 10.)/10.;
  ROS_INFO("%f,%f,%f,%f,%f,%f",POS_X,POS_Y,POS_Z,ANGLE_MADE,IniX,IniAngleMade);
  if(count <= 0){
  	IniX = POS_X;
  	IniY = POS_Y;
  	IniZ = POS_Z;
  	IniAngleMade = ANGLE_MADE;
  	count++;}
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odom_listener");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/create/odom", 1000, odom);
  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
  
  ros::Rate loop_rate(10);
  ros::Time ros_time;
  geometry_msgs::Twist model_twist;

  //Now we'll call the publisher to move till certain position is achieved!
  //We'll have to take care in negative where floor is the less 
  	while(ros::ok()){
  		//checking if bot moves till 4m or not
  		if(POS_X < IniX + 2.5){          
  			ROS_INFO("moving forward %f",POS_X); 
  			state = STATE_FORWARD;
  		}else if(ANGLE_MADE < IniAngleMade + 5){
  			state = STATE_RIGHT_TURN;
  		}else{
  			state = STATE_STOP;
  		}
  		switch(state){
			case STATE_FORWARD:
				model_twist.linear.x =  1.0;
				model_twist.angular.z = 0.0;
				break;
			case STATE_RIGHT_TURN:
				model_twist.linear.x = 0;
				model_twist.angular.z = 1.0;
				break;
			default:
				model_twist.linear.x = 0;
				model_twist.angular.z = 0;
				ROS_INFO("Robot Stopped.");				
  		}
  		vel_pub.publish(model_twist);
		ros::spinOnce();
		loop_rate.sleep();
    }
  return 0;
}

