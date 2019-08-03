#include <ProjectMap.h>


namespace ProjectMap
{
    Map::Map (ros::NodeHandle &node_handle)
    {
        node_handle_ = node_handle;
        name_of_node_ = ros::this_node::getName();

        //static parameters
        node_handle_.param<std::string>("/all_kfs_pts_topic", all_kfs_pts_topic_param_, "/orb_slam2_mono/all_kfs_pts");
        node_handle_.param<std::string>("/single_kf_pts_topic", single_kf_pts_topic_param_, "/orb_slam2_mono/single_kf_pts");

        // subscriber
        single_kf_pts_subscriber_ = node_handle_.subscribe(single_kf_pts_topic_param_, 1, &Map::SingleCallback, this);
        all_kfs_pts_subscriber_ = node_handle_.subscribe(all_kfs_pts_topic_param_, 1, &Map::AllCallback, this);
        // publisher
        grid_map_publisher_ = node_handle_.advertise<nav_msgs::OccupancyGrid> (name_of_node_+"/grid_map", 1);

        SetParameter ();
        CreateCvMat (h_, w_);
    }

    void Map::SetParameter ()
    {
        for (auto i=0; i<4; i++)
            grid_lim_[i] = cloud_lim_[i] * scale_fac_;

        h_ = grid_lim_[1] - grid_lim_[0];
        w_ = grid_lim_[3] - grid_lim_[2];

        norm_fac_[0] = float(h_ - 1) / float(h_);
        norm_fac_[1] = float(w_ - 1) / float(w_);
    }

    void Map::CreateCvMat (const unsigned int h, const unsigned int w)
    {
        global_occupied_counter_.create(h, w, CV_32SC1);
        global_visit_counter_.create(h, w, CV_32SC1);
        global_occupied_counter_.setTo(cv::Scalar(0));
        global_visit_counter_.setTo(cv::Scalar(0));

        grid_map_msg_.data.resize(h*w);
        grid_map_msg_.info.width = w;
        grid_map_msg_.info.height = h;
        grid_map_msg_.info.resolution = 1.0/scale_fac_;
        grid_map_int_ = cv::Mat(h, w, CV_8SC1, (char*)(grid_map_msg_.data.data()));

        grid_map_.create(h, w, CV_32FC1);
        grid_map_thresh_.create(h, w, CV_8UC1);
        grid_map_thresh_resized_.create(h * resize_fac_, w * resize_fac_, CV_8UC1);

        local_occupied_counter_.create(h, w, CV_32SC1);
        local_visit_counter_.create(h, w, CV_32SC1);
        local_map_pt_mask_.create(h, w, CV_8UC1);
    }

    void Map::SingleCallback (const geometry_msgs::PoseArray &msg)
    {

    }

    void Map::AllCallback (const geometry_msgs::PoseArray &msg)
    {

    }

}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "GridMap");
    ros::start();

    if(argc > 1) {
        ROS_WARN ("Arguments supplied via command line are neglected.");
    }

    ros::NodeHandle node_handle;
    ProjectMap::Map map (node_handle);

    ros::spin();

    ros::shutdown();

    return 0;
}