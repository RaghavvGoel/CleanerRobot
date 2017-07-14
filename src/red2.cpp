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
	ros::init(argc,argv,"red2");
	ros::NodeHandle n2;
	ros::Publisher vel_pub = n2.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
	geometry_msgs::Twist model_twist;
	int count = 0;
	ros::Rate loop_rate(10);
	
	ros::Time start = ros::Time::now();
	while(ros::Time::now() - start < ros::Duration(10.0))
	{
		geometry_msgs::Twist model_twist;
		model_twist.linear.x = 1.0;
		model_twist.angular.z = 0.0;
		vel_pub.publish(model_twist);
		ros::spinOnce();
    	loop_rate.sleep();
    	//ROS_INFO("yomo");
    	//count++;
	}
	ros::Time start2 = ros::Time::now();
	while(ros::Time::now() - start2 < ros::Duration(10.0))
	{
		geometry_msgs::Twist model_twist;
		model_twist.linear.x = 1.0;
		model_twist.angular.z = 0.0;
		vel_pub.publish(model_twist);
		ros::spinOnce();
    	loop_rate.sleep();
    	//ROS_INFO("yomo");
    	//count++;
	}
	//int count = 0;
	return 0;
} 	
//yoyo
