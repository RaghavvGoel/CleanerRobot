#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <string>
#include "geometry_msgs/Vector3.h"
#include "ros/ros.h"
#include "gazebo_msgs/SetModelState.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <cstdlib>
#include "yo_description/creator.h"
#include "test.h"
#include <cmath>
using namespace std;

 
MoveRobot::MoveRobot()
{
	//brain_sub = n_move.subscribe("BrainToMove", 10, &MoveRobot::brain_to_move, this);
	DistanceToMove = 0.0;
	DistanceMoved  = 0.0;

	brain_sub = n_move.subscribe("brain_to_move", 10, &MoveRobot::brain_move, this);	
	move_pub  = n_move.advertise<std_msgs::Bool>("MoveToBrain", 1000);
	vel_pub   = n_move.advertise<geometry_msgs::Twist>("/create/cmd_vel", 10);
	bump_sub  = n_move.subscribe("bumped", 10, &MoveRobot::bumper, this);
	dist_sub  = n_move.subscribe("DistTravelled", 10, &MoveRobot::dist_travelled, this);
	visit_sub = n_move.subscribe("visited_nodes",10,&MoveRobot::point_visited, this);
	//DistanceToMove = -1.0;	
	border_sub = n_move.subscribe("finding_border",10, &MoveRobot::find_border, this);
	border_pub = n_move.advertise<std_msgs::Bool>("border_found",10); 

	current_pos_sub = n_move.subscribe("current_position",1000,&MoveRobot::get_current_pos, this);

	traversal_sub          = n_move.subscribe("traverse_matrix",10,&MoveRobot::traverse_matrix, this);
	traversal_complete_pub = n_move.advertise<std_msgs::Bool>("traversal_completed",10);
}
MoveRobot::~MoveRobot()
{
}

//=====================================Changes-for-traversal-and-finding-border-done-here===================================
void MoveRobot::find_border(const std_msgs::Bool::ConstPtr& msg){
	if(msg->data){
		//find the border point now that you have the current matrix point
		ROS_INFO("FINDIGN BORDER.........................");
		if(cur_x < cur_y){
			//move up
			turn_180();
			while(!bump_happened){ //move robot till no bump happens : finding the edge
				ROS_INFO("MOVING FOWARD TILL BORDER FOUND ...................");
				forward();vel_pub.publish(vel);
			}stop();vel_pub.publish(vel);
		}else{
			//move left
			ROS_INFO("MOVING LEFT TILL BORDER FOUND .....................");
			face_left();	
			while(!bump_happened){
				forward();vel_pub.publish(vel);
			}stop();vel_pub.publish(vel);
		}
		
		//border found
		border_found = true;
		border_pub.publish(border_found);
		ROS_INFO("BORDER-FOUND NOW START TRAVERSAL");

	}
}

//void MoveRobot::

void MoveRobot::get_current_pos(const yo_description::current_pos::ConstPtr& msg){
	cur_x    = msg->currentX;
	cur_y    = msg->currentY;
	cur_angle = msg->current_angle;
	ROS_INFO("currentX %d  currentY %d and angle %f ",cur_x,cur_y,floor(cur_angle));
}

void MoveRobot::traverse_matrix(const std_msgs::Bool::ConstPtr& msg){
	if(msg->data){
		//write algo for traversal
		ROS_INFO("startign traversal");
		//if(cur_x < cur_y){ // this condition implies robot is at top or at bottom initially
			//face_left();

			while(mat[cur_x-1][cur_y] == 0 || mat[cur_x + 1][cur_y] == 0 || mat[cur_x][cur_y-1] == 0 || mat[cur_x][cur_y-1] == 0){
				
				//if(mat[cur_x][cur_y-1] == 0 || mat[cur_x][cur_y+1] == 0){
					face_left();
					vel_pub.publish(vel);
					//mat[cur_x][cur_y] = count;
					while(!bump_happened || mat[cur_x][cur_y] == 0){
						mat[cur_x][cur_y] = 1;
						forward();
						vel_pub.publish(vel);						
						//count++;	
					}stop();vel_pub.publish(vel);
				//}else{
				//	face_left();
				//}
			}
		//}		
	}
}

//===========================================================================================================================
void MoveRobot::face_left()
{
	while(!fabs(floor(cur_angle) - 7.0) < 0.1){
		turn_left();vel_pub.publish(vel);
		ROS_INFO("difference between cur_angle and 9.0: %f, %f",floor(cur_angle),floor(cur_angle) - 7.0);
	}stop();vel_pub.publish(vel);	
}
void MoveRobot::turn_180()
{
	while(fabs(floor(cur_angle) - 9.0) > 0.1){
		turn_left();vel_pub.publish(vel);
		ROS_INFO("difference between cur_angle and 9.0: %f , %f",floor(cur_angle),floor(cur_angle) - 9.0);
	}stop();vel_pub.publish(vel);
}
void MoveRobot::forward()
{
	vel.linear.x  =  1.0;
	vel.angular.z =  0.0;
	//vel_pub.publish(vel);
}
void MoveRobot::reverse()
{
	vel.linear.x  = -1.0;
	vel.angular.z =  0.0;
	//vel_pub.publish(vel);
}
void MoveRobot::turn_right()
{
	vel.linear.x   =  0.0;
	vel.angular.z  = -1.0;
}
void MoveRobot::turn_left()
{
	vel.linear.x   =  0.0;
	vel.angular.z  =  1.0;
}
void MoveRobot::stop()
{
	vel.linear.x   =  0.0;
	vel.angular.z  =  0.0;
	
}

