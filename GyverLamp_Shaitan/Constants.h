#pragma once

#include <ESP8266WebServer.h>

//#define USE_BLYNK  ("ID")    // раскомментируйте эту строку, если вы используете приложение Blynk (для iOS и Android) https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proshivki-ot-gunner47.2418/post-53535
#ifdef USE_BLYNK
uint32_t blynkTimer;
#endif
#define HTML_REMOTE           (1)                           // раскомментируйте эту строку, если вы используете управление по http

// ============= НАСТРОЙКИ =============
// --- ESP -----------------------------
#define ESP_MODE              (1U)                          // 0U - WiFi точка доступа, 1U - клиент WiFi (подключение к роутеру)
uint8_t espMode = ESP_MODE;                                 // ESP_MODE может быть сохранён в энергонезависимую память и изменён в процессе работы лампы без необходимости её перепрошивки
#define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе ESP может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
#if defined (ESP_USE_BUTTON)
#define BUTTON_STEP_TIMEOUT   (100U)                        // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#define BUTTON_TIMEOUT        (700U)                        // с какого момента начинает считаться, что кнопка удерживается в мс
#endif
#define ESP_RESET_ON_START    (false)                       // true - если при старте нажата кнопка (или кнопки нет!), сохранённые настройки будут сброшены; false - не будут
#define ESP_HTTP_PORT         (80U)                         // 80U номер порта, который будет использоваться во время первой утановки имени WiFi сети (и пароля), к которой потом будет подключаться лампа в режиме WiFi клиента (лучше не менять)
#define ESP_UDP_PORT          (8888U)                       // номер порта, который будет "слушать" UDP сервер во время работы лампы как в режиме WiFi точки доступа, так и в режиме WiFi клиента (лучше не менять)
#define ESP_CONN_TIMEOUT      (7U)                          // время в секундах (ДОЛЖНО БЫТЬ МЕНЬШЕ 8, иначе сработает WDT), которое ESP будет пытаться подключиться к WiFi сети, после его истечения автоматически развернёт WiFi точку доступа
#define ESP_CONF_TIMEOUT      (300U)                        // время в секундах, которое ESP будет ждать ввода SSID и пароля WiFi сети роутера в конфигурационном режиме, после его истечения ESP перезагружается
#define GENERAL_DEBUG                                       // если строка не закомментирована, будут выводиться отладочные сообщения
#define WIFIMAN_DEBUG         (true)                        // вывод отладочных сообщений при подключении к WiFi сети: true - выводятся, false - не выводятся; настройка не зависит от GENERAL_DEBUG
#define OTA                                                 // если строка не закомментирована, модуль будет ждать два последдовательных запроса пользователя на прошивку по воздуху (см. документацию в "шапке")
#ifdef OTA
#define ESP_OTA_PORT          (8266U)                       // номер порта, который будет "прослушиваться" в ожидании команды прошивки по воздуху
#endif
#define LED_PIN               (2U)//(0U)                          // пин ленты                (D3)  (2U - D4)
#define BTN_PIN               (4U)                          // пин кнопки               (D2)
#define MOSFET_PIN            (5U)                          // пин MOSFET транзистора   (D1) - может быть использован для управления питанием матрицы/ленты
#define ALARM_PIN             (16U)                         // пин состояния будильника (D0) - может быть использован для управления каким-либо внешним устройством на время работы будильника
#define MOSFET_LEVEL          (HIGH)                        // логический уровень, в который будет установлен пин MOSFET_PIN, когда матрица включена - HIGH или LOW
#define ALARM_LEVEL           (HIGH)                        // логический уровень, в который будет установлен пин ALARM_PIN, когда "рассвет"/будильник включен

// --- ESP (WiFi клиент) ---------------
const uint8_t STA_STATIC_IP[] = {};                         // статический IP адрес: {} - IP адрес определяется роутером; {192, 168, 1, 66} - IP адрес задан явно (если DHCP на роутере не решит иначе); должен быть из того же диапазона адресов, что разадёт роутер
                                                            // SSID WiFi сети и пароль будут запрошены WiFi Manager'ом в режиме WiFi точки доступа, нет способа захардкодить их в прошивке

