#Read and follow the instruction bellow

1. First of all please setting up Amazon AWS IoT Core Dashboard. 
2. Provide some related data like policy, and things.
3. Download the related data included certificate, private key, and public key.<
4. Download FarmAssist.Ino and Secrets.h, and save it into one folder.
5. Open the Secrets.h, copy the AmazonRootCA1 data, Device Sertificate, and Private key.
```
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";
```

```
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";
```

```
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";
```
6. Don't forget to edit this line of code:
```
    #define THINGNAME "*********"                         //change this
 
    const char WIFI_SSID[] = "***********";               //change this
    const char WIFI_PASSWORD[] = "***********";           //change this
    const char AWS_IOT_ENDPOINT[] = "************";       //change this
 ```   
7. Save, compile and upload.
