//Восстановлене нарисованной картинки
void PaintRoutine(bool flag_load_EEPROM)
{
  if (loadingFlag)
  {
    loadingFlag = false;
    if (flag_load_EEPROM) Read_Picure_from_EEPROM();
    for (uint8_t x =0; x<WIDTH; x++)
    for (uint8_t y =0; y<HEIGHT; y++)
    {
      drawPixelXY(x, y, paint[x][y]);
    }
  }

}


//Функция точки на цилиндре
void drawPixelXYCylinder(int x, int y, CRGB color)
{
 if (y>=0 && y<HEIGHT) {
   while (x<0) x+=WIDTH;
    drawPixelXY(x % WIDTH, y, color);
   }
}

uint32_t getPixColorXYCylinder(uint8_t x, uint8_t y)
{
  while (x<0) x+=WIDTH;
  return getPixColor(getPixelNumber(x % WIDTH, y));
}


//Разноцветная матрица (Дождь)
// 
// Масштаб - 1 - изменение цвета

void RainRoutine()
{
  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    // заполняем случайно верхнюю строку
    CRGB thisColor = getPixColorXY(x, HEIGHT - 1U);
    if ((uint32_t)thisColor == 0U)
    {
     if (random(0, 50) == 0U)
      drawPixelXY(x, HEIGHT - 1U, CHSV(modes[currentMode].Scale*2.40+random(0, 16),255,255));
    }
    else
       leds[getPixelNumber(x,HEIGHT - 1U)]-=CHSV(0,0,random(96, 128));
  }
  // сдвигаем всё вниз
  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
    {
      drawPixelXY(x, y, getPixColorXY(x, y + 1U));
    }
  }
}

/*
 Эффект часов для gyver лампы
 Цвет часовой и минутной стрелки регулируются скоростью и масштабом
 
 вызов : ClockRoutine(WIDTH,HEIGHT,true);
 цвет меток циферблата - радужный, цвет метки возле минутной стрелки стремится к цвету минутной стрелки
 фон черный.
 Ширина, высота, требуется ли очистить экран;

 вызов : ClockRoutine(WIDTH,HEIGHT,false);
 цвет всех елементов - белый
 фон предыдущий.
 
 Примеры вызова
   с наложением
   case EFF_COLOR:          colorRoutine();   ClockRoutine(WIDTH,HEIGHT,false);           break;
   без наложения
   case EFF_новое:         ClockRoutine(WIDTH,HEIGHT,true);           break;
   для цилиндрических ламп наверно лучще будет
   case EFF_новое:         ClockRoutine(10,16,true);           break;
*/

