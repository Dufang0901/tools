#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


/* Globals */ 
static int uinp_fd = -1; 
struct uinput_user_dev uinp; // uInput device structure 
struct input_event event; // Input device structure 
/* Setup the uinput device */ 
int setup_uinput_device() 
{ 
   // Temporary variable 
   int i=0; 
   
   // Open the input device 
   uinp_fd = open("/dev/input/uinput", O_WRONLY | O_NDELAY); 
   if (uinp_fd <= 0) 
   { 
      printf("Unable to open /dev/uinput/n"); 
      return -1; 
   } 
   
   memset(&uinp,0,sizeof(uinp)); // Intialize the uInput device to NULL 
   strncpy(uinp.name, "PolyVision Touch Screen", UINPUT_MAX_NAME_SIZE); 
   uinp.id.version = 4; 
   uinp.id.bustype = BUS_USB; 
   uinp.absmin[ABS_X] = 0;
   uinp.absmax[ABS_X] = 65535;
   uinp.absmin[ABS_Y] = 0;
   uinp.absmax[ABS_Y] = 65535;
   uinp.absmin[ABS_PRESSURE] = 0;
   uinp.absmax[ABS_PRESSURE] = 0xFFF;
   
   // Setup the uinput device 
   //  ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
   //  ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
	 ioctl(uinp_fd, UI_SET_EVBIT, EV_ABS);
	 
	// ioctl(uinp_fd, UI_SET_KEYBIT, BTN_LEFT);
    // ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MIDDLE);
    // ioctl(uinp_fd, UI_SET_KEYBIT, BTN_RIGHT);
    // ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TOUCH);

   //  ioctl(uinp_fd, UI_SET_RELBIT, REL_X);
   //  ioctl(uinp_fd, UI_SET_RELBIT, REL_Y);
	   
     ioctl(uinp_fd, UI_SET_ABSBIT, ABS_X);
     ioctl(uinp_fd, UI_SET_ABSBIT, ABS_Y);
     ioctl(uinp_fd, UI_SET_ABSBIT, ABS_PRESSURE);


   /* Create input device into input sub-system */ 
   write(uinp_fd, &uinp, sizeof(uinp)); 
   if (ioctl(uinp_fd, UI_DEV_CREATE)) 
   { 
      printf("Unable to create UINPUT device."); 
      return -1; 
   } 
   return 1; 
} 


/* This function will open the uInput device. Please make 
sure that you have inserted the uinput.ko into kernel. */ 
int main() 
{ 
   // Return an error if device not found. 
   if (setup_uinput_device() < 0) 
   { 
   printf("Unable to find uinput device/n"); 
   return -1; 
   } 
   
   while(1)
   {
		//send_a_button(); // Send a "A" key 
		//send_click_events(); // Send mouse event 
		sleep(1);
   }
   
   /* Destroy the input device */ 
   ioctl(uinp_fd, UI_DEV_DESTROY); 
   
   /* Close the UINPUT device */ 
   close(uinp_fd); 
}