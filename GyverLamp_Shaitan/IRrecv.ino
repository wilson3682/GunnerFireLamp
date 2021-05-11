/*
 в Constants.h вставить
 // ----ИК Управление
 #define IR_PIN                (5U)                          // D1 Пин, куда подключет IR 
 в setup() вставить
#ifdef IR_PIN
  ir_setup();
#endif
 в loop() вставить
#ifdef IR_PIN
  ir_loop();
#endif

# Сделано
0-60 - режим
90 - Включить все будильники
91-97 - Включить Будильник 91-Пн, 97-Вс
80 - Отключить все будильники
81-87 - Отключить Будильник 81-Пн, 87-Вс
1000-1255 Яркость 0-255
2000-2255 Скорость 0-255
3000-3100 Масштаб 0-100
up/down - Яркость
left/right - Громкость
ок - включить/выключить
10000-12359 - Будильник Понедельник
...
70000-72359 - Будильник Воскресенье
80000-82359 - Будильники Пн-Пт
90000-92359 - Будильники Сб-Вс
100-130 - громкость 0-30
200-230 - громкость голоса
99  - Включить режим избранное
98  - Включить текущий эффект в избранное
88  - Отключить режим избранное
89  - Исключить текущий эффект из избранного
CH- - Предыдущий эффект
CH+ - Следуэщий эффект
4000-4255 Яркость Рассвет 0-255 
5000-5999 - Задать время работы будильника Sleep после сигнала (мин)
300-399 - Выбор папки для воспроизведения (300 без звука)
400-499 - Выбор папки для воспроизведения Рассвет (400 без звука)
500-599 - мин. Sleep (500 отключить Sleep, 501 - через минуту, 599 - 99 мин)
  Ниже коды клавишь вписать свои
  
 */

#ifdef IR_PIN
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

//Подключение IR приемника с именем irrecv
//Используется в модуле 
   IRrecv irrecv(IR_PIN);

#define COUNT_PRESS              (10U)   // Количество кнопок с удержанием
#define COUNT_CLICK              (20U)   // Количество кнопок 

//Распределение кнопок в массиве   Имена для удобства восприятия
// с удержанием
#define _Press_1_UP      (0)
#define _Press_1_DOWN    (1)
#define _Press_2_UP      (2)
#define _Press_2_DOWN    (3)
#define _Press_3_UP      (4)
#define _Press_3_DOWN    (5)
#define _Press_4_UP      (6)
#define _Press_4_DOWN    (7)
#define _Press_5_UP      (8)
#define _Press_5_DOWN    (9)
// нажимные              
#define _Click_ON          (0) // Включить выключить
#define _Click_SPA         (1) // Stop/Play/Auto
#define _Click_MODE_UP     (2) // След. режим
#define _Click_MODE_DOWN   (3) // Пред. режим
#define _Click_MP3_UP      (4) // след. папка
#define _Click_MP3_DOWN    (5) // пред. папка
#define _Click_SAY_TIME    (6) // Сказать время
#define _Click_STAR        (7) // *
#define _Click_DIEZ        (8) // #

                           



class IRButton {
  public:
   void tick();
   uint32_t NumisClick();   // возвращает true при вводе числа. Сбрасывается после вызова

   bool IsPress(int num);          // возвращает true при нажатии на кнопку удержании. 
   bool IsClick(int num);          // возвращает true при клике. Сбрасывается после вызова

   bool pressed;
   bool pressed_num;
   void show_br_sp_sc();           // Выводит результат нажатия
   void set_br_sp_sc(int num);     // Задает вывод символа
   uint16_t br_sp_sc=6;
   bool show_symbol=false;         // Определяет нарисован ли символ и сохранен ли фон под символом
   void ReadMass();
  private:
   void WriteMass();
   void clearBTN();
   uint32_t IR_delay;
   uint32_t NUM_delay;
   uint32_t IR_XX;  
   bool IR_XX_S=false;  
   bool IR_XX_byte=false;  
   decode_results results;
   uint8_t symbol;
   uint32_t br_sp_sc_delay;
   bool _pressed[COUNT_PRESS]={false};
   bool _clicked[COUNT_CLICK]={false};
   CRGB mass_fon[WIDTH][8];
  //Коды клавиш цифр          0          1           2        3          4         5           6          7          8          9         
   uint64_t IR_Digit[10] = {0xFF6897,  0xFF30CF,  0xFF18E7, 0xFF7A85,  0xFF10EF,  0xFF38C7,  0xFF5AA5,  0xFF42BD, 0xFF4AB5,  0xFF52AD};   // 0..9    Китай 2
   uint64_t IR_Digit1[10]= {0xFF6897,  0xFF30CF,  0xFF18E7, 0xFF7A85,  0xFF10EF,  0xFF38C7,  0xFF5AA5,  0xFF42BD, 0xFF4AB5,  0xFF52AD};   // 0..9    Китай 2
};

