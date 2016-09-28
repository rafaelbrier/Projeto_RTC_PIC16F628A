
#define Select PORTA.RA0
 #define Nchange PORTA.RA1
 
// Software I2C connections
sbit Soft_I2C_Scl           at RA7_bit;
sbit Soft_I2C_Sda           at RA6_bit;
sbit Soft_I2C_Scl_Direction at TRISA7_bit;
sbit Soft_I2C_Sda_Direction at TRISA6_bit;
// End Software I2C connections
 
 
 // LCD module connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End LCD module connections

    //variaveis globais
   unsigned char second;
   unsigned char minute;
   unsigned char hour;
   unsigned char day;
   unsigned char month;
   unsigned char year;
   unsigned char i;
   unsigned char a;
   unsigned char AHour;
   unsigned char AMin;
   unsigned char ASeg;
   char time[] = "00:00:00  ";
   char date[] = "00-00-00";
   
   //diasemana 3   //hora 2   //segundo 0   //minuto 1   //ano 6   //hora 2   //mes 5   //dia 4
   //Minuto MSB 3
  
  void AdjustTime(unsigned char Value)
  {

                             delay_ms(300);
                             time[Value] = (time[Value] - '0');
                             while(1)
                             {
                              if(~Nchange){
                              delay_ms(400);
                              time[Value]++;
                              }
                              if(Value ==0 && time[Value] >=3)   //msb hora
                              time[Value] =0;
                              if(Value == 3 && time[Value]>=6)    //msb minuto
                              time[Value]=0;
                              if(Value == 6 && time[Value]>=6)   //msb segundo
                              time[Value]=0;
                              if(Value == 4 && time[Value]>=10)   //lsb minuto
                              time[Value]=0;
                              if(Value == 7 && time[Value]>=10)  //lsb segundo
                              time[Value]=0;
                              if(Value==1)
                              {
                              if(time[Value-1] == 0 || time[Value-1] == 1)   //lsb hora
                              {
                                 if(time[Value] >=10)
                                 time[Value]=0;
                              }
                              if(time[Value-1] ==2){
                                 if(time[Value]>=4)
                                 time[Value]=0;
                              }
                              }
                              Lcd_Chr_Cp(time[Value]+'0');
                              Lcd_Cmd(_LCD_MOVE_CURSOR_LEFT);
                              if(~Select)
                              break;
                              }
    }
    
    void AdjustDate(unsigned char Value)
    {
                            delay_ms(300);
                             date[Value] = (date[Value] - '0');
                             while(1)
                             {
                              if(~Nchange){
                              delay_ms(400);
                              date[Value]++;
                              }
                              if(Value == 0 && date[Value]>=4)   // MSB DIA
                              date[Value]=0;
                              if(Value == 3 && date[Value]>=2)   // MSB MES
                              date[Value]=0;
                              if(Value == 6 && date[Value]>=10)   // MSB Ano
                              date[Value]=0;
                              if(Value == 7 && date[Value]>=10)   // LSB Ano
                              date[Value]=0;
                              if(Value == 1)                       //LSB DIA
                              {
                              if(date[Value-1] == 1 || date[Value-1] == 2)
                              {
                               if(date[Value] >=10)
                                 date[Value]=0;
                                 }

                              if(date[Value-1] == 0)
                              {
                                 if(date[Value] >=10)
                                 date[Value]=1;
                              }
                              if(date[Value-1] ==3){
                                 if(date[Value]>=1)
                                 date[Value]=0;
                              }
                              }
                              if(Value == 4)                   //lsb mes
                              {
                              if(date[Value-1] == 0)
                              {
                                 if(date[Value] >=10)
                                 date[Value]=1;
                              }
                              if(date[Value-1] == 1){
                                 if(date[Value]>=3)
                                 date[Value]=0;
                              }
                              }
                              Lcd_Chr_Cp(date[Value]+'0');
                              Lcd_Cmd(_LCD_MOVE_CURSOR_LEFT);
                              if(~Select)
                              break;
                              }

    }
   
   void gravarRTC(unsigned char MSB, unsigned char LSB, unsigned char Adress)
{
             unsigned char x;
             x = ((MSB << 4) | LSB);

   Soft_I2C_Start(); // Inicia comunicação I2C
   Soft_I2C_Write(0xD0); // Endereço do RTC é 0x68, porém o último bit é 0 para Write
                  // Assim ficamos com 0xD0
   Soft_I2C_Write(Adress);
   Soft_I2C_Write(x);
   Soft_I2C_Stop(); // Termina a comunicação
   delay_ms(100);
}

   
   unsigned char read_ds1307(unsigned char address)
     {
       unsigned char read_data;
       Soft_I2C_Start();
       Soft_I2C_Write(0xD0);    //Endereço do RTC + Write
       Soft_I2C_Write(address); //Endereço com a informação a ser lida
       Soft_I2C_Start();
       Soft_I2C_Write(0xD1);  // Endereça o ds1307 em modo leitura
       read_data = Soft_I2C_Read(0); // o valor lido é salvo na variavel read_data
       Soft_I2C_Stop();
       return (read_data);
     }
     

   void LCD_positionxy(unsigned char x, unsigned char y)
   {
         char i=0;
        if(y == 1)
        Lcd_Cmd(_LCD_FIRST_ROW);
        if(y == 2)
        Lcd_Cmd(_LCD_SECOND_ROW);

        while( i < x)  {
        Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
        i++;
        }
   }
     unsigned char MSB(unsigned char x)
     {
               return ((x >> 4)+'0');
     }

     unsigned char LSB(unsigned char x)
     {
               return((x & 0x0F)+'0');
     }
     
     void SetAlarm(unsigned char HoraMSB, unsigned char HoraLSB, unsigned char MinutoMSB, unsigned char MinutoLSB, unsigned char SegundoMSB, unsigned char SegundoLSB)
     {
      Ahour = ((HoraMSB << 4) | HoraLSB);

      AMin = ((MinutoMSB << 4) | MinutoLSB);

      Aseg = ((SegundoMSB << 4) | SegundoLSB);
     }
     
     void AlarmActivate(unsigned char HoraA,unsigned char MinA,unsigned char SegA)
     {
     if(a==1)
     {
        if(AHour == HoraA && AMin == MinA && ASeg == SegA)
      PORTB.RB6= 0;
     }
     }


