#pragma once

/*
 * Схема использования EEPROM памяти:
 * Используются адреса:
 * Начало    Длина    Описание
 *                    массив alarm
 * 0         1        будильник пн вкл/выкл
 * 1-2       2        будильник пн время в минутах от начала суток (0 - 1440), 2 байта
 * 3         1        будильник вт вкл/выкл
 * 4-5       2        будильник вт время в минутах от начала суток (0 - 1440), 2 байта
 * 6         1        будильник ср вкл/выкл
 * 7-8       2        будильник ср время в минутах от начала суток (0 - 1440), 2 байта
 * 9         1        будильник чт вкл/выкл
 * 10-11     2        будильник чт время в минутах от начала суток (0 - 1440), 2 байта
 * 12        1        будильник пт вкл/выкл
 * 13-14     2        будильник пт время в минутах от начала суток (0 - 1440), 2 байта
 * 15        1        будильник сб вкл/выкл
 * 16-17     2        будильник сб время в минутах от начала суток (0 - 1440), 2 байта
 * 18        1        будильник вс вкл/выкл
 * 19-20     2        будильник вс время в минутах от начала суток (0 - 1440), 2 байта
 *                    массив modes
 * 21        1        признак "кнопка разблокирована"
 * 22        1        рабочий режим лампы (ESP_MODE)
 * 23        1        состояние лампы (вкл/выкл)
 * 24        1        признак первого запуска (определяет необходимость первоначальной записи всех хранимых настроек)
 * 25        1        время до "рассвета" (dawnMode)
 * 26        1        текущий режим (currentMode)
 * 27        1        Яркость рассвета
 * 28        1        Папка мелодий рассвета
 * 29-32     4        Время после рассвета
 *         
 * 33-49     23       Резерв 
 *         
 * 50-55     6        настройки режима избранных эффектов (вкл/выкл - 1 байт; интервал - 2 байта; разброс - 2 байта; инициализировать вкл/выкл - 1 байт;
 * 56-56+MODE_AMOUNT вкл/выкл каждого эффекта - (MODE_AMOUNT) байт; 
 * 
 * 56+MODE_AMOUNT+1 3        режим №0:  яркость, скорость, масштаб (по одному байту)
 * ...
 * 
 * Итого:      
 * 56+MODE_AMOUNT*4
 
*/

#include <EEPROM.h>
#include "types.h"
#define EEPROM_ESP_BUTTON_ENABLED_ADDRESS    (21U)         // адрес в EEPROM памяти для записи признака разблокированной кнопки
#define EEPROM_ESP_MODE                      (22U)         // адрес в EEPROM памяти для записи режима работы модуля ESP (точка доступа/WiFi клиент)
#define EEPROM_LAMP_ON_ADDRESS               (23U)         // адрес в EEPROM памяти для записи состояния лампы (вкл/выкл)
#define EEPROM_FIRST_RUN_ADDRESS             (24U)         // адрес в EEPROM памяти для записи признака первого запуска (определяет необходимость первоначальной записи всех хранимых настроек)
#define EEPROM_DAWN_MODE_ADDRESS             (25U)         // адрес в EEPROM памяти для записи времени до "рассвета"
#define EEPROM_CURRENT_MODE_ADDRESS          (26U)         // адрес в EEPROM памяти для записи номера текущего эффекта лампы

#define EEPROM_ALARM_STRUCT_SIZE             (3U)          // 1 байт - вкл/выкл; 2 байта - время от начала суток в минутах (0 - 1440)
#define EEPROM_ALARM_START_ADDRESS           (0U)          // начальный адрес в EEPROM памяти для записи настроек будильников

#define EEPROM_MODE_STRUCT_SIZE              (3U)          // 1 байт - яркость; 1 байт - скорость; 1 байт - масштаб

                                                           // начальный адрес в EEPROM памяти для записи настроек эффектов (яркость, скорость, масштаб)
#define EEPROM_MODES_START_ADDRESS           (50U)
                                                           // начальный адрес в EEPROM памяти для записи настроек режима избранных эффектов
