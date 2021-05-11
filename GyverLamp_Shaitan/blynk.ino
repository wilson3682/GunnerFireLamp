#ifdef USE_BLYNK
#define CYCLE_DONT_OFF          (1U)          // Не отключать режим Цикл при выключении лампы = 1U, отключать = 0U
#define CYCLE_TIMER             (60U)         // Интервал смены эффектов 60 секунд
#define CYCLE_TIMER_PLUS        (0U)          // + случайное время от нуля до 0U секунд
#define CYCLE_1ST_EFFECT        (2U)          // Эффекты до "2. Смена цвета" не будут демонстрироваться (в Блинке это номер 3)
#define CYCLE_LAST_EFFECT       (79U)         // Эффекты после "79. Блуждающий кубик" не будут демонстрироваться (в Блинке это номер 80)

uint8_t V4_Mode;
uint8_t Blynk_H;
uint8_t Blynk_M;

BLYNK_CONNECTED()
{
  updateRemoteBlynkParams();
}
BLYNK_WRITE(V0)
{
  String action = param.asStr();

  if (action == "play") {
    processParams("P_ON", "");
  }
  if (action == "stop") {
    processParams("P_OFF", "");
  }
  if (action == "next") {
    currentMode=(currentMode+1) % MODE_AMOUNT;
    loadingFlag = true;
  }
  if (action == "prev") {
    currentMode=(currentMode+MODE_AMOUNT-1) % MODE_AMOUNT;
    loadingFlag = true;
  }
 V4_Mode=currentMode;
 updateRemoteBlynkParams();
 settChanged = true;
 eepromTimeout = millis();
}

BLYNK_WRITE(V1)
{
  if (V4_Mode<MODE_AMOUNT)   processParams("BRI", param.asString());
  else if (V4_Mode<MODE_AMOUNT+7) {
    Blynk_H=param.asInt();
    processParams("TXT-alarm", String((V4_Mode-MODE_AMOUNT+1)*10000+Blynk_H*100+Blynk_M).c_str());
  }
 settChanged = true;
 eepromTimeout = millis();
}

BLYNK_WRITE(V2)
{
  if (V4_Mode<MODE_AMOUNT) {
    processParams("SPD", param.asString());
    updateRemoteBlynkParams();
  }
  else if (V4_Mode<MODE_AMOUNT+7) {
    Blynk_M=param.asInt();
    processParams("TXT-alarm", String((V4_Mode-MODE_AMOUNT+1)*10000+Blynk_H*100+Blynk_M).c_str());
  }
 settChanged = true;
 eepromTimeout = millis();
}

BLYNK_WRITE(V3)
{
  if (V4_Mode<MODE_AMOUNT) {
  processParams("SCA", param.asString());
  updateRemoteBlynkParams();
  }
  else if (V4_Mode<MODE_AMOUNT+7) {
    if (param.asInt()==0) {
      processParams("TXT-alarm", String(String(V4_Mode-MODE_AMOUNT+1)+" off").c_str());
      BLYNK_WRITE_ALARM();    }
    else {
      processParams("TXT-alarm", String(String(V4_Mode-MODE_AMOUNT+1)+" on").c_str());
      BLYNK_WRITE_ALARM();    }
  }
 settChanged = true;
 eepromTimeout = millis();
}

BLYNK_WRITE(V4)
{
  V4_Mode = param.asInt() - 1;
  if (V4_Mode<MODE_AMOUNT) {
    currentMode=V4_Mode;
    loadingFlag = true;
    updateRemoteBlynkParams();
#ifdef IR_PIN
    IR_BTN.set_br_sp_sc(4);
#endif
    settChanged = true;
    eepromTimeout = millis();
 } else
  if (V4_Mode<MODE_AMOUNT+7) BLYNK_WRITE_ALARM();

}


BLYNK_WRITE(V6)
{
  int value = param.asInt();
  modes[EFF_VOICE].Speed= value;
  set_mp3_volume=value;
  if (currentMode==EFF_VOICE) updateRemoteBlynkParams();
  settChanged = true;
  eepromTimeout = millis();
}

