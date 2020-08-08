#ifndef __LCD_H__
#define __LCD_H__

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>  
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 16, 2); //lcd 객체 선언
int lcdflag = 0;

void UpdateLCDbyRegister(){
    int Register = digitalRead(A0);
    if(lcdflag == 0 && Register == 1)
  lcd.begin();
  if(Register == 0){
    lcdflag = 0;
  }else{
    lcdflag = 1;
  }

}

void printGestureRunning(){
    lcd.setCursor(0, 0);
    lcd.print("Gesture Running");
    lcd.setCursor(15, 0); 
}

void printBluetoothOkay(){
    lcd.setCursor(0, 0);
    lcd.print("Bluetooth Okay. ");
    lcd.setCursor(15, 0); 
}

#endif