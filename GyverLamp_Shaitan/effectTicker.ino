//
uint32_t effTimer;

void effectsTick()
{
  if (!dawnFlag)
  {
#ifdef MP3_TX_PIN
    if (mp3_play_time>0) {
      // printTime(thisTime, true, ONflag);
      char stringTime[10U];                              
      sprintf_P(stringTime, PSTR("%u:%02u"), (uint8_t)((thisTime - thisTime % 60U) / 60U), (uint8_t)(thisTime % 60U));
      FastLED.setBrightness(getBrightnessForPrintTime(thisTime, ONflag));
//     if (fillString(stringTime, CRGB::White)) mp3_play_time=0;
//     if (fillString(stringTime, CHSV(map(thisTime,0,1439,0,255),255,getBrightnessForPrintTime(thisTime, ONflag)))) mp3_play_time=0;
//    Вывод с пульта цветом в зависимости от времени суток
       if (fillString(stringTime, CHSV(map(thisTime,0,1439,0,255),255,255))) mp3_play_time=0;
    }
    else
#endif
    if (ONflag && (millis() - effTimer >= DelayEffect())
    //&& (mp3_play_time==0 || mp3_play_time==5)
    )
    {
      effTimer = millis();
      //Определить папку для проигрывания
      mp3_folder=modes[EFF_VOICE].Brightness;
      if (mp3_folder>count_mp3_folders)  mp3_folder=((mp3_folder-1) % count_mp3_folders)+1;
      if (effects_name[currentMode].Folder<count_mp3_folders+1) mp3_folder=effects_name[currentMode].Folder;
      #ifdef IR_PIN
      IR_BTN.ReadMass(); // Восстановление фона под символом
      #endif

      #ifdef USE_BLYNK
  //     if (loadingFlag) updateRemoteBlynkParams();
      #endif
     
      switch (currentMode)
      {
        case EFF_SPARKLES:       sparklesRoutine();                    break;  // (0U)  Конфетти
        case EFF_FIRE:           fireRoutine(true);                    break;  // (1U)  Огонь
        case EFF_FIRE2012:       fire2012WithPalette();                break;  // (2U)  Водопад                
        case EFF_RAINBOW_VER:    rainbowVerticalRoutine();             break;  // (3U)  Радуга вертикальная    
        case EFF_RAINBOW_HOR:    rainbowHorizontalRoutine();           break;  // (4U)  Радуга горизонтальная  
        case EFF_RAINBOW_DIAG:   rainbowDiagonalRoutine();             break;  // (5U)  Радуга диагональная    
        case EFF_COLORS:         colorsRoutine();                      
                         ClockRoutine(WIDTH,HEIGHT,false);             break;  // (6U)  Смена цвета
        case EFF_MADNESS:        madnessNoiseRoutine();                break;  // (7U)  Безумие 3D 
        case EFF_CLOUDS:         cloudsNoiseRoutine();                 break;  // (8U)  Облака 3D  
        case EFF_LAVA:           lavaNoiseRoutine();                   break;  // (9U)  Лава 3D    
        case EFF_PLASMA:         plasmaNoiseRoutine();                 break;  // (10U) Плазма 3D  
        case EFF_RAINBOW:        rainbowNoiseRoutine();                break;  // (11U) Радуга 3D  
        case EFF_RAINBOW_STRIPE: rainbowStripeNoiseRoutine();          break;  // (12U) Павлин 3D  
        case EFF_ZEBRA:          zebraNoiseRoutine();                  break;  // (13U) Зебра 3D   
        case EFF_FOREST:         forestNoiseRoutine();                 break;  // (14U) Лес 3D     
        case EFF_OCEAN:          oceanNoiseRoutine();                  break;  // (15U) Океан 3D   
        case EFF_COLOR:          colorRoutine();                       
                      ClockRoutine(WIDTH,HEIGHT,false);                break;  // (16U) Цвет                 
        case EFF_SNOW:           snowRoutine();                        break;  // (17U) Снегопад             
        case EFF_SNOWSTORM:      snowStormRoutine();                   break;  // (18U) Метель               
        case EFF_STARFALL:       starfallRoutine();                    break;  // (19U) Звездопад            
        case EFF_MATRIX:         matrixRoutine();                      break;  // (20U) Матрица              
        case EFF_LIGHTERS:       lightersRoutine();                    break;  // (21U) Светлячки            
        case EFF_LIGHTER_TRACES: ballsRoutine();                       break;  // (22U) Светлячки со шлейфом 
        case EFF_PAINTBALL:      lightBallsRoutine();                  break;  // (23U) Пейнтбол             
        case EFF_CUBE:           ballRoutine();                        break;  // (24U) Блуждающий кубик     
        case EFF_COMET:          RainbowCometRoutine();                break;  // (25U) Комета               
        case EFF_PULSE:          pulse();                              break;  // (26U) Пульс                
        case EFF_MATRIX2:        matrix2Routine();                     break;  // (27U) Матрица 2            
        case EFF_WATERPOLL:      waterpoolRoutine();                   break;  // (28U) Бассейн              
        case EFF_NOISESTREAMS:   NoiseStreamingRoutine();              break;  // (29U)
        case EFF_METABALLS:      metaBallsRoutine();                   break;  // (30U)
        case EFF_SINUSOID3:      Sinusoid3Routine();                   break;  // (31U)
        case EFF_FIRE_2020:      fire2020Routine2();                   break;  // (32U) Oгoнь 2020
        case EFF_SPIRO:          spiroRoutine();                       break;  // (33U) Спираль          
        case EFF_BBALS:          BBallsRoutine();                      break;  // (34U) Прыгающие мячики 
        case EFF_FLOCK: flockRoutine(modes[currentMode].Scale==100);   break;  // (35U) Стая             
        case EFF_RAIN:           RainRoutine();                        break;  // (36U) Типа дождь          
        case EFF_SALUTE:         SaluteRoutine();                      break;  // (37U) Салют               
        case EFF_FIRE_2018:      Fire2018_2();                         break;  // (38U) Огонь 2018          
        case EFF_PRISMATA:       PrismataRoutine();                    break;  // (39U) Призмата            
        case EFF_SIMPLE_RAIN:    simpleRain();                         break;  // (40U) Тучка в банке       
        case EFF_STORMY_RAIN:    stormyRain();                         break;  // (41U) Гроза в банке       
        case EFF_COLOR_RAIN:     coloredRain();                        break;  // (42U) Осадки              
        case EFF_SMOKE:          MultipleStreamSmoke(false);           break;  // (43U) Дым                 
        case EFF_SMOKE_COLOR:    MultipleStreamSmoke(true);            break;  // (44U) Paзнoцвeтный дым    
        case EFF_PICASSO:        PicassoRoutine();                     break;  // (45U) Пикacco             
        case EFF_PICASSO2:       PicassoRoutine2();                    break;  // (46U) Пикacco 2           
        case EFF_PICASSO3:       PicassoRoutine3();                    break;  // (47U) Kpyги Пикacco       
        case EFF_CUBE2D:         cube2dRoutine();                      break;  // (48U) Kyбик Pyбикa        
        case EFF_DNA:            DNARoutine();                         break;  // (49U) ДHK                 
        case EFF_BALLS_BOUNCE:   bounceRoutine();                      break;  // (50U) 3мeйки              
        case EFF_SNAKES:         snakesRoutine();                      break;  // (51U) Mячики бeз гpaниц   
        case EFF_POPCORN:        popcornRoutine();                     break;  // (52U) Пoпкopн             
        case EFF_BUTTERFLYS:     butterflysRoutine(true);              break;  // (53U) Moтыльки            
        case EFF_BUTTERFLYS_LAMP:butterflysRoutine(false);             break;  // (54U) Лaмпa c мoтылькaми  
        case EFF_LIQUIDLAMP:     LiquidLampRoutine(true);              break;  // (55U) Жидкaя лaмпa        
        case EFF_LIQUIDLAMP_AUTO:LiquidLampRoutine(false);             break;  // (56U) Жидкaя лaмпa (auto) 
        case EFF_LEAPERS:        LeapersRoutine();                     break;  // (57U) Пpыгyны             
        case EFF_TWINKLES:       twinklesRoutine();                    break;  // (58U) Mepцaниe            
//        case EFF_RINGS:          ringsRoutine();                       break;  // (59U) Koдoвый зaмoк
        case EFF_BBALLS_TRACES:  BBallsRoutine();                      break;  // (59U) Mячики co шлeйфoм
        case EFF_SHADOWS:        shadowsRoutine();                     break;  // (60U) Teни

        case EFF_TEXT:           fillString(TextTicker, 
                   CHSV(modes[EFF_TEXT].Scale*2.55, 255U, 255U));      break;  // Бегущая строка          
        case EFF_PAINTING:       PaintRoutine(true);                   break;  // Рисование из приложения 
        case EFF_CLOCK:          ClockRoutine(WIDTH,HEIGHT,true);      break;  // стрелочные часы
        case EFF_WHITE_COLOR:    whiteColorStripeRoutine();            break;  // Белый свет     
        case EFF_VOICE:          MP3Routine();                         break;  // Громкость      
        default:                                                       break;
      }
      //Вывод символа яркость/скорость/масштаб
      #ifdef IR_PIN
      IR_BTN.show_br_sp_sc();
      #endif
      if (currentMode != EFF_TEXT) FastLED.show();
      mp3_time_test_play_now=true; //Разрешить доступ к mp3
    }
  }
}

