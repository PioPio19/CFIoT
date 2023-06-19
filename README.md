#Read and follow the instruction bellow
## This section is the basic step for EC2 configuration
1. First of all make an account for AWS.
2. Create an Instance in EC2.
3. Creat elastic IP, and then associate the elastic IP to a specific EC2 Instance.
4. Open the EC2 Instance, and install XAMPP inside that Instance.
5. Setting the firewall, so the network can be accessed from outside the network.

## This section is for the Arduino configuration
### You can find this code inside CFIoT Folder 
1. Make sure to change this code:
```
    // Replace with your network credentials (STATION)
    const char* ssid = "WIFI_ID";
    const char* password = "WIFI_PASSWORD";
    
 ```   
2. REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS.
```
    http.begin("http://REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS/REPLACE_WITH_PROJECT_FOLDER_NAME_IN_htdocs_FOLDER/getdata.php");  //--> Specify request destination
```
4. But make sure that the IP address used is "IPv4 address".
5. Example : http.begin("http://192.168.0.0/ESP32_MySQL_Database/Final/getdata.php");
6. Do a similar step to step number two.
```
    http.begin("http://REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS/REPLACE_WITH_PROJECT_FOLDER_NAME_IN_htdocs_FOLDER/updateDHT11data_and_recordtable.php");  //--> Specify request destination
```
