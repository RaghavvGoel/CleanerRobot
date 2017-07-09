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
		return;
	}
	
	string col1 = msg->states[0].collision1_name;
	//string col2 = msg->collision2_name;
	
	double colx = msg->states[0].contact_positions[0].x;
	double coly = msg->states[0].contact_positions[0].y;
	
	//double z = msg->z;1
	ROS_INFO("name: %s,x: %f,y:%f",col1.c_str(),colx,coly);
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"Contact");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("base_contact_sensor_state", 1000, print);
	ros::spin();
	return 0;
	//int count = 0;
}
