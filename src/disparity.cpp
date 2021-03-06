#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
#include <ros/callback_queue.h>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
//using namespace cv;
char filename[50];


bool done=false;

static const int MY_ROS_QUEUE_SIZE = 100;
void disparity_node_callback(const sensor_msgs::Image::ConstPtr& msg)
{
// The message's data is a raw buffer. While the type is uint8_t*, the
// data itself is an array of floats (for depth data), the value of
// the float being the distance in meters.

cv_bridge::CvImageConstPtr cv_ptr;
cv_ptr = cv_bridge::toCvShare(msg);
// imshow expects a float value to lie in [0,1], so we need to normalize
// for visualization purposes.
double max,min;
cv::minMaxIdx(cv_ptr->image, &min, &max);
cv::Mat normalized,op,falsecolor;
//cv_ptr->image.convertTo(normalized, CV_32F, 1.0/max, 0) ;
//cv_ptr->image.convertTo(op, CV_8U, 255/max, 0) ;
cv_ptr->image.convertTo(op, CV_8UC1, 255/(max-min), -min);

applyColorMap(op,falsecolor,cv::COLORMAP_AUTUMN);

cv::imwrite(filename,falsecolor);
cout<<"Been there done that"<<endl;
done=true;
}

int main(int argc, char **argv)
{
strcpy(filename,argv[1]);


ros::init(argc, argv, "disparity_node");

cout << "Ready to save image" << endl;
ros::NodeHandle nh;

ros::Subscriber sub = nh.subscribe("camera/depth/image_raw",MY_ROS_QUEUE_SIZE, disparity_node_callback);


while(!done && ros::ok()) ros::spinOnce();
cout <<filename<<" Saved" << endl;
return 0;
}
