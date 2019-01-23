#include <msp430.h> 

#define BTN1 BIT4//P1.4
#define BTN2 BIT5//P1.5


#define LED1 BIT0 //P2.0
#define LED2 BIT6 //P1.6
#define LED3 BIT2 //P2.2

#define PIR1 BIT3 //P2.3
#define PIR2 BIT4 //P2.4
#define PIR3 BIT3 //P2.5

int pir1state = 0;
int pir2state = 0;
int pir3state = 0;

//ADC

unsigned int btn1state = 0;
unsigned int btn2state = 0;

unsigned int timerCount = 0;

unsigned int adc[2] = {0};
int speed = 1;
int turndir = 0;


void Read_Acc(void);
int decidespeed(void);
void setvertical(void);
void sethorizontal(void);

void init();


#define SERVO1_BIT BIT2//P1.2
#define SERVO2_BIT BIT1 //P2.1

#define SERVO_STEPS         180     // Maximum amount of steps in degrees (180 is common)
#define SERVO_MIN           650     // The minimum duty cycle for this servo
#define SERVO_MAX           2700    // The maximum duty cycle


unsigned int servo_stepval, servo_stepnow;
unsigned int servo_lut[ SERVO_STEPS+1 ];
unsigned int i;
int angle = 179;

int previousposition;


#define PIR1POS 149
#define PIR2POS 90
#define PIR3POS 29


#define SM_clk 1100000

#define servo_freq 50



