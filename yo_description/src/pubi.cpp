#include "ros/ros.h"
#include "geometry_msgs/Twist.h"


int main(int argc, char **argv)
{
	ros::init(argc,argv,"pubi");
	ros::NodeHandle n;
	ros::Publisher vel = n.advertise<geometry_msgs::Twist>("chat", 10);
	ros::Rate loop_rate(5);

	int count = 0;

  while (ros::ok())
  {
    geometry_msgs::Twist vel_pub;

    vel_pub.linear.x = 0.5;
    vel_pub.linear.y = 0.0;
    vel_pub.linear.z = 0.0;
    vel_pub.angular.x = 0.0;
    vel_pub.angular.y = 0.0;
    vel_pub.angular.z = 0.5;


    vel.publish(vel_pub);
    
    ros::spinOnce();

    loop_rate.sleep();

    ++count;
  }

  return 0;
}

