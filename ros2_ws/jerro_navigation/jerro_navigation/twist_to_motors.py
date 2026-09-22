import rclpy
import geometry_msgs
import math
import time
from rclpy.node import Node
from geometry_msgs.msg import Twist
from jerro_msgs.msg import MotorSpeed
from std_msgs.msg import Int32




class TwistToMotorPublisher(Node):

    WHEELBASE = 0.164
    WHEEL_RADIUS = 0.068
    TICKS_BY_ROTATION = 632.8

    def __init__(self):
        #---VARIABLES---
        self.previousTickCountLeft = 0
        self.previousTickCountRight = 0
        self.previousTime = 0
        self.speedCommand = [0,0]

        super().__init__('twist_to_motor')
        #---LISTENERS---
        #cmd vitesse
        self.subscription = self.create_subscription(
            Twist,
            '/cmd_vel',
            self.cmd_vel_callback,
            10)
        self.subscription  # prevent unused variable warning
        #encodeur A
        self.subscription = self.create_subscription(
            Int32,
            '/encoder_a',
            self.encoder_a_callback,
            10)
        self.subscription  # prevent unused variable warning
        #encodeur B
        self.subscription = self.create_subscription(
            Int32,
            '/encoder_b',
            self.encoder_b_callback,
            10)
        self.subscription  # prevent unused variable warning

        #---PUBLISHERS---
        #
        self.publisher_ = self.create_publisher(MotorSpeed, '/motor/set_speed', 10)
        #timer_period = 0.5  # seconds
        #self.timer = self.create_timer(timer_period, self.timer_callback)
        #self.i = 0

    #---CALLBACKS---
    def cmd_vel_callback(self, msg):
        v_lin_x = msg.linear.x
        w_ang_z = msg.angular.z
        msgOut = MotorSpeed()
        linearSpeed = [self.convertSpeedLeft(v_lin_x, w_ang_z), self.convertSpeedRight(v_lin_x, w_ang_z)]

        self.speedCommand[0] = self.v_LinToTicks(linearSpeed[0])
        msgOut.motor_speed_a = self.speedCommand[0]
        self.speedCommand[1] = self.v_LinToTicks(linearSpeed[1])
        msgOut.motor_speed_b = self.speedCommand[1]
        self.publisher_.publish(msgOut)

    def encoder_a_callback(self, msg):
        currentTickCount = msg.data
        currentTime = time.time()

        
        tickSpeed = (currentTickCount - self.previousTickCountLeft)/(currentTime - self.previousTime)
        msSpeed = self.v_TicksToMs(tickSpeed)
        error = self.v_TicksToMs(abs(self.speedCommand[0]-tickSpeed))

        self.previousTickCountLeft = currentTickCount
        self.previousTime = currentTime

        self.get_logger().info(f"Roue gauche - Consigne: {self.speedCommand[0]:.3f} m/s, Mesure: {msSpeed:.3f} m/s, Erreur: {error:.3f} m/s")


        
    def encoder_b_callback(self, msg):
        currentTickCount = msg.data
        currentTime = time.time()

        tickSpeed = (currentTickCount-self.previousTickCountRight)/(currentTime-self.previousTime)
        msSpeed = self.v_TicksToMs(tickSpeed)
        error = self.v_TicksToMs(abs(self.speedCommand[1]-tickSpeed))

        self.previousTickCountRight = currentTickCount
        self.previousTime = currentTime

        self.get_logger().info(f"Roue droite - Consigne: {self.speedCommand[1]:.3f} m/s, Mesure: {msSpeed:.3f} m/s, Erreur: {error:.3f} m/s")

    #---UTILITIES---
    def convertSpeedLeft(self, v_lineaire, w_angulaire):
        return (v_lineaire - (w_angulaire*self.WHEELBASE)/2)

    def convertSpeedRight(self, v_lineaire, w_angulaire):
        return (v_lineaire + (w_angulaire*self.WHEELBASE)/2)

    def msToTicks(self, v_lineaire, w_angulaire):
        return (v_lineaire + (w_angulaire*1)/2)
    
    def v_LinToTicks(self, v_lineaire):
        return (v_lineaire * self.TICKS_BY_ROTATION)/(2.0 * math.pi * self.WHEEL_RADIUS)
    
    def v_TicksToMs(self, v_lineaire):
        return (v_lineaire*2.0 * math.pi * self.WHEEL_RADIUS)/self.TICKS_BY_ROTATION


def main(args=None):
    rclpy.init(args=args)

    twistToMotor = TwistToMotorPublisher()

    rclpy.spin(twistToMotor)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    twistToMotor.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()