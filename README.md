Secure IoT Framework

Overview:
This project sets up a secure way for IoT devices to communicate using MQTT, SSL/TLS encryption, and CRC32-based tamper detection. It ensures that only trusted devices can send and receive data safely.

Features:
Secure Communication – All messages are encrypted with SSL/TLS.
Data Integrity Check – CRC32 is used to detect any tampering.
Device Authentication – Only devices with valid certificates and credentials can connect.

How Authentication Works:
Devices must present a TLS client certificate for verification.
The MQTT broker checks the username and password before allowing a connection.
If the credentials are invalid, the device is blocked from accessing the network.

How to Compile:
Make sure OpenSSL and MQTT libraries are installed.
Run the following command to compile the project: bash

*****For deatailed understanding please read my REPORT*****
