#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/TransformStamped.h"

std::string turtle_name;


void doPose(const turtlesim::Pose::ConstPtr& pose){
	
    static tf2_ros::TransformBroadcaster broadcaster;
    
    geometry_msgs::TransformStamped tfs;
    tfs.header.frame_id = "world";
    tfs.header.stamp = ros::Time::now();
    tfs.child_frame_id = "turtle1";
    tfs.transform.translation.x = pose->x;
    tfs.transform.translation.y = pose->y;
    tfs.transform.translation.z = 0.0;
    tf2::Quaternion qtn;
    qtn.setRPY(0,0,pose->theta);
    tfs.transform.rotation.x = qtn.getX();
    tfs.transform.rotation.y = qtn.getY();
    tfs.transform.rotation.z = qtn.getZ();
    tfs.transform.rotation.w = qtn.getW();
    
    broadcaster.sendTransform(tfs);

} 

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    
    ros::init(argc,argv,"pub5");

    ros::NodeHandle nh;
    
    ros::Subscriber sub = nh.subscribe<turtlesim::Pose>("turtle1/pose",1000,doPose);
    
    ros::spin();
    return 0;
}
