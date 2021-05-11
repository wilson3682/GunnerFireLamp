#ifdef MP3_TX_PIN
/*
// При наступлении ночи NIGHT_HOURS_START MP3 отключается
// При наступлении время будильника - подключаются (включен или нет, не важно)

// Переменные, которые были использованиы в модуле для анализа

// dawnFlag - Идет рассвет
// dawnPosition - Яркость рассвета
// ONflag - включена/выключена лампа
// currentMode - текущий эффект

*/

//Костыль (из за неточного определения количества файлов)
 uint8_t kostyl[count_mp3_folders+1]={0};//{0,29,19,2,4,2,176};

//буффер команд
 uint8_t cmdbuf[8] = {0x7E, 0xFF, 06, 00, 00, 00, 00, 0xEF};
 uint8_t ansbuf[10] = {0};
 

void command(int8_t cmd, int8_t dat5, int8_t dat6)
{
  // Посылка команды MP3 плееру
//  cmdbuf[0] = 0x7e; 
//  cmdbuf[1] = 0xFF; 
//  cmdbuf[2] = 0x06; 
  cmdbuf[3] = cmd;  // polecenie
  cmdbuf[4] = 0x00; // 0x00 = no feedback, 0x01 = feedback
  cmdbuf[5] = dat5; // parametr DAT1
  cmdbuf[6] = dat6; // parametr DAT2
  for (uint8_t i = 0; i < 8; i++)
  {
    mp3.write(cmdbuf[i]);
    delay(3);
  }
}
//------------------------------- 
boolean answer(void)
  // Ответ от MP3 плеера
{
  uint8_t i;
  uint8_t b = 0;
  while(mp3.available() && (b!=0x7E))
  {
    b = mp3.read();
    delay(3);
  }
  ansbuf[0] = b;
  i = 1;
  while(mp3.available() && (i < 10))
  {
    b = mp3.read();
    ansbuf[i] = b;
    i++;
    delay(3);
  }

  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
  {
    return true;
  }
 
  return false;
}
//-------------------------------
byte play_status(bool ans)
{
 int pl=255; 
 if (ans)
  {
   answer();
   command(0x42, 0, 2);
  }
 uint32_t timer_1=millis();
 while ((not mp3.available()) && (millis()-timer_1<1000)) delay(5);
  if (answer())
   {
   if (ansbuf[3] == 0x42)  pl=ansbuf[6];  // Состояние 0 - Stop, 1 - Play, 2 - Pause 
   if (ansbuf[3] == 0x3D)  pl=0;          // Неожиданно пришел ответ об окончании проигрывания файла
   }
  
  return pl; 
}
//-------------------------------

void playFolder(int8_t p_path,int8_t p_file)
{
//  Проигрывание файлов 001, 002 - 255 из папок 01-99
  command(0x0f, p_path,p_file); 
  mp3_play_now=true;
  delay(200);
}

void playFolderbig(int8_t p_path,int16_t p_file)
{
//  Проигрывание файлов 0001, 0002 - 4095 из папок 01-15
  int8_t Para_MSB=p_path*16 + p_file/256;
  int8_t Para_LSB=p_file;
  command(0x14, Para_MSB,Para_LSB); 
}

void mp3_read_max_file()
{
  uint32_t timefilecout=millis();
  //Считываем количество файлов из папок
  delay(500);
  for (byte i=1;i<count_mp3_folders+1;i++) {
    command(0x4E, 0, i);
    delay(1000);
    timefilecout=millis();
    while (not mp3.available() && millis()-timefilecout>10000) delay(5);// Ожидаем ответа до 10 сек
    if (mp3.available())
     {
       answer();
       count_mp3_files[i]=ansbuf[5]*256+ansbuf[6];
     } else  count_mp3_files[i]=kostyl[i]; //Если ответа нет, берем данные из таблицы
       Serial.println("count_mp3_files["+String(i)+"]="+String(count_mp3_files[i]));
     
   }
}   

void mp3_stop()
{
  command(0x16,0,0); 
  mp3_play_now=false;
}


