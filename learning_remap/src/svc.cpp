#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "turtlesim/Spawn.h"

int main(int argc,char *argv[]){
	setlocale(LC_ALL,"");
	
    ros::init(argc,argv,"create_turtle");
    
    ros::NodeHandle h;
    
    ros::ServiceClient client = h.serviceClient<turtlesim::Spawn>("/spawn");

    ros::service::waitForService("/spawn");
    
    turtlesim::Spawn spawn;
    
    spawn.request.name = "turtle2";
    spawn.request.x = 3.0;
    spawn.request.y = 3.0;
    spawn.request.theta = 0;
    bool flag = client.call(spawn);
    if (flag)
    {
        ROS_INFO("Succeeded creating %s",spawn.response.name.c_str());
    }
    else
    {
        ROS_INFO("Failed creating %s",spawn.response.name.c_str());
    }

	return 0;

}