// --- AP (WiFi точка доступа) ---
#define AP_NAME               ("LedLamp")                   // имя WiFi точки доступа, используется как при запросе SSID и пароля WiFi сети роутера, так и при работе в режиме ESP_MODE = 0
#define AP_PASS               ("31415926")                  // пароль WiFi точки доступа
const uint8_t AP_STATIC_IP[] = {192, 168, 4, 1};            // статический IP точки доступа (лучше не менять)
// ----ИК Управление
#define IR_PIN                (5U)                          // D1 Пин, куда подключет IR 
// --- ВРЕМЯ ---------------------------
#define USE_NTP                                             // закомментировать или удалить эту строку, если нужно, чтобы устройство не лезло в интернет
//#define NTP_ADDRESS         ("ntp2.colocall.net")         // сервер времени Глючит
//#define NTP_ADDRESS         ("ru.pool.ntp.org")
//#define NTP_ADDRESS         ("pool.ntp.org")
#define NTP_ADDRESS         ("time.windows.com") 
//#define NTP_ADDRESS         ("ntp3.stratum2.ru")
//define NTP_ADDRESS          ("ntp.msk-ix.ru")
#define NTP_INTERVAL          (30UL * 60UL * 1000UL)        // интервал синхронизации времени (30 минут)
#define SUMMER_WINTER_TIME                                  // для тех, кому нужен переход на зимнее/летнее время - оставить строку, остальным - закомментировать или удалить
#if !defined(SUMMER_WINTER_TIME)
#define LOCAL_WEEK_NUM        (week_t::Last)                // для стран, где нет перехода на зимнее/летнее время это технический параметр, не нужно его изменять
#define LOCAL_WEEKDAY         (dow_t::Sun)                  // для стран, где нет перехода на зимнее/летнее время это технический параметр, не нужно его изменять
#define LOCAL_MONTH           (month_t::Mar)                // для стран, где нет перехода на зимнее/летнее время это технический параметр, не нужно его изменять
#define LOCAL_HOUR            (1U)                          // для стран, где нет перехода на зимнее/летнее время это технический параметр, не нужно его изменять
#define LOCAL_OFFSET          (3 * 60)                      // смещение локального времени относительно универсального координированного времени UTC в минутах
#define LOCAL_TIMEZONE_NAME   ("MSK")                       // обозначение локального часового пояса; до 5 символов; может быть использовано, если понадобится его вывести после вывода времени
#else
#define SUMMER_WEEK_NUM       (week_t::Last)                // номер недели в месяце, когда происходит переход на летнее время (возможные варианты: First - первая, Second - вторая, Third - третья, Fourth - четвёртая, Last - последняя)
#define SUMMER_WEEKDAY        (dow_t::Sun)                  // день недели, когда происходит переход на летнее время (возможные варианты: Mon - пн, Tue - вт, Wed - ср, Thu - чт, Sat - сб, Sun - вс)
#define SUMMER_MONTH          (month_t::Mar)                // месяц, в котором происходит переход на летнее время (возможные варианты: Jan - январь, Feb - февраль, Mar - март, Apr - апрель, May - май, Jun - июнь, Jul - июль, Aug - август, Sep - сентябрь, Oct - октябрь, Nov - ноябрь, Dec - декабрь)
#define SUMMER_HOUR           (3U)                          // час (по зимнему времени!), когда заканчивается зимнее время и начинается летнее; [0..23]
#define SUMMER_OFFSET         (3 * 60)                      // смещение летнего времени относительно универсального координированного времени UTC в минутах
#define SUMMER_TIMEZONE_NAME  ("EEST")                      // обозначение летнего времени; до 5 символов; может быть использовано, если понадобится его вывести после вывода времени; может быть "ЛЕТ"
#define WINTER_WEEK_NUM       (week_t::Last)                // номер недели в месяце, когда происходит переход на зимнее время (возможные варианты: First - первая, Second - вторая, Third - третья, Fourth - четвёртая, Last - последняя)
#define WINTER_WEEKDAY        (dow_t::Sun)                  // день недели, когда происходит переход на зимнее время (возможные варианты: Mon - пн, Tue - вт, Wed - ср, Thu - чт, Sat - сб, Sun - вс)
#define WINTER_MONTH          (month_t::Oct)                // месяц, в котором происходит переход на зимнее время (возможные варианты: Jan - январь, Feb - февраль, Mar - март, Apr - апрель, May - май, Jun - июнь, Jul - июль, Aug - август, Sep - сентябрь, Oct - октябрь, Nov - ноябрь, Dec - декабрь)
#define WINTER_HOUR           (4U)                          // час (по летнему времени!), когда заканчивается летнее время и начинается зимнее; [0..23]
#define WINTER_OFFSET         (2 * 60)                      // смещение зимнего времени относительно универсального координированного времени UTC в минутах
#define WINTER_TIMEZONE_NAME  ("EET")                       // обозначение зимнего времени; до 5 символов; может быть использовано, если понадобится его вывести после вывода времени; может быть "ЗИМ"
#endif
#define PRINT_TIME            (0U)                          // 0U - не выводить время бегущей строкой; 1U - вывод времени каждый час; 2U - каждый час + каждые 30 минут; 3U - каждый час + каждые 15 минут
                                                            // 4U - каждый час + каждые 10 минут; 5U - каждый час + каждые 5 минут; 6U - каждый час + каждую минуту