BLYNK_WRITE(V10)
{
    String action = param.asStr();
    processParams("TXT-", String(action).c_str());
    settChanged = true;
    eepromTimeout = millis();
}

void updateRemoteBlynkParams()
{
  if (ONflag==1) {
     String vaf_str="";
     if (FavoritesManager::FavoritesRunning) vaf_str="FavOn ";
     vaf_str+=String(currentMode)+"."+effects_name[currentMode].Name;
     if (TimerManager::TimerRunning)
          { 
            uint32_t blynk_sleep=(TimerManager::TimeToFire-millis())/1000;
            if (blynk_sleep>100) vaf_str+=" off "+String(blynk_sleep/60)+"мин";
                           else  vaf_str+=" off "+String(blynk_sleep)+"сек";
          }
     Blynk.setProperty(V0, "label", vaf_str);    
     Blynk.virtualWrite(V0, "play");
  } else {
        Blynk.setProperty(V0, "label", "Disable");
        Blynk.virtualWrite(V0, "stop");
  }
 
  // Blynk.virtualWrite(V0, ONflag ? 1 : 0);
  if (V4_Mode<MODE_AMOUNT) BLYNK_WRITE_EFFECT();
  else if (V4_Mode<MODE_AMOUNT+7) BLYNK_WRITE_ALARM();

}

void BLYNK_WRITE_EFFECT()
{
  Blynk.setProperty(V1, "label", "Яркость");    
  Blynk.setProperty(V1, "min", 1);
  Blynk.setProperty(V1, "max", 255);
  Blynk.virtualWrite(V1, modes[currentMode].Brightness);
  
//  Blynk.setProperty(V2, "min", effects_name[currentMode].Speed_min);
//  Blynk.setProperty(V2, "max", effects_name[currentMode].Speed_max);
  Blynk.setProperty(V2, "min", 1);
  Blynk.setProperty(V2, "max", 255);
  Blynk.virtualWrite(V2, modes[currentMode].Speed);

//  Blynk.setProperty(V3, "min", effects_name[currentMode].Scale_min);
//  Blynk.setProperty(V3, "max", effects_name[currentMode].Scale_max);
  Blynk.setProperty(V3, "min", 1);
  Blynk.setProperty(V3, "max", 100);
  Blynk.virtualWrite(V3, modes[currentMode].Scale);

  Blynk.virtualWrite(V4, currentMode + 1);
  Blynk.virtualWrite(V6, modes[EFF_VOICE].Speed);

  // Раскраска избранного
  if (FavoritesManager::FavoriteModes[currentMode]==0) Blynk.setProperty(V4, "color", "#CC5555");
                                                  else Blynk.setProperty(V4, "color", "#55CC55");

      switch (currentMode) {
         case EFF_FIRE:
         case EFF_FIRE2012:
         case EFF_COLOR:
         case EFF_CUBE:           
         case EFF_WATERPOLL:      
         case EFF_FIRE_2020:  
         case EFF_TWINKLES:     
         case EFF_RAIN:           
         case EFF_SMOKE:          
         case EFF_BUTTERFLYS_LAMP:
         case EFF_LIQUIDLAMP:    
         case EFF_TEXT:           
           Blynk.setProperty(V2, "color", "#AA8888");
           Blynk.setProperty(V2, "label", "Скорость");    
           Blynk.setProperty(V3, "color", CRGB_to_String(modes[currentMode].Scale * 2.55));
           Blynk.setProperty(V3, "label", "Цвет");    
         break;
         case EFF_CLOCK:          
           Blynk.setProperty(V2, "color", CRGB_to_String(modes[currentMode].Speed));
           Blynk.setProperty(V2, "label", "Цвет часовой стрелки");    
           Blynk.setProperty(V3, "color", CRGB_to_String(modes[currentMode].Scale * 2.55));
           Blynk.setProperty(V3, "label", "Цвет минутной стрелки");    
         break;
         case EFF_WHITE_COLOR:
           Blynk.setProperty(V2, "color", "#AA8888");                                  
           Blynk.setProperty(V2, "label", "Теплота цвета");    
           Blynk.setProperty(V3, "color", "#8888AA");
           Blynk.setProperty(V3, "label", "Масштаб");    
         break;
         case EFF_VOICE:
           Blynk.setProperty(V2, "color", "#66AA66");
           Blynk.setProperty(V2, "label", "Громкость музыки");    
           Blynk.setProperty(V3, "color", "#88AAAA");
           Blynk.setProperty(V3, "label", "Громкость речи");    
           Blynk.setProperty(V2, "min", 0);
           Blynk.setProperty(V2, "max", 30);
           Blynk.setProperty(V3, "min", 0);
           Blynk.setProperty(V3, "max", 30);
         break;
         case EFF_COLORS:
           Blynk.setProperty(V2, "color", "#AA8888");
           Blynk.setProperty(V2, "label", "Скорость");    
           Blynk.setProperty(V3, "color", "#8888AA");
           Blynk.setProperty(V3, "label", "Насыщенность");    
         break;
         case EFF_COLOR_RAIN:     
           Blynk.setProperty(V2, "color", "#AA8888");
           Blynk.setProperty(V2, "label", "Насыщенность");    
           Blynk.setProperty(V3, "color", CRGB_to_String(modes[currentMode].Scale * 2.55));
           Blynk.setProperty(V3, "label", "Цвет");    
         default:
           Blynk.setProperty(V2, "color", "#AA8888");
           Blynk.setProperty(V2, "label", "Скорость");    
           Blynk.setProperty(V3, "color", "#8888AA");
           Blynk.setProperty(V3, "label", "Масштаб");    
         break;
      }
}

