# angleSubscrible 
- ROS 
    - publish ExoAngle.msg
- Arduino 
    - Subscribe : ExoAngle.msg
# anglePublish
- Arduino 
    - Publish : Encoder(H/K)(L/R).msg
# singleMotorRos 
- ROS 
    - publish ExoAngle.msg
- Arduino 
    - Subscribe : ExoAngle.msg
    - Rotate motor as per angle recived 
# singleMotorPubSub
- ROS 
    - publish ExoAngle.msg
- Arduino 
    - Subscribe : ExoAngle.msg
    - Rotate motor as per angle recived 
    - Publish : Encoder(H/K)(L/R).msg
# duty_cycle_test 
- direction control only
# encoder 
- Prints the angle on serial montior
# ExoConfig 
- Not used 
- Subscribe topic and run motor for perticular motor
# PID_controller
- Rotate motor to pre-coded angle
- Show the result on serial monitor