void MoveRobot::publish_vel()
{

	if(forward_till_infinity == 1){  //for moving forward endlessly.
		switch(state) {
			case STATE_STOP:
				if(point_is_visited){
					stop();
				}else{
					ROS_INFO("Robot stopped, going forward.");
					state = STATE_FORWARD;
					forward();
				}
				break;
			case STATE_FORWARD:
				if(bump_happened) {
					ROS_INFO("Bump detected");
					state = STATE_REVERSE;
					reverse();
					ros_time = ros::Time::now();
				}
				break;
			case STATE_REVERSE:
				if(ros::Time::now() > ros_time + ros::Duration(1.0)) {
					ROS_INFO("Turning Right.");
					state = STATE_RIGHT_TURN;
					turn_right();
					ros_time = ros::Time::now();
				}
				break;
			case STATE_RIGHT_TURN:
				if(ros::Time::now() > ros_time + ros::Duration(0.5)){
					ROS_INFO("Going forward again.");
					state = STATE_FORWARD;
					forward();
				}
				break;	
		}
	
	}
	
	else{  //for moving a fixed distance.
	
		switch(state)  
		{
		  	case STATE_STOP:
		  		if(position_reached()){
		  			stop();
		  		}else if(state == -1){       //no need of state == -1 here as already put up there.
					ROS_INFO("Robot stopped, going forward.");
					state = STATE_FORWARD;
				}else{
					state = state_original;
				}
				break;
			case STATE_FORWARD:
				if(bump_happened) {
					ROS_INFO("Bump detected");
					state = STATE_REVERSE;
					reverse();
					ros_time = ros::Time::now();
				}else if(state_original == state){
					ROS_INFO("moving forward");
					forward();
				}
				break;
			case STATE_REVERSE:
				if(ros::Time::now() > ros_time + ros::Duration(0.5)) {
					ROS_INFO("Turning Right.");
					state = STATE_RIGHT_TURN;
					turn_right();
					ros_time = ros::Time::now();
				}else if(state_original == state){
					reverse();
				}
				break;
			case STATE_RIGHT_TURN:
				if(ros::Time::now() > ros_time + ros::Duration(0.7)){
					ROS_INFO("Checking for bump again:");
					//if(forward_till_infinity  ==  1){
					state = state_original;
					//forward();
				}else if(state_original == state){
					turn_right();
				}
				
				break;	
			default:
				ROS_INFO("State undefined.");
				state = STATE_STOP;
		}
	}
	/*if(bump_happened){
		
		//the bot will move back and then a bit right and then move
		ros_time = ros::Time::now();
		ROS_INFO("moving back!");
		while(ros::Time::now() < ros_time + ros::Duration(0.5)) {
			reverse();		
		}
		ROS_INFO("now turning right");
		ros_time = ros::Time::now();
		while(ros::Time::now() < ros_time + ros::Duration(0.5)) {
			turn_right();
		}
		ROS_INFO("moving again");		
		
	}else{
		switch(state)  //MORE STATES TO ADD
		{
		  	case STATE_STOP:
		  		stop();
		  		break;
		  	case STATE_FORWARD_INFINITY:
		  		forward();
		  		break;
		  	case STATE_FORWARD:
		  		ROS_INFO("moving forward");
		  		forward();
		  		break;
		  	case STATE_REVERSE:
		  		reverse();
		  		break;
		  	case STATE_RIGHT_TURN:
		  		turn_right();
		  		break;
		  	default:
				ROS_INFO("State undefined.");
				state = STATE_STOP;
		  			
		}
	}*/
	
	//ROS_INFO("publishing vel");
	vel_pub.publish(vel);

}

/*void MoveRobot::brain_to_move(const std_msgs::String::ConstPtr& msg)
{
  std::string sms = msg->data.c_str();	
  ROS_INFO("I heard: [%s]", msg->data.c_str());
  int space_pos = sms.find(" ");
  std::string action = sms.substr(0,space_pos);
  DistanceToMove = sms.substr(space_pos+1,sms.length());
  int DistMove = std::atoi(DistanceToMove.c_str());
  ROS_INFO("DistToMove is %s and action is %s.",DistanceToMove.c_str(),action.c_str());	
  //now we'll control the robot from here
  
  //geometry_msgs/Twist vel;
  if(action.compare("forward") == 0){
  	//We can set the velocities here itself or make function call to the clss we've made
  	//while()
	forward();
  	ROS_INFO("moving forward");
  	//r.forward();
  }else if(action.compare("reverse") == 0){
  	ROS_INFO("moving backward");
  }else{
  }
}
*/

