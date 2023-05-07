First of all please setting up Amazon AWS IoT Core Dashboard.
Provide some related data like policy, and things. 
Download the related data included certificate, private key, and public key.
Download FarmAssist.Ino and Secrets.h, and save it into one folder.
Open the Secrets.h, copy the AmazonRootCA1 data, Device Sertificate, and Private key.
Don't forget to edit this :
    #define THINGNAME "*********"                         //change this
 
    const char WIFI_SSID[] = "***********";               //change this
    const char WIFI_PASSWORD[] = "***********";           //change this
    const char AWS_IOT_ENDPOINT[] = "************";       //change this
Save, compile and upload.
