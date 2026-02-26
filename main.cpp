  //putting all my libraries here for my gyroscope AND my ESP
  #include <Arduino.h>
  #include <stdio.h>
  #include<Wire.h>
  #include <USB.h>
  #include<USBHIDMouse.h>
  #include<Adafruit_MPU6050.h>
  #include<Adafruit_Sensor.h>

  Adafruit_MPU6050 mpu; //we make an object for the blueprint of the MPU6050 and the mouse
  USBHIDMouse mouse;

  bool MPU_ACTIVE_TEST = 1; // we set a bool to use to see if the MPU has been activated and working
  float pitch , roll , yaw , MousePosX , MousePosY;
  int i=1 , mouse_sensitivity=3;

  void setup(){
    Wire.begin(21,22); //we start the Wire.h library and join the I2C as a controller 
    Serial.begin(115200);
    USB.begin();
    mouse.begin();
    while(!Serial){ //here it checks if ESP is activated and if it isnt it delays for an infinite amount of time which isnt the smartest choice but idrc
      delay(1);
    }
    if(!mpu.begin()){//we initialize the MPU6050 in this if statement and we have some kind of failsafe
      MPU_ACTIVE_TEST = 0;
      while (MPU_ACTIVE_TEST == 0){
        printf("MPU6050 has failed to activate\n"); //le failsafe i mentioned incase it breaks
        delay(1000);
        mpu.begin();
        if(mpu.begin()){
          MPU_ACTIVE_TEST =1; //incase it activates it goes back to 1 to show its working
        }
      }
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G); //we set a limit of how fast it tracks
    mpu.setGyroRange(MPU6050_RANGE_500_DEG); //we set a limit on how many degress it can track per second
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); //we set a limit how many times it tracks per second in order to make the movement feel smoother and not junky
    printf("MPU6050 has been calibrated\n");
  }

  void loop() {
    sensors_event_t a , g , temp; //we create events in order for the MPU to store values
    mpu.getEvent(&a,&g,&temp); // we tell it to read these values and print the below
    while(i==1){
    printf("Acceleration is X:  %f\n" , a.acceleration.x);
    printf("Acceleration is Y: %f\n" , a.acceleration.y);
    printf("Acceleration is Z: %f\n" , a.acceleration.z);
    printf("Rotation is X: %f\n" , g.gyro.x);
    printf("Rotation is Y: %f\n" , g.gyro.y);
    printf("Rotation is Z: %f\n" , g.gyro.z);
    printf("Temperature is %f" , temp.temperature); //those are all tests to see if we are getting accurate data
    i++;
    }

    pitch = g.gyro.x*(180.0/PI); //we turn the radian readings into degrees so its easier to work with
    roll = g.gyro.y*(180.0/PI);
    yaw = g.gyro.z*(180.0/PI);
    delay(20);

    MousePosX = yaw * mouse_sensitivity; //here we find the position that the mouse needs to turn on the X axis based on how much we turned our head
    MousePosY = -(pitch * mouse_sensitivity); //here we find the position that the mouse needs to turn on the Y axis based on how much we turned our head
    mouse.move(MousePosX , MousePosY); //after finding the coordinates we move the mouse to those exact positions we previously found
  }