#define EEPROM_FAVORITES_START_ADDRESS       (EEPROM_MODES_START_ADDRESS+MODE_AMOUNT*EEPROM_MODE_STRUCT_SIZE+11)         
                                                           // место храниния картинки
#define EEPROM_PICTURE_ADDRESS               (EEPROM_FAVORITES_START_ADDRESS+MODE_AMOUNT+27)         
                                                           // общий размер используемой EEPROM памяти (сумма всех хранимых настроек + 1 байт)    
#define EEPROM_TOTAL_BYTES_USED              (EEPROM_PICTURE_ADDRESS+WIDTH*HEIGHT*3)         

#define EEPROM_FIRST_RUN_MARK                (49U)         // счисло-метка, если ещё не записно в EEPROM_FIRST_RUN_ADDRESS, значит нужно проинициализировать EEPROM и записать все первоначальные настройки
#define EEPROM_WRITE_DELAY                   (30000UL)     // отсрочка записи в EEPROM после последнего изменения хранимых настроек, позволяет уменьшить количество операций записи в EEPROM


class EepromManager
{
  public:
    static void InitEepromSettings(ModeType modes[], AlarmType alarms[], uint8_t* espMode, bool* onFlag, uint8_t* dawnMode, int8_t* currentMode, bool* buttonEnabled,
      void (*readFavoritesSettings)(), void (*saveFavoritesSettings)())
    {
      // записываем в EEPROM начальное состояние настроек, если их там ещё нет
      EEPROM.begin(EEPROM_TOTAL_BYTES_USED);
      delay(50);

      if (EEPROM.read(EEPROM_FIRST_RUN_ADDRESS) != EEPROM_FIRST_RUN_MARK)
      {
        EEPROM.write(EEPROM_FIRST_RUN_ADDRESS, EEPROM_FIRST_RUN_MARK);
        EEPROM.commit();

        for (uint8_t i = 0; i < MODE_AMOUNT; i++)
        {
          EEPROM.put(EEPROM_MODES_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * i, modes[i]);
          EEPROM.commit();
        }

        for (uint8_t i = 0; i < 7; i++)
        {
          EEPROM.write(EEPROM_ALARM_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * i, alarms[i].State);
          WriteUint16(EEPROM_ALARM_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * i + 1, alarms[i].Time);
          EEPROM.commit();
        }
        // Очистка картинки и запись в память
        for (uint16_t i = 0; i < WIDTH*HEIGHT*3; i++)
         EEPROM.write(EEPROM_PICTURE_ADDRESS+i,0);
        // 
        EEPROM.write(EEPROM_ESP_MODE, ESP_MODE);
        EEPROM.write(EEPROM_LAMP_ON_ADDRESS, 0);
        EEPROM.write(EEPROM_DAWN_MODE_ADDRESS, 0);
        EEPROM.write(EEPROM_CURRENT_MODE_ADDRESS, 0);
        EEPROM.write(EEPROM_ESP_BUTTON_ENABLED_ADDRESS, 1);
 
       /*Рассвет
        * 27       1        Яркость рассвета
        * 28       1        Папка мелодий рассвета
        * 29-32    4        Время после рассвета
        */
        EEPROM.write(27,200);
        EEPROM.write(28,1);
        WriteInt32(29,1800);

        saveFavoritesSettings();
    
        EEPROM.commit();
      }

      // инициализируем настройки лампы значениями из EEPROM
      for (uint8_t i = 0; i < MODE_AMOUNT; i++)
      {
        EEPROM.get(EEPROM_MODES_START_ADDRESS + EEPROM_MODE_STRUCT_SIZE * i, modes[i]);
      }

      for (uint8_t i = 0; i < 7; i++)
      {
        alarms[i].State = EEPROM.read(EEPROM_ALARM_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * i);
        alarms[i].Time = ReadInt16(EEPROM_ALARM_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * i + 1);
      }

      readFavoritesSettings();

      *espMode = (uint8_t)EEPROM.read(EEPROM_ESP_MODE);
      *onFlag = (bool)EEPROM.read(EEPROM_LAMP_ON_ADDRESS);
      *dawnMode = EEPROM.read(EEPROM_DAWN_MODE_ADDRESS);
      *currentMode = EEPROM.read(EEPROM_CURRENT_MODE_ADDRESS);
      *buttonEnabled = EEPROM.read(EEPROM_ESP_BUTTON_ENABLED_ADDRESS);
    }
    
