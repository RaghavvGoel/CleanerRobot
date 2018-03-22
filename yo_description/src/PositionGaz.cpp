#include "ros/ros.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/SetModelState.h>
#include "geometry_msgs/Twist.h"
#include "test.h"
#include "nav_msgs/Odometry.h"

//void odom()
float POS_X=0,POS_Y=0,POS_Z=0,ANGLE_MADE=0;
float IniX=0,IniY,IniZ,IniAngleMade;
int count_for_pos=0;
//the following function is for giving the relative position
//Now we can do so by taking the initial position and then marking the final position by
//adding into the initial position and then again changing the initial position.
//Now to pass the final position we can pass ARGUEMENTS BY REFERENCE to the function find_pos 
//Arguements like posX, posY, posZ, AngleMade
//THE VARIALBLES FOR INITIAL POSITION ARE ALREADY GLOBAL AND SO CNA BE USED DIRECTLY
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
	  POS_Y =      floor(msg->pose.pose.position.y);
	  POS_Z = 	   floor(msg->pose.pose.position.z);
	  ANGLE_MADE = floor(10*msg->pose.pose.orientation.z);
	  ROS_INFO("%f,%f,%f,%f,%f,%f",POS_X,POS_Y,POS_Z,ANGLE_MADE,IniX,IniAngleMade);
	  if(count_for_pos <= 0){
	  	IniX = POS_X;
	  	IniY = POS_Y;
	  	IniZ = POS_Z;
	  	IniAngleMade = ANGLE_MADE;
	  	count_for_pos++;}
	}
	

void find_pos()
{
		ros::NodeHandle n;
	    ros::Subscriber sub = n.subscribe("/create/odom", 1000, odom);
		return;
}