void IRButton::clearBTN(){
  for (int i=0; i<COUNT_PRESS; i++)  _pressed[i]=false;
}

void IRButton::tick() {
  if (irrecv.decode(&results)) {
     settChanged = true;
     eepromTimeout = millis();
      switch (results.value)
      {
         case 0xFF906F:      // Китай 2 EQ
         case 0xE5CFBD7F:
         case 0xCF3204C7:    // WD Enter Переключает режим управления Яркость/Скорость/масштаб
         case 0x219E10EF:
           clearBTN();
           br_sp_sc=(br_sp_sc+2) % 10;
           set_br_sp_sc(1);
           break;
         case 0xFFA857:       // Китай2   +
         case 0xA3C8EDDB:
         case 0x219E906F:     // WD Right
         case 0x36D45A41: 
           clearBTN();
           _pressed[br_sp_sc]=true;
           pressed=true;
           IR_delay= millis();
           set_br_sp_sc(1);
           if (br_sp_sc==6) set_br_sp_sc(2);
           if (br_sp_sc==8) set_br_sp_sc(5);
           break;
         case 0xFFE01F:       // Китай 2   -
         case 0xF076C13B:
         case 0x6E060085:     // WD Left
         case 0x219EE01F:
           clearBTN();
           _pressed[br_sp_sc+1]=true;
           pressed=true;
           IR_delay= millis();
           set_br_sp_sc(1);
           if (br_sp_sc==6) set_br_sp_sc(2);
           if (br_sp_sc==8) set_br_sp_sc(5);
           break;
         case 0xFF629D:     //  Китай 2 CH
         case 0x511DBB:
         case 0x219E48B7:   //  WD Power
         case 0x36876503:
           clearBTN();
           _clicked[_Click_ON]=true;
           pressed=true;
           break;
        case 0x219E40BF:    // WD Prev
        case 0x55303BC7:   
        case 0xFFA25D:       // Китай 2   CH-
        case 0xE318261B:
           clearBTN();
           _clicked[_Click_STAR]=true;
           pressed=true;
           set_br_sp_sc(4);
           break;
        case 0xB1133625:   // WD Next
        case 0x219E807F:   
        case 0xFFE21D:       // Китай2  CH+
        case 0xEE886D7F:
           clearBTN();
           _clicked[_Click_DIEZ]=true;
           pressed=true;
           set_br_sp_sc(4);
           break;
       // Стрелки
        case 0x219EA05F: // WD UP Громкость +
        case 0x4B3012A1:
           clearBTN();
           _pressed[_Press_4_UP]=true;
           pressed=true;
           IR_delay= millis();
           set_br_sp_sc(2);
           br_sp_sc=6;
           break;
        case 0x94F3DC67:  // WD Stop
        case 0x219E20DF:
           mp3_mode=0;
           set_br_sp_sc(7);
           break;
        case 0x219E50AF: // WD Play/Pause
        case 0x6D98B8AB:
           if (mp3_mode==2) mp3_mode=1; else mp3_mode=2;
           set_br_sp_sc(7);
           break;
        case 0xFFC23D:   // Китай 2 stop/play
        case 0x20FE4DBB:   
           mp3_mode++;
           if (mp3_mode==3) mp3_mode=0;   
//           mp3_mode=mp3_mode % 3;
           set_br_sp_sc(7);
           break;
        case 0xFF9867:   // Китай 2 100+
        case 0x97483BFB:
           mp3_play_time=1;
           mp3_play_time_adv=false;
           break;
        case 0xFFB04F:   // Китай 2 200+
           mp3_play_time=1;
           mp3_play_time_adv=true;
//           espMode = 1U;
//           EepromManager::SaveEspMode(&espMode);
//           showWarning(CRGB::Blue, 1000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
//           ESP.restart();
         break;
        case 0x3D4EB4EB: // WD DOWN Громкость - 
        case 0x219E00FF:
           clearBTN();
           _pressed[_Press_4_DOWN]=true;
           pressed=true;
           IR_delay= millis();
           set_br_sp_sc(2);
           br_sp_sc=0;
           break;
        case 0x394C25A3:  //WD FF Следующая папка
        case 0x219E7887:
        case 0xFF02FD:     // Китай 2 Next
           clearBTN();
           if (modes[EFF_VOICE].Brightness%count_mp3_folders==count_mp3_folders-1) modes[EFF_VOICE].Brightness-=count_mp3_folders;
           if (modes[EFF_VOICE].Brightness+count_mp3_folders>255) modes[EFF_VOICE].Brightness-=count_mp3_folders;
           modes[EFF_VOICE].Brightness++;
           mp3_folder=modes[EFF_VOICE].Brightness;
           set_br_sp_sc(6);
         break;
        case 0xCF36E4E5: //WD REV Предыдущая папка
        case 0x219EF807:
        case 0xFF22DD:      // Китай 2 Prev
        case 0x52A3D41F:
           if (modes[EFF_VOICE].Brightness%count_mp3_folders==0) modes[EFF_VOICE].Brightness+=count_mp3_folders;
           modes[EFF_VOICE].Brightness--;
           mp3_folder=modes[EFF_VOICE].Brightness;
           set_br_sp_sc(6);
         break;
        case 0xFFFFFFFFFFFFFFFF: // Повтор
        case 0xFFFFFFFF:
           pressed=true;
           IR_delay= millis();
           break;
      }
         #ifdef USE_BLYNK
         updateRemoteBlynkParams();
         #endif
      
    // Ввод числа
    for (int i=0; i<10; i++) 
     if ((results.value==IR_Digit[i]) || (results.value==IR_Digit1[i])) 
     {
      if (millis() - NUM_delay > 2000) //Если число не вводилось более секунды назад
      {
        IR_XX=i;
        IR_XX_S=true;
        NUM_delay= millis();
      } else
      {
        if (IR_XX_S) IR_XX=IR_XX*10+i;
        NUM_delay= millis();
      } 
      set_br_sp_sc(3);
     } 
     
//    serialPrintUint64(results.value, HEX);
//    Serial.println();
    irrecv.resume(); // Receive the next value
  }
    //Таймаут ввода числа
    if (millis() - NUM_delay > 2000 && IR_XX_S) //По истечении 2000ms считается окончание ввода числа
    { 
      IR_XX_S=false;
      pressed_num=true;
    }
    if (millis() - IR_delay > 2000) clearBTN();
    
     
  
}
void IRButton::set_br_sp_sc(int num)
{
     br_sp_sc_delay= millis();
     symbol=num;
}
void IRButton::ReadMass()
{
 if (show_symbol)
  {
   for (int x=0; x<WIDTH; x++) 
    for (int y=0; y<8; y++) 
      drawPixelXY(x, y, mass_fon[x][y]);
   show_symbol=false;
  }
}