    static void SaveModesSettings(int8_t* currentMode, ModeType modes[])
    {
      EEPROM.put(EEPROM_MODES_START_ADDRESS + EEPROM_MODE_STRUCT_SIZE * (*currentMode), modes[*currentMode]);
      EEPROM.commit();
    }
    
    static void HandleEepromTick(bool* settChanged, uint32_t* eepromTimeout, bool* onFlag, int8_t* currentMode, ModeType modes[], void (*saveFavoritesSettings)())
    {
      if (*settChanged && millis() - *eepromTimeout > EEPROM_WRITE_DELAY)
      {
        *settChanged = false;
        *eepromTimeout = millis();
        SaveOnFlag(onFlag);
        SaveModesSettings(currentMode, modes);
        if (EEPROM.read(EEPROM_CURRENT_MODE_ADDRESS) != *currentMode)
        {
          EEPROM.write(EEPROM_CURRENT_MODE_ADDRESS, *currentMode);
        }
        saveFavoritesSettings();
        EEPROM.commit();
      }
    }

    static void SaveAlarmsSettings(uint8_t* alarmNumber, AlarmType alarms[])
    {
      EEPROM.write(EEPROM_ALARM_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * (*alarmNumber), alarms[*alarmNumber].State);
      WriteUint16(EEPROM_ALARM_START_ADDRESS + EEPROM_ALARM_STRUCT_SIZE * (*alarmNumber) + 1, alarms[*alarmNumber].Time);
      EEPROM.commit();
    }

    static void SaveEspMode(uint8_t* espMode)
    {
      EEPROM.write(EEPROM_ESP_MODE, *espMode);
      EEPROM.commit();
    }

    static void SaveOnFlag(bool* onFlag)
    {
      EEPROM.write(EEPROM_LAMP_ON_ADDRESS, *onFlag);
      EEPROM.commit();
    }

    static void SaveDawnMode(uint8_t* dawnMode)
    {
      EEPROM.write(EEPROM_DAWN_MODE_ADDRESS, *dawnMode);
      EEPROM.commit();
    }

    static void SaveButtonEnabled(bool* buttonEnabled)
    {
      EEPROM.write(EEPROM_ESP_BUTTON_ENABLED_ADDRESS, *buttonEnabled);
      EEPROM.commit();
    }

    static uint16_t ReadUint16(uint16_t address)
    {
      uint16_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      return val;
    }

    static void WriteUint16(uint16_t address, uint16_t val)
    {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.commit();
    }

    static int16_t ReadInt16(uint16_t address)
    {
      int16_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      return val;
    }

    static void WriteInt16(uint16_t address, int16_t val)
    {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.commit();      
    }

    static uint32_t ReadUint32(uint16_t address)
    {
      uint32_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      *(p + 2)  = EEPROM.read(address + 2);
      *(p + 3)  = EEPROM.read(address + 3);
      return val;
    }

    static void WriteUint32(uint16_t address, uint32_t val)
    {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.write(address + 2, *(p + 2));
      EEPROM.write(address + 3, *(p + 3));
      EEPROM.commit();
    }

    static int32_t ReadInt32(uint16_t address)
    {
      int32_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      *(p + 2)  = EEPROM.read(address + 2);
      *(p + 3)  = EEPROM.read(address + 3);
      return val;
    }

    static void WriteInt32(uint16_t address, int32_t val)
    {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.write(address + 2, *(p + 2));
      EEPROM.write(address + 3, *(p + 3));
      EEPROM.commit();      
    }

  private:
};
