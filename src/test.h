#ifndef TEST_H
#define TEST_H

#include "std_msgs/String.h"
#include <string>
#include "yo_description/brain.h"
#include "yo_description/relpos.h"
#include "yo_description/current_pos.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#define STATE_FORWARD_INFINITY  -1 
#define STATE_STOP 				 0
#define STATE_FORWARD 			 1
#define STATE_REVERSE			 2
#define STATE_LEFT_TURN			 3
#define STATE_RIGHT_TURN		 4


class MoveRobot
{
	ros::NodeHandle n_move;
	
	ros::Publisher  vel_pub , move_pub , border_pub , traversal_complete_pub;
	ros::Subscriber brain_sub , dist_sub , bump_sub , visit_sub , border_sub , traversal_sub , current_pos_sub;
	
	ros::Time ros_time;
	geometry_msgs::Twist  vel;
	std_msgs::String      message;
	std_msgs::Bool        task_done;
	
	float DistanceToMove;
	float DistanceMoved;
	float AngleRotated;
	bool  bump_happened , point_is_visited;
	int   state , state_original;
	int   cur_x , cur_y , prev_x , prev_y , mat[90][90];
	float cur_angle;
	bool  border_found;
	//int count;
	//float linearX,angularZ;
	
	public:
		MoveRobot();
		~MoveRobot();
		void forward();
		void reverse();
		void turn_right();
		void turn_left();
		void stop();
		void face_left();
		void turn_180();
		
		void brain_to_move(const std_msgs::String::ConstPtr& msg);
		void brain_move(const yo_description::brain::ConstPtr& msg);		
		
		void bumper(const std_msgs::Bool::ConstPtr& msg);
		
		void dist_travelled(const yo_description::relpos::ConstPtr& msg);
		
		void point_visited(const std_msgs::Bool::ConstPtr& msg);

		void publish_vel();
		void publish_message_to_brain();
		
		void subscribe_dist();
		bool DistCompared();
		bool position_reached();

		void find_border(const std_msgs::Bool::ConstPtr& msg);
		void get_current_pos(const yo_description::current_pos::ConstPtr& msg);
		void traverse_matrix(const std_msgs::Bool::ConstPtr& msg);
};

int forward_till_infinity;

#endif
