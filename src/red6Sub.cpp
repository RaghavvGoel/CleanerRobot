#include "ros/ros.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/SetModelState.h>
#include "geometry_msgs/Twist.h"
void print(const gazebo_msgs::ModelState::ConstPtr& msg)
{
	double x = msg->twist.linear.x;
	double y = msg->twist.linear.y;
	ROS_INFO("x:%f,y:%f",x,y);
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"red6Pub");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/gazebo/get_model_state", 10,print);
	ros::spin();
	return 0;
	//int count = 0;
}