void BLYNK_WRITE_ALARM()
{

          Blynk.setProperty(V4, "color", "#1111FF");
          Blynk_H=alarms[V4_Mode-MODE_AMOUNT].Time / 60;
          Blynk.setProperty(V1, "color", "#AA0000");
          Blynk.setProperty(V1, "label", "Часы");    
          Blynk.setProperty(V1, "min", 0);
          Blynk.setProperty(V1, "max", 23);
          Blynk.virtualWrite(V1, Blynk_H);
          
          Blynk_M=alarms[V4_Mode-MODE_AMOUNT].Time % 60;
          Blynk.setProperty(V1, "color", "#AA0000");
          Blynk.setProperty(V2, "label", "Минуты");    
          Blynk.setProperty(V2, "min", 0);
          Blynk.setProperty(V2, "max", 59);
          Blynk.virtualWrite(V2, Blynk_M);
          
          Blynk.setProperty(V3, "min", 0);
          Blynk.setProperty(V3, "max", 1);
          if (alarms[V4_Mode-MODE_AMOUNT].State==true) {
              Blynk.virtualWrite(V3, 1);
              Blynk.setProperty(V3, "color", "#00AA00");
              Blynk.setProperty(V3, "label", "Включен");    
          } else {
              Blynk.virtualWrite(V3, 0);
              Blynk.setProperty(V3, "color", "#AA0000");
              Blynk.setProperty(V3, "label", "Выключен");    
          }
  
}

String CRGB_to_String(uint8_t x)
{
      String   Blynk_color_s;
      uint32_t Blynk_color_u;
      CRGB Blynk_color_c;

      Blynk_color_c = CHSV(x, 255U, 255U);
      Blynk_color_u= (Blynk_color_c.r << 16) | (Blynk_color_c.g << 8 ) | Blynk_color_c.b;
      if (Blynk_color_s.length()>7) Blynk_color_s.remove(1,Blynk_color_s.length()-7);
      Blynk_color_s = "#000000"+String(Blynk_color_u,HEX);
      if (Blynk_color_s.length()>7) Blynk_color_s.remove(1,Blynk_color_s.length()-7);

      return Blynk_color_s;
}
#endif

void processParams(char *prefix, const char *paramValue)
{
  char charBuf[80];
  String value = prefix + String(paramValue);
  value.toCharArray(charBuf, 80);
  processInputBuffer(charBuf, NULL, false);
}
