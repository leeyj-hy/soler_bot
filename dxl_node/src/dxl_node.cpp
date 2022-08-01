#include "ros/ros.h"
#include "soler_msgs/dxl_enabler.h"
#include "soler_msgs/track.h"
#include "std_msgs/Float64.h"



using namespace std;
using namespace ros;

class dxl_node
{
    private:
        ros::NodeHandle n;
        ros::ServiceServer dxl_srv;
        ros::Publisher dxl_pub;
        ros::Subscriber dxl_sub;
        
 
    public:
        bool enabler = 0;
        std_msgs::Float64 bbox_data[2];
        soler_msgs::track track_obj;

        dxl_node()
        {
            dxl_srv = n.advertiseService("/sol_srv", &dxl_node::dxl_con_callback, this);
            dxl_sub = n.subscribe("/tracker", 1, &dxl_node::track_callback, this);
            dxl_pub = n.advertise<soler_msgs::track>("/track_val", 1);
        }

        bool dxl_con_callback(soler_msgs::dxl_enabler::Request &req, soler_msgs::dxl_enabler::Response &res);

        void track_callback(const soler_msgs::track &msg);

};



int main(int argc, char **argv)
{
    ros::init(argc, argv, "dxl_con_node");
    dxl_node DXL_obj;
    
    ros::spin();
    return 0;
}


inline bool dxl_node::dxl_con_callback(soler_msgs::dxl_enabler::Request &req, soler_msgs::dxl_enabler::Response &res)
{
    if(req.enable)
    {
        res.is_enabled=true;
        dxl_node::enabler = true;
        ROS_WARN("dxl_enabled");
    }
    else
    {
        res.is_enabled=false;
        dxl_node::enabler = false;
        ROS_WARN("dxl_disabled");
    }

    return true;
}

inline void dxl_node::track_callback(const soler_msgs::track &msg)
{
    if(dxl_node::enabler)
    {
        float x = msg.position[0];
        float y = msg.position[1];

        ROS_INFO("%f %f", x,y);
        
    }

    else
    {
        ROS_WARN("dxl is not enabled");
    }
}