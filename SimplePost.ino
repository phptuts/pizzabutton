
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Twitter.h>


// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("YOUR_TWITTER_OAUTH_TOKEN ");

// Message to post
const char* ssid = "ChiconCollective-Guest";
const char* timehost = "www.timeapi.org";
int btn = D3;
int led = D6;
WiFiClient client;



void setup()
{

  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
//  Serial.println("connecting ...");
//  if (twitter.post(msg)) {
//    // Specify &Serial to output received response to Serial.
//    // If no output is required, you can just omit the argument, e.g.
//    // int status = twitter.wait();
//    int status = twitter.wait(&Serial);
//    if (status == 200) {
//      Serial.println("OK.");
//    } else {
//      Serial.print("failed : code ");
//      Serial.println(status);
//    }
//  } else {
//    Serial.println("connection failed.");
//  }
}

String getTime() {

  if (client.connect(timehost, 80)) {
          String url =  "/utc/now";
          client.println("GET " + url + " HTTP/1.1");
          client.println("Host: "+ (String)timehost);
          client.println("User-Agent: Arduino/1.0");
          client.println("Connection: close");
          client.println();
          client.println();
          delay(500);
          String line = "";
          while (client.connected()) {
            line = client.readStringUntil('\n');
          }
          return line;
  }

  return "NO TIME";
}

void loop()
{
  if(!digitalRead(btn)) {
    digitalWrite(led, HIGH);
    String time = getTime();
    Serial.println("Time is : " + time);
    String msg = "Noah is tweeting at " + time;
    char  twitterMessage[ msg.length() + 1];
    msg.toCharArray(twitterMessage, msg.length() + 1);
    if (twitter.post(twitterMessage)) {
       int status = twitter.wait(&Serial);
       if (status == 200) {
          digitalWrite(led,LOW);
          delay(500);
          digitalWrite(led,HIGH);
          delay(500);
          digitalWrite(led,LOW);
          delay(500);
          digitalWrite(led,HIGH);
          delay(500);
      } else {
          Serial.print("failed : code ");
          Serial.println(status);
      }
    }
  }
  else {
    digitalWrite(led, LOW);
  }
}