byte DelayEffect()
{
 if (currentMode == EFF_CLOCK) return 255;
 if (currentMode == EFF_VOICE) return 50;
 if (currentMode == EFF_TEXT)  return 10;
 return map(modes[currentMode].Speed,1,255,effects_name[currentMode].Speed_max,effects_name[currentMode].Speed_min);
}
void changePower()
{
 if (ONflag)
  {
    effectsTick();
    for (uint8_t i = 0U; i < modes[currentMode].Brightness; i = constrain(i + 8, 0, modes[currentMode].Brightness))
    {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.setBrightness(modes[currentMode].Brightness);
    delay(2);
    FastLED.show();
  }
  else
  {
    effectsTick();
    for (uint8_t i = modes[currentMode].Brightness; i > 0; i = constrain(i - 8, 0, modes[currentMode].Brightness))
    {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.clear();
    delay(2);
    FastLED.show();
  }

  #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
  #endif
  
  TimerManager::TimerRunning = false;
  TimerManager::TimerHasFired = false;
  TimerManager::TimeToFire = 0ULL;

  if (FavoritesManager::UseSavedFavoritesRunning == 0U)     // если выбрана опция Сохранять состояние (вкл/выкл) "избранного", то ни выключение модуля, ни выключение матрицы не сбрасывают текущее состояние (вкл/выкл) "избранного"
  {
      FavoritesManager::TurnFavoritesOff();
  }

  #if (USE_MQTT)
  if (espMode == 1U)
  {
    MqttManager::needToPublish = true;
  }
  #endif
}