void mp3_setup()
{
  mp3.begin(9600);
  delay(500); 
  command(0x09, 0, 2);  // Выбор TF
  
//  while (!fillString("Определяем количество MP3 файлов", CHSV(modes[EFF_TEXT].Scale, 255U, 255U))) {delay(1); }
  delay(300);
  mp3_read_max_file();
  mp3_timer=millis();
#ifdef BTN_PIN_PLAY
  touch_mp3_play.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch_mp3_play.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  touch_mp3_stop.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch_mp3_stop.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  touch_mp3_play.setTimeout(BUTTON_TIMEOUT);
  touch_mp3_stop.setTimeout(BUTTON_TIMEOUT);
#endif
  dawnFlagEnd=false;
  mp3_folder=(modes[EFF_VOICE].Brightness % count_mp3_folders) + 1;
  set_mp3_volume=modes[EFF_VOICE].Speed;
  set_adv_volume=modes[EFF_VOICE].Scale;
//  Serial.println("Метка");
  pinMode(MIC_PIN,INPUT);
  
}


void play_time_ADVERT()
{
 int pt_h=(uint8_t)((thisTime - thisTime % 60U) / 60U);
 int pt_m=(uint8_t)(thisTime % 60U);
 
 if (pt_h==0) pt_h=24;
 if (mp3_play_time_adv) {
  if (mp3_play_time==1 || mp3_play_time==3) 
    {
      command(0x06, 0,set_adv_volume); // Передать на плеер изменеие громкости
      command(0x13, 0, pt_h); 
      mp3_play_time=(mp3_play_time==1)?2:0;
      mp3_time_timer=millis();
//      Serial.println(mp3_play_time);
    } else 
  if (mp3_play_time==2) 
   {
    if (millis()-mp3_time_timer>clock_delay[pt_h]+250)
    {
      command(0x13, 0, pt_m+100); 
      mp3_play_time=5;
      mp3_time_timer=millis();
//      Serial.println(mp3_play_time);
    }
   } else  
  if (mp3_play_time==5) 
   { 
     if (millis()-mp3_time_timer>3000)
      {
        command(0x06, 0,set_mp3_volume); // Передать на плеер изменеие громкости
        mp3_play_time=0;
        loadingFlag=true;
//        Serial.println(mp3_play_time);
      }
   } 
 }
}

bool StopDirection;
bool PlayDirection;

static bool startStopHolding = false;     
static bool startPlayHolding = false;     

