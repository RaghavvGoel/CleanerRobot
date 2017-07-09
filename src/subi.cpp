#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

void print(const geometry_msgs::Twist::ConstPtr& msg)
{
	double x = msg->linear.x;
	double y = msg->linear.y;
	ROS_INFO("x:%f,y:%f",x,y);
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"subi");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("chat", 10, print);
	ros::spin();
	return 0;
	//int count = 0;
}
