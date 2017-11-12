/*password
 user 1 = 12345;
*/
//******************************************************************************* 
#include <Keypad.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);
 
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11,12,A0,A1}; 
byte colPins[COLS] = {A2,A3,A4,A5}; 
 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
 

const int RELAY=10; 
const int BUZZER= 13; 
char keycount=0;
char code[5]; 
//*******************************************************************************
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
    
  pinMode(RELAY,OUTPUT);  
  pinMode(BUZZER,OUTPUT);  
  digitalWrite(RELAY,HIGH);
  lcd.setCursor(0,0); 
  lcd.print(" Voice And code ");
  lcd.setCursor(0,1); 
  lcd.print("   Lock System  ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Password & Voice");
  lcd.setCursor(0,1); 
  lcd.cursor();
  digitalWrite(BUZZER,LOW);      
  }

//*******************************************************************************

void serial_get_command()
{
  char inchar=0;
  char cmd_arr[30];
  int cmd_count=0;
  if(Serial.available() > 0)
  {
    inchar = Serial.read();    
    if(inchar == '*')
    {
      cmd_count=0;
      while(inchar != '#' && cmd_count<30)
      {
        if(Serial.available() > 0)
        {
          inchar = Serial.read();
          cmd_arr[cmd_count] = '\0';
        }        
      }
      if(inchar == '#')
      {
        cmd_arr[cmd_count-1] = '\0';
        if(strcmp(cmd_arr,"unlock")==0 || strcmp(cmd_arr,"please unlock")==0)
        {
         
          digitalWrite(RELAY,LOW);         
          lcd.setCursor(0,1);
          lcd.print("    WELCOME    ");
          delay(10000);    
          Lock();
          
        }
        else 
        {
          digitalWrite( BUZZER, HIGH);
          lcd.setCursor(0,1);lcd.print(F("Voice Cmd Err   "));
          delay(2000); 
          Lock();
        }
      }
    }
  }
}

//*******************************************************************************
void loop(){
  serial_get_command();
  char customKey = customKeypad.getKey();
  if (customKey && (keycount<5) && (customKey !='=') && (customKey !='C'))
  {      
      
      lcd.print('*');
      Serial.println(customKey);    
      code[keycount]=customKey;
      keycount++;
  }
  
  if(customKey == 'C')   
  {
    Lock();   
  }
  
  if(customKey == 'A')   // PASSWORD WRITE GET PREES "A"
  {
    if((code[0]=='1') && (code[1]=='2') && (code[2]=='3') && (code[3]=='4') && (code[4]=='5'))  
    {
      digitalWrite(RELAY,LOW);         
      lcd.setCursor(0,1);
      lcd.print("    WELCOME    ");
      delay(5000);    
      Lock();
    }
    
   
    else
    {
      lcd.setCursor(0,1);
      lcd.print("Invalid Password");
      digitalWrite(BUZZER,HIGH);        
      delay(2000); 
      Lock();
    } 
  }
}
 
//*******************************************************************************
void Lock()
{
    lcd.setCursor(0,1);
    lcd.print("Door Locked     ");
    digitalWrite(RELAY,HIGH);  
    digitalWrite(BUZZER,LOW);  
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("                "); 
    lcd.setCursor(0,1);
    keycount=0;
              
}
//*******************************************************************************
