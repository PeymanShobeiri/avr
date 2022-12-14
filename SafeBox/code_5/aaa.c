#include <mega32.h>
#include <alcd.h>
#include <sleep.h>
#include <delay.h>
#include <stdlib.h>
flash char Pattern[4]={0xFE, 0xFD, 0xFB, 0xF7};
flash char keypad [4][4]=    {'7', '8', '9', '/',
                              '4', '5', '6', '*',
                              '1', '2', '3', '-',
                              'C', '0', '=', '+'};
char password[5],cur[5];
int totall=0,cnt=0,sw=0,pw=0,lk=0;

int check_pass(){
    int j;
    for(j=0;j<4;j++){
        if (password[j] != cur[j]){
            return 0;}
        }
        return 1;
}

void set_pass(char x){
        if ((x>='0' && x<='9') && totall < 4){
          password[totall]=x;
          totall ++;
          }
        else if (totall == 4 && x == '+'){
          lcd_clear();
          lcd_puts("PASSWORD CHANGED");
          totall =0;
          sw=0;
          pw=1;
          cnt=0;
          delay_ms(300);
          lcd_clear();
        }
        else {
          lcd_clear();
          lcd_puts("INVALID CHAR !");
          totall = 0;
          cnt=0;
          delay_ms(700);
          lcd_clear();
         }
         }

interrupt [EXT_INT2] void ext_int2_isr(void){

  char row, column;
  char tmp;


  for( row=0; row<4; row++){
     PORTD = Pattern[row];

     if (PIND.4 == 0){
       column  = 0;
       break;
       }
     else if (PIND.5 == 0){
       column  = 1;
       break;
         }
     else if (PIND.6 == 0){
       column  = 2;
       break;
             }
     else if (PIND.7 == 0){
       column  = 3;
       break;
                 }
  }

  tmp = keypad[row][column];

  if ( tmp =='C' ){
      if (pw==1){
      lcd_puts("ENTER PASSWORD");
      delay_ms(500);
      cnt =0;
      lcd_clear();
      }
      else {
      lcd_puts("ENTER PASSWORD  FIRST");
      delay_ms(500);
      lcd_clear();
      }
   }

  if (tmp !='C' ){
      lcd_putchar(tmp);
      cur[cnt]=tmp;
      cnt ++;
  }
  if (cnt==4 && sw==0 ){
    if (check_pass()==1){
        if(lk==0){
        lk=1;
        PORTB = 0x81;
        delay_ms(590);
        PORTB = 0x01;
        lcd_clear();
        lcd_puts("LOCK");
        delay_ms(500);
        cnt=0;
        }
        else{
        lk=0;
        PORTB = 0x80;
        delay_ms(530);
        PORTB = 0x00;
        lcd_clear();
        lcd_puts("OPEN");
        delay_ms(500);
        cnt=0;
    }
    lcd_clear();
    }
    else{
    lcd_clear();

    lcd_puts("WRONG PASS");
    delay_ms(500);
    lcd_clear();
    cnt=0;
    }
    }
  if (sw==1 && pw==1){
    if (cnt==4){
    delay_ms(700);
    lcd_clear();
    lcd_puts("****");
    delay_ms(400);
    lcd_clear();
    if (check_pass()){
        pw=0;
        sw=1;
        lcd_puts("ENTER NEW PASS");
        delay_ms(800);
        lcd_clear();
        cnt =0;

        }
    else{
     lcd_clear();
     lcd_puts("ERRORE");
     delay_ms(300);
     lcd_clear();
     cnt=0;
    }
   }
  }

  if (sw==1 && pw==0){
      set_pass(tmp);
      }


  if ((cur[0]=='=' && cur[1]=='=' && cur[2]=='=') || (cur[3]=='=' && cur[2]=='=' && cur[1]=='=')){
        cnt = 0;
        sw=1;
        lcd_clear();
        lcd_puts("Set pass : ");
        delay_ms(600);
        lcd_clear();
  }


  PORTD = 0xF0;


}

void main(void)
{

DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);

PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);


DDRB=(1<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);

PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);


DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);

PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);


DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);

PORTD=(1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5) | (1<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

GICR|=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);


lcd_init(16);

#asm("sei")
lcd_puts("PRESS THREE = TO SET PASSWORD");
delay_ms(800);
lcd_clear();
while (1)
      {


          idle();

      }
}