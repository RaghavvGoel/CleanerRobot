#include <iostream>
#include "test.h"
#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/ModelState.h>
#include "gazebo_msgs/ContactsState.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include <cstddef>
#include "test.h"


void bump_event(const gazebo_msgs::ContactsState::ConstPtr& msg)
{
	if(msg->states.size() == 0) {
		bump_happened = false;
	} else {
		bump_happened = true;
		ROS_INFO("Collision detected: %s with %s", msg->states[0].collision1_name.c_str(), msg->states[0].collision2_name.c_str());
	}
/*		ROS_INFO("lolo")
		 d = velo(c);
		 c = d;
		 ROS_INFO("inside if");
		 return;
	}
	ROS_INFO("out if");
	d = velo(c*-1);
	c = d;
	//for(int i=0;i<msg->states.size();i++)
	{
		string col1 = msg->states[i].collision1_name;
		//string col2 = msg->collision2_name;
		double colx = msg->states[i].contact_positions[0].x;
		double coly = msg->states[i].contact_positions[0].y;
		double colz = msg->states[i].contact_positions[0].z;
		ROS_INFO("name: %s,x: %f,y:%f,z:%f",col1.c_str(),colx,coly,colz);
		ROS_INFO("true");
		
	}*/
}

