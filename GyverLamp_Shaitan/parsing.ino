//
void parseUDP()
{
  int32_t packetSize = Udp.parsePacket();

  if (packetSize)
  {
    int16_t n = Udp.read(packetBuffer, MAX_UDP_BUFFER_SIZE);
    packetBuffer[n] = '\0';
    strcpy(inputBuffer, packetBuffer);

    #ifdef GENERAL_DEBUG
    LOG.print(F("Inbound UDP packet: "));
    LOG.println(inputBuffer);
    #endif

    Serial.print("Входящая строка - ");
    Serial.println(inputBuffer);

    if (Udp.remoteIP() == WiFi.localIP())                   // не реагировать на свои же пакеты
    {
      return;
    }

    char reply[MAX_UDP_BUFFER_SIZE];
    processInputBuffer(inputBuffer, reply, true);

    #if (USE_MQTT)                                          // отправка ответа выполнения команд по MQTT, если разрешено
    if (espMode == 1U)
    {
      strcpy(MqttManager::mqttBuffer, reply);               // разрешение определяется при выполнении каждой команды отдельно, команды GET, DEB, DISCOVER и OTA, пришедшие по UDP, игнорируются (приходят раз в 2 секунды от приложения)
    }
    #endif
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(reply);
    Udp.endPacket();

    #ifdef GENERAL_DEBUG
    LOG.print(F("Outbound UDP packet: "));
    LOG.println(reply);
    LOG.println();
    #endif
  }
}


