#ifdef USE_NTP

#define RESOLVE_INTERVAL      (5UL * 60UL * 1000UL)                       // интервал проверки подключения к интеренету в миллисекундах (5 минут)
                                                                          // при старте ESP пытается получить точное время от сервера времени в интрнете
                                                                          // эта попытка длится RESOLVE_TIMEOUT
                                                                          // если при этом отсутствует подключение к интернету (но есть WiFi подключение),
                                                                          // модуль будет подвисать на RESOLVE_TIMEOUT каждое срабатывание таймера, т.е., 3 секунды
                                                                          // чтобы избежать этого, будем пытаться узнать состояние подключения 1 раз в RESOLVE_INTERVAL (5 минут)
                                                                          // попытки будут продолжаться до первой успешной синхронизации времени
                                                                          // до этого момента функции будильника работать не будут
                                                                          // интервал последующих синхронизаций времени определяён в NTP_INTERVAL (30 минут)
                                                                          // при ошибках повторной синхронизации времени функции будильника отключаться не будут
#define RESOLVE_TIMEOUT       (1500UL)                                    // таймаут ожидания подключения к интернету в миллисекундах (1,5 секунды)
uint64_t lastResolveTryMoment = 0UL;
uint64_t lasttimeClientupdate = 0UL;
IPAddress ntpServerIp = {0, 0, 0, 0};
static CHSV dawnColor = CHSV(0, 0, 0);                                    // цвет "рассвета"
static CHSV dawnColorMinus1 = CHSV(0, 0, 0);                              // для большей плавности назначаем каждый новый цвет только 1/10 всех диодов; каждая следующая 1/10 часть будет "оставать" на 1 шаг
static CHSV dawnColorMinus2 = CHSV(0, 0, 0);
static CHSV dawnColorMinus3 = CHSV(0, 0, 0);
static CHSV dawnColorMinus4 = CHSV(0, 0, 0);
static CHSV dawnColorMinus5 = CHSV(0, 0, 0);
static uint8_t dawnCounter = 0;                                           // счётчик первых 10 шагов будильника


