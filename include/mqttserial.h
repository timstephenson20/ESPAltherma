#ifndef mqttSerial_h
#define mqttSerial_h
#include "Stream.h"
#include <PubSubClient.h>
#ifdef ARDUINO_M5Stick_C_Plus2
#include <M5StickCPlus2.h>
#elif ARDUINO_M5Stick_C_Plus
#include <M5StickCPlus.h>
#elif ARDUINO_M5Stick_C
#include <M5StickC.h>
#elif ARDUINO_M5Stack_Tough
#include <M5Tough.h>
#endif
class MQTTSerial: public Stream
{
private:
    /* data */
    PubSubClient* _client = nullptr;
    char _topic[64];
public:
    inline void begin(PubSubClient* client,const char* topic){
    _client=client;
    strcpy(_topic,topic);
    };

    inline size_t write(uint8_t){return 0;};
    size_t write(const uint8_t *buffer, size_t size);
    inline int available(void){return _client->connected();};
    inline int availableForWrite(void){return 0;};
    inline int peek(void){return 0;};
    inline int read(void){return 0;};
    inline void flush(void){};
    inline size_t write(const char * s)
    {
        return write((uint8_t*) s, strlen(s));
    }
    inline size_t write(unsigned long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t) n);
    }


    MQTTSerial();
    ~MQTTSerial();
};

MQTTSerial mqttSerial;

MQTTSerial::MQTTSerial()
{
}
size_t MQTTSerial::write(const uint8_t *buffer, size_t size)
{
#if defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stack_Tough)
    if (M5.Lcd.getCursorY()+13>M5.Lcd.height()){
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0,0);
    }
    M5.Lcd.print((const char*) buffer);
#endif
#ifndef DISABLE_LOG_MESSAGES
    if (WiFi.status() == WL_CONNECTED && _client!=nullptr &&_client->connected()){
        _client->publish(_topic,buffer,size);
    }
    Serial.write(buffer,size);
#endif
    return size;
}

MQTTSerial::~MQTTSerial()
{
}

#endif