#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

void moveCallback(const geometry_msgs::Twist::ConstPtr& msg)
{		
	geometry_msgs::Twist model_twist;
	
	model_twist.linear.x = msg->linear.x;
	model_twist.linear.y = msg->linear.y;
	model_twist.linear.z = msg->linear.z;
	ROS_INFO("%f",msg->linear.x);
}		


int main(int argc, char **argv)
{
	ros::init(argc,argv,"subi");
	ros::NodeHandle n;
	ros::Subscriber sub_message = n.subscribe("chatter", 10, chatterCallback);
	ros::Subscriber sub_move    = n.subscribe("mover", 10, moveCallback);
	ros::spin();
	return 0;
	//int count = 0;
}