void timeTick()
{
    if (timeTimer.isReady())
    {
      if (!ntpServerAddressResolved)
      {
        if (millis() - lastResolveTryMoment >= RESOLVE_INTERVAL || lastResolveTryMoment == 0)
        {
          Serial.println("resolveNtpServerAddress(ntpServerAddressResolved);// пытаемся получить IP адрес сервера времени (тест интернет подключения) до тех пор, пока время не будет успешно синхронизировано");
          resolveNtpServerAddress(ntpServerAddressResolved);              // пытаемся получить IP адрес сервера времени (тест интернет подключения) до тех пор, пока время не будет успешно синхронизировано
          lastResolveTryMoment = millis();
        }
        if (!ntpServerAddressResolved && !timeSets)
        {
          return;                                                         // если нет интернет подключения и время не установлено, отключаем будильник до тех пор, пока оно не будет восстановлено
        }
      }
      if (millis() - lasttimeClientupdate >= NTP_INTERVAL || lasttimeClientupdate == 0)
      {
      if (ntpServerAddressResolved)  // если время синхронизировано, устанавливаем время на ардуинке
        {
          Serial.println("timeClient.update()");  
          if (timeClient.update()){
             timeSynched = true;
             manualTimeShift = localTimeZone.toLocal(timeClient.getEpochTime());// - millis() / 1000UL;
             setTime(manualTimeShift);
          }
        }
        lasttimeClientupdate = millis();      
      }

      if (!timeSynched && !timeSets)  // если время не было синхронизиировано или установлено
      {
        return; // отключаем будильник до тех пор, пока оно не будет восстановлено
      }

      time_t currentLocalTime = now(); // берем время с ардуинки
      
      uint8_t thisDay = dayOfWeek(currentLocalTime);
      if (thisDay == 1) thisDay = 8;                                      // в библиотеке Time воскресенье - это 1; приводим к диапазону [0..6], где воскресенье - это 6
      thisDay -= 2;
      thisTime = hour(currentLocalTime) * 60 + minute(currentLocalTime);
      uint32_t thisFullTime = hour(currentLocalTime) * 3600 + minute(currentLocalTime) * 60 + second(currentLocalTime);

      printTime(thisTime, false, ONflag);                                 // проверка текущего времени и его вывод (если заказан и если текущее время соответстует заказанному расписанию вывода)

      // проверка рассвета
      if (alarms[thisDay].State &&                                                                                          // день будильника
          thisTime >= (uint16_t)constrain(alarms[thisDay].Time - pgm_read_byte(&dawnOffsets[dawnMode]), 0, (24 * 60)) &&    // позже начала
          thisTime < (alarms[thisDay].Time + DAWN_TIMEOUT))                                                                 // раньше конца + минута
      {
        if (!manualOff)                                                   // будильник не был выключен вручную (из приложения или кнопкой)
        {
          // величина рассвета 0-255
          int32_t dawnPosition = 255 * ((float)(thisFullTime - (alarms[thisDay].Time - pgm_read_byte(&dawnOffsets[dawnMode])) * 60) / (pgm_read_byte(&dawnOffsets[dawnMode]) * 60));
          dawnPosition = constrain(dawnPosition, 0, 255);
          //тут можно привязать громкость map(dawnPosition, 0, 255, 1, 30)
          //и запустить проигрывание мелодии, если она не играет
          dawnColorMinus5 = dawnCounter > 4 ? dawnColorMinus4 : dawnColorMinus5;
          dawnColorMinus4 = dawnCounter > 3 ? dawnColorMinus3 : dawnColorMinus4;
          dawnColorMinus3 = dawnCounter > 2 ? dawnColorMinus2 : dawnColorMinus3;
          dawnColorMinus2 = dawnCounter > 1 ? dawnColorMinus1 : dawnColorMinus2;
          dawnColorMinus1 = dawnCounter > 0 ? dawnColor : dawnColorMinus1;
          dawnColor = CHSV(map(dawnPosition, 0, 255, 10, 35),
                           map(dawnPosition, 0, 255, 255, 170),
                           map(dawnPosition, 0, 255, 10, DAWN_BRIGHT));
          dawnCounter++;
          // fill_solid(leds, NUM_LEDS, dawnColor);
          for (uint16_t i = 0U; i < NUM_LEDS; i++)
          {
            if (i % 6 == 0) leds[i] = dawnColor;                          // 1я 1/10 диодов: цвет текущего шага
            if (i % 6 == 1) leds[i] = dawnColorMinus1;                    // 2я 1/10 диодов: -1 шаг
            if (i % 6 == 2) leds[i] = dawnColorMinus2;                    // 3я 1/10 диодов: -2 шага
            if (i % 6 == 3) leds[i] = dawnColorMinus3;                    // 3я 1/10 диодов: -3 шага
            if (i % 6 == 4) leds[i] = dawnColorMinus4;                    // 3я 1/10 диодов: -4 шага
            if (i % 6 == 5) leds[i] = dawnColorMinus5;                    // 3я 1/10 диодов: -5 шагов
          }
          FastLED.setBrightness(255);
          delay(1);
          FastLED.show();
          dawnFlag = true;
// Start Shaitan
#ifdef MP3_TX_PIN
          if (mp3_volume!=map(dawnPosition, 0, 255, 0, set_mp3_volume))
          {
             mp3_volume=map(dawnPosition, 0, 255, 0, set_mp3_volume); // Нарастание громкости от 0 до установленной
             command(0x06, 0,mp3_volume); // Передать на плеер изменеие громкости
             Serial.println("Alarm_mp3_volume"+String(mp3_volume));
          }
          if (DAWN_FOLDER>0)
          {
             set_mp3_play_now=true;          //Включить плеер
             mp3_time_test_play_now=true;    //Разрешить доступ к mp3
             mp3_folder=DAWN_FOLDER;         //Будильник из папки с номером DAWN_FOLDER
          }
#endif
// End Shaitan
        }

        #if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
        if (thisTime == alarms[thisDay].Time)                             // установка, только в минуту, на которую заведён будильник
        {
          digitalWrite(ALARM_PIN, manualOff ? !ALARM_LEVEL : ALARM_LEVEL);// установка сигнала в зависимости от того, был ли отключен будильник вручную
          //Остановить MP3
        }
        #endif

        #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, матрица должна быть включена на время работы будильника
        digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
        #endif
      }
      else
      {
        // не время будильника (ещё не начался или закончился по времени)
        if (dawnFlag)
        {
          dawnFlag = false;
//          FastLED.clear();
//          delay(2);
//          FastLED.show();
            changePower();                                                  // выключение матрицы или установка яркости текущего эффекта в засисимости от того, была ли включена лампа до срабатывания будильника
// Start Shaitan
            if (!ONflag) //Если была выключена, включить
            {
              ONflag = true;                                 
              //Запустить таймер на 30 мин
              TimerManager::TimerRunning = true;
              TimerManager::TimerHasFired = false;
//              Serial.println(TimeToFireEnd);
              TimerManager::TimeToFire = millis() + TimeToFireEnd * 1000; 
              //Записать состояние в EPROM
              settChanged = true;
              eepromTimeout = millis();
              loadingFlag = true;
            }
            // Включить определенный режим, Например облака
            currentMode=EFF_CLOUDS;
            // Включить режим избраннное
            FavoritesManager::FavoritesRunning = true;
            FavoritesManager::UseSavedFavoritesRunning=true;
#ifdef MP3_TX_PIN
            mp3_time_test_play_now=true; //Разрешить доступ к mp3
            //Сказать время
            mp3_play_time=1;
#endif
// End Shaitan
          
        }
        manualOff = false;
        dawnColorMinus1 = CHSV(0, 0, 0);
        dawnColorMinus2 = CHSV(0, 0, 0);
        dawnColorMinus3 = CHSV(0, 0, 0);
        dawnColorMinus4 = CHSV(0, 0, 0);
        dawnColorMinus5 = CHSV(0, 0, 0);
        dawnCounter = 0;

        #if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
        digitalWrite(ALARM_PIN, !ALARM_LEVEL);
        #endif

        #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
        digitalWrite(MOSFET_PIN, ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
        #endif
      }
    }
}

void resolveNtpServerAddress(bool &ntpServerAddressResolved)              // функция проверки подключения к интернету
{
  if (ntpServerAddressResolved)
  {
    return;
  }

  WiFi.hostByName(NTP_ADDRESS, ntpServerIp, RESOLVE_TIMEOUT);
  if (ntpServerIp[0] <= 0)
  {
    #ifdef GENERAL_DEBUG
    if (ntpServerAddressResolved)
    {
      LOG.println(F("Подключение к интернету отсутствует"));
    }
    #endif

    ntpServerAddressResolved = false;
  }
  else
  {
    #ifdef GENERAL_DEBUG
    if (!ntpServerAddressResolved)
    {
      LOG.println(F("Подключение к интернету установлено"));
    }
    #endif

    ntpServerAddressResolved = true;
  }
}

void getFormattedTime(char *buf)
{
//  time_t currentLocalTime = localTimeZone.toLocal(timeClient.getEpochTime());
  //time_t currentLocalTime = getCurrentLocalTime();
  time_t currentLocalTime = now();
  sprintf_P(buf, PSTR("%02u:%02u:%02u"), hour(currentLocalTime), minute(currentLocalTime), second(currentLocalTime));
}
#endif


//Тут дополнительные библиотеки для работы с будильником

void set_time_alarm(uint32_t v_cl)
{
//         Serial.println(v_cl);
         for (uint32_t i=1;i<8;i++)                 // Будильник
           if (v_cl>i*10000 && v_cl<=2359+i*10000) {
              alarms[i-1].Time=IRtoTime(v_cl-i*10000);
              #ifdef USE_BLYNK
                Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[i]+" "+String(v_cl-i*10000));    
              #endif
              uint8_t alarmNum=i-1;
              EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
          } 
          if (v_cl>80000 && v_cl<=2359+80000) {      // Будильники пн-пт
              for (uint32_t i=1;i<6;i++) {
                alarms[i-1].Time=IRtoTime(v_cl-80000);
                uint8_t alarmNum=i-1;
                EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
              }
              #ifdef USE_BLYNK
                Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[8]+" "+String(v_cl-80000));    
              #endif
          }  
          if (v_cl>90000 && v_cl<=2359+90000)  {    // Будильники сб-вс
              for (uint32_t i=6;i<7;i++){
                alarms[i-1].Time=IRtoTime(v_cl-90000);
                uint8_t alarmNum=i-1;
                EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
              }
              #ifdef USE_BLYNK
                Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[9]+" "+String(v_cl-90000));    
              #endif
          }  
         
}

