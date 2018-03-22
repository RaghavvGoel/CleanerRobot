#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/ModelState.h>
#include "gazebo_msgs/ContactsState.h"
#include "std_msgs/String.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include <cstddef>
#include "nav_msgs/Odometry.h"

#define STATE_STOP 			0
#define STATE_FORWARD 		1
#define STATE_REVERSE		2
#define STATE_LEFT_TURN		3
#define STATE_RIGHT_TURN	4



using namespace std;
void move_robot(ros::Publisher vel_pub);
void bump_event();
void odom();

int nof=0; //nof is count
int POS_X=0,POS_Y=0,POS_Z=0,ANGLE_MADE=0;
int IniX=0,IniY,IniZ,IniAngleMade;
bool bump_happened;
int state;

void odom(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Seq: [%d]", msg->header.seq);
  /*ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
  ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);
  */
  /*POS_X = static_cast<int>(std::round(msg->pose.pose.position.x));
  POS_Y = static_cast<int>(std::round(msg->pose.pose.position.y));
  POS_Z = static_cast<int>(std::round(msg->pose.pose.position.z));
  ANGLE_MADE = static_cast<int>(std::round(10*msg->pose.pose.orientation.z));
  */
  POS_X =      floor(msg->pose.pose.position.x);
  POS_Y =      floor(msg->pose.pose.position.y);
  POS_Z = 	   floor(msg->pose.pose.position.z);
  ANGLE_MADE = floor(10*msg->pose.pose.orientation.z);
  ROS_INFO("%d,%d,%d,%d,%d,%d",POS_X,POS_Y,POS_Z,ANGLE_MADE,IniX,IniAngleMade);
  if(nof <= 0){
  	IniX = POS_X;
  	IniY = POS_Y;
  	IniZ = POS_Z;
  	IniAngleMade = ANGLE_MADE;
  	nof++;}
  //count++;
}

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

void move_robot(ros::Publisher vel_pub)
{		
	geometry_msgs::Twist model_twist;

	switch(state)
	{
		case STATE_FORWARD:
			model_twist.linear.x = -1.0;
			break;
		case STATE_REVERSE:
			model_twist.linear.x =  1.0;
			break;
		default:
			ROS_INFO("State undefined.");
	}
	vel_pub.publish(model_twist);
}		
			
int main(int argc, char **argv)
{
	//robo r1;
	
	bump_happened = false;
	state = STATE_STOP;
	
	ros::init(argc,argv,"red1");
	ros::NodeHandle n;
	ros::NodeHandle od;	
	ros::Subscriber bumper_sub = n.subscribe("bumper_contact_sensor_state", 1000, bump_event);
	ros::Subscriber position = n.subscribe("bumper_contact_sensor_state", 1000, odom);
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
	
	ros::Rate loop_rate(10);
	ros::Time ros_time;
	geometry_msgs::Twist model_twist;


	//-- The FSM (the brain)
	//   Decision making process	
	while(ros::ok()) {
		if(POS_X < IniX + 3){
			switch(state) {
				case STATE_STOP:
					ROS_INFO("Robot stopped, going forward.");
					state = STATE_FORWARD;
					break;
				case STATE_FORWARD:
					if(bump_happened) {
						ROS_INFO("Bump detected");
						state = STATE_REVERSE;
						ros_time = ros::Time::now();
					}
					break;
				case STATE_REVERSE:
					if(ros::Time::now() > ros_time + ros::Duration(2.0)) {
						ROS_INFO("Turning Right.");
						state = STATE_RIGHT_TURN;
						ros_time = ros::Time::now();
					}
					break;
				case STATE_RIGHT_TURN:
					if(ros::Time::now() > ros_time + ros::Duration(1.0)){
						ROS_INFO("Going forward again.");
						state = STATE_FORWARD;
					}
					break;	
			}
		}
	
	
	
		//-- move the robot based on the decision calculated earlier
		switch(state)
		{
			case STATE_FORWARD:
				model_twist.linear.x =  1.0;
				model_twist.angular.z = 0.0;
				break;
			case STATE_REVERSE:
				model_twist.linear.x = -1.0;
				model_twist.angular.z = 0.0;
				break;
			case STATE_RIGHT_TURN:
				model_twist.linear.x =  0.0;
				model_twist.angular.z = -1.0;
				break;
			default:
				model_twist.linear.x =  0.0;
				ROS_INFO("State undefined.");
		}
		vel_pub.publish(model_twist);
		ros::spinOnce();
		loop_rate.sleep();
	}


		
		
	
	//ros::spin();
	  //frequency of loop


	//gazebo_msgs::ModelState cmd_vel;
	//cmd_vel.modnntel_name = "create";
	// cmd_vel.reference_frame = "world";


/*	geometry_msgs::Twist model_twist;
	model_twist.linear.x = 1.0;
	model_twist.linear.y = 0.0;
	model_twist.linear.z = 0.0;
	model_twist.angular.x = 0.0;
	model_twist.angular.y = 0.0;
	model_twist.angular.z = 0.0;
*/

/*while (n.ok())
{
    

	//cmd_vel.twist = model_twist;
	
	vel_pub.publish(model_twist);
    
    
   /* if(sub == -1)
    {
    	model_twist.linear.x = -1.0;
		model_twist.linear.y = 0.0;
		model_twist.linear.z = 0.0;
		model_twist.angular.x = 0.0;
		model_twist.angular.y = 0.0;
		model_twist.angular.z = 0.0;
		
    }
    ros::spinOnce();

    loop_rate.sleep();
	
	//if(count>5)
	//{
		//ros::shutdown();
	//}
    count++;
}*/

  return 0;
}