void processInputBuffer(char *inputBuffer, char *outputBuffer, bool generateOutput)
{
    char buff[MAX_UDP_BUFFER_SIZE], *endToken = NULL;
    String BUFF = String(inputBuffer);
    if (!strncmp_P(inputBuffer, PSTR("DEB"), 3))
    {
        #ifdef USE_NTP
        getFormattedTime(inputBuffer);
        sprintf_P(inputBuffer, PSTR("OK %s"), inputBuffer);
        #else
        strcpy_P(inputBuffer, PSTR("OK --:--"));
        #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("GET"), 3))
    {
        memcpy(buff, &inputBuffer[4], strlen(inputBuffer)); 
        time_t tttt = atoi(buff);
        if (tttt>10000) { 
           setTime(tttt);
           timeSets=true;
           Serial.println("GetTimeShift "+String(tttt));
           Serial.println("input time "+String(hour())+":"+String(minute()));
        }
      sendCurrent(inputBuffer);
    }
    
    else if (!strncmp_P(inputBuffer, PSTR("TXT"), 3)) {     // Принимаем текст для бегущей строки
        if (!strncmp_P(inputBuffer, PSTR("TXT-time="), 9) && (BUFF.length() > 15)){ 
            // 0000000000111111
            // 0123456789012345
            // TXT-time=07:25 7
            uint8_t mtH = BUFF.substring(9, 11).toInt();
            uint8_t mtM = BUFF.substring(12, 14).toInt();
            uint8_t mtD = BUFF.substring(15, 16).toInt();
            if (mtH < 24U && mtM < 60U && mtD < 8U && mtD > 0U){
              manualTimeShift = (((3650UL + mtD) * 24UL + mtH) * 60UL + mtM) * 60UL - millis() / 1000UL; // 3650 дней (521 полная неделя + 3 дня для сдвига на понедельник???)
              setTime(manualTimeShift);
              timeSets = true;
              showWarning(CRGB::Blue, 2000U, 500U);     // мигание голубым цветом 2 секунды (2 раза) - время установлено
            }
            else
              showWarning(CRGB::Red, 2000U, 500U);      // мигание красным цветом 2 секунды (ошибка)
        }
      if (!strncmp_P(inputBuffer, PSTR("TXT-fav"), 7)){
        if (BUFF.substring(7, 12).toInt()>0) {
            FavoritesManager::Interval=BUFF.substring(7, 12).toInt();
        } 
        if (BUFF.indexOf("off")>7) {
            FavoritesManager::FavoritesRunning = false;
            FavoritesManager::UseSavedFavoritesRunning=true;
        } 
        if (BUFF.indexOf("on")>7) {
            FavoritesManager::FavoritesRunning = true;
            FavoritesManager::UseSavedFavoritesRunning=true;
        }
        if (BUFF.indexOf("del")>7) {
            FavoritesManager::FavoriteModes[currentMode]=0;
            showWarning(CRGB::Red, 1000U, 500U);               // мигание цветом 0,5 секунды (1 раза)
        }
        if (BUFF.indexOf("add")>7) {
            FavoritesManager::FavoriteModes[currentMode]=1;
            showWarning(CRGB::Green, 1000U, 500U);             // мигание цветом 0,5 секунды (1 раза)
        }
            #ifdef USE_BLYNK
             Blynk.setProperty(V4, "color", "#55CC55");
             updateRemoteBlynkParams();
            #endif
        
      }
      if (!strncmp_P(inputBuffer, PSTR("TXT-sleep"), 9)){
        if (BUFF.substring(9, 11).toInt()>0) processParams("TMR_SET", String("1 10 "+String((BUFF.substring(9, 11).toInt())*60)).c_str());
        if (BUFF.indexOf("off")>7) processParams("TMR_SET", "0 0 0");
      } else
      if (!strncmp_P(inputBuffer, PSTR("TXT-alarm-"), 10) && (BUFF.substring(10, 11).toInt()>0) ){
//         processInputBuffer("DAWN"+BUFF.substring(10, 11));
//         processParams("DAWN", BUFF.substring(10, 11).c_str());   
         memcpy(buff, &inputBuffer[10], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 11
//         Serial.println(buff);
         dawnMode = atoi(buff) - 1;
         EepromManager::SaveDawnMode(&dawnMode);
//         sendAlarms(inputBuffer);
      
         #if (USE_MQTT)
         if (espMode == 1U)
         {
           MqttManager::needToPublish = true;
         }
         #endif
      
      } else
      if (!strncmp_P(inputBuffer, PSTR("TXT-alarm0"), 10) && (BUFF.substring(10, 14).toInt()>0) ){
//         Serial.println(BUFF);
         TimeToFireEnd=BUFF.substring(9, 14).toInt()*60;
         EepromManager::WriteInt32(29,TimeToFireEnd);
         EEPROM.commit();           
//         Serial.println(TimeToFireEnd);
      } else
      if (!strncmp_P(inputBuffer, PSTR("TXT-alarm"), 9) && (BUFF.substring(9, 10).toInt()>0) ){
//         Serial.println(BUFF);
         set_status_alarm(BUFF);
         set_time_alarm(BUFF.substring(9, 14).toInt());
         settChanged = true;
      } else
      if (!strncmp_P(inputBuffer, PSTR("TXT-load"), 8)){
         #ifdef USE_BLYNK
            BlynkParamAllocated items(3000); 
//            Serial.println("Загрузка списка");
            for (uint8_t x=0;x<MODE_AMOUNT;x++) {
                Blynk.setProperty(V0, "label", String(x)+"."+effects_name[x].Name);    
//                Serial.println(String(x)+"."+effects_name[x].Name);
                items.add(String(x)+"."+effects_name[x].Name);
            } 
            for (uint8_t x=1;x<8;x++) {
                Blynk.setProperty(V0, "label", "Будильник "+String(x)+". "+NameDayFoWeek[x]);    
//                Serial.println(String(x)+"."+effects_name[x].Name);
                items.add("Будильник "+String(x)+". "+NameDayFoWeek[x]);
            } 

            Blynk.setProperty(V4, "labels", items);
        #endif
    } else if (!strncmp_P(inputBuffer, PSTR("TXT-esp_mode=0"), 14)){
            if (espMode == 1U){
              espMode = 0U;
              EepromManager::SaveEspMode(&espMode);
              showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
              ESP.restart();
            }
            else
              showWarning(CRGB::Red, 2000U, 500U);                     // мигание красным цветом 2 секунды (ошибка)
          }
         else if (!strncmp_P(inputBuffer, PSTR("TXT-esp_mode=1"), 14)){
            if (espMode == 0U){
              espMode = 1U;
              EepromManager::SaveEspMode(&espMode);
              showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
              ESP.restart();
            }
            else
              showWarning(CRGB::Red, 2000U, 500U);                     // мигание красным цветом 2 секунды (ошибка)
          }
         else if (!strncmp_P(inputBuffer, PSTR("TXT-reset=wifi"), 14)){
            wifiManager.resetSettings();                             // сброс сохранённых SSID и пароля (сброс настроек подключения к роутеру)
            showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
          }
         else if (!strncmp_P(inputBuffer, PSTR("TXT-reset=effects"), 17)){
            restoreSettings();
            loadingFlag = true;
            settChanged = true;
            eepromTimeout = millis();
            #if (USE_MQTT)
            if (espMode == 1U)
            {
              MqttManager::needToPublish = true;
            }
            #endif
            showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды - смена рабочего режима лампы, перезагрузка
          }
         else if (!strncmp_P(inputBuffer, PSTR("TXT-rfolder="), 12) && (BUFF.length() > 12)){ 
            uint16_t vll = BUFF.substring(12, 15).toInt();
//            Serial.println(vll);
            if (vll<count_mp3_folders+1) {
              DAWN_FOLDER=vll;
              EEPROM.write(28,DAWN_FOLDER);
              EEPROM.commit();
            }
          }
         else if (!strncmp_P(inputBuffer, PSTR("TXT-rbridge="), 12) && (BUFF.length() > 12)){ 
            uint16_t vll = BUFF.substring(12, 16).toInt();
//            Serial.println(vll);
            if (vll<256) {
              DAWN_BRIGHT=vll;
              EEPROM.write(27,DAWN_BRIGHT);
              EEPROM.commit();           
            }
          }
  
         else if (!strncmp_P(inputBuffer, PSTR("TXT-rdelay="), 11) && (BUFF.length() > 11)){ 
            uint16_t vll = BUFF.substring(11, 15).toInt();
//            Serial.println(vll);
            TimeToFireEnd=vll*60;
            EepromManager::WriteInt32(29,TimeToFireEnd);
            EEPROM.commit();           
          }

       else
      { 
          String str = (BUFF.length() > 4) ? BUFF.substring(4, BUFF.length()) : "";
          str.toCharArray(TextTicker, str.length() + 1);
      }
    }
//   ---
    else if (!strncmp_P(inputBuffer, PSTR("RESET"), 5)) { // сброс настроек WIFI по запросу от приложения
      wifiManager.resetSettings();
    }
   else if (!strncmp_P(inputBuffer, PSTR("LIST"), 4)) // передача списка эффектов по запросу от приложения (если поддерживается приложением)
    {
       memcpy(buff, &inputBuffer[4], strlen(inputBuffer));  // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 5
       switch (atoi(buff))
         {
           case 1U:
           {
             Udp.write(efList_1.c_str());
             Udp.write("\0");
             break;
           }
           case 2U:
           {
             Udp.write(efList_2.c_str());
             Udp.write("\0");
             break;
           }
           case 3U:
           {
             Udp.write(efList_3.c_str());
             Udp.write("\0");
             break;
           }
         }
    }
//    ---
    else if (!strncmp_P(inputBuffer, PSTR("GBR"), 3))
    {
      memcpy(buff, &inputBuffer[3], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 4
      uint8_t ALLbri = constrain(atoi(buff), 1, 255);
      for (uint8_t i = 0; i < MODE_AMOUNT-1; i++) {
        modes[i].Brightness = ALLbri;
      }
      FastLED.setBrightness(ALLbri);
      loadingFlag = true;
    }
    else if (!strncmp_P(inputBuffer, PSTR("DRW"), 3)) {
      String xx = getValue(BUFF, ';', 1);
      String yy = getValue(BUFF, ';', 2);
      int8_t X = (int8_t)xx.toInt();
      int8_t Y = (int8_t)yy.toInt();
      drawPixelXY(X, Y, DriwingColor);
      paint[X][Y]=DriwingColor;
      FastLED.show();
    }
    else if (!strncmp_P(inputBuffer, PSTR("CLR"), 3)) {
     for (uint8_t x =0; x<WIDTH; x++)
       for (uint8_t y =0; y<HEIGHT; y++)
          paint[x][y]=CRGB(0,0,0);
//      loadingFlag=true;
      FastLED.clear();
      FastLED.show();
    }
    else if (!strncmp_P(inputBuffer, PSTR("COL"), 3)) {
      String R = getValue(BUFF, ';', 1);
      String G = getValue(BUFF, ';', 2);
      String B = getValue(BUFF, ';', 3);
      DriwingColor = CRGB(R.toInt(), G.toInt(), B.toInt());
    }
    else if (!strncmp_P(inputBuffer, PSTR("DRAWOFF"), 7)) {
      if (Painting == 1)
      {
          Painting = 0;
          Write_Picure_to_EEPROM();
          currentMode=oldcurrentMode;
          oldcurrentMode=MODE_AMOUNT;
          loadingFlag = true;
      }
    }
    else if (!strncmp_P(inputBuffer, PSTR("DRAWON"), 6)) {
      if (Painting == 0)
      {
          Painting = 1;
          oldcurrentMode=currentMode;
          currentMode=EFF_PAINTING;
          FavoritesManager::FavoritesRunning = false;
          FavoritesManager::UseSavedFavoritesRunning=true;
          loadingFlag = true;
      }
    }
    else if (!strncmp_P(inputBuffer, PSTR("EFF"), 3))
    {
      EepromManager::SaveModesSettings(&currentMode, modes);
      memcpy(buff, &inputBuffer[3], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 4
      currentMode = (uint8_t)atoi(buff);
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      FastLED.clear();
      delay(1);
      sendCurrent(inputBuffer);
      FastLED.setBrightness(modes[currentMode].Brightness);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("BRI"), 3))
    {
      memcpy(buff, &inputBuffer[3], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 4
      modes[currentMode].Brightness = constrain(atoi(buff), 1, 255);
      FastLED.setBrightness(modes[currentMode].Brightness);
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      sendCurrent(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    #ifdef USE_BLYNK
//    updateRemoteBlynkParams();
    #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("SPD"), 3))
    {
      memcpy(buff, &inputBuffer[3], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 4
      modes[currentMode].Speed = atoi(buff);
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      sendCurrent(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    #ifdef USE_BLYNK
//    updateRemoteBlynkParams();
    #endif
    }
    else if (!strncmp_P(inputBuffer, PSTR("VOL"), 3))
    {
      memcpy(buff, &inputBuffer[3], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 4
      modes[EFF_VOICE].Speed = atoi(buff);
      Serial.println("Volume="+String(modes[EFF_VOICE].Speed));
      set_mp3_volume=modes[EFF_VOICE].Speed;
      #ifdef IR_PIN
      IR_BTN.set_br_sp_sc(2);
      #endif
      settChanged = true;
      eepromTimeout = millis();
      sendCurrent(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    #ifdef USE_BLYNK
//    updateRemoteBlynkParams();
    #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("SCA"), 3))
    {
      memcpy(buff, &inputBuffer[3], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 4
      modes[currentMode].Scale = atoi(buff);
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      sendCurrent(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    #ifdef USE_BLYNK
//    updateRemoteBlynkParams();
    #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("P_ON"), 4))
    {
      ONflag = true;
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      changePower();
      sendCurrent(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
      #ifdef USE_BLYNK
       updateRemoteBlynkParams();
      #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("P_OFF"), 5))
    {
      ONflag = false;
      settChanged = true;
      eepromTimeout = millis();
      changePower();
      sendCurrent(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("ALM_SET"), 7))
    {
      uint8_t alarmNum = (char)inputBuffer[7] - '0';
      alarmNum -= 1;
      if (strstr_P(inputBuffer, PSTR("ON")) - inputBuffer == 9)
      {
        alarms[alarmNum].State = true;
        sendAlarms(inputBuffer);
      }
      else if (strstr_P(inputBuffer, PSTR("OFF")) - inputBuffer == 9)
      {
        alarms[alarmNum].State = false;
        sendAlarms(inputBuffer);
      }
      else
      {
        memcpy(buff, &inputBuffer[8], strlen(inputBuffer)); // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 9
        alarms[alarmNum].Time = atoi(buff);
        sendAlarms(inputBuffer);
      }
      EepromManager::SaveAlarmsSettings(&alarmNum, alarms);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        strcpy(MqttManager::mqttBuffer, inputBuffer);
        MqttManager::needToPublish = true;
      }
      #endif
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("ALM_GET"), 7))
    {
      sendAlarms(inputBuffer);
    }

    else if (!strncmp_P(inputBuffer, PSTR("DAWN"), 4))
    {
      memcpy(buff, &inputBuffer[4], strlen(inputBuffer));   // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 5
      dawnMode = atoi(buff) - 1;
      EepromManager::SaveDawnMode(&dawnMode);
      sendAlarms(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("DISCOVER"), 8))  // обнаружение приложением модуля esp в локальной сети
    {
      if (espMode == 1U)                                    // работает только в режиме WiFi клиента
      {
        sprintf_P(inputBuffer, PSTR("IP %u.%u.%u.%u:%u"),
          WiFi.localIP()[0],
          WiFi.localIP()[1],
          WiFi.localIP()[2],
          WiFi.localIP()[3],
          ESP_UDP_PORT);
      }
    }

    else if (!strncmp_P(inputBuffer, PSTR("TMR_GET"), 7))
    {
      sendTimer(inputBuffer);
    }

    else if (!strncmp_P(inputBuffer, PSTR("TMR_SET"), 7))
    {
      memcpy(buff, &inputBuffer[8], 2);                     // взять подстроку, состоящую из 9 и 10 символов, из строки inputBuffer
      TimerManager::TimerRunning = (bool)atoi(buff);

      memcpy(buff, &inputBuffer[10], 2);                    // взять подстроку, состоящую из 11 и 12 символов, из строки inputBuffer
      TimerManager::TimerOption = (uint8_t)atoi(buff);

      memcpy(buff, &inputBuffer[12], strlen(inputBuffer));  // взять подстроку, состоящую последних символов строки inputBuffer, начиная с символа 13
      TimeToFireEnd = strtoull(buff, &endToken, 10);
      TimerManager::TimeToFire = millis() + TimeToFireEnd * 1000;
 

      TimerManager::TimerHasFired = false;
      sendTimer(inputBuffer);

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("FAV_GET"), 7))
    {
      FavoritesManager::SetStatus(inputBuffer);
    }

    else if (!strncmp_P(inputBuffer, PSTR("FAV_SET"), 7))
    {
      FavoritesManager::ConfigureFavorites(inputBuffer);
      FavoritesManager::SetStatus(inputBuffer);
      settChanged = true;
      eepromTimeout = millis();

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        MqttManager::needToPublish = true;
      }
      #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("OTA"), 3))
    {
      #ifdef OTA
      otaManager.RequestOtaUpdate();
      delay(50);
      otaManager.RequestOtaUpdate();
      currentMode = EFF_MATRIX;                             // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
      FastLED.clear();
      delay(1);
      ONflag = true;
      changePower();
      #endif
    }

    else if (!strncmp_P(inputBuffer, PSTR("BTN"), 3))
    {
      if (strstr_P(inputBuffer, PSTR("ON")) - inputBuffer == 4)
      {
        buttonEnabled = true;
        EepromManager::SaveButtonEnabled(&buttonEnabled);
        sendCurrent(inputBuffer);
      }
      else if (strstr_P(inputBuffer, PSTR("OFF")) - inputBuffer == 4)
      {
        buttonEnabled = false;
        EepromManager::SaveButtonEnabled(&buttonEnabled);
        sendCurrent(inputBuffer);
      }

      #if (USE_MQTT)
      if (espMode == 1U)
      {
        strcpy(MqttManager::mqttBuffer, inputBuffer);
        MqttManager::needToPublish = true;
      }
      #endif
    }

    else
    {
      inputBuffer[0] = '\0';
    }

    if (strlen(inputBuffer) <= 0)
    {
      return;
    }

    if (generateOutput)                                     // если запрошен вывод ответа выполнения команд, копируем его в исходящий буфер
    {
      strcpy(outputBuffer, inputBuffer);
    }
    inputBuffer[0] = '\0';                                  // очистка буфера, читобы не он не интерпретировался, как следующий входной пакет
}

void sendCurrent(char *outputBuffer)
{
  sprintf_P(outputBuffer, PSTR("CURR %u %u %u %u %u %u"),
    currentMode,
    modes[currentMode].Brightness,
    modes[currentMode].Speed,
    modes[currentMode].Scale,
    ONflag,
    espMode);
  
  #ifdef USE_NTP
  strcat_P(outputBuffer, PSTR(" 1"));
  #else
  strcat_P(outputBuffer, PSTR(" 0"));
  #endif

  sprintf_P(outputBuffer, PSTR("%s %u"), outputBuffer, (uint8_t)TimerManager::TimerRunning);
  sprintf_P(outputBuffer, PSTR("%s %u"), outputBuffer, (uint8_t)buttonEnabled);

//  #ifdef USE_NTP
//  char timeBuf[9];
//  getFormattedTime(timeBuf);
//  sprintf_P(outputBuffer, PSTR("%s %s"), outputBuffer, timeBuf);
//  #else
      time_t currentTicks = now();
      Serial.println("currentTicks "+String(currentTicks));
  sprintf_P(outputBuffer, PSTR("%s %02u:%02u:%02u"), outputBuffer, hour(currentTicks), minute(currentTicks), second(currentTicks));
//#endif
  
  #ifdef MP3_TX_PIN
   sprintf_P(outputBuffer, PSTR("%s %u"), outputBuffer, modes[EFF_VOICE].Speed);
  #else
   strcat_P(outputBuffer, PSTR("%s 0"), outputBuffer);
  #endif
}

void sendAlarms(char *outputBuffer)
{
  strcpy_P(outputBuffer, PSTR("ALMS"));

  for (byte i = 0; i < 7; i++)
  {
    sprintf_P(outputBuffer, PSTR("%s %u"), outputBuffer, (uint8_t)alarms[i].State);
  }

  for (byte i = 0; i < 7; i++)
  {
    sprintf_P(outputBuffer, PSTR("%s %u"), outputBuffer, alarms[i].Time);
  }

  sprintf_P(outputBuffer, PSTR("%s %u"), outputBuffer, dawnMode + 1);
}

void sendTimer(char *outputBuffer)
{
  sprintf_P(outputBuffer, PSTR("TMR %u %u %u"),
    TimerManager::TimerRunning,
    TimerManager::TimerOption,
   (TimerManager::TimerRunning ? (uint16_t)floor((TimerManager::TimeToFire - millis()) / 1000) : 0));
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void Write_Picure_to_EEPROM()
{
 for (uint8_t x =0; x<WIDTH; x++)
  for (uint8_t y =0; y<HEIGHT; y++)
  {
   EEPROM.write(EEPROM_PICTURE_ADDRESS+(x*HEIGHT+y)*3+0, paint[x][y].r);
   EEPROM.write(EEPROM_PICTURE_ADDRESS+(x*HEIGHT+y)*3+1, paint[x][y].g);
   EEPROM.write(EEPROM_PICTURE_ADDRESS+(x*HEIGHT+y)*3+2, paint[x][y].b);
  }
    #ifdef GENERAL_DEBUG
    LOG.println(F("Write_Picure_to_EEPROM "));
    #endif

}
void Read_Picure_from_EEPROM()
{
 for (uint8_t x =0; x<WIDTH; x++)
  for (uint8_t y =0; y<HEIGHT; y++)
  {
   paint[x][y].r=EEPROM.read(EEPROM_PICTURE_ADDRESS+(x*HEIGHT+y)*3+0);
   paint[x][y].g=EEPROM.read(EEPROM_PICTURE_ADDRESS+(x*HEIGHT+y)*3+1);
   paint[x][y].b=EEPROM.read(EEPROM_PICTURE_ADDRESS+(x*HEIGHT+y)*3+2);
  }
    #ifdef GENERAL_DEBUG
    LOG.println(F("Read_Picure_from_EEPROM "));
    #endif
}
