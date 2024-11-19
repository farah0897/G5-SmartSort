#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <arduinobot_msgs/srv/euler_to_quaternion.hpp>
#include <arduinobot_msgs/srv/quaternion_to_euler.hpp>
#include <tf2/utils.h>

class AngleConversion : public rclcpp::Node
{
public:
    AngleConversion() : Node("angle_conversion") 
    {
        euler_to_quaternion_ = create_service<arduinobot_msgs::srv::EulerToQuaternion>("euler_to_quaternion", std::bind(&AngleConversion::eulerToQuaternionCallback, this, std::placeholders::_1, std::placeholders::_2));
        quaternion_to_euler_ = create_service<arduinobot_msgs::srv::QuaternionToEuler>("quaternion_to_euler", std::bind(&AngleConversion::quaternionToEulerCallback, this, std::placeholders::_1, std::placeholders::_2));
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Angle conversion services are Ready");
    }


private:
    rclcpp::Service<arduinobot_msgs::srv::EulerToQuaternion>::SharedPtr euler_to_quaternion_;
    rclcpp::Service<arduinobot_msgs::srv::QuaternionToEuler>::SharedPtr quaternion_to_euler_;

    void eulerToQuaternionCallback(const std::shared_ptr<arduinobot_msgs::srv::EulerToQuaternion::Request> request, 
                                    const std::shared_ptr<arduinobot_msgs::srv::EulerToQuaternion::Response> response)
    {

        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Incoming request to convert Euler to Quaternion Received: roll: " << request->roll << " pitch: " << request->pitch << " yaw: " << request->yaw << "into a quaternion");
        tf2::Quaternion q;
        q.setRPY(request->roll, request->pitch, request->yaw);
        response->x = q.getX();
        response->y = q.getY();
        response->z = q.getZ();
        response->w = q.getW();
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Quaternion Response: x: " << response->x << " y: " << response->y << " z: " << response->z << " w: " << response->w);
    }

    void quaternionToEulerCallback(const std::shared_ptr<arduinobot_msgs::srv::QuaternionToEuler::Request> request, 
                                  const std::shared_ptr<arduinobot_msgs::srv::QuaternionToEuler::Response> response)
    {
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Incoming request to convert Quaternion to Euler Received: x: " << request->x << " y: " << request->y << " z: " << request->z << " w: " << request->w << "into a euler");
        tf2::Quaternion q(request->x, request->y, request->z, request->w);
        tf2::Matrix3x3 m(q);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);
        response->roll = roll;
        response->pitch = pitch;
        response->yaw = yaw;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Euler Response: roll: " << response->roll << " pitch: " << response->pitch << " yaw: " << response->yaw);
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AngleConversion>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
