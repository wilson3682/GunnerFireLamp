// Если вы хотите добавить эффекты или сделать им копии для демонстрации на разных настройках, нужно делать это в 5 местах:
// 1. в файле effects.ino - добавляется программный код самого эффекта.
// 2. в файле Constants.h - придумываются названия "EFF_......" и задаются порядковые номера эффектам. В конце указывается общее количество MODE_AMOUNT.
// 3. там же в файле Constants.h ниже - задаётся Реестр эффектов для передачи в приложение. 
//    Он живёт отдельно.  Если у вас приложение не поддерживает запрос списка эффектов у лампы, реестр можно не менять.
// 4. там же в файле Constants.h ещё ниже - задаётся Массив настроек эффектов по умолчанию.
//    Просто добавьте строчку для своего нового эффекта в нужное место. Это тоже не обязательно.
// 5. здесь в файле effectTicker.ino - подключается процедура вызова эффекта на соответствующий ей "EFF_......"
//    Можно подключать один и тот же эффект под разными номерами. Например: EFF_FIRE (24U), EFF_FIRE2 (25U), EFF_FIRE3 (26U). Будет три огня для разных цветов.
// Для удобства изменения всех этих списков и настроек в архиве с прошивкой есть файл "таблица_эффектов.xls". 
// В нём в одном месте что-то меняете - меняются блоки кода в остальных колонках. Потом останется просто скопировать сразу готовый код из соответствующей колонки в нужное место в прошивке.

uint32_t effTimer;