#define NIGHT_HOURS_START     (1350U)                       // начало действия "ночного времени" (в минутах от начала суток, 22:30), текущее время бегущей строкой будет выводиться с яркостью NIGHT_HOURS_BRIGHTNESS
//#define NIGHT_HOURS_START     (1100)
#define NIGHT_HOURS_STOP      (479U)                        // конец действия "ночного времени" (в минутах от начала суток, 7:59)
#define DAY_HOURS_BRIGHTNESS  (100)                         // яркость для вывода текущего времени бегущей строкой днём; если -1, будет использована яркость текущего эффекта (она известна, даже когда матрица выключена)
#define NIGHT_HOURS_BRIGHTNESS (5)                          // яркость для вывода текущего времени бегущей строкой ночью; если -1, будет использована яркость текущего эффекта (она известна, даже когда матрица выключена)
                                                            // константы DAY_HOURS_BRIGHTNESS и NIGHT_HOURS_BRIGHTNESS используются только, когда матрица выключена, иначе будет использована яркость текущего эффекта

// --- ВНЕШНЕЕ УПРАВЛЕНИЕ --------------
#define USE_MQTT              (false)                       // true - используется mqtt клиент, false - нет
#if USE_MQTT
#define MQTT_RECONNECT_TIME   (10U)                         // время в секундах перед подключением к MQTT брокеру в случае потери подключения
#endif

// --- РАССВЕТ -------------------------
uint8_t DAWN_BRIGHT =         200U;                         // максимальная яркость рассвета (0-255)
#define DAWN_TIMEOUT          (1U)                         // сколько рассвет светит после времени будильника, минут
uint8_t DAWN_FOLDER =         1;                            // Номер папки для рассвета

// --- МАТРИЦА -------------------------
#define BRIGHTNESS            (40U)                         // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT         (3000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
//#define CURRENT_LIMIT         (0U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

//#define WIDTH                 (4U)                          // ширина матрицы
//#define HEIGHT                (17U)                         // высота матрицы

#define WIDTH                 (16U)                         // ширина матрицы
#define HEIGHT                (16U)                         // высота матрицы

#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (1U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (3U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
                                                            // шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/


// ============= ДЛЯ РАЗРАБОТЧИКОВ =====
                                                            // список и номера эффектов ниже в списке согласованы с android приложением!
