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
	ros::Publisher movement_pub = n2.advertise<geometry_msgs::Twist>("/create/cmd_vel", 1);
	//geometry_msgs::Twist model_twist;
	int count = 0;
	ros::Rate rate(10);
	
ros::Time start = ros::Time::now();
while(ros::Time::now() - start < ros::Duration(10.0))
{
    geometry_msgs::Twist move;
    //velocity controls
    move.linear.x = 1.1; //speed value m/s
    move.angular.z = 0;
    movement_pub.publish(move);
	ROS_INFO("popo");
    ros::spinOnce();
    rate.sleep();
}
//turn right
ros::Time start_turn = ros::Time::now();
while(ros::Time::now() - start_turn < ros::Duration(4.0))
{
    geometry_msgs::Twist move;
    //velocity controls
    move.linear.x = 0; //speed value m/s
    move.angular.z = -2.25;
    movement_pub.publish(move);

    ros::spinOnce();
    rate.sleep();
}
//move forward again
ros::Time start2 = ros::Time::now();
while(ros::Time::now() - start2 < ros::Duration(5.0))
{
    geometry_msgs::Twist move;
    //velocity controls
    move.linear.x = 1.1; //speed value m/s
    move.angular.z = 0;
    movement_pub.publish(move);

    ros::spinOnce();
    rate.sleep();
}

return 0;
} 	
