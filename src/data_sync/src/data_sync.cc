#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Image.h>
#include <message_filters/subscriber.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <iostream>

ros::Publisher pointcloud_pub;
ros::Publisher image_pub;
typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::PointCloud2,sensor_msgs::Image> MySyncPolicy;

void callback(const sensor_msgs::PointCloud2ConstPtr& pointcloud,const sensor_msgs::ImageConstPtr& image)
{
    pointcloud_pub.publish(pointcloud);
    image_pub.publish(image);
    ROS_INFO("sync succeed.");
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"data_sync_node");
    ros::NodeHandle nh;

    pointcloud_pub = nh.advertise<sensor_msgs::PointCloud2>("sync/pointcloud",1);
    image_pub = nh.advertise<sensor_msgs::Image>("sync/image",1);

    message_filters::Subscriber<sensor_msgs::PointCloud2> pointcloud_sub(nh,"/rslidar_points",1);
    message_filters::Subscriber<sensor_msgs::Image> image_sub(nh,"/zed/zed_node/left/image_rect_color",1);

    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10),pointcloud_sub,image_sub);
    sync.registerCallback(boost::bind(&callback,_1,_2));

    ros::spin();

    return 0;    
}