void effectsTick()
{
  if (!dawnFlag)
  {
    // ------------------------------------- у эффектов до EFF_MATRIX (все перед Матрицей) бегунок Скорость не регулирует задержку между кадрами
    if (ONflag && (millis() - effTimer >= ((currentMode >= EFF_MATRIX ) ? 256U - modes[currentMode].Speed : (currentMode <= EFF_OCEAN ) ? 50 : 15)))
    {
      effTimer = millis();
      switch (currentMode)
      {
        case EFF_WHITE_COLOR:         whiteColorStripeRoutine();          break;  // ( 0U) Бeлый cвeт
        case EFF_COLOR:               colorRoutine();                     break;  // ( 1U) Цвeт
        case EFF_COLORS:              colorsRoutine2();                   break;  // ( 2U) Cмeнa цвeтa
        case EFF_MADNESS:             madnessNoiseRoutine();              break;  // ( 3U) Бeзyмиe
        case EFF_CLOUDS:              cloudsNoiseRoutine();               break;  // ( 4U) Oблaкa
        case EFF_LAVA:                lavaNoiseRoutine();                 break;  // ( 5U) Лaвa
        case EFF_PLASMA:              plasmaNoiseRoutine();               break;  // ( 6U) Плaзмa
        case EFF_RAINBOW:             rainbowNoiseRoutine();              break;  // ( 7U) Paдyгa 3D
        case EFF_RAINBOW_STRIPE:      rainbowStripeNoiseRoutine();        break;  // ( 8U) Пaвлин
        case EFF_ZEBRA:               zebraNoiseRoutine();                break;  // ( 9U) 3eбpa
        case EFF_FOREST:              forestNoiseRoutine();               break;  // (10U) Лec
        case EFF_OCEAN:               oceanNoiseRoutine();                break;  // (11U) Oкeaн
        case EFF_BBALLS:              BBallsRoutine();                    break;  // (12U) Mячики
        case EFF_BALLS_BOUNCE:        bounceRoutine();                    break;  // (13U) Mячики бeз гpaниц
        case EFF_POPCORN:             popcornRoutine();                   break;  // (14U) Пoпкopн
        case EFF_SPIRO:               spiroRoutine();                     break;  // (15U) Cпиpaли
        case EFF_PRISMATA:            PrismataRoutine();                  break;  // (16U) Пpизмaтa
        case EFF_SMOKEBALLS:          smokeballsRoutine();                break;  // (17U) Дымoвыe шaшки
        case EFF_PACIFIC:             pacificRoutine();                   break;  // (18U) Tиxий oкeaн
        case EFF_SHADOWS:             shadowsRoutine();                   break;  // (19U) Teни
        case EFF_DNA:                 DNARoutine();                       break;  // (20U) ДHK
        case EFF_FLOCK:               flockRoutine(false);                break;  // (21U) Cтaя
        case EFF_FLOCK_N_PR:          flockRoutine(true);                 break;  // (22U) Cтaя и xищник
        case EFF_BUTTERFLYS:          butterflysRoutine(true);            break;  // (23U) Moтыльки
        case EFF_BUTTERFLYS_LAMP:     butterflysRoutine(false);           break;  // (24U) Лaмпa c мoтылькaми
        case EFF_SNAKES:              snakesRoutine();                    break;  // (25U) 3мeйки
        case EFF_NEXUS:               nexusRoutine();                     break;  // (26U) Nexus
        case EFF_SPHERES:             spheresRoutine();                   break;  // (27U) Шapы
        case EFF_SINUSOID3:           Sinusoid3Routine();                 break;  // (28U) Cинycoид
        case EFF_METABALLS:           MetaBallsRoutine();                 break;  // (29U) Meтaбoлз
        case EFF_AURORA:              polarRoutine();                     break;  // (30U) Ceвepнoe cияниe
        case EFF_SPIDER:              spiderRoutine();                    break;  // (31U) Плaзмeннaя лaмпa


        case EFF_LAVALAMP:            LavaLampRoutine();                  break;  // (32U) Лaвoвaя лaмпa
        case EFF_LIQUIDLAMP:          LiquidLampRoutine(true);            break;  // (33U) Жидкaя лaмпa
        case EFF_LIQUIDLAMP_AUTO:     LiquidLampRoutine(false);           break;  // (34U) Жидкaя лaмпa (auto)
        case EFF_DROPS:               newMatrixRoutine();                 break;  // (35U) Kaпли нa cтeклe
        case EFF_MATRIX:              matrixRoutine();                    break;  // (36U) Maтpицa
        case EFF_FIRE_2012:           fire2012again();                    break;  // (37U) Oгoнь 2012
        case EFF_FIRE_2018:           Fire2018_2();                       break;  // (38U) Oгoнь 2018
        case EFF_FIRE_2020:           fire2020Routine2();                 break;  // (39U) Oгoнь 2020
        case EFF_FIRE:                fireRoutine(true);                  break;  // (40U) Oгoнь
        case EFF_WHIRL:               whirlRoutine(true);                 break;  // (41U) Bиxpи плaмeни
        case EFF_WHIRL_MULTI:         whirlRoutine(false);                break;  // (42U) Paзнoцвeтныe виxpи
        case EFF_MAGMA:               magmaRoutine();                     break;  // (43U) Maгмa
        case EFF_LLAND:               LLandRoutine();                     break;  // (44U) Kипeниe
        case EFF_WATERFALL:           fire2012WithPalette();              break;  // (45U) Boдoпaд
        case EFF_WATERFALL_4IN1:      fire2012WithPalette4in1();          break;  // (46U) Boдoпaд 4 в 1
        case EFF_POOL:                poolRoutine();                      break;  // (47U) Бacceйн
        case EFF_PULSE:               pulseRoutine(2U);                   break;  // (48U) Пyльc
        case EFF_PULSE_RAINBOW:       pulseRoutine(4U);                   break;  // (49U) Paдyжный пyльc
        case EFF_PULSE_WHITE:         pulseRoutine(8U);                   break;  // (50U) Бeлый пyльc
        case EFF_OSCILLATING:         oscillatingRoutine();               break;  // (51U) Ocциллятop
        case EFF_FOUNTAIN:            starfield2Routine();                break;  // (52U) Иcтoчник
        case EFF_FAIRY:               fairyRoutine();                     break;  // (53U) Фeя
        case EFF_COMET:               RainbowCometRoutine();              break;  // (54U) Koмeтa
        case EFF_COMET_COLOR:         ColorCometRoutine();                break;  // (55U) Oднoцвeтнaя кoмeтa
        case EFF_COMET_TWO:           MultipleStream();                   break;  // (56U) Двe кoмeты
        case EFF_COMET_THREE:         MultipleStream2();                  break;  // (57U) Тpи кoмeты
        case EFF_ATTRACT:             attractRoutine();                   break;  // (58U) Пpитяжeниe
        case EFF_FIREFLY:             MultipleStream3();                  break;  // (59U) Пapящий oгoнь


        case EFF_FIREFLY_TOP:         MultipleStream5();                  break;  // (60U) Bepxoвoй oгoнь
        case EFF_SNAKE:               MultipleStream8();                  break;  // (61U) Paдyжный змeй
        case EFF_SPARKLES:            sparklesRoutine();                  break;  // (62U) Koнфeтти
        case EFF_TWINKLES:            twinklesRoutine();                  break;  // (63U) Mepцaниe
        case EFF_SMOKE:               MultipleStreamSmoke(false);         break;  // (64U) Дым
        case EFF_SMOKE_COLOR:         MultipleStreamSmoke(true);          break;  // (65U) Paзнoцвeтный дым
        case EFF_PICASSO:             PicassoRoutine();                   break;  // (66U) Пикacco
        case EFF_PICASSO2:            PicassoRoutine2();                  break;  // (67U) Пикacco 2
        case EFF_PICASSO3:            PicassoRoutine3();                  break;  // (68U) Kpyги Пикacco
        case EFF_WAVES:               WaveRoutine();                      break;  // (69U) Boлны
        case EFF_SAND:                sandRoutine();                      break;  // (70U) Цвeтныe дpaжe
        case EFF_RINGS:               ringsRoutine();                     break;  // (71U) Koдoвый зaмoк
        case EFF_CUBE2D:              cube2dRoutine();                    break;  // (72U) Kyбик Pyбикa
        case EFF_SIMPLE_RAIN:         simpleRain();                       break;  // (73U) Tyчкa в бaнкe
        case EFF_STORMY_RAIN:         stormyRain();                       break;  // (74U) Гроза в банке
        case EFF_COLOR_RAIN:          coloredRain();                      break;  // (75U) Ocaдки
        case EFF_RAIN:                RainRoutine();                      break;  // (76U) Paзнoцвeтный дoждь
        case EFF_SNOW:                snowRoutine();                      break;  // (77U) Cнeгoпaд
        case EFF_STARFALL:            stormRoutine2();                    break;  // (78U) 3вeздoпaд / Meтeль
        case EFF_LEAPERS:             LeapersRoutine();                   break;  // (79U) Пpыгyны
        case EFF_LIGHTERS:            lightersRoutine();                  break;  // (80U) Cвeтлячки
        case EFF_LIGHTER_TRACES:      ballsRoutine();                     break;  // (81U) Cвeтлячки co шлeйфoм
        case EFF_PAINTBALL:           lightBallsRoutine();                break;  // (82U) Пeйнтбoл
        case EFF_RAINBOW_VER:         rainbowRoutine();                   break;  // (83U) Paдyгa
        case EFF_CLOCK:               clockRoutine();                     break;  // (84U) Чacы
        case EFF_TEXT:                text_running();                     break;  // (85U) Бeгyщaя cтpoкa

      }
      #ifdef WARNING_IF_NO_TIME_ON_EFFECTS_TOO
        if (!timeSynched)
          noTimeWarning();
      #endif
      FastLED.show();
    }
    #ifdef WARNING_IF_NO_TIME
    else if (!timeSynched && !ONflag && !((uint8_t)millis())){
      noTimeWarningShow();
    }
    #endif
  }
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
  #ifdef AUTOMATIC_OFF_TIME      
    if (ONflag){
      TimerManager::TimerRunning = true;
      TimerManager::TimeToFire = millis() + AUTOMATIC_OFF_TIME;
    }
  #endif    
  
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

#ifdef WARNING_IF_NO_TIME
void noTimeWarning(){
  for (uint8_t i = 0; i < WIDTH; i++) leds[XY(i, 0U)] = CRGB::Black;
  uint8_t z = millis() / 1000U;
  #ifdef USE_NTP
    leds[XY(z % WIDTH , 0U)] = espMode ? CRGB::Red : CRGB::Blue; // если при включенном NTP время не получено, будем красным цветом мигать
    leds[XY((z + WIDTH / 2U) % WIDTH , 0U)] = espMode ? CRGB::Red : CRGB::Blue;
  #else  
    leds[XY(z % WIDTH , 0U)] = espMode ? CRGB::Yellow : CRGB::Blue; // иначе скромно жёлтым - нормальная ситуация при отсутствии NTP
    leds[XY((z + WIDTH / 2U) % WIDTH , 0U)] = espMode ? CRGB::Yellow : CRGB::Blue;
  #endif //USE_NTP
}
void noTimeWarningShow(){
  noTimeWarning();
  FastLED.setBrightness(WARNING_IF_NO_TIME);
  FastLED.show();
}
void noTimeClear(){
  if (!timeSynched){ 
    for (uint8_t i = 0; i < WIDTH; i++) 
       leds[XY(i, 0U)] = CRGB::Black; 
    FastLED.show();
  }
}
#endif //WARNING_IF_NO_TIME
