#include "mbed.h"
#include "Motor.h"
#include <stdio.h>
#include <math.h>
#include "rtos.h"

// ROBOT -----------------------------------------------------------------------------------------------------------------
///*
Motor right(p26, p25, p24); // pwm, fwd, rev
Motor left(p21, p14, p23); // pwm, fwd (A1), rev(A2)
Motor pillPusher (p22, p11, p12);

Serial arduino(p9, p10);
DigitalOut led(LED1);
RawSerial  dev(p28,p27);
RawSerial pc(USBTX, USBRX);
char bdir = '0';
int dispensing = 0;

void bleControl(void const *args) {
    while(1) {
        if (dev.readable()) {
            if (dev.getc()=='!') {
                if (dev.getc()=='B') { //button data
                    bdir = dev.getc(); //button direction
                    if (dev.getc()=='0') { //button data
                        bdir = '0'; //button direction
                    }
                }
            }
        }
    }
}

void robotControl(void const *args) {
    while(1) {
        // 5: up, 8: right, 6: down, 7: left
        if (bdir == '5') {
            // forward
            left.speed(0.8);
            right.speed(-0.8);
            pc.printf("forward\r\n");
        } else if (bdir == '6') {
            // backward
            left.speed(-0.8);
            right.speed(0.8);
            pc.printf("backward\r\n");
        } else if (bdir == '7') {
            // left
            left.speed(-0.8);
            right.speed(-0.8);
            pc.printf("left\r\n");
        } else if (bdir == '8') {
            // right
            left.speed(0.8);
            right.speed(0.8);
            pc.printf("right\r\n");
        } else if (bdir == '1' && dispensing == 0) {
            dispensing = 1;
            pc.printf("dispense\r\n");
            bdir = '0';
        } else {
            left.speed(0);
            right.speed(0);
        }
    }
}

void dispense(void const *args) {
    while(1) {
        if (dispensing == 1) {
            // dispense
            pillPusher.speed(0.3);
            Thread::wait(1000.0*0.45);
            pillPusher.speed(-0.3);
            Thread::wait(1000.0*0.35);
            pillPusher.speed(0);
            dispensing = 0;
            Thread::wait(2000);
            arduino.putc(1);
            Thread::wait(200);
            arduino.putc(0);
            Thread::wait(500);
        }
    }
}

int main() {
    Thread thread1(bleControl);
    Thread thread2(robotControl);
    Thread thread3(dispense);
    while(1) {
        // pc.printf("running\r\n");
        sleep();
    }
}