#define EFF_SPARKLES          (0U)                          // (0U)  Конфетти
#define EFF_FIRE              (1U)                          // (1U)  Огонь
#define EFF_FIRE2012          (2U)                          // (2U)  Водопад
#define EFF_RAINBOW_VER       (3U)                          // (3U)  Радуга вертикальная
#define EFF_RAINBOW_HOR       (4U)                          // (4U)  Радуга горизонтальная
#define EFF_RAINBOW_DIAG      (5U)                          // (5U)  Радуга диагональная
#define EFF_COLORS            (6U)                          // (6U)  Смена цвета
#define EFF_MADNESS           (7U)                          // (7U)  Безумие 3D
#define EFF_CLOUDS            (8U)                          // (8U)  Облака 3D
#define EFF_LAVA              (9U)                          // (9U)  Лава 3D
#define EFF_PLASMA            (10U)                         // (10U) Плазма 3D
#define EFF_RAINBOW           (11U)                         // (11U) Радуга 3D
#define EFF_RAINBOW_STRIPE    (12U)                         // (12U) Павлин 3D
#define EFF_ZEBRA             (13U)                         // (13U) Зебра 3D
#define EFF_FOREST            (14U)                         // (14U) Лес 3D
#define EFF_OCEAN             (15U)                         // (15U) Океан 3D
#define EFF_COLOR             (16U)                         // (16U) Цвет
#define EFF_SNOW              (17U)                         // (17U) Снегопад
#define EFF_SNOWSTORM         (18U)                         // (18U) Метель
#define EFF_STARFALL          (19U)                         // (19U) Звездопад
#define EFF_MATRIX            (20U)                         // (20U) Матрица
#define EFF_LIGHTERS          (21U)                         // (21U) Светлячки
#define EFF_LIGHTER_TRACES    (22U)                         // (22U) Светлячки со шлейфом
#define EFF_PAINTBALL         (23U)                         // (23U) Пейнтбол
#define EFF_CUBE              (24U)                         // (24U) Блуждающий кубик
#define EFF_COMET             (25U)                         // (25U) Комета
#define EFF_PULSE             (26U)                         // (26U) Пульс
#define EFF_MATRIX2           (27U)                         // (27U) Матрица 2
#define EFF_WATERPOLL         (28U)                         // (28U) Бассейн              
#define EFF_NOISESTREAMS      (29U)                         // (29U) 
#define EFF_METABALLS         (30U)                         // (30U) 
#define EFF_SINUSOID3         (31U)                         // (31U) 
#define EFF_FIRE_2020         (32U)                         // (32U) Новый огонь 2020
#define EFF_SPIRO             (33U)                         // (33U) Спираль
#define EFF_BBALS             (34U)                         // (34U) Прыгающие мячики
#define EFF_FLOCK             (35U)                         // (35U) Стая
#define EFF_RAIN              (36U)                         // (36U) Типа дождь
#define EFF_SALUTE            (37U)                         // (37U) Салют
#define EFF_FIRE_2018         (38U)                         // (38U) Огонь 2018
#define EFF_PRISMATA          (39U)                         // (39U) Призмата
#define EFF_SIMPLE_RAIN       (40U)                         // (40U) Тучка в банке
#define EFF_STORMY_RAIN       (41U)                         // (41U) Гроза в банке
#define EFF_COLOR_RAIN        (42U)                         // (42U) Осадки
#define EFF_SMOKE             (43U)                         // (43U) Дым 
#define EFF_SMOKE_COLOR       (44U)                         // (44U) Paзнoцвeтный дым
#define EFF_PICASSO           (45U)                         // (45U) Пикacco
#define EFF_PICASSO2          (46U)                         // (46U) Пикacco 2
#define EFF_PICASSO3          (47U)                         // (47U) Kpyги Пикacco
#define EFF_CUBE2D            (48U)                         // (48U) Kyбик Pyбикa
#define EFF_DNA               (49U)                         // (49U) ДHK
#define EFF_SNAKES            (50U)                         // (50U) 3мeйки
#define EFF_BALLS_BOUNCE      (51U)                         // (51U) Mячики бeз гpaниц
#define EFF_POPCORN           (52U)                         // (52U) Пoпкopн
#define EFF_BUTTERFLYS        (53U)                         // (53U) Moтыльки
#define EFF_BUTTERFLYS_LAMP   (54U)                         // (54U) Лaмпa c мoтылькaми
#define EFF_LIQUIDLAMP        (55U)                         // (55U) Жидкaя лaмпa
#define EFF_LIQUIDLAMP_AUTO   (56U)                         // (56U) Жидкaя лaмпa (auto)
#define EFF_LEAPERS           (57U)                         // (57U) Пpыгyны
#define EFF_TWINKLES          (58U)                         // (58U) Mepцaниe
#define EFF_BBALLS_TRACES     (59U)                         // (59U) Mячики co шлeйфoм
#define EFF_SHADOWS           (60U)                         // (60U) Teни

