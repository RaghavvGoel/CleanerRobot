#include "ros/ros.h"
#include "gazebo_msgs/ContactsState.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include <cstddef>
using namespace std;
//using std::string;
void print(const gazebo_msgs::ContactsState::ConstPtr& msg)
{
	if(msg->states.size() == 0) {
		return ;
	}
	for(int i=0;i<msg->states.size();i++)
	{
		string col1 = msg->states[i].collision1_name;
		//string col2 = msg->collision2_name;
		double colx = msg->states[i].contact_positions[0].x;
		double coly = msg->states[i].contact_positions[0].y;
		double colz = msg->states[i].contact_positions[0].z;
		ROS_INFO("name: %s,x: %f,y:%f,z:%f",col1.c_str(),colx,coly,colz);
		ROS_INFO("true");
	
	}
}


int main(int argc, char **argv)
{
	ros::init(argc,argv,"Contact");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("bumper_contact_sensor_state", 1000, print);
	ros::spin();
	return 0;
	//int count = 0;
}
