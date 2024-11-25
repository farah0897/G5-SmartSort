#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import pyrealsense2 as rs
import numpy as np
import cv2

class RealSenseNode(Node):
    def __init__(self):
        super().__init__('realsense_node')
        
        # Configure the depth and color streams
        self.pipeline = rs.pipeline()
        config = rs.config()
        config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

        # Start the pipeline
        self.pipeline.start(config)
        
        # Create a timer for camera callback
        self.create_timer(0.033, self.camera_callback)  # 30 Hz
        
        self.get_logger().info('RealSense node started')

    def camera_callback(self):
        try:
            # Wait for a coherent pair of frames: depth and color
            frames = self.pipeline.wait_for_frames()
            depth_frame = frames.get_depth_frame()
            color_frame = frames.get_color_frame()

            if not depth_frame or not color_frame:
                return

            # Convert images to numpy arrays
            depth_image = np.asanyarray(depth_frame.get_data())
            color_image = np.asanyarray(color_frame.get_data())

            # Process color image for object detection
            hsv_image = cv2.cvtColor(color_image, cv2.COLOR_BGR2HSV)

            color_ranges = {
                "red": [(0, 100, 100), (10, 255, 255)],
                "green": [(70, 255, 51), (80, 255, 150)],
                "blue": [(100, 255, 80), (125, 255, 255)]
            }

            for color, (lower, upper) in color_ranges.items():
                mask = cv2.inRange(hsv_image, np.array(lower), np.array(upper))
                contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                for contour in contours:
                    if cv2.contourArea(contour) > 500:
                        M = cv2.moments(contour)
                        if M['m00'] != 0:
                            cx = int(M['m10'] / M['m00'])
                            cy = int(M['m01'] / M['m00'])
                            depth = depth_frame.get_distance(cx, cy)

                            if depth > 0 and depth < 10:
                                depth_point = rs.rs2_deproject_pixel_to_point(
                                    depth_frame.profile.as_video_stream_profile().intrinsics,
                                    [cx, cy],
                                    depth
                                )
                                depth_point = tuple(coord * 10 for coord in depth_point)
                                
                                # Draw on image
                                cv2.drawContours(color_image, [contour], -1, (0, 255, 0), 2)
                                cv2.circle(color_image, (cx, cy), 5, (255, 255, 255), -1)
                                cv2.putText(
                                    color_image,
                                    f"{color} ({depth_point[0]:.2f}, {depth_point[1]:.2f}, {depth_point[2]:.2f})",
                                    (cx, cy - 10),
                                    cv2.FONT_HERSHEY_SIMPLEX,
                                    0.5,
                                    (255, 255, 255),
                                    2
                                )

                                self.get_logger().info(f"{color} object detected at: {depth_point}")

            # Display the combined image
            depth_colormap = cv2.applyColorMap(
                cv2.convertScaleAbs(depth_image, alpha=0.1),
                cv2.COLORMAP_JET
            )
            combined_image = np.hstack((color_image, depth_colormap))
            cv2.imshow('Color and Depth Image', combined_image)
            cv2.waitKey(1)

        except Exception as e:
            self.get_logger().error(f'Error in camera callback: {str(e)}')

    def __del__(self):
        self.pipeline.stop()
        cv2.destroyAllWindows()

def main(args=None):
    rclpy.init(args=args)
    node = RealSenseNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main() 