#define EFF_TEXT              (61U)                         // (61U) Бегущая строка
#define EFF_PAINTING          (62U)                         // (62U) Рисование из приложения 
#define EFF_CLOCK             (63U)                         // (63U) стрелочные часы
#define EFF_WHITE_COLOR       (64U)                         // (64U) Белый свет
#define EFF_VOICE             (65U)                         // (65U) Громкость
#define MODE_AMOUNT           (66U)                         // (66U) количество режимов

//#define MAX_UDP_BUFFER_SIZE (UDP_TX_PACKET_MAX_SIZE + 1)
#define MAX_UDP_BUFFER_SIZE   (255U)                        // максимальный размер буффера UDP сервера

//#define GENERAL_DEBUG_TELNET  (true)                        // true - отладочные сообщения будут выводиться в telnet вместо Serial порта (для удалённой отладки без подключения usb кабелем)
#define TELNET_PORT           (23U)                         // номер telnet порта

#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
WiFiClient telnet;                                          // обработчик событий telnet клиента
bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"
#define LOG                   telnet
#else
#define LOG                   Serial
#endif

// --- БИБЛИОТЕКИ ----------------------
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER

#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
#define SPARKLES 1        // вылетающие угольки вкл выкл



// ================ РЕЕСТР ДОСТУПНЫХ ЭФФЕКТОВ ===================
// ==== ДЛЯ ПЕРЕДАЧИ В ПРИЛОЖЕНИЯ С ПОДДЕРЖКОЙ ЭТОЙ ФУНКЦИИ =====
// формат записи:
//"Название эффекта,min_скорость,max_скорость,min_масштаб,max_масштаб,выбор_ли_цвета_это(0-нет,1-да, 2-для кометы);"
//                  это максимальная длина строчки с названием и параметрами
// Порядок эффектов можно менять на своё усмотрение, не забывая при этом менять номера в списке ВЫШЕ
// Можно добавлять новые или удалять ненужные эффекты сохраняя количество эффектов в каждой строке примерно одинаковым, т.е.
// общее количество эффектов делим на три строки
// Передаются частями из 3-х строк. !!!ЭФФЕКТ БЕГУЩАЯ СТРОКА ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ ВСЕГДА!!!
// ОЧЕНЬ ВАЖНО!!! Следите за знаками при редактировании эффектов. Не допускайте пробелов, кроме как в названии эффекта.
// В кавычках Имя эффекта и его параметры должны быть разделены запятой, а вконце точка с запятой.

