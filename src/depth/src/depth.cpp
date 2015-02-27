#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
//#include <ifs.h>

using namespace std;
//using namespace cv;
char filename[100];


static const int MY_ROS_QUEUE_SIZE = 100;
void depth_node_callback(const sensor_msgs::Image::ConstPtr& msg)
{
// The message's data is a raw buffer. While the type is uint8_t*, the
// data itself is an array of floats (for depth data), the value of
// the float being the distance in meters.
cout << "Top-left corner: " << *reinterpret_cast<const float*>(&msg->data[0]) << "m" << endl;
try {
cv_bridge::CvImageConstPtr cv_ptr;
cv_ptr = cv_bridge::toCvShare(msg);
// imshow expects a float value to lie in [0,1], so we need to normalize
// for visualization purposes.
double max = 0.0;
cv::minMaxLoc(cv_ptr->image, 0, &max, 0, 0);
cv::Mat normalized,op;
cv_ptr->image.convertTo(normalized, CV_32F, 1.0/max, 0) ;
cv_ptr->image.convertTo(op, CV_8U, 255/max, 0) ;

cv::imshow("depth_node", normalized);
cv::imwrite(filename,op);
cv::waitKey(1);
} catch (const cv_bridge::Exception& e) {
ROS_ERROR("cv_bridge exception: %s", e.what());
}
}

int main(int argc, char **argv)
{

strcpy(filename,argv[1]);


ros::init(argc, argv, "depth_node");

cout << "Ready to stream" << endl;
ros::NodeHandle nh;
ros::Subscriber sub = nh.subscribe("camera/depth/image_raw",MY_ROS_QUEUE_SIZE, depth_node_callback);

cv::namedWindow("depth_node");
ros::spin();
cv::destroyWindow("depth_node");
cout << "Stream Ended" << endl;
return 0;
}
