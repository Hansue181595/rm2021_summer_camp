#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"

int main(int argc,char *argv[]){
	setlocale(LC_ALL,"");
	
    ros::init(argc,argv,"sub2");
    
    ros::NodeHandle nh;
    
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);
    


    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle3/cmd_vel",1000);

    ros::Rate rate(10);
    while (ros::ok())
    {
        try
        {
            // first get tur1's coordinate relating to tur2
            geometry_msgs::TransformStamped tfs = buffer.lookupTransform("turtle3","pos3",ros::Time(0));

            //gen vel msg accoring to coordinate
            geometry_msgs::Twist twist;
            twist.linear.x = 0.5 * sqrt(pow(tfs.transform.translation.x,2) + pow(tfs.transform.translation.y,2));
            twist.angular.z = 4 * atan2(tfs.transform.translation.y,tfs.transform.translation.x);

            //pub vel msg  (need to create publish object first)
            pub.publish(twist);
        }
        catch(const std::exception& e)
        {
            // std::cerr << e.what() << '\n';
            ROS_INFO("ERR:%s",e.what());
        }



        rate.sleep();
        // 6.spin
        ros::spinOnce();
        
    }

	return 0;

}