void main() {

      i=0;
      TRISB = 0;
      PORTB = 0b01000000;
      TRISA = 0xFF;

      Soft_I2C_Init();
      Lcd_Init();
      Lcd_Cmd(_LCD_CURSOR_OFF) ;
      
      Lcd_out(1,1,"Time:");
      Lcd_out(2,1,"Date:");
      
      while(1)
            {
      second = read_ds1307(0);
       minute = read_ds1307(1);
       hour = read_ds1307(2);
       day = read_ds1307(4);
       month = read_ds1307(5);
       year = read_ds1307(6);

       time[0] = MSB(hour);
       time[1] = LSB(hour);
       time[3] = MSB(minute);
       time[4] = LSB(minute);
       time[6] = MSB(second);
       time[7] = LSB(second);
       date[0] = MSB(day);
       date[1] = LSB(day);
       date[3] = MSB(month);
       date[4] = LSB(month);
       date[6] = MSB(year);
       date[7] = LSB(year);

       Lcd_out(1,7, time);
       Lcd_out(2,7, date);
       delay_ms(50);
       AlarmActivate(hour,minute,second);

                    if(~Nchange)
                    {
                     time[9]=' ';
                     a=0;
                     delay_ms(300);
                     PORTB.RB6=1;
                     
                    }
                    
                    if(~Select){
                                 delay_ms(300);
                                 Lcd_Cmd(_LCD_CLEAR);
                                 Lcd_out(1,4, "Set Time/Date");
                                 Lcd_out(2,4, "Set Alarm");

                                  while(1)
                                  {
                                 if(~Nchange)
                                 {
                                 delay_ms(300);
                                 i++;
                                 }
                                 if(i%2 == 0)
                                 {
                                 Lcd_out(2,1, "   ");
                                 Lcd_out(1,1, "-->");
                                 }
                                 if(i%2 != 0)
                                 {
                                 Lcd_out(1,1, "   ");
                                 Lcd_out(2,1, "-->");
                                 }
                                 if(~Select)
                                 {
                                 delay_ms(180);
                                 break;
                                 }
                                 }
                                 
                                 Lcd_Cmd(_LCD_CLEAR);
                                 Lcd_out(1,1,"Time:");
                                 Lcd_out(2,1,"Date:");
                                 Lcd_out(1,7, time);
                                 Lcd_out(2,7, date);
                    
                              //Gravar MSB Hora

                             LCD_positionxy(6,1);
                             Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
                             AdjustTime(0);
                              
                               //Gravar LSB Hora

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustTime(1);
                              
                              //Gravar MSB minuto

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustTime(3);
                              
                               //Gravar LSB minuto

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustTime(4);
                              
                                 //Gravar MSB segundo

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustTime(6);

                               //Gravar LSB segundo

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustTime(7);
                              
                              //Gravar MSB DIA
                             if(i%2 == 0)
                             {
                             LCD_positionxy(6,2);
                             AdjustDate(0);
                              
                                    //Gravar LSB Dia

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustDate(1);
                              
                               //Gravar MSB mes

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustDate(3);

                                    //Gravar LSB mes

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustDate(4);
                              
                                          //Gravar MSB ano

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustDate(6);

                               //Gravar LSB ano

                             Lcd_Cmd(_LCD_MOVE_CURSOR_RIGHT);
                             AdjustDate(7);
                             }

                              
                               if(i%2 == 0)
                               {
                              gravarRTC(time[0],time[1],0x02);  //hora
                              gravarRTC(time[3],time[4],0x01);  //minuto
                              gravarRTC(time[6],time[7],0x00);   //segundo
                              gravarRTC(date[0],date[1], 0x04);   //dia
                              gravarRTC(date[3],date[4],0x05);     //mes
                              gravarRTC(date[6],date[7],0x06);   //ano
                              delay_ms(100);
                              }
                              if(i%2 != 0)
                               {
                                  SetAlarm(time[0],time[1],time[3],time[4],time[6],time[7]);
                                  a=1;
                                   time[9] = 'A';
                                 }
                               i=0;
                              Lcd_Cmd(_LCD_CURSOR_OFF);
                              delay_ms(300);
                             }
                    
                    
                    }

            }