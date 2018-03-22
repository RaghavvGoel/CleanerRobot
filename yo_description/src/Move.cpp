#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <cstdlib>


using namespace std;
geometry_msgs::Twist vel;

class MoveRobot
{
	geometry_msgs::Twist vel_class;
	float linearX,angularZ;
	
	public:
		MoveRobot();
		~MoveRobot();
		void forward();
		
};
MoveRobot::MoveRobot()
{
	//ros::Publisher vel_pub = n_move.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
}
MoveRobot::~MoveRobot()
{
}
void MoveRobot::forward()
{
	vel_class.linear.x = 1.0;
	vel_class.angular.z = 0.0;
	//vel_pub.publish(vel);
}
void brain_to_move(const std_msgs::String::ConstPtr& msg)
{
  std::string sms = msg->data.c_str();	
  ROS_INFO("I heard: [%s]", msg->data.c_str());
  int space_pos = sms.find(" ");
  std::string action = sms.substr(0,space_pos);
  std::string DistanceToMove = sms.substr(space_pos+1,sms.length());
  int DistMove = std::atoi(DistanceToMove.c_str());
  ROS_INFO("DistToMove is %s and action is %s.",DistanceToMove.c_str(),action.c_str());	
  //now we'll control the robot from here
  
  //geometry_msgs/Twist vel;
  if(action.compare("forward") == 0){
  	//We can set the velocities here itself or make function call to the clss we've made
  	//while()
  	vel.linear.x = 1.0;
  	vel.angular.z = 0.0;
  	ROS_INFO("moving forward");
  	//r.forward();
  }else if(action.compare("reverse") == 0){
  	ROS_INFO("moving backward");
  }else{
  }
}

void dist_travelled(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
  
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"move");
	ros::NodeHandle n_move;
	MoveRobot r1;
	//Subscriber for message from Brain
	ros::Subscriber brain_sub = n_move.subscribe("BrainToMove", 10, brain_to_move);
	//Declaring publisher for message to be answered back
	ros::Publisher move_pub   = n_move.advertise<std_msgs::String>("MoveToBrain", 1000);
	ros::Publisher vel_pub    = n_move.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
	
	ros::Rate loop_rate(10);
	ros::Time ros_time;
	std_msgs::String message;
	//geometry_msgs::Twist vel;
	
	while(ros::ok()) {
		
		vel_pub.publish(vel);
    	std::stringstream ss;
    	//enter the message for the brain
    	ss << "done";
    	message.data = ss.str();
    	//Subscribing for the Dist. Travelled
		ros::Subscriber dist_sub = n_move.subscribe("DistTravelled", 10, dist_travelled);    	
 	    //ROS_INFO("%s", message.data.c_str());
		move_pub.publish(message);
		ros::spinOnce();
		loop_rate.sleep(); 
	}
	
	return 0;
	
}
