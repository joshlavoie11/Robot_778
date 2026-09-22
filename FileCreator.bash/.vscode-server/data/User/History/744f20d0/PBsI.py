import rclpy
import geometry_msgs
import math
import time
from rclpy.node import Node
from geometry_msgs.msg import Twist
from jerro_msgs.msg import MotorSpeed
from std_msgs.msg import Int32



class MinimalPublisher(Node):

    WHEELBASE = 0.164
    WHEEL_RADIUS = 0.068
    TICKS_BY_ROTATION = 632.8

    def __init__(self):
        #---VARIABLES---
        self.previousTickCount = 0
        self.PreviousTime = 0

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

        v_lin_x = msg.data.linear.x
        w_ang_z = msg.data.angular.z
        linearSpeed = [self.convertSpeedLeft(self, v_lin_x, w_ang_z), self.convertSpeedLeft(self, v_lin_x, w_ang_z)]

        msg = self.v_LinToTicks(linearSpeed)
        self.publisher_.publish(msg)

    def encoder_a_callback(self, msg):
        pulse = msg

        
    def encoder_b_callback(self, msg):
        pulse = msg        
        #v_lin_x = data.linear.x
        #w_ang_z = data.angular.z
        #msg = [self.v_left(self, v_lin_x, w_ang_z), self.v_right(self, v_lin_x, w_ang_z)]
        #self.publisher_.publish(msg)

    #---UTILITIES---
    def convertSpeedLeft(self, v_lineaire, w_angulaire):


        return (v_lineaire - (w_angulaire*self.WHEELBASE)/2)

    def convertSpeedRight(self, v_lineaire, w_angulaire):
        return (v_lineaire + (w_angulaire*self.WHEELBASE)/2)

    def msToTicks(self, v_lineaire, w_angulaire):
        return (v_lineaire + (w_angulaire*1)/2)
    
    def v_LinToTicks(self, v_lineaire):
        return (v_lineaire * self.TICKS_BY_ROTATION)/(2 * math.pi * self.WHEEL_RADIUS)
    
    def v_TicksToMs(self, v_lineaire):
        return (v_lineaire*2 * math.pi * self.WHEEL_RADIUS)/self.TICKS_BY_ROTATION

        



#*****************
class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(String, 'topic', 10)
        timer_period = 0.5  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    def timer_callback(self):
        msg = String()
        msg.data = 'Hello World: %d' % self.i
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)
        self.i += 1
#*****************



def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()