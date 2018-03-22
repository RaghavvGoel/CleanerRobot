#include "ros/ros.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/SetModelState.h>
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
// THIS MODULE SETS THE ROBOT'S SPEED AND THEN CALLS THE POSITION FUNCTION 
// HOW WE'LL CONTROL THE POSITION

using namespace std;
//void find_pos();
/*int POS_X=0,POS_Y=0,POS_Z=0,ANGLE_MADE=0;
int IniX=0,IniY,IniZ,IniAngleMade;
int count_for_pos=0;
*/
/*void odom(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Seq: [%d]", msg->header.seq);
  /*ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
  ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);
  POS_X = static_cast<int>(std::round(msg->pose.pose.position.x));
  POS_Y = static_cast<int>(std::round(msg->pose.pose.position.y));
  POS_Z = static_cast<int>(std::round(msg->pose.pose.position.z));
  ANGLE_MADE = static_cast<int>(std::round(10*msg->pose.pose.orientation.z));
  */
/*POS_X =      floor(msg->pose.pose.position.x);
  POS_Y =      floor(msg->pose.pose.position.y);
  POS_Z = 	   floor(msg->pose.pose.position.z);
  ANGLE_MADE = floor(10*msg->pose.pose.orientation.z);
  ROS_INFO("%d,%d,%d,%d,%d,%d",POS_X,POS_Y,POS_Z,ANGLE_MADE,IniX,IniAngleMade);
  if(count_for_pos <= 0){
  	IniX = POS_X;
  	IniY = POS_Y;
  	IniZ = POS_Z;
  	IniAngleMade = ANGLE_MADE;
  	count_for_pos++;}
}*/

 
void move_robot(ros::Publisher vel_pub)
{		

		find_pos();
	    /*ros::Subscriber sub = n.subscribe("/create/odom", 1000, odom);
		*/
		ros::NodeHandle n;
		geometry_msgs::Twist model_twist;
		
		switch(state)
		{
			case STATE_FORWARD:
				model_twist.linear.x =  1.0;
				model_twist.angular.z = 0.0;
				break;
			case STATE_REVERSE:
				model_twist.linear.x = -1.0;
				model_twist.angular.z = 0.0;
				break;
			case STATE_RIGHT_TURN:
				model_twist.linear.x =   0.0;
				model_twist.angular.z = -1.0;
				break;
			default:
				model_twist.linear.x =  0.0;
				ROS_INFO("State undefined.");
		}
		vel_pub.publish(model_twist);

		return;
}

		

