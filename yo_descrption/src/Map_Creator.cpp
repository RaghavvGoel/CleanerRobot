#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <cstdlib>
#include "gazebo_msgs/ContactsState.h"
#include "nav_msgs/Odometry.h"
#include <fstream>
#include <yo_description/creator.h>


float POS_X=0.0,POS_Y=0.0,POS_Z=0.0,ANGLE_MADE=0.0;
float IniX=0.0,IniY,IniZ,IniAngleMade;

int x,y,count=0;
bool bump_happened,visited;

int map[81][81];
ros::Time visit_map[85][85];

yo_description::creator message;

//std_msgs::Bool visited;

void bump_check(const gazebo_msgs::ContactsState::ConstPtr& msg)
{
	if(msg->states.size() == 0) {
		bump_happened = false;
	} else {
		bump_happened = true;		
		//ROS_INFO("Collision detected: %s with %s", msg->states[0].collision1_name.c_str(), msg->states[0].collision2_name.c_str());
		//here we'll send a boolean type message
	}
}

void odom(const nav_msgs::Odometry::ConstPtr& msg)
{
  //ROS_INFO("Seq: [%d]", msg->header.seq);
  /*ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
  ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);
  */
  /*POS_X = static_cast<int>(std::round(msg->pose.pose.position.x));
  POS_Y = static_cast<int>(std::round(msg->pose.pose.position.y));
  POS_Z = static_cast<int>(std::round(msg->pose.pose.position.z));
  ANGLE_MADE = static_cast<int>(std::round(10*msg->pose.pose.orientation.z));
  */
  POS_X =      floor(msg->pose.pose.position.x * 10.)/10.;
  POS_Y =      floor(msg->pose.pose.position.y * 10.)/10.;
  POS_Z = 	   floor(msg->pose.pose.position.z * 10.)/10.;
  ANGLE_MADE = floor(10*msg->pose.pose.orientation.z * 10.)/10.;
  ROS_INFO("%f,%f,%f,%f,%f,%f",POS_X,POS_Y,POS_Z,ANGLE_MADE,IniX,IniAngleMade);
  if(count <= 0){
  	IniX = POS_X;
  	IniY = POS_Y;
  	IniZ = POS_Z;
  	IniAngleMade = ANGLE_MADE;
  	count++;
  }
  POS_X = POS_X - IniX;
  POS_Y = POS_Y - IniY;
  POS_Z = POS_Z - IniZ;

    x = (POS_X + 4.0)*10;
    y = (POS_Y + 4.0)*10;

    message.x = x;
    message.y = y;
    message.bump = bump_happened;


    // here we'll publish the x,y and bump detected and holder will subscribe to it.
    //check if you can send via array or you'll need to create a message type which you can always do!
  	if(bump_happened){
		ROS_INFO("Bump Detected");
			if (map[x][y] < 15){
				map[x][y]++;  //max value can't be more than 15 	
			}
	}else if(!bump_happened && map[x][y] == 0){
		//map[x][y] = 1;
		//remains zero only 
	}else{
		map[x][y]--;
	}
	ROS_INFO("position %d, %d, value %d",x,y,map[x][y]);

	/*for(int i=0;i<81;i++){
		for(int j=0;j<81;j++){
			std::cout<<map[i][j];
		}
		
	}
	std::cout<<"kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk \n";
	/*
	std::ofstream out("matrix.txt");

	for (int i=0;i<81;i++)
	{
		for (int j=0;j<81;j++)
		{
			out << map[i][j] << " "; // behaves like cout - cout is also a stream
		}
		out << "\n";
	} 
	*/
	
	//Putting visit_map matrix here
	//we're giving it ros::Time type values so that we can compare it with time
	//currently we're checking the visited point with 5s passed, so if current time is <= then time + 5 then you've
	//alresdy visited this point and stop.
	
	std::cout<<"this is the ros time in seconds "<<ros::Time::now().toSec()<<" "<<visit_map[x][y].toSec()<<"\n";
	
	if(visit_map[x][y].toSec() <= 0.0){
		visit_map[x][y] = ros::Time::now();
		std::cout<<"initialising the visit matrix "<<visit_map[x][y].toSec()<<"\n";
	}

	//for corner parts it takes longer than 5s 
	if(visit_map[x][y].toSec() + ros::Duration(20).toSec() <= ros::Time::now().toSec() ){
		//point is visited most probably
		visited = true;
		ROS_INFO("The point is visited");
	}else{
		visited = false;
		visit_map[x][y] = ros::Time::now();
		ROS_INFO("The point is NOT visited: ");
	}
	
	/*
	for(int i=0;i<81;i++){
		for(int j=0;j<81;j++){
			std::cout<<visit_map[i][j];
		}
		
	}
	std::cout<<"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm \n";
	*/
}


int main(int argc, char **argv)
{
	ros::init(argc,argv,"Map");
	ros::NodeHandle n_map;
	
	//subscribe from contact_sensor_bumper or subscribe from Brain the topic bumped
	ros::Subscriber bump_sub  = n_map.subscribe("bumper_contact_sensor_state", 100, bump_check);
	
	//subscriber for relpos
	//we'll take realtive position till first place and multiply it by 10
	ros::Subscriber pos_sub   = n_map.subscribe("create/odom", 10, odom);
	
	//publisher for holder, publishing x, y and bump detected [x,y wrt to matrix]
	ros::Publisher create_pub = n_map.advertise<yo_description::creator>("creator_to_holder", 1000);
	ros::Rate loop_rate(10);
	ros::Time ros_time;
	
	//publisher for VISITED points
	ros::Publisher visit_pub = n_map.advertise<std_msgs::Bool>("visited_nodes", 100);
	
	int x,y;

	//if bump happened then 0 if not then 1 and 
	//if a point is above matrix size then let's see
	while(ros::ok()){

		create_pub.publish(message);
		visit_pub.publish(visited);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
