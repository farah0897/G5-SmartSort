import pyrealsense2 as rs
import numpy as np
import cv2

# Configure the depth and color streams
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

# Start the pipeline
pipeline.start(config)

try:
    while True:
        # Wait for a coherent pair of frames: depth and color
        frames = pipeline.wait_for_frames()
        depth_frame = frames.get_depth_frame()
        color_frame = frames.get_color_frame()

        if not depth_frame or not color_frame:
            continue

        # Convert images to numpy arrays
        depth_image = np.asanyarray(depth_frame.get_data())
        color_image = np.asanyarray(color_frame.get_data())

        # Normalize depth image for visualization and convert to 8-bit
        # Increase the alpha factor to make heatmap more sensitive (closer depth = more visible color)
        depth_colormap = cv2.applyColorMap(
            cv2.convertScaleAbs(depth_image, alpha=0.1), cv2.COLORMAP_JET  # Increased alpha for sensitivity
        )

        # Resize depth colormap to match the size of the color image if needed
        depth_colormap = cv2.resize(depth_colormap, (640, 480))

        # Apply a color filter for red, green, and blue objects
        hsv_image = cv2.cvtColor(color_image, cv2.COLOR_BGR2HSV)

        # Define color ranges and mask for each color
        color_ranges = {
            "red": [(0, 100, 100), (10, 255, 255)],
            "green": [(70, 255, 51), (80, 255, 150)],  # Widened green range
            "blue": [(100, 255, 80), (125, 255, 255)]  # Widened blue range
        }

        for color, (lower, upper) in color_ranges.items():
            mask = cv2.inRange(hsv_image, np.array(lower), np.array(upper))
            result = cv2.bitwise_and(color_image, color_image, mask=mask)

            # Find contours of the masked areas
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            for contour in contours:
                # Filter by size to avoid small artifacts
                if cv2.contourArea(contour) > 500:
                    # Calculate the center of the contour
                    M = cv2.moments(contour)
                    if M['m00'] != 0:
                        cx = int(M['m10'] / M['m00'])
                        cy = int(M['m01'] / M['m00'])

                        # Get the depth at the center of the contour
                        depth = depth_frame.get_distance(cx, cy)

                        # Check if the depth value is valid (non-zero and within a valid range)
                        if depth > 0 and depth < 10:  # Assuming 10 meters is a reasonable range for the camera
                            depth_point = rs.rs2_deproject_pixel_to_point(
                                depth_frame.profile.as_video_stream_profile().intrinsics,
                                [cx, cy],
                                depth
                            )

                            # Multiply the 3D coordinates by 10
                            depth_point = tuple(coord * 10 for coord in depth_point)

                            # Draw the contour and center point
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

                            print(f"{color} object detected at 3D coordinates: {depth_point}")
                        else:
                            print(f"Invalid depth value at ({cx}, {cy}): {depth} meters")

        # Stack both the color image and depth heatmap horizontally
        combined_image = np.hstack((color_image, depth_colormap))

        # Display the combined image
        cv2.imshow('Color and Depth Image', combined_image)

        # Break the loop on 'q' key press
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    # Stop the pipeline
    pipeline.stop()
    cv2.destroyAllWindows()