void set_status_alarm(String BFF)
{
          uint32_t v_cl = BFF.substring(9, 10).toInt();
//          Serial.println(v_cl);
          if (v_cl>0 && v_cl<8) 
             {         
              if (BFF.indexOf("on")>8)
                 { 
                   alarms[v_cl-1].State=true;
                   #ifdef USE_BLYNK
                   Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[v_cl]+" On");    
                   #endif
                 };
              if (BFF.indexOf("off")>8)
                 { 
                   alarms[v_cl-1].State=false;
                   #ifdef USE_BLYNK
                   Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[v_cl]+" Off");    
                   #endif
                 };
              uint8_t alarmNum=v_cl-1;
              EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
             };   
          if (v_cl==8) {
              for (uint32_t i=1;i<6;i++) {               // Будильник
                  if (BFF.indexOf("on")>8) alarms[i-1].State=true;
                  else if (BFF.indexOf("off")>8) alarms[v_cl].State=false;
                  uint8_t alarmNum=i-1;
                  EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
              };   
             #ifdef USE_BLYNK
              if (BFF.indexOf("on")>8) Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[8]+" On");    
              if (BFF.indexOf("off")>8) Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[8]+" Off");    
             #endif
          }
          if (v_cl==9) {
              for (uint32_t i=6;i<8;i++) {               // Будильник
                  if (BFF.indexOf("on")>8) alarms[i-1].State=true;
                  else if (BFF.indexOf("off")>8) alarms[v_cl].State=false;
                  uint8_t alarmNum=i-1;
                  EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
          };   
             #ifdef USE_BLYNK
              if (BFF.indexOf("on")>8) Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[8]+" On");    
              if (BFF.indexOf("on")>8) Blynk.setProperty(V0, "label", "Alarm "+NameDayFoWeek[8]+" Off");    
             #endif
          }
}

//Возврат текущего времени
time_t getCurrentLocalTime()
{
/*  #if defined(USE_NTP) 
  if (timeSynched)
  {
    if (ntpServerAddressResolved)
     {
      Serial.println("NTP time"+String(hour(timeClient.getEpochTime()))+":"+String(minute(timeClient.getEpochTime())));
      return localTimeZone.toLocal(timeClient.getEpochTime());
     } 
  }
  #endif
*/  
//   Serial.println("Local time"+String(hour())+":"+String(minute()));
   return now();
}
