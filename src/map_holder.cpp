//this will now store the map and save it in the text file.
//if an object is dtected it'll increase the value of that cell by one ad this can go up till 15 max.
//similarly if it doesnt bump it'll decrement but cant gio below 0.
//so the creator will publish the matrix X and Y coordinates and true or false for bump_happen.

//also we are finding the matrix x,y in creator but if our matrix changes then the conversion also changes.
// the conversion is basically x = [POS_X + (size-1)/2]*10, acc. for y 
//so we can find the coordinates in holder only.


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
#include "nav_msgs/OccupancyGrid.h"
#include <fstream>
#include <yo_description/creator.h>

using namespace std;
int x,y;
bool bump_happened;
int matrix[100][100] ;
ros::Time visit_matrix[100][100];
nav_msgs::OccupancyGrid map_occupancy;

void update_matrix(const yo_description::creator::ConstPtr& msg)
{
	x = msg->x;
	y = msg->y;
	bump_happened = msg->bump;

	ROS_INFO("x is %d y is %d",x,y);

	if(bump_happened){
		ROS_INFO("Bump Detected");
			if (matrix[x][y] < 15){
				matrix[x][y]++;  //max value can't be more than 15 	
			}
	}else if(!bump_happened && matrix[x][y] <= 1){
		//map[x][y] = 1;
		//remains zero only 
	}else{
		//no bump implies we can reduce the value of that coordinate
		matrix[x][y]--;
	}

	visit_matrix[x][y] = ros::Time::now();

	std::ofstream MAP,map_visit;

	//std::ofstream MAP("matrix_room.txt");
	MAP.open ("/home/raghavv/catkin_ws/src/yo_description/src/matrix.txt");
	for (int i=0;i<90;i++)
	{
		for (int j=0;j<90;j++)
		{
			MAP << matrix[i][j]; // behaves like cout - cout is also a stream
		}
		MAP << "\n";
	}
	
	map_visit.open ("/home/raghavv/catkin_ws/src/yo_description/src/visit.txt");
	for (int i=0;i<90;i++)
	{
		for (int j=0;j<90;j++)
		{
			map_visit << visit_matrix[i][j]; // behaves like cout - cout is also a stream
		}
		map_visit << "\n";
	}

	map_visit.close();
	MAP.close();
}

/*void visit_matrix(const std_msgs::Bool::ConstPtr& msg)
{
	
	
	//if()
}
*/

int main(int argc, char **argv)
{
	ros::init(argc,argv,"holder");
	ros::NodeHandle n_pos;
	
	ros::Subscriber holder_sub = n_pos.subscribe("creator_to_holder", 10, update_matrix);
	//ros::Subscriber visit_sub = n_pos.subscribe("visited_nodes", 10, visit_matrix);
	
	//publisher for occupancy grid:
	ros::Publisher occupancy_pub = n_pos.advertise<nav_msgs::OccupancyGrid>("Occupancy_Grid",1000);
		
	ros::Rate loop_rate(10);
	
	while(ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