void IRButton::WriteMass()
{
 if (!show_symbol)
  {
   for (int x=0; x<WIDTH; x++) 
    for (int y=0; y<8; y++) 
     mass_fon[x][y] = getPixColorXY(x,y);
   show_symbol=true;
  }
}

void IRButton::show_br_sp_sc()
{

      if (millis()-br_sp_sc_delay>3000) br_sp_sc=0;  // Через 10 сек выбрать для ввода режим яркость
      else
      {
       WriteMass();
       switch (symbol)
       {
         case 1:                                                              // вывести яркость/скорость масштаб
           drawLetter(br_sp_sc/2, WIDTH/2-2, CRGB::Gray, false);  break; 
         case 2:                                                               // вывести громкость
           drawLetter(3, WIDTH/2-2, CRGB::Gray, false);  
           for (int i=map(set_mp3_volume,0,30,WIDTH/2,0);i<map(set_mp3_volume,0,30,WIDTH/2,WIDTH);i++) drawPixelXY(i, 0, CRGB::Gray); // шкала громкости
           break; 
         case 3: drawDigit(IR_XX, CRGB::Gray, false); break;                   // вывести число
         case 4: drawDigit(currentMode, CRGB::Gray, false); break;             // вывести номер режима
         case 5:                                                               // вывести громкость речи
           drawLetter(4, WIDTH/2-2, CRGB::Gray, false);  
           for (int i=map(set_adv_volume,0,30,WIDTH/2,0);i<map(set_adv_volume,0,30,WIDTH/2,WIDTH);i++) drawPixelXY(i, 0, CRGB::Gray); // шкала громкости голоса
           break; 
         case 6: drawDigit(mp3_folder, CRGB::Yellow, false); break;             // вывести номер папки
         case 7: drawLetter(mp3_mode+5,WIDTH/2-2, CRGB::Gray, false);  break;   // stop play auto
       }
      }
} 
  
