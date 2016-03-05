
#include <SPI.h>
#include <Ethernet.h>
#include <stdio.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(172, 16, 3, 100);
IPAddress gateway(172, 16, 0, 1);
IPAddress subnet(255, 255, 252, 0);

String clientNames[4] = { "NULL", "NULL", "NULL", "NULL" };

// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

//SETUP ----------------------------------------------------

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}
//SETUP ----------------------------------------------------

//LOOP ----------------------------------------------------
void loop() {

  // wait for a new client:
  EthernetClient client = server.available();
  if (client) {

    int i = 0;

    String msgContainer;
    while (client.available())
    { 

      char thisChar = client.read();
      msgContainer = msgContainer + String(thisChar);
      if (client.available() == 0)
      {

        Serial.println(msgContainer);
        //const char* charMessageContainer = msgContainer.c_str();
        //server.write(charMessageContainer);
        int index = msgContainer.indexOf('>');
        int checkIndex=msgContainer.length();
        String subString = msgContainer.substring(0, index);
        String checkDisconnect=msgContainer.substring(index+4,checkIndex);

        while (i<4)
        {
          if (!msgContainer.equals(subString))
          {
            if (clientNames[i].equals("NULL") || clientNames[i].equals(subString))
            {
              clientNames[i] = subString;
              break;
            }
          }
          i++;
        }
        i = 0;

if(checkDisconnect =="DisconnectAllRightNow")
{
  DisconnectUser(subString);
}

      
else{
        //creates xml msg
        //String userList = UserLists(clientNames);
       
        String stuff = xmlCreator(msgContainer, clientNames);
        const char* charMessageContainer = stuff.c_str();
       // server.println("-------------------------");
        server.write(charMessageContainer);
}
      }
    }
    client.flush();

  }
}
//LOOP ----------------------------------------------------
//xmlcreator
static String xmlCreator(String msgContainer, String clientNames[])
{
    String userList;

  int i = 0;
  while (i<4)
  {
    String temp = "<UserName>" + clientNames[i] + "</UserName>";
    userList = userList + temp;
    i++;
  }
  
  String xml= "<Msg><MainMsg>" + msgContainer + "</MainMsg>" + userList+"</Msg>";
  
  return xml;
}

//UserList  ----------------------------------------------------

static String UserLists(String clientNames[]){

  String newList;

  int i = 0;
  while (i<4)
  {
    String temp = "<UserName>" + clientNames[i] + "</UserName>";
    newList = newList + temp;
    i++;
  }
  return newList;
}
//UserList  ----------------------------------------------------
static void DisconnectUser(String Name)
  {
    Serial.println("they are trying to disconnect "+ Name);
    int i=0;
    
        while (i<4)
        {      
          Serial.println(clientNames[i]+ "checking here");
            if (clientNames[i].equals(Name))
            {
              
              clientNames[i] = "NULL";
              break;
            }
          i++;
        }
        i=0;
                
         Serial.println("debug is being tested");
        while(i<4)
        {
         Serial.println(clientNames[i]);
         i++;
         }
          Serial.println("end of debugging");

        sortList();           
        xmlCreator("",clientNames);
  }

static void sortList()
{
  int i = 0;
            while (i < 3)
            { 
            if(clientNames[i]=="NULL")
                {
                    clientNames[i] = clientNames[i+1];
                    clientNames[i + 1] = "NULL";
                    if(clientNames[i]!=clientNames[i+1])
                    sortList();
                }
            i++;
            }
}


