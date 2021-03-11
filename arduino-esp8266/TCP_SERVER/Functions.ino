void ESPWIFIsetup()
{
  CommLaunch("AT+CIOBAUD?\r", 2*1000, true, 0);
  CommLaunch("AT+RST\r", 2*1000, true, 0);
  CommLaunch("AT+CWMODE=3\r", 1000, true, 0);
  CommLaunch("AT+CIPMUX=1\r", 1000, true, 0);
  CommLaunch("AT+CIFSR\r", 2*1000, true, 0);
  //CommLaunch("AT+CWLAP\r", 6*1000, true, 1);
}

void ESPWIFIassociate()
{
  String usr, pw, asso;
  usr = "QoS";
  pw = "23456";
  
  STR.ReplaceBase(Storage);
  Storage = ""; // Clear the memory for nxt time

  if(STR.Contains(usr))
  {
    asso = "AT+CWJAP=\""+usr+"\",\""+pw+"\"";
    while(true)
    {
      CommLaunch(asso, 15*1000, true, 1);
      
      if(ESPCommandSucess("OK",1))
        break;
    }
  }
  else
    Serial.println("[ERROR0] : no matching result!!");
    
  Serial.println("FINISH ASSOCIATION!!");
  CommLaunch("AT+CIFSR\r", 2*1000, true, 0); 
}
void ESPsetServer(){
     
}
void ESPTCPconnection()
{
  String URL;
  
  URL = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
  
  while(true)
  {
    CommLaunch(URL, 15*1000, true, 1);
    
    if(ESPCommandSucess("OK",1))
        break;
  }
  
  Serial.println("FINISH TCP CONNECTION!!");
  CommLaunch("AT+CIPSTATUS\r", 2*1000, true, 0); 
}

void ESPTCPsend(int length)
{
  String message = "AT+CIPSEND="; 
  message+=length;
  CommLaunch(message, 2*1000, true, 0); 
}

void ESPUpload(int input)
{
  String content;
  int SIZE;
  content = "GET /update?key=";
  content += API_KEY;
  content += "&field1=";
  content += input;
  SIZE = content.length()+1;
  
  ESPTCPsend(SIZE);
  CommLaunch(content, 15*1000, true, 0); 
  
  ESPTCPcheck();
}

void ESPTCPcheck()
{
  while(true)
  {
    CommLaunch("AT+CIPCLOSE", 5*1000, true, 1);
    if(ESPCommandSucess("OK+ERROR",2))
        break;
  }
  ESPTCPconnection();
}

void ESPDownload()
{
  String content;
  int SIZE;
  content = "GET /channels/26355/feeds.json?results=1\r\n";
  SIZE = content.length()-1;

  ESPTCPsend(SIZE);
  CommLaunch(content, 15*1000, true, 0); 
  
  ESPTCPcheck(); 
}

void ESPFinish()
{
    CommLaunch("AT+CIPCLOSE", 6*1000, true, 0);
    CommLaunch("AT+CWQAP", 2*1000, true, 0);
}

void CommLaunch(String cmd,unsigned int duration, boolean res, int keyword)
{
  Serial1.println(cmd);    //send command to ESP8266

  if(res)
    CommResponse( keyword, duration);
}

void CommResponse(int keyword, unsigned int duration)
{
  Serial.println("ESP RESPONSE:");
  String tmp ="";
  long deadline = millis()+duration;
  
  while(millis() < deadline)
  {
    while (Serial1.available()>0)  {
      tmp += char(Serial1.read());
    }

    if(tmp.length() > 0) {
      if(keyword==1)
        Storage = tmp; // if the keyword is triggered, copy the string

      Serial.print(tmp);
      tmp = "";
    }
  }
  Serial.println("\n------------------------------");
}

boolean ESPCommandSucess(String KEY, int index)
{
  int iteration;
  STR.ReplaceBase(Storage);
  Storage = "";
  
  if(index==1)
    return STR.Contains(KEY);
  
  boolean result = false;
  String  component;
  STR.StoreKey(KEY);
  for(iteration = 0; iteration < index; iteration++)
  {
    component = STR.Delimitation('+');

    result = result || STR.Contains(component);
  }
  
  if(STR.Contains("busy"))
  {
    Serial.println("ENTER REST");
    delay(1000);
    Serial.println("EXIT REST");  
}
  
  return result;
}
