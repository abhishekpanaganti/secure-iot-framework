{\rtf1\ansi\ansicpg1252\cocoartf2821
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww28600\viewh16380\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #include <stdio.h>\
#include <stdlib.h>\
#include <string.h>\
#include <openssl/ssl.h>\
#include <openssl/err.h>\
#include <openssl/bio.h>\
#include <mosquitto.h>\
\
#define MQTT_HOST "mqtt.example.com"\
#define MQTT_PORT 8883\
#define MQTT_TOPIC "test/topic"\
#define MQTT_KEEP_ALIVE_INTERVAL 60\
\
SSL_CTX *ssl_ctx;\
\
void init_openssl() \{\
    SSL_load_error_strings();\
    OpenSSL_add_ssl_algorithms();\
    ssl_ctx = SSL_CTX_new(TLS_client_method());\
    if (!ssl_ctx) \{\
        fprintf(stderr, "Error initializing SSL context\\n");\
        exit(EXIT_FAILURE);\
    \}\
\}\
\
void cleanup_openssl() \{\
    SSL_CTX_free(ssl_ctx);\
    EVP_cleanup();\
\}\
\
void mqtt_on_connect(struct mosquitto *mosq, void *obj, int rc) \{\
    if (rc == MOSQ_ERR_SUCCESS) \{\
        printf("MQTT Connected successfully.\\n");\
        mosquitto_subscribe(mosq, NULL, MQTT_TOPIC, 0);\
    \} else \{\
        fprintf(stderr, "Failed to connect, return code: %d\\n", rc);\
    \}\
\}\
\
void mqtt_on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) \{\
    printf("Received message: %s\\n", (char *)message->payload);\
\}\
\
int main() \{\
    // Initialize OpenSSL and MQTT\
    init_openssl();\
\
    mosquitto_lib_init();\
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);\
    if (!mosq) \{\
        fprintf(stderr, "Failed to create mosquitto instance.\\n");\
        return EXIT_FAILURE;\
    \}\
\
    // Set SSL options\
    mosquitto_tls_set(mosq, NULL, NULL, NULL, NULL, NULL);\
    mosquitto_tls_insecure_set(mosq, true);  // This can be false with a valid CA\
\
    // Set MQTT callbacks\
    mosquitto_connect_callback_set(mosq, mqtt_on_connect);\
    mosquitto_message_callback_set(mosq, mqtt_on_message);\
\
    // Connect to the MQTT broker with SSL\
    int rc = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, MQTT_KEEP_ALIVE_INTERVAL);\
    if (rc != MOSQ_ERR_SUCCESS) \{\
        fprintf(stderr, "Unable to connect to broker: %s\\n", mosquitto_strerror(rc));\
        mosquitto_destroy(mosq);\
        mosquitto_lib_cleanup();\
        cleanup_openssl();\
        return EXIT_FAILURE;\
    \}\
\
    // Loop and wait for messages\
    mosquitto_loop_start(mosq);\
\
    // Sending a test message\
    char payload[] = "Hello, MQTT with SSL!";\
    rc = mosquitto_publish(mosq, NULL, MQTT_TOPIC, strlen(payload), payload, 0, false);\
    if (rc != MOSQ_ERR_SUCCESS) \{\
        fprintf(stderr, "Failed to send message: %s\\n", mosquitto_strerror(rc));\
    \}\
\
    // Wait for a while\
    sleep(10);\
\
    // Cleanup\
    mosquitto_loop_stop(mosq, true);\
    mosquitto_destroy(mosq);\
    mosquitto_lib_cleanup();\
    cleanup_openssl();\
\
    return EXIT_SUCCESS;\
\}\
}