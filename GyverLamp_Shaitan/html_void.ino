#ifdef HTML_REMOTE


void handle_main() {
client.print(
"<html>\n<html_head>\n<meta charset='utf-8'>\n<meta http-equiv='Cache-Control' content='no-cache'/>\n<title>LedLamp</title>\n<style>\n\
.slider {-webkit-appearance: none; width: 90%;height: 5%;background: #d3d3d3;}\n\
.slider:hover {opacity: 1;}\n\
.slider::-webkit-slider-runnable-track {height: 130%;-webkit-appearance: none;color: #13bba4;margin-top: -1px;}\n\
.slider::-webkit-slider-thumb {-webkit-appearance: none;appearance: none;width: 10%;height: 130%;background: #4CAF50;cursor: pointer;}\n\
.form_radio_group {display: inline-block;overflow: hidden; width: 99%;}\n\
.form_radio_group-item {display: inline-block;float: left;width: 9.9%;}\n\
.form_radio_group input[type=radio] {display: none;}\n\
.form_radio_group label {display: inline-block;cursor: pointer;line-height: 30px; width: 100%; border: 1px solid #999;border-right: none;user-select: none; background: linear-gradient(to bottom, #f8f8fa 0%,#c8c8c7 100%)}\n\
.form_radio_group .form_radio_group-item:first-child label {border-radius: 6px 0 0 6px;}\n\
.form_radio_group .form_radio_group-item:last-child label {border-radius: 0 6px 6px 0;border-right: 1px solid #999;}\n\
.form_radio_group input[type=radio]:checked + label {background: linear-gradient(to bottom, #ffe0a6 0%,#dfb0a6 100%);}\n\
.form_radio_group input[type=checkbox]:checked + span { background: linear-gradient(to bottom, #ffe0a6 0%,#dfb0a6 100%);}\n\
.form_radio_group input[type=time]:checked + span { background: linear-gradient(to bottom, #ffe0a6 0%,#dfb0a6 100%);}\n\
.form_radio_group .form_radio_group-item input[type=checkbox] {position: absolute;  left: -10000px;}\n\
.form_radio_group .form_radio_group-item label span {  display: block; padding: 5px;  border: 2px;  border-radius: 5px;}\n\
</style>\n\
<script>\n\
var touchStart = null;\n\
var touchPosition = null;\n\
function TouchStart(e) {\n\
  touchStart = { x: e.changedTouches[0].clientX, y: e.changedTouches[0].clientY };\n\
  touchPosition = { x: touchStart.x, y: touchStart.y };\n }\n\n\
function TouchEnd(e) {\n\
 touchPosition = { x: e.changedTouches[0].clientX, y: e.changedTouches[0].clientY };\n\
 var d =\n\
    {\n\
     x: touchStart.x - touchPosition.x,\n\
     y: touchStart.y - touchPosition.y\n\
    }; \n\
  if(Math.abs(d.x) > Math.abs(d.y))\n\
    {\n\
      if(Math.abs(d.x) > 20)\n\
     {\n\
       if(d.x > 0) { document.location.href = '?WWW="+String((HTML_page+1) % HTML_pages)+"';}\n\
       else {document.location.href = '?WWW="+String((HTML_page+HTML_pages-1) % HTML_pages)+"';}\n\
     }\n\
    }\n\
 }");
client.print("</script>\n</html_head>\n\
<body ontouchstart='TouchStart(event);' ontouchend='TouchEnd(event);' touchstart='TouchStart(event);' touchend='TouchEnd(event);'>\n\
<center>\n\ 
<form>\n\
<button name='WWW' id='WWW' type='submit' value='");
client.print((HTML_page+HTML_pages-1) % HTML_pages);
client.print("' style='height:10%; width:10%; font-size:200%'> < </button>\n<button name='currentMode' id='PREV' type='submit' value='");
client.print((currentMode+MODE_AMOUNT-1) % MODE_AMOUNT);
client.print("' style='height:8%; width:15%;font-size:200%'>PREV</button>");

if (ONflag) client.print("<input name='ON' id='ON' type='submit' value='ON' style='background: #00FF00");
       else client.print("<input name='OFF' id='OFF' type='submit' value='OFF' style='background: #FF0000");

client.print("; height:7%; width:25%; font-size:200%'><button name='FAV' id='FAV' type='submit' value='");
if (FavoritesManager::FavoritesRunning) client.print("F_ON' style='background: #00FF00");
                                   else client.print("F_OFF' style='background: #FF0000");
client.print("; height:7%; width:15%; font-size:200%'>Fav</button>\n<button name='currentMode' id='NEXT' type='submit' value='");
client.print((currentMode+1) % MODE_AMOUNT);
client.print("' style='height:8%; width:15%; font-size:200%'>NEXT</button>\n<button name='WWW' id='WWW' type='submit' value='");
client.print((HTML_page+1) % HTML_pages);
client.print("' style='height:10%; width:10%; font-size:200%'> > </button>\n</form>\n");
}
// Окончание страниц
void down_main() {
client.print("</center>\n</body>\n</html>\n");
}
// Основная страница
void Page0() {
 client.print("<h1><a href='?WWW=3'>"+ effects_name[currentMode].Name+"</a></h1>");
 client.print(
"<form onsubmit='return false' onchange=\"location='\/?BRI='+BRI.valueAsNumber+'\&SPD='+SPD.valueAsNumber+'\&SCA='+SCA.valueAsNumber+'&VOL='+VOL.valueAsNumber+'&'\" class='slider100'>\
<div style='font-size:300%'>Яркость<br>\n<input name='BRI' id='BRI' type='range' min='1' max='255' value='");
client.print(modes[currentMode].Brightness);
client.print("' step='1' class='slider'  style='background: linear-gradient(to right, black, brown, yellow, #FFFFDD);'></div>\<br><br><div style='font-size:300%'>SPD<br>\n<input name='SPD' id='SPD' type='range' min='");
if (currentMode==EFF_VOICE) client.print("0' max='30' value='");
                       else client.print("1' max='255' value='");
client.print(modes[currentMode].Speed);  
client.print("' step='1' class='slider' style='background: linear-gradient(to right,");
if (currentMode==EFF_CLOCK) 
   client.print(" red, orange, yellow, green, cyan, blue, violet, red);'");
   else
   client.print(" blue, red);'");
client.print("></div>\n<br><br><div style='font-size:300%'>SCA<br>\n<input name='SCA' id='SCA' type='range' min='");
if (currentMode==EFF_VOICE) client.print("0' max='30' value='");
                       else client.print("1' max='100' value='");
client.print(modes[currentMode].Scale);
client.print("' step='1' class='slider' style='background: linear-gradient(");
if (effects_name[currentMode].Scale==1) 
   client.print("to right, red, orange, yellow, green, cyan, blue, violet, red);'");
   else
   client.print("45deg,black 25%, transparent 25%, transparent 74%, black 75%, black),\n\ 
linear-gradient(45deg,black 25%, transparent 25%, transparent 74%, black 75%, black),\n\
linear-gradient(to right, black, brown, yellow, #FFFFDD);\n\
background-position: 0 0, 10px 10px;\n\
background-size: 20px 20px, 20px 20px, 100%;'");
client.print("></div>\n<br><br><div style='font-size:300%'>Громкость<br>\n<input name='VOL' id='VOL' type='range' min='0' max='30' value='");
client.print(modes[EFF_VOICE].Speed);
client.print("' step='1' class='slider' style='background: linear-gradient(to right, black, green, #DDFFDD);'>\n</div>\n</form>\n");

if (currentMode==EFF_TEXT) 
client.print("<br><h1>Бегущая строка</h1>\n<form>\n\
<input type='text' name='TXT' size='40' style='width:99%; font-size:250%' value='"+String(TextTicker)+"'><br>\n\
<button type='submit' style='width:99%; font-size:250%'>Set</button>\n</form>\n");

}

// Страница настроки будильников и усьтановки времени
void Page1() {
client.print("<h1>Будильники</h1>\n<form>\n");
//Будильники
for (int i=1;i<8;i++)
 {
   String html_H=String(alarms[i-1].Time / 60);
   if (html_H.length()==1) html_H="0"+html_H;
   String html_M=String(alarms[i-1].Time % 60);
   if (html_M.length()==1) html_M="0"+html_M;
//
   client.print("\
<div class='form_radio_group' style='height:6%; width:50%;font-size:250%;'>\n\
<div class='form_radio_group-item' style='width:30%;'>\n<label style='height:85%'>\n");
   
   client.print("<input type='checkbox' name='on_alarm"+String(i)+"' value='on' ");
   if (alarms[i-1].State) client.print("checked");
   client.print(">\n<span style='height:80%'>");
//Название
   client.print(String(i)+"."+NameDayFoWeek[i]+".");
   
   client.print("</span>\n</label>\n</div>\n");
   client.print("<div class='form_radio_group-item' style='width:70%;'>\n");
   client.print("<input type='time' name='set_alarm"+String(i)+"' value='"+html_H+":"+html_M+"' style='width:100%; height:94%; font-size:100%'/>\n");
   client.print("</div>\n</div>\n<br>\n");
}
  client.print("<h2>Время до рассвета</h2>\n<div class='form_radio_group'>");

uint8_t time_from_rassver[9] = {5, 10, 15, 20, 25, 30, 40, 50, 60};
for (int i=0;i<9;i++)
 {
  client.print("<div class='form_radio_group-item' style='width:11%'>\n<input id='option0"+String(i)+"' name='dawnMode' type='radio' value='"+String(i)+"'");
  if (i==dawnMode) client.print(" checked");
//  Serial.println("dawnMode "+String(dawnMode));
  client.print("/>\n<label for='option0"+String(i)+"' style='font-size:200%'><center>"+String(time_from_rassver[i])+"<br>мин</center></label>\n</div>\n");
 }
  client.print("</div><h2>Время после рассвета</h2>\n<div class='form_radio_group'>\n");
for (int i=0;i<10;i++)
 {
  client.print(
    "<div class='form_radio_group-item' style='width:9.9%'>\n<input id='option1"+String(i)+"' name='rdelay' type='radio' value='"+String(i*10)+"'");
    if (TimeToFireEnd/60>i*10-5)
     if (TimeToFireEnd/60<=i*10+5) client.print(" checked");
    client.print("/>\n<label for='option1"+String(i)+"' style='font-size:200%'><center>"+String(i*10)+"<br>мин</center></label>\n</div>\n");
 }
 client.print("</div>\n<br><br><button type='submit' style='font-size:300%'>Установить будильники</button>");
 client.print("</form>\n");

 // Вывод яркости будильника
 client.print("<form onsubmit='return false' onchange=\"location='\/?DAWN_BRIGHT='+DAWN_BRIGHT.valueAsNumber+'&'\" class='slider100'>\n");
 client.print("\<div style='font-size:200%'>Яркость будильника<br>\n<input name='DAWN_BRIGHT' id='DAWN_BRIGHT' type='range' min='1' max='255' value='");
 client.print(DAWN_BRIGHT);
 Serial.println("DAWN_BRIGHT -> HTML"+String(DAWN_BRIGHT));
 client.print("' step='1' class='slider'  style='background: linear-gradient(to right, black, brown, yellow, #FFFFDD);'></div>");
 client.print("</form>\n");

//Часы
//   time_t currentLocalTime = localTimeZone.toLocal(timeClient.getEpochTime());
   //time_t currentLocalTime = getCurrentLocalTime();
   time_t currentLocalTime = now();
   uint8_t thisDay = dayOfWeek(currentLocalTime);
   String html_H=String(hour(currentLocalTime));
   if (html_H.length()==1) html_H="0"+html_H;
   String html_M=String(minute(currentLocalTime));
   if (html_M.length()==1) html_M="0"+html_M;

 client.print("<form><div><input type='time' name='time' value='"+html_H+":"+html_M+"' style='font-size:300%'/>");
   html_H=String(day(currentLocalTime));
   if (html_H.length()==1) html_H="0"+html_H;
   html_M=String(month(currentLocalTime));
   if (html_M.length()==1) html_M="0"+html_M;
   client.print("<input type='date' name='date' value='"+
   String(year(currentLocalTime))+"-"+html_M+"-"+html_H+
   "' style='font-size:300%'/> <button type='submit' style='font-size:300%'>Set Time</button></form>\n");

}
// Страница настройки избранного
void Page2() {
 client.print("<h1>Избранное</h1>\n<div class='form_radio_group' style='font-size:250%'>\n<form>\n");

uint8_t tmp_interval[10] = {1, 2, 5, 10, 15, 20, 30, 40, 50, 60};
client.print("<h2>Интервал смены эффектов</h2>");
for (int i=0;i<10;i++)
 {
  client.print("\
    <div class='form_radio_group-item'>\n<input id='option1"+String(i)+"' name='fav_interval' type='radio' value='"+String(tmp_interval[i])+"'");
    if (FavoritesManager::Interval/60==tmp_interval[i]) client.print(" checked");
    client.print("/>\n<label for='option1"+String(i)+"'><center>"+String(tmp_interval[i])+"<br>мин</center></label>\n</div>\n");
 }
 
 client.print("<br><h2>Используемые эффекты</h2>\n");
 for (int i=0;i<MODE_AMOUNT;i++)
  {
    String st=String(i);
    if (i<10) st="0"+st;

    client.print("<div class='form_radio_group-item' style='width:100%;'>\n<label>\n");
    client.print("<input type='checkbox' name='FAV_change"+st+"' value='on' ");
    if (FavoritesManager::FavoriteModes[i]!=0) client.print("checked");
    client.print(">\n<span>"+effects_name[i].Name+"</span>\n</label>\n</div>\n<br>\n");
    }
 client.print("<button type='submit' style='width:99%; font-size:100%'>Set</button>\n</form>\n</div>\n</center>");
 
}

//Выбор эффекта
void Page3() {
 client.print("<h1>Выбор эффекта</h1>\n</center>");
 
 for (int i=0;i<MODE_AMOUNT;i++)
 {
  client.print("<a href='\?currentMode="+String(i)+"'>");
  if (i==currentMode) client.print("<font color=green>");
                 else client.print("<font color=grey>");
  client.print(String(i)+"."+effects_name[i].Name+"</font></a><br>");
 } 

  client.print("</center>");
}
 

// Настройка сети
void Page4() {
client.print("<h1>Сеть</h1>\n<div class='form_radio_group'>\n<form>\n");
 for (int i=0;i<2;i++) {

    client.print("<div class='form_radio_group-item' style='width:45%'>\n<input id='option1"+String(i)+"' name='espmode' type='radio' value='"+String(i)+"'");
    if (espMode==i) client.print(" checked");
    client.print("/>\n<label for='option1"+String(i)+"' style='font-size:100%'><center> espmode="+String(i)+"<br></center></label>\n</div>\n");
 }  
client.print("<button type='submit' style='width:99%; font-size:200%'>Set</button>\n</form>\n</div>\n");
   
}
int HTML_ARG_to_INT(String ARG) {
  if (req.indexOf(ARG+"=") != -1) {
    String s_temp=req.substring(req.indexOf(ARG+"=")+ARG.length()+1,req.indexOf("&",req.indexOf(ARG+"=")));
    if (ARG=="VOL") {
      modes[EFF_VOICE].Speed= s_temp.toInt();
      set_mp3_volume=s_temp.toInt();
      settChanged = true;
      eepromTimeout = millis();
    }
    if (ARG=="BRI") processParams("BRI", String(s_temp.toInt()).c_str());
    if (ARG=="SPD") processParams("SPD", String(s_temp.toInt()).c_str());
    if (ARG=="SCA") processParams("SCA", String(s_temp.toInt()).c_str());

    return s_temp.toInt();
  }
  
}

void set_html_alarm(){

for (int i=1;i<8;i++) alarms[i-1].State=false;

for (int i=1;i<8;i++) {
 if (req.indexOf("on_alarm"+String(i)+"=on") != -1) alarms[i-1].State=true;
 if (req.indexOf("set_alarm"+String(i)+"=") != -1)
  {
    String html_H=req.substring(

      req.indexOf("set_alarm"+String(i)+"=")+11,
      req.indexOf("set_alarm"+String(i)+"=")+13
      );
    String html_M=req.substring(
      req.indexOf("set_alarm"+String(i)+"=")+16,
      req.indexOf("set_alarm"+String(i)+"=")+18
      );
    alarms[i-1].Time=html_H.toInt()*60+html_M.toInt();
    uint8_t alarmNum=i-1;
    EepromManager::SaveAlarmsSettings(&alarmNum, alarms);
  }

 }  
}

String convert_html_txt(String st_in) {
String st_out="";  
for (int i=0;i<st_in.length();i++) 
  if (st_in.substring(i,i+1)=="%") {
    int ii;
    sscanf(String("0x"+st_in.substring(i+1,i+3)).c_str(), "%x", &ii);
    char chr=ii;
    st_out+=chr;
    i+=2;
  } 
   else if (st_in.substring(i,i+1)=="+") st_out+=" ";
   else st_out+=st_in.substring(i,i+1);
  return st_out;
}

void html_setup() {
  delay(2000);
  wifiServerLamp.begin();
//  Serial.println("wifiServerLamp.begin(); OK");
  HTMLTimer=millis();
}


void html_loop() {
//  return;
  if (millis()-HTMLTimer<100) return;
  HTMLTimer=millis();
  client = wifiServerLamp.available();
  if (!client) {
   return;
  }
  Serial.println(F("new client"));
  unsigned long timeout = millis() + 2000;
  while (!client.available() && millis() < timeout) {
    delay(1);
  }
  if (millis() > timeout) {
    Serial.println("timeout; client.stop();");
    client.flush();
    client.stop();
    return;
  }

  // Read the first line of the request
    req = client.readStringUntil('\r');
    Serial.println(F("request: "));
    Serial.println(req);

  if (req.indexOf("ON=ON") != -1) processParams("P_OFF", "");
  if (req.indexOf("OFF=OFF") != -1) processParams("P_ON", "");

  if (req.indexOf("currentMode=") != -1) 
   {
//    Serial.println("currentMode:"+req.substring(req.indexOf("currentMode=")+12));
    currentMode=req.substring(req.indexOf("currentMode=")+12).toInt();
    loadingFlag = true;
    settChanged = true;
    HTML_page=0;
   }

  if (req.indexOf("FAV_change") != -1) 
   {
   for (int i=0;i<MODE_AMOUNT;i++) FavoritesManager::FavoriteModes[i]=0;
    for (int i=0;i<MODE_AMOUNT;i++) {
      String st=String(i);
      if (i<10) st="0"+st;
      if (req.indexOf("FAV_change"+st+"=") != -1) FavoritesManager::FavoriteModes[i]=1;
    }
    settChanged = true;
   }

  
  if (req.indexOf("FAV=F_ON") != -1) {
    FavoritesManager::FavoritesRunning = false;
    FavoritesManager::UseSavedFavoritesRunning=true;
    settChanged = true;
  }
  if (req.indexOf("FAV=F_OFF") != -1) {
    FavoritesManager::FavoritesRunning = true;
    FavoritesManager::UseSavedFavoritesRunning=true;
    settChanged = true;
  }

    if (req.indexOf("BRI") != -1) BRI=HTML_ARG_to_INT("BRI");
    if (req.indexOf("SPD") != -1) SPD=HTML_ARG_to_INT("SPD");
    if (req.indexOf("SCA") != -1) SCA=HTML_ARG_to_INT("SCA");
    if (req.indexOf("VOL") != -1) VOL=HTML_ARG_to_INT("VOL");
 
    // Яркость будильника
    if (req.indexOf("DAWN_BRIGHT") != -1) {
         DAWN_BRIGHT=HTML_ARG_to_INT("DAWN_BRIGHT");
         Serial.println("HTML -> DAWN_BRIGHT "+String(DAWN_BRIGHT));
         EEPROM.write(27,DAWN_BRIGHT);
         EEPROM.commit(); 
         }

  if (req.indexOf("WWW=") != -1) HTML_page=req.substring(req.indexOf("WWW=")+4).toInt();
 
  if (req.indexOf("alarm") != -1) set_html_alarm();
  if (req.indexOf("dawnMode") != -1) {
    dawnMode=req.substring(req.indexOf("dawnMode=")+9,req.indexOf("&",req.indexOf("dawnMode="))).toInt();
    EepromManager::SaveDawnMode(&dawnMode);
  }
  if (req.indexOf("rdelay") != -1) {
    TimeToFireEnd=req.substring(req.indexOf("rdelay=")+7,req.indexOf("&",req.indexOf("rdelay="))).toInt()*60;
    EepromManager::WriteInt32(29,TimeToFireEnd);
    EEPROM.commit();           
  }
  if (req.indexOf("time=") != -1) {
    int HTML_T_H=req.substring(req.indexOf("time=")+5,req.indexOf("time=")+7).toInt();
    int HTML_T_M=req.substring(req.indexOf("time=")+10,req.indexOf("time=")+12).toInt();
    int HTML_D_Y=req.substring(req.indexOf("date=")+5,req.indexOf("date=")+9).toInt();
    int HTML_D_M=req.substring(req.indexOf("date=")+10,req.indexOf("date=")+12).toInt();
    int HTML_D_D=req.substring(req.indexOf("date=")+13,req.indexOf("date=")+15).toInt();
    setTime(0,HTML_T_M,HTML_T_H,HTML_D_D,HTML_D_M,HTML_D_Y);
    timeSets = true;

//    EEPROM.commit();           
  }
  
  if (req.indexOf("fav_interval=") !=-1) {
    int fav_int=req.substring(req.indexOf("fav_interval=")+13,req.indexOf("fav_interval=")+15).toInt();
    processParams("TXT-fav", String(fav_int*60).c_str());
  }

  if (req.indexOf("espmode=") !=-1) {
    int esp_int=req.substring(req.indexOf("espmode=")+8,req.indexOf("espmode=")+9).toInt();
    processParams("TXT-esp_mode=", String(esp_int).c_str());
  }
  if (req.indexOf("TXT=") !=-1) {
    String str_int=req.substring(req.indexOf("TXT=")+4,req.indexOf(" ",req.indexOf("TXT=")));
    processParams("TXT-", String(convert_html_txt(str_int)).c_str());

  }

    
    handle_main();
    switch (HTML_page)
    {  
     case 0: Page0(); break; //Яркость и т.д.
     case 1: Page1(); break; //Будильники
     case 2: Page2(); break; //Избранное 
     case 3: Page3(); break; //Выбор Эффекта
     case 4: Page4(); break; //esp mode
    }

    down_main();

}


#endif
