#include <PWMServo.h>
#include <ros.h>
#include <std_msgs/Int16.h>

// rosrun rosserial_python serial_node.py /dev/ttyACM0 NOT ttyUSB0

// pin assignments: RH SERVO_PIN_A = 9, LH SERVO_PIN_B = 10

// NOTE: in ArduinoIncludes.h line 44:
// write this --> #if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK65FX512__) || defined(__MK66FX1M0__)
// not this   --> #if defined(__MK20DX128__) || defined(__MK20DX256__)

int braking_current_state, braking_desired_state;

int pos_lh_inactive = 72; // servo position when brakes are inactive
int pos_lh = pos_lh_inactive;    // variable to store the servo position
int pos_lh_active = 53; // servo position when brakes are active
int pos_rh_inactive = 124; // servo position when brakes are inactive
int pos_rh = pos_rh_inactive;    // variable to store the servo position
int pos_rh_active = 153; // servo position when brakes are active

ros::NodeHandle  nh;
std_msgs::Int16 handshake;
void messageCb(const std_msgs::Int16& msg);
ros::Subscriber<std_msgs::Int16> sub("braking_desired_state", messageCb );
ros::Publisher pub("braking_current_state", &handshake);

// http://wiki.ros.org/rosserial/Overview/Logging
// nh.loginfo("Debug note.")

PWMServo lh_brake;
PWMServo rh_brake;

void messageCb(const std_msgs::Int16& msg) {
  braking_desired_state = msg.data; // read the desired braking state
  handshake.data = braking_desired_state; // stuff this message to prepare for handshake
  pub.publish(&handshake);
}

void setup() {
  lh_brake.attach(SERVO_PIN_B);  // attaches the servo on pin 10 to the LH brake
  rh_brake.attach(SERVO_PIN_A);  // attaches the servo on pin 9 to the RH brake
  nh.initNode(); // create ROS node
  nh.subscribe(sub); // set up subscriber
  nh.advertise(pub); // set up publisher
}

void loop() {
   if(braking_desired_state){
     pos_lh = pos_lh_active;
     pos_rh = pos_rh_active;
   }
   else{
     pos_lh = pos_lh_inactive;
     pos_rh = pos_rh_inactive;
   }
   lh_brake.write(pos_lh);
   rh_brake.write(pos_rh);
   nh.spinOnce();
  // for debugging vvv
//   pos_lh = pos_lh_active;
//   lh_brake.write(pos_lh);
//   pos_rh = pos_rh_active;
//   rh_brake.write(pos_rh);
//   delay(1000);
//   pos_lh = pos_lh_inactive;
//   lh_brake.write(pos_lh);
//   pos_rh = pos_rh_inactive;
//   rh_brake.write(pos_rh);
//   delay(1000);
}
