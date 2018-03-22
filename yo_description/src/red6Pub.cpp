#include "ros/ros.h"
#include "gazebo_msgs/GetModelState.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/ModelState.h>
int main(int argc, char **argv)
{
	ros::init(argc,argv,"red6Pub");
	ros::NodeHandle n;
	ros::Publisher vel_pub = n.advertise<gazebo_msgs::ModelState>("/gazebo/get_model_state", 10);
	ros::Rate loop_rate(5);
	gazebo_msgs::ModelState get;
	

	vel_pub.publish(get);
    
        ros::spinOnce();
return 0;
}
