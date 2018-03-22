#include "ros/ros.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/SetModelState.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include <string>
#include "nav_msgs/Odometry.h"
#include "yo_description/relpos.h"
#include "yo_description/current_pos.h"
#include "iostream"

float POS_X=0.0,POS_Y=0.0,POS_Z=0.0,ANGLE_MADE=0.0;
float IniX=0.0,IniY,IniZ,IniAngleMade;
int count=0;

void odom(const nav_msgs::Odometry::ConstPtr& msg)
{
  //ROS_INFO("Seq: [%d]", msg->header.seq);
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
  //ROS_INFO("%f,%f,%f,%f,%f,%f",POS_X,POS_Y,POS_Z,ANGLE_MADE,IniX,IniAngleMade);
  if(count <= 0){
  	IniX = POS_X;
  	IniY = POS_Y;
  	IniZ = POS_Z;
  	IniAngleMade = ANGLE_MADE;
  	count++;}
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"RelPos");
	ros::NodeHandle n_pos;
	//Subscriber for odom
	ros::Subscriber sub = n_pos.subscribe("/create/odom", 1000, odom);
	//Publishing message for the Move, Dist. travelled 
	//ros::Publisher chatter_pub = n_pos.advertise<std_msgs::String>("DistTravelled", 1000);
	ros::Publisher chatter_pub = n_pos.advertise<yo_description::relpos>("DistTravelled", 1000);
	ros::Publisher current_pub = n_pos.advertise<yo_description::current_pos>("current_position", 1000);
	
	ros::Rate loop_rate(10);
	ros::Time ros_time;
	yo_description::relpos message1;
	yo_description::current_pos message2;

	while(ros::ok()) {

    	//std::stringstream ss;
    	
    	//enter the message
    	/*
    	message.data = ss.str();
    	ss << POS_X-IniX <<"  " << ANGLE_MADE - IniAngleMade;
 	    ROS_INFO("dist moved and angle moved : %s", message.data.c_str());
		*/
		message1.dist_moved = POS_X-IniX;
		message1.angle_moved = ANGLE_MADE - IniAngleMade;
		
		//publishing initial coordinates and angle to move for traversing purposes
		message2.currentX = (POS_X + 4.0)*10;
		message2.currentY = (POS_Y)*10; // by using control we found that Y is going from 40 to 120 so we dont need to : POS_Y + 4.0
		message2.current_angle = ANGLE_MADE;

		ROS_INFO("currentX %d  currentY %d and angle %f ",message2.currentX,message2.currentY,floor(message2.current_angle));
		//chatter_pub.publish(message1); 
		current_pub.publish(message2);
	
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;		

}	
	
	
	
	
	
	
	
	
