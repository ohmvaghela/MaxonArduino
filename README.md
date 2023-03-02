# angle#ubscrible 
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
# speedTest
- ROS 
    - publish ExoAngle.msg // This message is not used in arduino
- Arduino 
    - Subscribe : ExoAngle.msg
    - Rotate motor as per angle recived 
    - Publish : Encoder(H/K)(L/R).msg

- Gait motion referecnce
https://musculoskeletalkey.com/biomechanics-2/

- Approch
- Find time taken to complete one phase for each knee and hip
- and then manuplate dutycycle to match time

hip joint :- 40 degree duty :- 30 + 20()
4.73
4.96
4.78
5.05

knee jopint 90 degree  duty :- 30 + 20()
9.5
9.4
9.3
9.5

knee jopint 90 degree  duty :- 50 + 20()
11.4
11.6

knee jopint 90 degree  duty :- 70 + 20()