void MoveRobot::brain_move(const yo_description::brain::ConstPtr& msg)
{
  //std::string sms = msg->data.c_str();

  ROS_INFO("I heard: %f, %d", msg->dist,msg->direction);
  DistanceToMove =  (floor(msg->dist * 10.0))/10.0;
  ROS_INFO("DistanceToMove %f",DistanceToMove);
  //now we'll control the robot from here
  state = msg->direction; 
  state_original = state; //we need this to retain the original state
  if(state == -1){
	  forward_till_infinity = 1;
	  state = STATE_STOP;
  }else{
	  forward_till_infinity = 0;
  }
  //geometry_msgs/Twist vel;
/*  switch(state)  //MORE STATES TO ADD
  {
  	case STATE_STOP:
  		stop();
		ROS_INFO("Robot stopped, going forward.");
		state = STATE_FORWARD;
		forward();
		break;
	case STATE_FORWARD:
		if(bump_happened) {
		ROS_INFO("Bump detected");
		state = STATE_REVERSE;
		reverse();
		ros_time = ros::Time::now();
		}
		break;
	case STATE_REVERSE:
		if(ros::Time::now() > ros_time + ros::Duration(1.0)) {
		ROS_INFO("Turning Right.");
		state = STATE_RIGHT_TURN;
		turn_right();
		ros_time = ros::Time::now();
		}
		break;
	case STATE_RIGHT_TURN:
		if(ros::Time::now() > ros_time + ros::Duration(0.5)){
		ROS_INFO("Going forward again.");
		state = STATE_FORWARD;
		forward();
		}
		break;	
	default:
		ROS_INFO("State undefined.");
  }
 */ 

}

void MoveRobot::dist_travelled(const yo_description::relpos::ConstPtr& msg)
{
  //ROS_INFO("I heard: %f, %f", msg->dist_moved,msg->angle_moved);
  //ROS_INFO("entered dist_travelled");
  /*
  std::string sms_from_RelPos = msg->data.c_str();	
  int space_pos = sms_from_RelPos.find(" ");
  DistanceMoved = sms_from_RelPos.substr(0,space_pos);
  AngleRotated  = sms_from_RelPos.substr(space_pos+1,sms_from_RelPos.length());
  */
  DistanceMoved = (floor(msg->dist_moved * 10.))/10.;
  AngleRotated  = (floor(msg->angle_moved * 10.))/10.;
  ROS_INFO("dist moved is : %f and angle rotated is %f",DistanceMoved,AngleRotated);
}

void MoveRobot::publish_message_to_brain()
{
	/*std::stringstream ss;
    //enter the message for the brain
   	ss << "done";
    message.data = ss.str();
    */
    //task_done = true;
	move_pub.publish(true);
	DistanceToMove = -1.0;
}

void MoveRobot::subscribe_dist()
{
	//ROS_INFO("Entering subscribe_disr to go to dist_travelled");
	    	
}

void MoveRobot::bumper(const std_msgs::Bool::ConstPtr& msg)
{
	
	if(msg->data){
	//now bump has happened: here we can move back and rotate right
	
	
	}else{
	//now bump hasn't happened : nothing
	}
	
	bump_happened = msg->data;
}

bool MoveRobot::DistCompared()
{
	
	if(fabs(DistanceMoved - DistanceToMove) < 0.001 && forward_till_infinity == 0){
		state = STATE_STOP;
		return true;
	}
	//return (fabs(DistanceMoved - DistanceToMove) < 0.001);
	return false;
}

void MoveRobot::point_visited(const std_msgs::Bool::ConstPtr& msg)
{
	
	//ROS_INFO(msg->data);
	point_is_visited = msg->data;
	if(msg->data){
		ROS_INFO("The point has been visited ");
		state = STATE_STOP;
	}
	
}

bool MoveRobot::position_reached()
{
	return (DistanceToMove >= 0.0); 
}
//////////////////////////////////////////////////------MAIN-------////////////////////////////////////////

int main(int argc, char **argv)
{
	ros::init(argc,argv,"move");
	MoveRobot r1;
	
	ros::Rate loop_rate(10);
	
	while(ros::ok())
	{	
		//r1.subscribe_dist();

		//if(r1.position_reached()){
			//ROS_INFO("Entering function publish_vel()");
		r1.publish_vel(); 
		//r1.publish_vel_border();
		//}
		 
		//now we have to include if r1.comapre(DistMoved == DistTravelled) then publish
		if(r1.DistCompared()){  //if state = -1 means we've to move forward till infinity
			r1.publish_message_to_brain();
		}
		//ROS_INFO("spinning");	
		ros::spinOnce();
		loop_rate.sleep(); 

	}
	return 0;
}
