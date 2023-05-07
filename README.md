1. First of all please setting up Amazon AWS IoT Core Dashboard. <br>
2. Provide some related data like policy, and things. <br>
3. Download the related data included certificate, private key, and public key.<br>
4. Download FarmAssist.Ino and Secrets.h, and save it into one folder.<br>
5. Open the Secrets.h, copy the AmazonRootCA1 data, Device Sertificate, and Private key.<br>
6. Don't forget to edit this :<br>
    #define THINGNAME "*********"                         //change this
 
    const char WIFI_SSID[] = "***********";               //change this
    const char WIFI_PASSWORD[] = "***********";           //change this
    const char AWS_IOT_ENDPOINT[] = "************";       //change this
7. Save, compile and upload.<br>
