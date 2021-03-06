#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    std::string in_topic;
    std::string out_topic;
    nh_.getParam("/to_rgb8/in_topic", in_topic); 
    nh_.getParam("/to_rgb8/out_topic", out_topic); 
    ROS_INFO_STREAM("IMAGE CONVERTER \n input topic: " << in_topic << " output topic: " << out_topic);

    image_sub_ = it_.subscribe(in_topic, 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise(out_topic, 1);
  }


  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      // convert to rgb8
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "to_rgb");
  ImageConverter ic;
  ros::spin();

  return 0;
}