//Обработка дополнительных клавиш
void touch_mp3()
{
#ifdef BTN_PIN_PLAY
  if (!buttonEnabled)                                       // события кнопки не обрабатываются, если она заблокирована
  {
    return;
  }

  touch_mp3_play.tick();
  touch_mp3_stop.tick();

  uint8_t clickCount_mp3_play = touch_mp3_play.hasClicks() ? touch_mp3_play.getClicks() : 0U;
  uint8_t clickCount_mp3_stop = touch_mp3_stop.hasClicks() ? touch_mp3_stop.getClicks() : 0U;

  if (ONflag && touch_mp3_play.isHolded())
  {
      PlayDirection = !PlayDirection;
//      Serial.print(PlayDirection);
  }
  if (ONflag && touch_mp3_stop.isHolded())
  {
      StopDirection = !StopDirection;
//      Serial.print(StopDirection);
  }

      //Удержание одной кнопки меняет громкость
   if (ONflag && touch_mp3_play.isStep())
    {
        modes[EFF_VOICE].Scale = constrain(PlayDirection?modes[EFF_VOICE].Scale+1:modes[EFF_VOICE].Scale-1, 1, 100);
        set_adv_volume=constrain(modes[EFF_VOICE].Scale,0,MP3_MAX_VOLUME); 
        IR_BTN.set_br_sp_sc(5);
    }
    
   if (touch_mp3_stop.isStep())
    {
        modes[EFF_VOICE].Speed = constrain(StopDirection?modes[EFF_VOICE].Speed + 1:modes[EFF_VOICE].Speed - 1, 1, 255);
        set_mp3_volume=constrain(modes[EFF_VOICE].Speed,0,MP3_MAX_VOLUME); 
        IR_BTN.set_br_sp_sc(2);
    }

    // однократное нажатие play смена режима stop / play / auto
  if (clickCount_mp3_play == 1U) { 
     mp3_mode=(mp3_mode+1) % 3; 
     IR_BTN.set_br_sp_sc(7);

  }
// Воспроизвелене времени и включение для будильника (воспроизведение времени только при воспроизведении мелодии)
  if (clickCount_mp3_stop == 1U) {
    mp3_play_time_adv=true;
    mp3_play_time=1;
  }

  // Двухкратное нажатие play смена папки
  if (clickCount_mp3_play == 2U) {
    if (modes[EFF_VOICE].Brightness%count_mp3_folders==count_mp3_folders-1) modes[EFF_VOICE].Brightness-=count_mp3_folders;
    if (modes[EFF_VOICE].Brightness+count_mp3_folders>255) modes[EFF_VOICE].Brightness-=count_mp3_folders;
    modes[EFF_VOICE].Brightness++;
    mp3_folder=modes[EFF_VOICE].Brightness;
//    if (mp3_folder>count_mp3_folders)  mp3_folder=((mp3_folder-1) % count_mp3_folders)+1;
//    while(!fillString(String(mp3_folder).c_str(), CRGB::White)) { delay(1); ESP.wdtFeed(); }
    IR_BTN.set_br_sp_sc(6);
  }
  // Двухкратное нажатие stop смена папки
  if (clickCount_mp3_stop == 2U) {
    if (modes[EFF_VOICE].Brightness%count_mp3_folders==0) modes[EFF_VOICE].Brightness+=count_mp3_folders;
    modes[EFF_VOICE].Brightness--;
    mp3_folder=modes[EFF_VOICE].Brightness;
//    if (mp3_folder>count_mp3_folders)  mp3_folder=((mp3_folder-1) % count_mp3_folders)+1;
//    while(!fillString(String(mp3_folder).c_str(), CRGB::White)) { delay(1); ESP.wdtFeed(); }
    IR_BTN.set_br_sp_sc(6);
  }
  
#endif
}
// end touch_mp3()

// dawnFlag - Идет рассвет
// dawnPosition - Яркость рассвета
// ONflag - включена/выключена лампа
// currentMode - текущий эффект

void mp3_loop()
{
  if ((millis() - mp3_timer >5000) && mp3_time_test_play_now) { // Раз в 5 секунд в промежуток между прорисовками послать запрос на состояние MP3 
      mp3_play_now=(play_status(true)>0);
      mp3_timer=millis();
      mp3_time_test_play_now=false;
 }
  if (mp3_folder>count_mp3_folders)  mp3_folder=((mp3_folder-1) % count_mp3_folders)+1;
  
  if (mp3_folder_last != mp3_folder) {
     mp3_play_next=true;
     mp3_folder_last=mp3_folder;
  }
  touch_mp3();

  if (mp3_time_test_play_now) play_time_ADVERT();
  switch (mp3_mode)
    {
     case 0: set_mp3_play_now=false;  break;
     case 1: set_mp3_play_now=true;   break;
     case 2: set_mp3_play_now=ONflag; break;
    }
  if (dawnFlag) set_mp3_play_now=true;
  if (mp3_time_test_play_now)
   if (set_mp3_volume!=mp3_volume && !dawnFlag) {
      command(0x06, 0,set_mp3_volume); // Передать на плеер изменеие громкости
      mp3_volume=set_mp3_volume;       // При изменении и без будильника
      mp3_time_test_play_now=false;
   }


  if (mp3_time_test_play_now && mp3_folder>0)
  {
//   time_t currentLocalTime = localTimeZone.toLocal(timeClient.getEpochTime());
   //time_t currentLocalTime = getCurrentLocalTime();
   time_t currentLocalTime = now();
   uint8_t thisDay = dayOfWeek(currentLocalTime);
   thisDay=(thisDay+5) % 7;
   //if ((thisTime < NIGHT_HOURS_START && thisTime > alarms[thisDay].Time) || dawnFlag || !ntpServerAddressResolved) // В дневное время или при будильнике или при отсутствии интернета
     if (mp3_play_now!=set_mp3_play_now || mp3_play_next) {
       if (set_mp3_play_now) {
         mp3_file=random(1,count_mp3_files[mp3_folder]+1);
         playFolder(mp3_folder,mp3_file);
//         Serial.println("mp3_folder:"+String(mp3_folder)+"  mp3_file:"+String(mp3_file)+"   from:"+String(count_mp3_files[mp3_folder]));
         mp3_play_next=false;
 
       }  else  mp3_stop();
       mp3_time_test_play_now=false;
     }
  }   
  if (mp3_play_now && mp3_folder==0) mp3_stop();
}


