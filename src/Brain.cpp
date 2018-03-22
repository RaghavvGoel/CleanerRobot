#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <string>
#include "ros/ros.h"
#include "gazebo_msgs/ContactsState.h"
#include <iostream>
#include "yo_description/brain.h"
//#include "test.h"

std_msgs::Bool bump_happened;
bool start_traversal = false;
int state;
bool task_completed = false;
bool find_border = true;

void bump_event(const gazebo_msgs::ContactsState::ConstPtr& msg)
{
	if(msg->states.size() == 0) {
		bump_happened.data = false;
		//std_msgs::Bool bump = false;
		
	} else {
		bump_happened.data = true;
		//std_msgs::Bool bump = true;		
		ROS_INFO("Collision detected: %s with %s", msg->states[0].collision1_name.c_str(), msg->states[0].collision2_name.c_str());
		//here we'll send a boolean type message
		
	}
	
/*		
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

void move_to_brain(const std_msgs::Bool::ConstPtr& msg)
{
	if(msg->data){
		//now take input
		task_completed = msg->data; //that is = true
	}else{
		//dont take input
		task_completed = msg->data;
	}
	std::cout<<"task_completed = "<<task_completed;
}

void border(const std_msgs::Bool::ConstPtr& msg){
	if(msg->data){
		//now we can traverse the matrix
		start_traversal = true;
	}else{
		//Don't start
		start_traversal = false;
	}
}	

void matrix_traversed(const std_msgs::Bool::ConstPtr& msg){
	if(msg->data){
		std::cout<<"TRAVERSAL DONE";		
	}else{
		std::cout<<"TRAVERSAL NOT NOT DONE";		
	}
}

void take_input(float distance,int direction)
{
	std::cout<<"enter direction to move and by how much ";

}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"brain");
	ros::NodeHandle n;
	
	//subsriber for Bumper
	ros::Subscriber bumper_sub = n.subscribe("bumper_contact_sensor_state", 1000,bump_event);
	
	//subscribes from Move to see if task completed or not
	ros::Subscriber chatter_sub = n.subscribe("MoveToBrain", 10, move_to_brain);	
	
	//Publish a bumper message to Move
	ros::Publisher bumper_pub = n.advertise<std_msgs::Bool>("bumped", 1000);
	
	//publishes message for Move	
	//ros::Publisher chatter_pub = n.advertise<std_msgs::String>("BrainToMove", 1000);
	ros::Publisher chatter_pub = n.advertise<yo_description::brain>("brain_to_move", 1000);	
	
	//publishing a message to move after task is completed to find the border
	ros::Publisher find_border_pub = n.advertise<std_msgs::Bool>("finding_border",1000);
	ros::Subscriber border_found_sub = n.subscribe("border_found",10,border);

	//publishing for traversal of matrix
	ros::Publisher traversal_pub = n.advertise<std_msgs::Bool>("traverse_matrix",1000);
	ros::Subscriber traversal_complete_sub = n.subscribe("traversal_completed",10,matrix_traversed);

	ros::Rate loop_rate(10);
	ros::Time ros_time;
	//std_msgs::String message;
	yo_description::brain message;
	
	//bool message for border finding
	
	

	while(ros::ok()) {
    	if(task_completed || start_traversal){
    		//if entered only if task_completed is true
    		//making it fasle as soon as we enter the if
    		find_border = true;
    		task_completed = false;
			std::cout<<"enter direction to move and by how much or Start Traversal";
			int choice; // 1 for traversal and 2 for fixed direction and distance
			float distance;
			int direction;
			std::cin>>choice;
		
			
			//message.dist 	  = floor(distance * 10.)/10.;
			/*ss << direction <<" "<< distance;
			message.data = ss.str();
	 	    ROS_INFO("%s", message.data.c_str());
			*/
			if(choice == 1){
				std::cin>>distance>>direction;
				distance = floor(distance * 10.)/10.;
				message.direction = direction;
				message.dist 	  = distance;	

				ROS_INFO("distance: %f, direction : %d",message.dist,direction);
			}else{
				ROS_INFO("Traversing..........");
				start_traversal = true; 	
			}

			//chatter_pub.publish(message);
			traversal_pub.publish(start_traversal); //change 
		}
		//subscribes to message from Move
		//ros::Subscriber chatter_sub = n.subscribe("MoveToBrain", 10, move_to_brain);
		bumper_pub.publish(bump_happened);
		find_border_pub.publish(find_border);
		//traversal_pub.publish(start_traversal);

		ros::spinOnce();
		loop_rate.sleep();
	}
		return 0;		

}