void ClockRoutine(uint8_t cl_w, uint8_t cl_h, bool fon_clear)
{
  float r_clock;
  int c_x1;
  int c_y1;
  int c_x2;
  int c_y2;
  float dx=(cl_w % 2 ==1) ? 0.49 : 0;
  float dy=(cl_h % 2 ==1) ? 0.49 : 0;
  int c_cx=(WIDTH-cl_w)/2;
  int c_cy=(HEIGHT-cl_h)/2;
  CHSV color = CHSV(0,(modes[currentMode].Speed>150)?0:255,(modes[currentMode].Speed<150)?0:255);
  int bridges=(thisTime < NIGHT_HOURS_START && thisTime > NIGHT_HOURS_STOP)?255:64;
//Очистка
 if (fon_clear) FastLED.clear();

//Циферблат
// метки 1,2,4,5, 7,8, 10,11 (цвет метка сопровождает цвет минутной стрелки
 for (int i=0; i<12; i++)
 {
  r_clock=PI*i/6;
  c_x1=sin(r_clock)*(cl_w/2+dx-0.1)+cl_w/2+dx+c_cx;
  c_y1=cos(r_clock)*(cl_h/2+dy-0.1)+cl_h/2+dy+c_cy;
  
  if (fon_clear) color=CHSV(i*21-map(thisTime%60,0,59,0,255)+map(modes[currentMode].Scale,1,100,0,255),255,bridges/4);
  drawPixelXY(c_x1, c_y1, color);
 }
// метки 3, 9, 
 if (cl_h % 2 ==0) {
 if (fon_clear) color= CHSV(192-map(thisTime%60,0,59,0,255)+map(modes[currentMode].Scale,1,100,0,255),255,bridges/2);
 DrawLine(c_cx, cl_h/2-1+c_cy,  c_cx, cl_h/2+c_cy, color);
 if (fon_clear) color= CHSV(64-map(thisTime%60,0,59,0,255)+map(modes[currentMode].Scale,1,100,0,255),255,bridges/2);
 DrawLine(c_cx+cl_w-1,cl_h/2-1+c_cy,c_cx+cl_w-1,cl_h/2+c_cy, color);
 }
// метки 12, 6, 
 if (cl_w % 2 ==0) {
 if (fon_clear) color= CHSV(128-map(thisTime%60,0,59,0,255)+map(modes[currentMode].Scale,1,100,0,255),255,bridges/2);
 DrawLine(cl_w/2-1+c_cx, c_cy,    cl_w/2+c_cx,    c_cy, color);
 if (fon_clear) color= CHSV(-map(thisTime%60,0,59,0,255)+map(modes[currentMode].Scale,1,100,0,255),255,bridges/2) ;
 DrawLine(cl_w/2-1+c_cx, cl_h-1+c_cy, cl_w/2+c_cx, cl_h-1+c_cy,color);
 } 
// Минутная стрелка
 r_clock=PI*(thisTime % 60)/30;
 c_x1=sin(r_clock)+cl_w/2+dx+c_cx;
 c_y1=cos(r_clock)+cl_h/2+dy+c_cy;
 c_x2=sin(r_clock)*(cl_w/2.5)+cl_w/2+dx+c_cx;
 c_y2=cos(r_clock)*(cl_h/2.5)+cl_h/2+dy+c_cy;
 if (fon_clear) color=CHSV(map(modes[currentMode].Scale,1,100,0,255),255,bridges);
 DrawLine(c_x1,c_y1,c_x2,c_y2,color);

// Часовая стрелка
 r_clock=PI*(thisTime % 720)/360;
 c_x1=sin(r_clock)+cl_w/2+dx+c_cx;
 c_y1=cos(r_clock)+cl_h/2+dy+c_cy;
 c_x2=sin(r_clock)*(cl_w/3)+cl_w/2+dx+c_cx;
 c_y2=cos(r_clock)*(cl_h/3)+cl_h/2+dy+c_cy;
 if (fon_clear) color=CHSV(modes[currentMode].Speed,255,bridges) ;
 DrawLine(c_x1,c_y1,c_x2,c_y2,color);
}


//Дополнитеьная функция построения линий
void DrawLine(int x1, int y1, int x2, int y2, CHSV color)
{
  int tmp;
  int x,y;
  int dx, dy;
  int err;
  int ystep;

  uint8_t swapxy = 0;
  
  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;

  if ( dy > dx ) 
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 ) 
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
  err = dx >> 1;
  if ( y2 > y1 ) ystep = 1; else ystep = -1;
  y = y1;

  for( x = x1; x <= x2; x++ )
  {
    if ( swapxy == 0 ) drawPixelXY(x, y, color);
    else drawPixelXY(y, x, color);
    err -= (uint8_t)dy;
    if ( err < 0 ) 
    {
      y += ystep;
      err += dx;
    }
  }
}


// Салют
uint8_t SaluteStep;

void SaluteRoutine()
{
  if (loadingFlag) {
    SaluteStep=0;
    loadingFlag = false;
  }
  switch (SaluteStep) {
   case 0:SaluteStart();          break;
   case 1:SaluteDrawLine();       break;
   case 2:SaluteExplosion();      break;
   case 3:SaluteDecay();          break;
  }
  SaluteFadeAll(32);
}

uint8_t SaluteX1;
uint8_t SaluteX2;
uint8_t SaluteY1;
uint8_t SaluteY2;
int SaluteX,SaluteY;
int SaluteDX, SaluteDY;
int Saluteerr;
int Saluteystep;
uint8_t Saluteswapxy;
CRGB SaluteColor;
uint8_t SaluteR;


