#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
#include <ros/callback_queue.h>

//#include <ifs.h>

using namespace std;
//using namespace cv;
char filename[50];
int imgtype;

bool done=false;

static const int MY_ROS_QUEUE_SIZE = 100;
void depth_node_callback(const sensor_msgs::Image::ConstPtr& msg)
{
// The message's data is a raw buffer. While the type is uint8_t*, the
// data itself is an array of floats (for depth data), the value of
// the float being the distance in meters.

cv_bridge::CvImageConstPtr cv_ptr;
cv_ptr = cv_bridge::toCvShare(msg);
// imshow expects a float value to lie in [0,1], so we need to normalize
// for visualization purposes.
double max = 0.0;
cv::minMaxLoc(cv_ptr->image, 0, &max, 0, 0);
cv::Mat normalized,op;
cv_ptr->image.convertTo(normalized, CV_32F, 1.0/max, 0) ;
cv_ptr->image.convertTo(op, CV_8U, 255/max, 0) ;

cv::imwrite(filename,op);
cout<<"Been there done that"<<endl;
done=true;
}

int main(int argc, char **argv)
{
string type;
strcpy(filename,argv[1]);
imgtype=atoi(argv[2]);


ros::init(argc, argv, "depth_node");

cout << "Ready to save image" << endl;
ros::NodeHandle nh;

if(imgtype==1)
type="camera/rgb/image_color";

else if(imgtype==0)
type="camera/depth/image_raw";

ros::Subscriber sub = nh.subscribe(type,MY_ROS_QUEUE_SIZE, depth_node_callback);


while(!done) ros::spinOnce();
cout <<filename<<" Saved" << endl;
return 0;
}
