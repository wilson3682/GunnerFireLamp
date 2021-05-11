#include <GyverButton.h>
#include <SoftwareSerial.h> 


#define MP3_TX_PIN            (14U)                         // artuinio TX mp3 player RX (D5) в случае отсутствия заремить
#define MP3_RX_PIN            (12U)                         // artuinio RX mp3 player TX (D6)
//#define BTN_PIN_PLAY          (13U)                         // пин кнопки                (D7) Закоментировать, если кнопок нету
//#define BTN_PIN_STOP          (15U)                         // пин кнопки                (D8)  
#define MP3_MAX_VOLUME        (30U)                         // Максимальная громкость (1..30)
#define count_mp3_folders     (6U)                          // Количество папок 01 02 03
#define MIC_PIN                A0                           // Снятие звука
#define SOUND_R_FREQ           A0    // аналоговый пин вход аудио для режима с частотами (через кондер)


////
// FFT
#include <fix_fft.h>
///

#ifdef MP3_TX_PIN
SoftwareSerial mp3(MP3_RX_PIN, MP3_TX_PIN); // RX, TX
//Дополнитеьные кнопки
#endif
#ifdef BTN_PIN_PLAY
GButton touch_mp3_play(BTN_PIN_PLAY, LOW_PULL, NORM_OPEN);
GButton touch_mp3_stop(BTN_PIN_STOP, LOW_PULL, NORM_OPEN);
#endif

uint8_t mp3_mode=2;           // Режим воспроизведения mp3 0 - stop, 1 - play, 2 - auto (зависит от ONflag)
bool mp3_play_now=false;      // Указывает, играет ли сейчас мелодия
bool set_mp3_play_now=false;  // Указывает, надо ли играть сейчас мелодии
bool mp3_play_next=false;     // Принудительно пнуть на следующий файл
bool mp3_play_time_adv=false; // Можно ли объявлять время
bool mp3_time_test_play_now=false; //Можно ли делать запрос плееру
uint8_t set_mp3_volume=25;    // Требуемая громкость (меняется доп. кнопками, можно привесить на одну в button.ino, после 4 клика)
uint8_t set_adv_volume=25;    // Требуемая громкость обявления
uint8_t adv_volume=25;        // Текущая громкость обявления
uint8_t mp3_volume=25;        // Текущая громкость
uint8_t mp3_folder=1;         // Текущая папка для воспроизведения
uint8_t mp3_folder_last=0;    // Предыдущая папка для воспроизведения
uint16_t mp3_file;            // Воспроизводящийся файл
uint8_t mp3_play_time=0;      // 1-Воспроизвести время (ч/м), 2-Воспроизвести минуты, 3-Воспроизвести часы
bool put_play=false;
// Воиспроиведение времени используем метод "ADVERT" или объявление
// Объявление времени сработает только во время звучания мелодии
// 
// Можно вставить в button.ino в секцию  if (clickCount == 6U) ... mp3_play_time=1;
// или в runningText.ino в секцию #if (PRINT_TIME ...

uint32_t mp3_timer = 0;
uint32_t mp3_time_timer = 0;
bool dawnFlagEnd=false;
// задержки в произнощении часов.
uint16_t clock_delay[25]={0,1200,1176,1080,1320,1152,1296,1200,1248,1368,1488,1488,1632,1560,1584,1440,1560,1584,1656,1680,1416,1752,1656,1656,1056};
// Имена папок 01 02 03 04 и т.д
uint16_t count_mp3_files[count_mp3_folders+1] = {0};     // Количество файлов в папках 
/*
// Имена файлов в папках 001 002 003 004 и т.д до 255 например 001_Птицы.mp3, 002_Соловей.mp2
// Если надо больше, то имена 0001 0002 0003 0004 и т.д до 4095 например 0001_Птицы.mp3
// в этом случае количество папок ограничено 01-15
// в случае использования 0001-4095 замените 
   playFolder(mp3_folder,mp3_file);
//на
   playFolderbig(mp3_folder,mp3_file);
*/   
// int32_t dawnPosition;
/*
// переменная int32_t dawnPosition; сделана глобальной (перенесена сюда)
// В модуле time.ino заменить 
int32_t dawnPosition = 
// на 
dawnPosition = 
// (Убрать int32_t, сделав глобальной)
*/