const String NameDayFoWeek[]= {"", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс", "Пн-Пт", "Сб,Вс"};

struct struct_effects {
 String Name;
 uint8_t Speed_min;
 uint8_t Speed_max;
 uint8_t Scale_min;
 uint8_t Scale_max;
 uint8_t Scale;
 uint8_t Folder;
};
//"Имя",Яркость от,Яркость до,Скорость от,Скорость до,Масштаб/цвет,папка MP3 (0-без звука, 255 выбор общей папки)
const struct_effects effects_name[MODE_AMOUNT] = {
{"Конфетти",               50,200,1,100,0,255  },    // (0U)  EFF_SPARKLES          Конфетти                   
{"Огонь",                  15, 50,1,100,1,3    },    // (1U)  EFF_FIRE              Огонь                      
{"Водопад",                50,150,1,100,1,5    },    // (2U)  EFF_FIRE2012          Водопад                    
{"Радуга горизонтальная",  30,200,1,100,0,255  },    // (3U)  EFF_RAINBOW_VER       Радуга вертикальная        
{"Радуга вертикальная",    30,200,1,100,0,255  },    // (4U)  EFF_RAINBOW_HOR       Радуга горизонтальная      
{"Радуга диагональная",    30,200,1,100,0,255  },    // (5U)  EFF_RAINBOW_DIAG      Радуга диагональная        
{"Смена цвета",            10,200,1,100,0,255  },    // (6U)  EFF_COLORS            Смена цвета                
{"Безумие 3D",             40,200,1,100,0,255  },    // (7U)  EFF_MADNESS           Безумие 3D                 
{"Облака 3D",              40,200,1,100,0,255  },    // (8U)  EFF_CLOUDS            Облака 3D                  
{"Лава 3D",                40,200,1,100,0,255  },    // (9U)  EFF_LAVA              Лава 3D                    
{"Плазма 3D",              40,200,1,100,0,255  },    // (10U) EFF_PLASMA            Плазма 3D                  
{"Радуга 3D",              40,200,1,100,0,255  },    // (11U) EFF_RAINBOW           Радуга 3D                  
{"Павлин 3D",              40,200,1,100,0,255  },    // (12U) EFF_RAINBOW_STRIPE    Павлин 3D                  
{"Зебра 3D",               40,200,1,100,0,255  },    // (13U) EFF_ZEBRA             Зебра 3D                   
{"Лес 3D",                 40,200,1,100,0,255  },    // (14U) EFF_FOREST            Лес 3D                     
{"Океан 3D",               40,200,1,100,0,4    },    // (15U) EFF_OCEAN             Океан 3D                   
{"Цвет",                   50,200,1,100,1,255  },    // (16U) EFF_COLOR             Цвет                       
{"Снегопад",               30,200,1,100,0,255  },    // (17U) EFF_SNOW              Снегопад                   
{"Метель",                 30,200,1,100,0,255  },    // (18U) EFF_SNOWSTORM         Метель                     
{"Звездопад",              30,200,1,100,0,255  },    // (19U) EFF_STARFALL          Звездопад                  
{"Радужный дождь",         30,200,1,100,0,255  },    // (20U) EFF_MATRIX            Матрица                    
{"Светлячки",              30,200,1,100,0,255  },    // (21U) EFF_LIGHTERS          Светлячки                  
{"Светлячки со шлейфом",   30,150,1,100,0,255  },    // (22U) EFF_LIGHTER_TRACES    Светлячки со шлейфом       
{"Пейнтбол",               50,200,1,100,0,255  },    // (23U) EFF_PAINTBALL         Пейнтбол                   
{"Блуждающий кубик",       30,150,1,100,1,255  },    // (24U) EFF_CUBE              Блуждающий кубик           
{"Комета",                 20,200,1,100,2,255  },    // (25U) EFF_COMET             Комета                     
{"Пульс",                  50,200,1,100,0,255  },    // (26U) EFF_PULSE             Пульс                      
{"Матрица",                20,200,1,100,0,255  },    // (27U) EFF_MATRIX2           Матрица 2                  
{"Бассейн",                50,200,1,100,1,4    },    // (28U) EFF_WATERPOLL         Бассейн                    
{"Кометы",                 10,200,1,100,2,255  },    // (29U) EFF_NOISESTREAMS      Кометы
{"Меташарики",             15, 15,1,100,0,255  },    // (30U) EFF_METABALLS         Meтaбoлз                            
{"Синусоидз",              15, 15,1,100,0,255  },    // (31U) EFF_SINUSOID3         Cинycoид
{"Новый огонь 2020",       15,100,1,100,1,3    },    // (32U) EFF_FIRE_2020         Новый огонь 2020           
{"Спираль",                15, 15,1,100,0,255  },    // (33U) EFF_SPIRO             Спираль                    
{"Прыгающие мячики",       10, 10,1,100,0,255  },    // (34U) EFF_BBALS             Прыгающие мячики           
{"Стая",                   10, 20,1,100,0,255  },    // (35U) EFF_FLOCK             Стая                       
{"Дождь",                  50,150,1,100,1,5    },    // (36U) EFF_RAIN              Типа дождь                 
{"Салют",                  30,100,1,100,0,255  },    // (37U) EFF_SALUTE            Салют                      
{"Степной пожар",          10,200,1,100,0,3    },    // (38U) EFF_FIRE_2018         Огонь 2018                 
{"Призмата",               15, 15,1,100,0,255  },    // (39U) EFF_PRISMATA          Призмата                   
{"Тучка в банке",          10,200,1,100,0,5    },    // (40U) EFF_SIMPLE_RAIN       Тучка в банке              
{"Гроза в банке",          10,200,1,100,0,5    },    // (41U) EFF_STORMY_RAIN       Гроза в банке              
{"Осадки",                 10,200,1,100,1,5    },    // (42U) EFF_COLOR_RAIN        Осадки                     
{"Дым",                    15, 15,1,100,1,255  },    // (43U) EFF_SMOKE             Дым                        
{"Paзнoцвeтный дым",       15, 15,1,100,0,255  },    // (44U) EFF_SMOKE_COLOR       Paзнoцвeтный дым           
{"Пикacco 1",              10,200,1,100,0,255  },    // (45U) EFF_PICASSO           Пикacco                    
{"Пикacco 2",              10,200,1,100,0,255  },    // (46U) EFF_PICASSO2          Пикacco 2                  
{"Kpyги Пикacco",          10,200,1,100,0,255  },    // (47U) EFF_PICASSO3          Kpyги Пикacco              
{"Kyбик Pyбикa",           10,200,1,100,0,255  },    // (48U) EFF_CUBE2D            Kyбик Pyбикa               
{"ДHK",                    15, 15,1,100,2,255  },    // (49U) EFF_DNA               ДHK                        
{"3мeйки",                 15, 15,1,100,0,255  },    // (50U) EFF_SNAKES            3мeйки                     
{"Mячики бeз гpaниц",      15, 15,1,100,0,255  },    // (51U) EFF_BALLS_BOUNCE      Mячики бeз гpaниц          
{"Пoпкopн",                15, 15,1,100,0,255  },    // (52U) EFF_POPCORN           Пoпкopн                    
{"Moтыльки",               15, 15,1,100,0,255  },    // (53U) EFF_BUTTERFLYS        Moтыльки                   
{"Лaмпa c мoтылькaми",     15, 15,1,100,1,255  },    // (54U) EFF_BUTTERFLYS_LAMP   Лaмпa c мoтылькaми         
{"Жидкaя лaмпa",           15, 15,1,100,1,255  },    // (55U) EFF_LIQUIDLAMP        Жидкaя лaмпa               
{"Жидкaя лaмпa (auto)",    15, 15,1,100,0,255  },    // (56U) EFF_LIQUIDLAMP_AUTO   Жидкaя лaмпa (auto)        
{"Пpыгyны",                10,200,1,100,0,255  },    // (57U) EFF_LEAPERS           Пpыгyны                    
{"Mepцaниe",               10,200,1,100,1,255  },    // (58U) EFF_TWINKLES          Mepцaниe                   
{"Mячики co шлeйфoм",      15, 15,1,100,0,255  },    // (59U) EFF_BBALLS_TRACES     Mячики co шлeйфoм
{"Тени",                   15, 15,1,100,0,255  },    // (60U) EFF_SHADOWS           Teни                       
                                                                                                              
{"Бегущая строка",         50, 50,1,100,1,255  },    // (61U) EFF_TEXT              Бегущая строка             
{"Рисование",              10,200,1,100,0,255  },    // (62U) EFF_PAINTING          Рисование из приложения    
{"Часы",                   10,200,1,100,1,0    },    // (63U) EFF_CLOCK             стрелочные часы            
{"Белый свет",             10,200,1,100,2,255  },    // (64U) EFF_WHITE_COLOR       Белый свет                 
{"Громкость",               0,30 ,0,30 ,0,255  }     // (65U) EFF_VOICE             Громкость                  
};                                 

// Для Котейки
String efList_1 = String("LIST1;");           // ПЕРВАЯ СТРОКА 
String efList_2 = String("LIST2;");           // ВТОРАЯ СТРОКА
String efList_3 = String("LIST3;");           // ТРЕТЬЯ СТРОКА
