#include "ros/ros.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ContactsState.h"
#include <sstream>

void chatterCallback(const gazebo_msgs::ContactsState::ConstPtr& cs) // this line has been edited by adding "::" between ContactState and ConstPtr&
{
  int a,b,c,d;
  a = sizeof(cs);
  b = sizeof(cs->header);
  c = sizeof(cs->states);
  d = sizeof(cs->states[0]);
  //std::stringstream ss;
  //ss << a;
  ROS_INFO("The size of recieved ContactsState message is: %d,b: %d,c: %d,d: %d", a,b,c,d);

  // double z = cs->states[0].total_wrench.force.z; // this line has been commented due to this error : ‘const struct gazebo_msgs::ContactState_<std::allocator<void> >’ has no member named ‘states’
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "ContactSub");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/bumper_contact_sensor_state", 1000, chatterCallback);

  ros::Publisher pub = n.advertise<std_msgs::String>("mymsg",1000); //here is for you question 2).
  std_msgs::String msg;
  msg.data = "Hello"; // this line has been edited due to this error: conversion from ‘const char [6]’ to non-scalar type ‘std_msgs::String’ requested

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
      pub.publish(msg);

      ros::spinOnce();
      loop_rate.sleep();
  }
  return 0;
}







/*#include "ros/ros.h"
#include "gazebo_msgs/ContactsState.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"

using namespace std;
using std::string;
void print(const gazebo_msgs::ContactsState::ConstPtr& msg)
{
	//string col1 = msg->collision1_name;
	//string col2 = msg->collision2_name;
	//double x = msg->contact_positions.x;
	//double y = msg->contact_positions.y;
	double z = msg->z;
	ROS_INFO("z:%f",z);
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"Contact");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("bumper_contact_sensor_state", 10, print);
	ros::spin();
	return 0;
	//int count = 0;
}*/
