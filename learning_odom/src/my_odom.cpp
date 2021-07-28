#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Odometry.h"
#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/PointStamped.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
using namespace std;

	double x,y,th;
	double mx,my,mth;
	double dt,dx,dy,dth;

void cb(const geometry_msgs::Twist::ConstPtr& msg){
	mx=msg->linear.x;
	my=msg->linear.y;
	mth=msg->angular.z;
	
	ROS_INFO("Call Back at( %3.3lf, %3.3lf)",x,y);
	
}

int main(int argc,char *argv[]){
	ros::init(argc,argv,"my_odom");
	
	cout<<"Check 1"<<endl;
	
	ros::NodeHandle n;
	ros::Publisher pub=n.advertise<nav_msgs::Odometry>("/turtle1/odometry",50);
	ros::Subscriber sub=n.subscribe<geometry_msgs::Twist>("/turtle1/cmd_vel",10,cb);
	
	ros::Time current_time, last_time;
	tf::TransformBroadcaster odom_broadcaster;
	
	last_time=current_time=ros::Time::now();
	x=y=th=0;
	
	ros::Rate loop_rate(10);
	
	while(ros::ok()){
		ros::spinOnce();
		current_time = ros::Time::now();
		
		dt = ( current_time - last_time ).toSec();
		dx = ( mx * cos(th) - my * sin(th)) * dt;
		dy = ( mx * sin(th) + my * cos(th)) * dt;
		dth = mth * dt;
			
		x+=dx;
		y+=dy;
		th+=dth;
			
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
		geometry_msgs::TransformStamped odom_trans;
		odom_trans.header.stamp = current_time;
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id = "base_link";
				
		odom_trans.transform.translation.x = x;
		odom_trans.transform.translation.y = y;
		odom_trans.transform.translation.z = 0.0;
		odom_trans.transform.rotation = odom_quat;
				
		odom_broadcaster.sendTransform(odom_trans);
		nav_msgs::Odometry odom;
		odom.header.stamp = current_time;
		odom.header.frame_id = "odom";
				
		odom.pose.pose.position.x = x;
		odom.pose.pose.position.y = y;
		odom.pose.pose.position.z = 0.0;
		odom.pose.pose.orientation = odom_quat;
				
		odom.child_frame_id = "base_link";
		odom.twist.twist.linear.x = mx;
		odom.twist.twist.linear.y = my;
		odom.twist.twist.angular.z = mth;
				
		pub.publish(odom);
			  
		last_time= current_time;
		
	}

	return 0;

}
