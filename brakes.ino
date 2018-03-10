#include <PWMServo.h>
#include <ros.h>
#include <std_msgs/Int16.h>

// pin assignments: LH SERVO_PIN_A = 9, RH SERVO_PIN_B = 10

int braking_current_state, braking_desired_state;

int pos_lh_inactive = 80; // servo position when brakes are inactive
int pos_lh = pos_lh_inactive;    // variable to store the servo position
int pos_lh_active = 150; // servo position when brakes are active

ros::NodeHandle  nh;
std_msgs::Int16 handshake;
void messageCb(const std_msgs::Int16& msg);
ros::Subscriber<std_msgs::Int16> sub("braking_desired_state", messageCb );
ros::Publisher pub("braking_current_state", &handshake);

PWMServo lh_brake;
//PWMServo rh_brake;

void messageCb(const std_msgs::Int16& msg){
  braking_desired_state = msg.data; // read the desired braking state
  handshake.data = braking_desired_state; // stuff this message to prepare for handshake
  pub.publish(&handshake);
}

void setup() {
  lh_brake.attach(SERVO_PIN_A);  // attaches the servo on pin 9 to the LH brake
  nh.initNode(); // create ROS node
  nh.subscribe(sub); // set up subscriber
  nh.advertise(pub); // set up publisher
}


void loop() {
  if(braking_desired_state){
    pos_lh = pos_lh_active;
//    pos_rh = pos_rh_active;
  }
  else{
    pos_lh = pos_lh_inactive;
//    pos_rh = pos_rh_inactive;
  }
  lh_brake.write(pos_lh);
//  rh_brake.write(pos_rh);
  nh.spinOnce();
}