uint32_t IRButton::NumisClick() {
  pressed=false;
  pressed_num=false;
//  Serial.println(IR_XX);
  return IR_XX;
}
bool IRButton::IsClick(int num) {
  if (pressed && _clicked[num])
    {
      pressed=false;
      _clicked[num]=false;
      return true;
    }
  return false;
}
bool IRButton::IsPress(int num) {
  if (pressed && _pressed[num])
    {
      pressed=false;
      return true;
    }
  return false;
}


IRButton IR_BTN;

void ir_setup()
{
// Serial.begin(115200);
 irrecv.enableIRIn(); // 
}
void ir_loop() {

  IR_BTN.tick(); //Опрос датчика Вставить в void loop() при использовании

  if (IR_BTN.pressed_num) {
      uint32_t IRBTN=IR_BTN.NumisClick();
      if (IRBTN<MODE_AMOUNT) {
           currentMode=IRBTN;
           FavoritesManager::FavoritesRunning = false;
           FavoritesManager::UseSavedFavoritesRunning=true;
           loadingFlag = true;
      }
      if (IRBTN>1000 && IRBTN<=1255)   processParams("BRI", String(IRBTN-1000).c_str());
      if (IRBTN>2000 && IRBTN<=2255)   processParams("SPD", String(IRBTN-2000).c_str());
      if (IRBTN>3000 && IRBTN<=3100)   processParams("SCA", String(IRBTN-3000).c_str());
      if (IRBTN>10000 && IRBTN<=92359) processParams("TXT-alarm", String(IRBTN).c_str());
      if (IRBTN>90 && IRBTN<=97) {               // Включить Будильник 91-Пн, 97-Вс
         alarms[IRBTN-91].State=true;   
         while(!fillString(String(IRBTN-90).c_str(), CRGB::Green)) { delay(1); ESP.wdtFeed(); }
      }
  
      if (IRBTN>80 && IRBTN<=87) {               // Отключить Будильник 81-Пн, 87-Вс
    
         alarms[IRBTN-81].State=false;
         while(!fillString(String(IRBTN-80).c_str(), CRGB::Red)) { delay(1); ESP.wdtFeed(); }
      }
      if (IRBTN==90) for (uint32_t i=0;i<7;i++) {// Включить все будильники

         alarms[i].State=true;
         while(!fillString("All", CRGB::Green)) { delay(1); ESP.wdtFeed(); }
      }
      if (IRBTN==80) for (uint32_t i=0;i<7;i++) { // Отключить все будильники
        alarms[i].State=false;
        while(!fillString("All", CRGB::Red)) { delay(1); ESP.wdtFeed(); }
      }
      if (IRBTN>=100 && IRBTN<131) modes[EFF_VOICE].Speed=(IRBTN-100); //Громкость
      if (IRBTN>=200 && IRBTN<231){modes[EFF_VOICE].Scale=(IRBTN-200); mp3_play_time=1;set_adv_volume=IRBTN-200;}//Громкость для голоса
      if (IRBTN==89) {                                     // Включить текущий эффект в избранное
        FavoritesManager::FavoriteModes[currentMode]=1;
        showWarning(CRGB::Green, 1000U, 500U);             // мигание цветом 0,5 секунды (1 раза)
    }  
      if (IRBTN==88) {            // 88 - Включить режим избранное
            FavoritesManager::FavoritesRunning = true;
            FavoritesManager::UseSavedFavoritesRunning=true;
      }
      if (IRBTN==98) {                                     //  Исключить текущий эффект из избранного
          FavoritesManager::FavoriteModes[currentMode]=0;
          showWarning(CRGB::Red, 1000U, 500U);               // мигание цветом 0,5 секунды (1 раза)
      }  
      if (IRBTN==99) {            // 99 - Отключить режим избранное
            FavoritesManager::FavoritesRunning = false;
            FavoritesManager::UseSavedFavoritesRunning=true;
      }

      if (IRBTN==290) {            // 
           espMode = 0U;
           EepromManager::SaveEspMode(&espMode);
           showWarning(CRGB::Blue, 1000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
           ESP.restart();
      }
      if (IRBTN==291) {            // 
           espMode = 1U;
           EepromManager::SaveEspMode(&espMode);
           showWarning(CRGB::Green, 1000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
           ESP.restart();
      }
      
      if (IRBTN>300 && IRBTN<301+count_mp3_folders) {           //Выбор папки 01-count_mp3_folders
           modes[EFF_VOICE].Brightness-=modes[EFF_VOICE].Brightness%count_mp3_folders;
           modes[EFF_VOICE].Brightness+=IRBTN-300;
           IR_BTN.set_br_sp_sc(6);
         }
      if (IRBTN==300) mp3_mode=0;
 
      if (IRBTN>=400 && IRBTN<401+count_mp3_folders) {           //Выбор папки Рассвет 00-count_mp3_folders
           DAWN_FOLDER=IRBTN-400;
           EEPROM.write(28,DAWN_FOLDER);
           EEPROM.commit();
         }
      if (IRBTN>4000 && IRBTN<=4255) {                           //Выбор яркости Рассвет 000-255
           DAWN_BRIGHT=IRBTN-4000;
           EEPROM.write(27,DAWN_BRIGHT);
           EEPROM.commit();           
         }
      if (IRBTN>=5000 && IRBTN<=5999) {                          //Выбор времени после Рассвет 000-999 минут
           TimeToFireEnd=(IRBTN-5000)*60;
           EepromManager::WriteInt32(29,TimeToFireEnd);
           EEPROM.commit();           
//           Serial.println(TimeToFireEnd);
         }
      //автовыключение через 01-99 минут
      if (IRBTN>500 && IRBTN<=599)  processParams("TMR_SET", String("1 1 "+String((IRBTN-500)*60)).c_str());   
      if (IRBTN==500) processParams("TMR_SET", "0 0 0");        //автовыключение выключено
   }
   
  uint8_t delta = modes[currentMode].Brightness < 10U?1U:count_mp3_folders;
  // Яркость
  if (IR_BTN.IsPress(_Press_1_UP  )) if (modes[currentMode].Brightness<255-count_mp3_folders)   modes[currentMode].Brightness+=delta;
  if (IR_BTN.IsPress(_Press_1_DOWN)) if (modes[currentMode].Brightness>delta) modes[currentMode].Brightness-=delta;
  FastLED.setBrightness(modes[currentMode].Brightness);
  delta = modes[currentMode].Speed < 10U?1U:5U; 
  if (IR_BTN.IsPress(_Press_2_UP  )) if (modes[currentMode].Speed<effects_name[currentMode].Speed_max) modes[currentMode].Speed+=delta;
  if (IR_BTN.IsPress(_Press_2_DOWN)) if (modes[currentMode].Speed>effects_name[currentMode].Speed_min+delta)      modes[currentMode].Speed-=delta;
  delta = modes[currentMode].Scale < 10U?1U:5U; 
  if (IR_BTN.IsPress(_Press_3_UP  )) if (modes[currentMode].Scale<effects_name[currentMode].Scale_max)        modes[currentMode].Scale+=delta;
  if (IR_BTN.IsPress(_Press_3_DOWN)) if (modes[currentMode].Scale>effects_name[currentMode].Scale_min+delta)      modes[currentMode].Scale-=delta;
  // Громкость
  delta = 1; 
  if (IR_BTN.IsPress(_Press_4_UP  )) {
         if (modes[EFF_VOICE].Speed<30)   modes[EFF_VOICE].Speed++;
         set_mp3_volume=modes[EFF_VOICE].Speed;
         }
  if (IR_BTN.IsPress(_Press_4_DOWN)) {
         if (modes[EFF_VOICE].Speed>0) modes[EFF_VOICE].Speed--;
         set_mp3_volume=modes[EFF_VOICE].Speed;
         }
  delta = 1; 
  if (IR_BTN.IsPress(_Press_5_UP  )) {
         if (modes[EFF_VOICE].Scale<30)   modes[EFF_VOICE].Scale++;
         set_adv_volume=constrain(modes[EFF_VOICE].Scale,0,MP3_MAX_VOLUME); 
         }
  if (IR_BTN.IsPress(_Press_5_DOWN)) {
         if (modes[EFF_VOICE].Scale>0) modes[EFF_VOICE].Scale--;
         set_adv_volume=constrain(modes[EFF_VOICE].Scale,0,MP3_MAX_VOLUME); 
         }
  
  if (IR_BTN.IsClick(_Click_ON)) {
    ONflag = !ONflag;
    if (ONflag)  processParams("P_ON", "");
        else     processParams("P_OFF", "");
  }
 
  if (IR_BTN.IsClick(_Click_STAR)) {            // Предыдущий эффект
    currentMode=(currentMode+MODE_AMOUNT-1) % MODE_AMOUNT;
    loadingFlag = true;
  }
  if (IR_BTN.IsClick(_Click_DIEZ)) {            // следующий эффект
    currentMode=(currentMode+1) % MODE_AMOUNT;
    loadingFlag = true;
  }
//-------------Пример использования

}
#endif

//Перевод времени с IR на time
uint16_t IRtoTime(uint32_t irt)
{
//  printTime((irt % 100) + irt/100*60,true,true);
  return (irt % 100) + irt/100*60;
}