uint32_t timer_fff;

/*
int vmin[8]={2000,2200,2200,2500,1500,1000,1800,1800};    // Минимальное значение аудиосигнала
int vmax[8]={2500,2500,2500,3000,2000,2000,2000,2000}; // Максимальное значение аудиосигнала
int ddd[128];
uint32_t timer_fff_analog;

void MP3Routine()
{
  char im[128], data[128];
  int data_a[14];  
  int data_d[14];
  
  set_mp3_volume=modes[EFF_VOICE].Speed;
  set_adv_volume=modes[EFF_VOICE].Scale;
  if (set_adv_volume==0) mp3_play_time_adv=false;
  FastLED.clear();
// Попытка cоздать эквалайзер
// Необходимо подключить микрофон на A0
// FFT
  uint32_t m=micros();
  double samplingFrequency = 32000; // частота семплирования для esp8266 (без разгонов) скорее всего не может быть выше 9500 если чтение через analogRead(MIC_PIN);
  double signalFrequency = 700;
  int sampling_period_us = round(1000000*(1.0/samplingFrequency));
  for (int i=0; i < 128; i++){                                     
    ddd[i] = analogRead(MIC_PIN);                                    
    data[i]=ddd[i];
    im[i] = 0;    
      while((micros() - m < sampling_period_us)){
        //empty loop
      }
      m += sampling_period_us;
  };
 if (millis() - timer_fff_analog >= 10000)
  {
    Serial.println("min/max/data");
    for (int i=0; i<64; i++)
     Serial.println(String(i)+" / " + String(ddd[i]));  
    timer_fff_analog= millis();
  }  
  fix_fft(data,im,7,0);

// Serial.println("data после преобразования");
  for (int i=0; i< 64; i++){                                      
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);  
//    Serial.println(data[i]);
  };       

 // Serial.println("min/max/data");
for (int i=0; i<8; i++)
{
  // усредняем соседние значения
  data_a[i]=0;
   for (int y = 0; y < 16; y++) data_a[i] += data[i*16+y];
  vmax[i] = max(vmax[i], data_a[i]); // запоминаем максимальное значение
  vmin[i] = min(vmin[i], data_a[i]); // запоминаем минимальное значение
  storm_max=max(storm_max,vmax[i]);
  storm_min=min(storm_min,vmin[i]);
  // масштабируем значения под аналоговый вывод
  data_d[i] = constrain(map(data_a[i], storm_min, storm_max, 0, 16), 0, 16); //цифровые
//  Serial.println(String(vmin[i])+" / " + String(vmax[i])+" / " + String(data_a[i]));
  }  


for (int i=0; i<8; i++)
     for (int yy = 0; yy < data_d[i]; yy++) { 
      leds[getPixelNumber(i+4, yy)]=CHSV(yy*6+i*15,255,64); 
     }
//end FFT  

 if (millis() - timer_fff >= 5000)
  {
  Serial.println("min/max/data");
    for (int i=0; i<8; i++)
     {
       Serial.println(String(vmin[i])+" / " + String(vmax[i])+" / " + String(data_a[i])+" / " + String(data_d[i]));
      if (vmax[i]>vmin[i]) {
        vmax[i]--;
        vmin[i]++;
      }
     } 
    storm_max--;
    storm_min++;
      
  }    


  for (uint8_t i = 0; i < set_mp3_volume/2+1; i++)
   {
     leds[getPixelNumber(1, i)]=CHSV(80-i*6+set_mp3_volume,255,64); 
     leds[getPixelNumber(2, i)]=CHSV(80-i*6+set_mp3_volume,255,64); 
   }
  for (uint8_t i = 0; i < set_adv_volume/2+1; i++)
   {   
        leds[getPixelNumber(13, i)]=CHSV(80-i*6+set_adv_volume,255,64); 
        leds[getPixelNumber(14, i)]=CHSV(80-i*6+set_adv_volume,255,64); 
   }     
}
*/
int vmin=30000; // Минимальное значение аудиосигнала
int vmax=0; // Максимальное значение аудиосигнала
void MP3Routine()
{
  char im[128], data[128];
  int data_a[14];  
  int data_d[14];
  
//  mp3_folder=(modes[EFF_VOICE].Brightness % count_mp3_folders) + 1;
  set_mp3_volume=modes[EFF_VOICE].Speed;
  if (set_adv_volume!=modes[EFF_VOICE].Scale)  
  {
    mp3_play_time_adv=true;
    mp3_play_time=1;
    set_adv_volume=modes[EFF_VOICE].Scale;
  }
  if (set_adv_volume==0) mp3_play_time_adv=false;
  FastLED.clear();
// Попытка cоздать эквалайзер
// Необходимо подключить микрофон на A0
// Так как микрофона сейчас нет, работы остановлены
// FFT
/*
  for (int i=0; i < 128; i++){                                     
    data[i] = analogRead(MIC_PIN);                                    
//    data[i] = random(500);
    im[i] = 0;    
  };
*/
/*
    for (int i=0; i < 128; i++){
      delayMicroseconds(200);
      data[i] = analogRead(MIC_PIN)/4 -128;
      im[i] = 0;
    }

  fix_fft(data,im,7,0);

  for (int i=0; i< 64; i++){                                      
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);  
//    data[i] = sqrt(im[i] * im[i]);  
  };       

for (int i=0; i<8; i++)
{
  // усредняем соседние значения
  data_a[i]=0;
   for (int y = 0; y < 16; y++) {
     data_a[i] += data[i*16+y];
     vmax = max(vmax, data_a[i]); // запоминаем максимальное значение
     vmin = min(vmin, data_a[i]); // запоминаем минимальное значение
   }
  // масштабируем значения под аналоговый вывод
  data_d[i] = constrain(map(data_a[i], 0, vmax, 0, 16), 0, 16); //цифровые
  int dyy= constrain(map(vmax, 0, vmax, 0, 16), 0, 16); //цифровые
  // Выводим сигнал
     for (int yy = 0; yy < data_d[i]; yy++) { 
      leds[getPixelNumber(i+4, yy)]=CHSV(yy*6+i*15,255,64); 
     }
  }  
//end FFT  
vmax--;
vmin++;
*/
  for (uint8_t i = 0; i < map(set_mp3_volume,0,30,0,HEIGHT-1); i++)
   {
     leds[getPixelNumber(1, i)]=CHSV(80-i*6+set_mp3_volume*8,255,64); 
     leds[getPixelNumber(2, i)]=CHSV(80-i*6+set_mp3_volume*8,255,64); 
   }
  for (uint8_t i = 0; i < map(set_adv_volume,0,30,0,HEIGHT-1); i++)
   {   
        leds[getPixelNumber(13, i)]=CHSV(80-i*6+set_adv_volume*8,255,64); 
        leds[getPixelNumber(14, i)]=CHSV(80-i*6+set_adv_volume*8,255,64); 
   }     
}

void read_microphone()
{
 int old_storm_max=storm_max;
  if (millis()-timer_fff>1000)
   {
    if (storm_max>storm_min) 
     {
      storm_max--;
      storm_min++;
     } 
    timer_fff=millis(); 
   }
  for (int i=0; i < 10; i++){                                     
    storm_max = max(analogRead(MIC_PIN),storm_max);
    storm_min = min(analogRead(MIC_PIN),storm_min);
  }
  
  if (storm_max>old_storm_max+2) storm_light=true;
  else storm_light=false;
  
}


#endif
