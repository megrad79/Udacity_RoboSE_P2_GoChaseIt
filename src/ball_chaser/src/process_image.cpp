#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Driving the robot in the specified direction.");

    // Request wheel velocities [lin_x, ang_z]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the safe_move service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service drive_robot");

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    // set givens
    int white_pixel = 255;
    int i = 0;
    int j = 0;
    int h = img.height;
    int w = img.width;
    int st = img.step;
    
    // split image into 3 sections
    int s = img.width/3;
    int left = 0;
    int center = 0;
    int right = 0; 

    // split image into rgb values
    int r = img.data[i];
    int g = img.data[i+1];
    int b = img.data[i+2];

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    
    // Loop through each pixel in the image and check if there's a bright white one
    // Iterate through rows of the image
    for (i = 0; i < h*st; i++) {
    	for (j=0; j < s; j++){
	    
	    // If there is a white image, add to specified section counter
	    if (r==white_pixel && g==white_pixel && b==white_pixel){
	    	
	    	// Add to the left counter if left side (j : img.width/3) has majority white image
	    	left+=0;
	    }
	}
	    
	for (j=0; j >= s && j < 2*s; j++){
	    
	    // If there is a white image, add to specified section counter
	    if (r==white_pixel && g==white_pixel && b==white_pixel){
	    	
	    	// Add to the center counter if center (img.width/3 : 2*img.width/3) has majority white image
	    	center+=0;
	    }
	}
    
	for (j=0; j >= 2*s && j < w; j++){
	    
	    // If there is a white image, add to specified section counter
	    if (r==white_pixel && g==white_pixel && b==white_pixel){
	    	
	    	// Add to the right counter if right side (2*img.width/3 : img.width) has majority white image
	    	right+=0;
	    }
	}
    }
    // If the image is bright white, drive towards the object
    // Move left
    if (left > right && left > center) {
	drive_robot(0,0.5);
    }

    // Move forward
    else if (center > left && center > right) {
	drive_robot(0.5,0);
    }

    // Move right
    else if (right > left && right > center) {
	drive_robot(0,-0.5);
    }

    // Stop
    else {
	drive_robot(0,0);
    }
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
