/*******************************************************************************
 * Copyright (c) 2012, 2023 IBM Corp., Ian Craggs
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>
#include "MQTTAsync.h"

#define ADDRESS     "tcp://192.168.99.36:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "data/time"
#define PAYLOAD     "Hello World!"
#define QOS         0
#define TIMEOUT     10000L
#define NUM_THREADS 30

char* username = "admin";
char* password = "szu123456";
atomic_int total_count = 0;

void connlost(void *context, char *cause);
void onDisconnect(void* context, MQTTAsync_successData* response);
void onSendFailure(void* context, MQTTAsync_failureData* response);
void onSend(void* context, MQTTAsync_successData* response);
void onConnectFailure(void* context, MQTTAsync_failureData* response);
void onConnect(void* context, MQTTAsync_successData* response);
double x = 0.2595;
double y = -0.1238;
double z = 0.0428;
double w = 0.9568;
double ax = 2.5800;
double ay = 4.8700;
double az = 8.1300;
double gx = -2.2500;
double gy = -0.6875;
double gz = -1.0625;
int s = 0;
double p = 99.676;
void* publish_messages(void* threadid)
{
    long tid = (long)threadid;
    MQTTAsync client;
    char clientId[60];
    char message_payload[200];
    sprintf(clientId, "%s_%ld", CLIENTID, tid);

    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    int rc;

    MQTTAsync_create(&client, ADDRESS, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTAsync_setCallbacks(client, NULL, connlost, NULL, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = username;
    conn_opts.password = password;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;

    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Thread %ld: Failed to start connect, return code %d\n", tid, rc);
        pthread_exit(NULL);
    }
    int flag = 1;
    int count = 0;
    while (flag)
    {
        struct timespec ts;
        struct tm* tm_info;
        char time_buffer[40];

        //clock_gettime(CLOCK_REALTIME, &ts);
        //tm_info = localtime(&ts.tv_sec);
        //strftime(time_buffer, 40, " %H:%M:%S", tm_info);
        //sprintf(message_payload, "Client %ld, Timestamp: %s.%03ld", tid, time_buffer, ts.tv_nsec / 1000000);
    
		// 获取当前时间戳
		clock_gettime(CLOCK_REALTIME, &ts);
		long timestamp = ts.tv_sec * 1000L + ts.tv_nsec / 1000000L; // 转换为毫秒

		// 创建 JSON 格式的消息负载
		sprintf(message_payload, 
    		"{\"n\":\"%ld\",\"x\":\"%.4f\",\"y\":\"%.4f\",\"z\":\"%.4f\",\"w\":\"%.4f\","
    		"\"ax\":\"%.4f\",\"ay\":\"%.4f\",\"az\":\"%.4f\","
    		"\"gx\":\"%.4f\",\"gy\":\"%.4f\",\"gz\":\"%.4f\","
    		"\"s\":\"%d\",\"p\":\"%.4f\",\"timestamp\":\"%ld\"}",
    		tid, x, y, z, w, ax, ay, az, gx, gy, gz, s, p, timestamp);

        pubmsg.payload = message_payload;
        pubmsg.payloadlen = (int)strlen(message_payload);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        opts.onSuccess = onSend;
        opts.onFailure = onSendFailure;
        opts.context = client;

        if ((rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
        {
            printf("Thread %ld: Failed to send message, return code %d\n", tid, rc);
        }
		count++;
        //if (count == 20000) {
        //    flag = 1;
        //    printf("client %ld finished \n", tid);
        //}
        atomic_fetch_add(&total_count, 1);
        usleep(10000); // 10ms delay for 100Hz
    }

    MQTTAsync_disconnect(client, NULL);
    MQTTAsync_destroy(&client);
    pthread_exit(NULL);
}

void connlost(void* context, char* cause)
{
    printf("\nConnection lost\n");
    if (cause)
        printf("     cause: %s\n", cause);
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
    printf("Message send failed token %d error code %d\n", response->token, response->code);
}

void onSend(void* context, MQTTAsync_successData* response)
{
    //printf("Message with token value %d delivery confirmed\n", response->token);
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("Connect failed, rc %d\n", response ? response->code : 0);
}

void onConnect(void* context, MQTTAsync_successData* response)
{
    printf("Successful connection\n");
}

void* print_count(void* arg)
{
    while (1)
    {
        printf("Total messages sent: %d\n", atomic_load(&total_count));
		sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t threads[NUM_THREADS];
    pthread_t thread_count;
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t], NULL, publish_messages, (void*)t);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // 创建打印线程
    rc = pthread_create(&thread_count, NULL, print_count, NULL);
    if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    for (t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    // 可以选择取消打印线程
    //pthread_cancel(thread_count);
    pthread_join(thread_count, NULL);

    printf("Total messages sent: %d\n", atomic_load(&total_count));
    return 0;
}