int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    init();
    int c;

    while(1){

        while (btn1state == 0) { // running auto
            P1OUT |= LED2;
            P2OUT &= ~LED3;
          while (btn2state == 0) {
            while (angle >= 0) {
              if (btn1state == 1) {
                 break;
              }
              if (btn2state == 1) {
                break;
              }
              Read_Acc();
              speed = decidespeed();
              setvertical();
              turndir = 1;
              TA0CCR1 = servo_lut[angle];

              if (pir1state) {
                turndir = 0;
                TA0CCR1 = servo_lut[PIR1POS];
                for ( c = 0; c  < 5; c++) { // calibrate PIR sensors for 5 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                TA0CCR1 = servo_lut[angle];
                for ( c = 0; c  < 1; c++) { // calibrate PIR sensors for 1 seconds
                  P2OUT ^= LED1;
                   __delay_cycles (1200000); // wait 1 second
                }
                turndir = 1;
                pir1state = 0;
                pir2state = 0;
                pir3state = 0;
              }
              else if (pir2state) {
                turndir = 0;
                TA0CCR1 = servo_lut[PIR2POS];
                for ( c = 0; c  < 5; c++) { // calibrate PIR sensors for 5 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                TA0CCR1 = servo_lut[angle];
                for ( c = 0; c  < 1; c++) { // calibrate PIR sensors for 1 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second

               }
                turndir = 1;
                pir2state = 0;
                pir1state = 0;
                pir3state = 0;
              }
              else if (pir3state) {
                turndir = 0;
                TA0CCR1 = servo_lut[PIR3POS];
                for ( c = 0; c  < 5; c++) { // calibrate PIR sensors for 5 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                TA0CCR1 = servo_lut[angle];
                  for ( c = 0; c  < 1; c++) { // calibrate PIR sensors for 1 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                turndir = 1;
                pir3state = 0;
                pir1state = 0;
                pir2state = 0;
              }
            }

            while (angle <= 179) {
              if (btn1state == 1) {
                break;
              }
              if (btn2state == 1) {
                break;
              }
              Read_Acc();
              speed = decidespeed();
              setvertical();
              turndir = 2;
              TA0CCR1 = servo_lut[angle];

              if (pir1state) {
                turndir = 0;
                TA0CCR1 = servo_lut[PIR1POS];
                for ( c = 0; c  < 5; c++) { // calibrate PIR sensors for 5 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                TA0CCR1 = servo_lut[angle];
                for ( c = 0; c  < 1; c++) { // calibrate PIR sensors for 1 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                turndir = 2;
                pir1state = 0;
              }
              else if (pir2state) {
                turndir = 0;
                TA0CCR1 = servo_lut[PIR2POS];
                for ( c = 0; c  < 5; c++) { // calibrate PIR sensors for 5 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second

                }
                TA0CCR1 = servo_lut[angle];
                for ( c = 0; c  < 1; c++) { // calibrate PIR sensors for 1 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                turndir = 2;
                pir2state = 0;
                pir1state = 0;
                pir3state = 0;
              }
              else if (pir3state) {
                turndir = 0;
                TA0CCR1 = servo_lut[PIR3POS];
                for ( c = 0; c  < 5; c++) { // calibrate PIR sensors for 5 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                TA0CCR1 = servo_lut[angle];
                for ( c = 0; c  < 1; c++) { // calibrate PIR sensors for 1 seconds
                  P2OUT ^= LED1;
                  __delay_cycles (1200000); // wait 1 second
                }
                turndir = 2;
                pir3state = 0;
                pir1state = 0;
                pir2state = 0;
              }
            }
          }
        }

        while (btn1state == 1) { //manual control
            P1OUT &= ~LED2;
            P2OUT |= LED3;
            Read_Acc();
            setvertical();
            sethorizontal();
            if (btn1state == 0) {
                break;
            }
        }
    }
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

    if (P1IFG & BTN1){
        btn1state ^= 0x01;

        __delay_cycles(40000);
        P1IFG &= ~BTN1;

    }
    if (P1IFG & BTN2){
        btn2state ^= 0x01;

        __delay_cycles(40000);
        P1IFG &= ~BTN2;

    }
    if (P1IFG & PIR3){
            pir3state =1;
            P1IFG &= ~PIR3;
        }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
    if (P2IFG & PIR1){
        pir1state =1;
        P2IFG &= ~PIR1;
    }
    if (P2IFG & PIR2){
        pir2state =1;

        P2IFG &= ~PIR2;
    }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
    timerCount = (timerCount + 1) % speed;
        if(timerCount ==0){
            if(turndir ==1 && angle>=0){
                angle--;
            }else if(turndir == 2 && angle<=179){
                angle++;
            }
        }
}

void Read_Acc(void){
    ADC10CTL0 &= ~ENC;
    ADC10SA = (unsigned int)adc;            // Copies data in ADC10SA to unsigned int adc array
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

}


int decidespeed(){
    if ((100 < adc[0]) & (adc[0] < 200)){
        speed = 1;

    }else if ((200 < adc[0]) & (adc[0] < 300)){
        speed = 2;

    }else if ((300 < adc[0]) & (adc[0] < 400)){
        speed = 3;

    }else if ((400 < adc[0]) & (adc[0] < 500)){
        speed = 4;
    }else if ((500 < adc[0]) & (adc[0] < 600)){
        speed = 5;
    }else if ((600 < adc[0]) & (adc[0] < 700)){
        speed = 6;
    }else if ((700 < adc[0]) & (adc[0] < 800)){
        speed = 7;
    }else if (800 < adc[0]){
        speed = 8;
    }

    return speed;
}

void setvertical(){

    if ((100 < adc[1]) & (adc[1] < 50)){
            TA1CCR1 = servo_lut[0];

        }else if ((50 < adc[1]) & (adc[1] < 100)){
            TA1CCR1 = servo_lut[20];

        }else if ((150 < adc[1]) & (adc[1] < 200)){
            TA1CCR1 = servo_lut[30];

        }else if ((250 < adc[1]) & (adc[1] < 300)){
            TA1CCR1 = servo_lut[40];
        }else if ((350 < adc[1]) & (adc[1] < 400)){
            TA1CCR1 = servo_lut[50];
        }else if ((450 < adc[1]) & (adc[1] < 500)){
            TA1CCR1 = servo_lut[60];
        }else if ((500 < adc[1]) & (adc[1] < 550)){
            TA1CCR1 = servo_lut[70];

        } else if ((600 < adc[1]) & (adc[1] < 650)) {
          TA1CCR1 = servo_lut[80];

        } else if ((700 < adc[1]) & (adc[1] < 750)) {
          TA1CCR1 = servo_lut[90];

        } else if ((800 < adc[1]) & (adc[1] < 850)) {
            TA1CCR1 = servo_lut[100];
        } else if ((900 < adc[1]) & (adc[1] < 950)) {
            TA1CCR1 = servo_lut[120];
        } else if ((1000 < adc[1]) & (adc[1] < 1050)) {
            TA1CCR1 = servo_lut[140];
        }else if (1050 < adc[1]){
            TA1CCR1 = servo_lut[179];
        }

    }
void sethorizontal(){
    if ((100 < adc[0]) & (adc[0] < 50)) {
      TA0CCR1 = servo_lut[0];

    } else if ((50 < adc[0]) & (adc[0] < 100)) {
      TA0CCR1 = servo_lut[20];

    } else if ((150 < adc[0]) & (adc[0] < 200)) {
      TA0CCR1 = servo_lut[30];

    } else if ((250 < adc[0]) & (adc[0] < 300)) {
      TA0CCR1 = servo_lut[40];
    } else if ((350 < adc[0]) & (adc[0] < 400)) {
      TA0CCR1 = servo_lut[50];
    } else if ((450 < adc[0]) & (adc[0] < 500)) {
      TA0CCR1 = servo_lut[60];
    } else if ((500 < adc[0]) & (adc[0] < 550)) {
      TA0CCR1 = servo_lut[70];

    } else if ((600 < adc[0]) & (adc[0] < 650)) {
      TA0CCR1 = servo_lut[80];

    } else if ((700 < adc[0]) & (adc[0] < 750)) {
      TA0CCR1 = servo_lut[90];

    } else if ((800 < adc[0]) & (adc[0] < 850)) {
      TA0CCR1 = servo_lut[100];
    } else if ((900 < adc[0]) & (adc[0] < 950)) {
      TA0CCR1 = servo_lut[120];
    } else if ((1000 < adc[0]) & (adc[0] < 1050)) {
      TA0CCR1 = servo_lut[140];
    } else if (1050 < adc[0]) {
      TA0CCR1 = servo_lut[179];
    }
}



void init(){
    //INPUT OUTPUT SETUP
        P1DIR |= LED2;
        P2DIR |= (LED1 + LED3);
        P1OUT &= ~LED2;
        P2OUT &= ~(LED1 + LED3);

        P1DIR &= ~(BTN1+ BTN2+ PIR3);//Set as inputs
        P2DIR &= ~(PIR1+ PIR2);//set as inputs


        P2OUT &= ~ (PIR1+ PIR2);
        P1OUT &= ~(PIR3);
        P2REN |=(PIR1+ PIR2);
        P1REN |= PIR3;


        P1IES &= ~(BTN1 + BTN2+PIR3);
        P2IES &= ~(PIR1 + PIR2);

        P1IE |= BTN1 + BTN2 + PIR3;//Interrupt enable for Port 2 and port 1
        P2IE |=  PIR1 + PIR2;

        __enable_interrupt();


        int c = 0;

        for( c = 0; c  < 15; c++){ // calibrate PIR sensors for 15 seconds
            P2OUT ^= LED1;
            __delay_cycles (1200000); // wait 1 second
        } // end calibration

        P2OUT |= LED1; //Keep callibration indicator LED On.

        //PWM0 SETUP
        int PWM_period = SM_clk /servo_freq;

        P1DIR |= SERVO1_BIT; //direction is set
        P1SEL |= SERVO1_BIT;// port 1 function is set.

        TA0CCR0 = PWM_period-1; // pwm period
        TA0CCTL0 = CCIE;
        TA0CCR1 = 0; //duty cycle = TA0CCR0/ TA0CCR1 set pwm for servo 1
        TA0CCTL1 = OUTMOD_7;

        TA0CTL=TASSEL_2+MC_1;

        //PWM1 SETUP



        P2DIR |= SERVO2_BIT; //direction is set
        P2SEL |= SERVO2_BIT;// port 1 function is set.
        TA1CCR0 = PWM_period-1; // pwm period
        TA1CCR1 = 0; //duty cycle = TA0CCR0/ TA0CCR1 //set pwm for servo2
        TA1CCTL1 = OUTMOD_7;
        TA1CTL=TASSEL_2+MC_1;


        servo_stepval   = ( (SERVO_MAX - SERVO_MIN) / SERVO_STEPS );
        servo_stepnow   = SERVO_MIN;


        for (i = 0; i< SERVO_STEPS; i++) {
                servo_stepnow += servo_stepval;
                servo_lut[i] = servo_stepnow;
            }

        TA0CCR1 = servo_lut[178];TA1CCR1 = servo_lut[178];
        __delay_cycles(1000000);

        TA0CCR1 = servo_lut[90];TA1CCR1 = servo_lut[90];
         __delay_cycles(1000000);

         TA0CCR1 = servo_lut[1];TA1CCR1 = servo_lut[1];
         __delay_cycles(1000000);

         TA0CCR1 = servo_lut[179];TA1CCR1 = servo_lut[90];
          __delay_cycles(1000000);


        //ADC SETUP

          WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
          ADC10CTL1 = INCH_1 + CONSEQ_1;            // A2/A1/A0, single sequence
          ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON;
          ADC10DTC1 = 0x02;                         // 2 conversions
          ADC10AE0 |= 0x03;                        // Disable digital I/O on P1.0 to P1.1


}
 
