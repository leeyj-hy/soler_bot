#include "ros/ros.h"
#include "soler_msgs/dxl_enabler.h"
#include "soler_msgs/track.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Point.h"

using namespace std;
using namespace ros;

class dxl_node
{
    private:
        ros::NodeHandle n;
        ros::ServiceServer dxl_srv;
        ros::Publisher dxl_pub;
        ros::Subscriber dxl_sub;
        ros::Publisher bbox_pub;

        soler_msgs::dxl_enabler DXL_ENA;
        soler_msgs::track TRACK;
        geometry_msgs::Point pnt_obj;

        bool is_dxl_enabled = 0;
        
        int  bbox_x = 0;
        int  bbox_y = 0;

        
    public:
        dxl_node()
        {
            // dxl_srv = n.advertiseService("/soler_enable", &dxl_node::dxl_con_callback, this);
            dxl_srv = n.advertiseService("/sol_srv", &dxl_node::dxl_con_callback, this);
            dxl_sub = n.subscribe("/track_sub", 1, &dxl_node::track_callback, this);
            dxl_pub = n.advertise<geometry_msgs::Point>("/track_val", 1);
            
        }

        bool dxl_con_callback(soler_msgs::dxl_enabler::Request &req,
                            soler_msgs::dxl_enabler::Response &res)
            {
                if(req.enable)
                {
                    res.is_enabled = true;
                    is_dxl_enabled = true;
                    ROS_WARN("dxl_enabled");
                }
                else
                {
                    res.is_enabled = false;
                    is_dxl_enabled = false;
                    ROS_WARN("dxl_disabled");
                }
                
                return true;
            }

        void track_callback(const soler_msgs::track &msg)
            {
                this -> bbox_x = msg.position[1];
                this -> bbox_y = msg.position[2];

                pnt_obj.x = this -> bbox_x;
                pnt_obj.y = this -> bbox_y;

                ROS_INFO("sub x: %d, y: %d", pnt_obj.x, pnt_obj.y);


                if(this->is_dxl_enabled == true)
                {
                    ROS_INFO("pub x: %d, y: %d", pnt_obj.x, pnt_obj.y);
                    dxl_pub.publish(pnt_obj);
                }

                else if(this -> is_dxl_enabled == false)
                {
                    ROS_ERROR("ERROR OCCURED DURING PUBLISHING BBOX SHUTTING DONW");
                    shutdown();
                }
            }

        


};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "dxl_con_node");
    dxl_node DXL_obj;

    ros::spin();
    return 0;
}

