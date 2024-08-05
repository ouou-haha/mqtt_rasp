#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "MQTTAsync.h"

#define ADDRESS     "tcp://192.168.99.36:1883"
#define CLIENTID    "ExampleClientSub_arm"
#define TOPIC       "data/time"
#define QOS         1
#define TIMEOUT     10000L

struct timespec ts;
struct tm* tm_info;
char time_buffer[40];
volatile int running = 1;
typedef struct MessageNode {
    char* data;
    struct MessageNode* next;
} MessageNode;

typedef struct {
    MessageNode* head;
    MessageNode* tail;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count;
	int cur_count;
} MessageQueue;

void initQueue(MessageQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
	queue->cur_count = 0;
    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

void enqueue(MessageQueue* queue, const char* data) {
    MessageNode* newNode = (MessageNode*)malloc(sizeof(MessageNode));
    newNode->data = strdup(data);
    newNode->next = NULL;

    pthread_mutex_lock(&queue->lock);
    if (queue->tail) {
        queue->tail->next = newNode;
    } else {
        queue->head = newNode;
    }
    queue->tail = newNode;
    queue->count++;
	queue->cur_count ++;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->lock);
}

char* dequeue(MessageQueue* queue) {
    pthread_mutex_lock(&queue->lock);
    while (queue->head == NULL && running) {
        pthread_cond_wait(&queue->cond, &queue->lock);
    }
    if (queue->head == NULL) {
        pthread_mutex_unlock(&queue->lock);
        return NULL;
    }

    MessageNode* temp = queue->head;
    char* data = strdup(temp->data);
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    queue->cur_count--;
    free(temp->data);
    free(temp);

    pthread_mutex_unlock(&queue->lock);
    return data;
}

void destroyQueue(MessageQueue* queue) {
    while (queue->head) {
        MessageNode* temp = queue->head;
        queue->head = queue->head->next;
        free(temp->data);
        free(temp);
    }
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->cond);
}

MessageQueue message_queue;

char* username = "admin";
char* password = "szu123456";
int disc_finished = 0;
int subscribed = 0;
int finished = 0;


void onConnect(void* context, MQTTAsync_successData* response);
void onConnectFailure(void* context, MQTTAsync_failureData* response);

void connlost(void* context, char* cause) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;

    printf("\nConnection lost\n");
    if (cause)
        printf("     cause: %s\n", cause);

    printf("Reconnecting\n");
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = username;
    conn_opts.password = password;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        finished = 1;
    }
}

int msgarrvd(void* context, char* topicName, int topicLen, MQTTAsync_message* message) {
    enqueue(&message_queue, (char*)message->payload);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

void onDisconnectFailure(void* context, MQTTAsync_failureData* response) {
    printf("Disconnect failed, rc %d\n", response->code);
    disc_finished = 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response) {
    printf("Successful disconnection\n");
    disc_finished = 1;
}

void onSubscribe(void* context, MQTTAsync_successData* response) {
    printf("Subscribe succeeded\n");
    subscribed = 1;
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response) {
    printf("Subscribe failed, rc %d\n", response->code);
    finished = 1;
}

void onConnectFailure(void* context, MQTTAsync_failureData* response) {
    printf("Connect failed, rc %d\n", response->code);
    finished = 1;
}

void onConnect(void* context, MQTTAsync_successData* response) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;

    printf("Successful connection\n");

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    opts.onSuccess = onSubscribe;
    opts.onFailure = onSubscribeFailure;
    opts.context = client;
    if ((rc = MQTTAsync_subscribe(client, TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start subscribe, return code %d\n", rc);
        finished = 1;
    }
}

void* check_input(void* arg) {
    char input[10];
    while (running) {
        if (fgets(input, sizeof(input), stdin)) {
            if (input[0] == 'c' || input[0] == 'C') {
                printf("count: %d\n", message_queue.count);
            } else if(input[0] == 'q' || input[0] == 'Q') {
                running = 0;
    			pthread_cond_signal(&message_queue.cond);
            }else if(input[0] == 'n' || input[0] == 'N'){
				printf("length of message now : %d\n", message_queue.cur_count);
			}
        }
    }
    pthread_exit(NULL);
}

void* process_messages(void* arg) {
    while (running || message_queue.cur_count > 0) {
        char* message = dequeue(&message_queue);
        if (message) {
            // Process the message here
            free(message);
        }
    }

    // Save remaining messages
    FILE* file = fopen("output.txt", "w");
    if (file != NULL) {
        while (message_queue.cur_count > 0) {
            char* message = dequeue(&message_queue);
            if (message) {
                //fprintf(file, "%s\n", message);
                free(message);
            }
        }
        fclose(file);
        printf("Saved data to file successfully\n");
    } else {
        printf("Failed to open file\n");
    }

    printf("Total messages: %d\n", message_queue.count);
    clock_gettime(CLOCK_REALTIME, &ts);
    tm_info = localtime(&ts.tv_sec);
    strftime(time_buffer, 40, " %H:%M:%S", tm_info);
    printf("End time: %s.%03ld\n", time_buffer, ts.tv_nsec / 1000000);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
    pthread_t input_thread, message_thread_1, message_thread_2;
    int rc;

    initQueue(&message_queue);

    if ((rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS) {
        printf("Failed to create client, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto exit;
    }

    if ((rc = MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS) {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = username;
    conn_opts.password = password;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    rc = pthread_create(&input_thread, NULL, check_input, NULL);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&message_thread_1, NULL, process_messages, NULL);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&message_thread_2, NULL, process_messages, NULL);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    tm_info = localtime(&ts.tv_sec);
    strftime(time_buffer, 40, " %H:%M:%S", tm_info);
    printf("Start time: %s.%03ld\n", time_buffer, ts.tv_nsec / 1000000);

    while (running && !subscribed && !finished) {
        usleep(10000L);
    }

    if (finished)
        goto exit;

    pthread_join(input_thread, NULL);
    pthread_join(message_thread_1, NULL);
    pthread_join(message_thread_2, NULL);

    disc_opts.onSuccess = onDisconnect;
    disc_opts.onFailure = onDisconnectFailure;
    if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start disconnect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }
    while (!disc_finished) {
        usleep(10000L);
    }

destroy_exit:
    MQTTAsync_destroy(&client);
    destroyQueue(&message_queue);
exit:
    return rc;
}