void SaluteStart()
{
 SaluteStep=1;
 SaluteColor=CHSV(random(0, 9) * 28, 255U, 255U);
 SaluteY1=0;
 SaluteY2=random(HEIGHT*2/3,HEIGHT);
 SaluteX1=random(0,WIDTH);;
 SaluteX2=random(SaluteX1,WIDTH);
// SaluteX2=random(0,WIDTH);
 Saluteswapxy = 0;
  
  SaluteDX = SaluteX2-SaluteX1;
  SaluteDY = SaluteY2;

  int tmp;
/*
  if ( SaluteX1 > SaluteX2 ) 
  {
    tmp = SaluteX1; SaluteX1 = SaluteX2; SaluteX2 = tmp;
    tmp = SaluteY1; SaluteY1 = SaluteY2; SaluteY2 = tmp;
  }
*/  
  if ( SaluteDY > SaluteDX ) 
  {
    Saluteswapxy = 1;
    tmp = SaluteDX; SaluteDX = SaluteDY; SaluteDY = tmp;
    tmp = SaluteX1; SaluteX1 = SaluteY1; SaluteY1 = tmp;
    tmp = SaluteX2; SaluteX2 = SaluteY2; SaluteY2 = tmp;
  }
  Saluteerr = SaluteDX >> 1;
  if ( SaluteY2 > SaluteY1 ) Saluteystep = 1; else Saluteystep = -1;
  SaluteY = SaluteY1;
  SaluteX = SaluteX1;
}

void SaluteFadeAll(uint8_t val)
{
  for (uint8_t x=0; x < WIDTH; x++)
   for (uint8_t y=0; y < HEIGHT; y++)
        leds[getPixelNumber(x,y)]-=CHSV(0,0,val);

}
void SaluteDrawLine()
{
  if (SaluteX<SaluteX2)
  {
    if ( Saluteswapxy == 0 ) drawPixelXY(SaluteX, SaluteY, SaluteColor);
    else drawPixelXY(SaluteY, SaluteX, SaluteColor);
    Saluteerr -= (uint8_t)SaluteDY;
    if ( Saluteerr < 0 ) 
    {
      SaluteY += Saluteystep;
      Saluteerr += SaluteDX;
    }
   SaluteX++; 
  } else { 
    SaluteStep=2;
    SaluteColor=CHSV(random(0, 9) * 28, 128U, 255U);
    SaluteR=1;
  }
}

void SaluteExplosion()
{
 if (SaluteR<map(modes[currentMode].Scale,1,100,2,WIDTH*0.7))
 {
   if ( Saluteswapxy == 0 ) 
//     DrawCircleCylinder(SaluteX, SaluteY, SaluteR, SaluteColor-CHSV(0, 0, SaluteR*64)+CHSV(0, SaluteR*32, 0)); //Использовать для цилиндрических ламп
           drawCircle(SaluteX, SaluteY, SaluteR, SaluteColor-CHSV(0, 0, SaluteR*64)+CHSV(0, SaluteR*32, 0));
   else 
//     DrawCircleCylinder(SaluteY, SaluteX, SaluteR, SaluteColor-CHSV(0, 0, SaluteR*64)+CHSV(0, SaluteR*32, 0)); //Использовать для цилиндрических ламп
           drawCircle(SaluteY, SaluteX, SaluteR, SaluteColor-CHSV(0, 0, SaluteR*64)+CHSV(0, SaluteR*32, 0));
   SaluteR++;  
 } else  SaluteStep=3;
} 

void SaluteDecay()
{
 if (SaluteR>0) SaluteR--; 
 else SaluteStep=0;
} 


//4 точки. Требуется для окружности и элипса
void pixel4(int x, int y, int xc, int yc, CRGB color)
{
    drawPixelXYCylinder(x+xc,y+yc,color);
    drawPixelXYCylinder(x+xc,-y+yc,color);
    drawPixelXYCylinder(-x+xc,-y+yc,color);
    drawPixelXYCylinder(-x+xc,y+yc,color);

}
//8 точек Требуется для окружности
void sim(int x, int y, int xc, int yc, CRGB color)
{
  pixel4(x,y,xc,yc, color);
  pixel4(y,x,xc,yc, color);
}

//Функция построения окружности на цилиндре
void DrawCircleCylinder(int xc, int yc, int r, CRGB color)
{
 int x=0;
 int y=r;
 int d=3-2*y;;
  while(x <= y) 
  {
     sim(x,y,xc,yc,color);
     if (d<0) d=d+4*x+6;
     else d=d+4*(x-y--)+10;
     x++;
    }
   if (x==y) sim(x,y,xc,yc,color); 
}     

// Бегущая строка от Koteyka
void text_running() {
//    while (!fillString(TextTicker, CHSV(modes[EFF_TEXT].Scale, 255U, 255U), true) && currentMode == EFF_TEXT) {
    while (!fillString(TextTicker, CHSV(modes[EFF_TEXT].Scale, 255U, 255U))) {
      parseUDP();
      #ifdef ESP_USE_BUTTON
      if (buttonEnabled)
      {
        buttonTick();
      }
      #endif
    }
//    Serial.println(TextTicker);
}
