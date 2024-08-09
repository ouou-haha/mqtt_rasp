import paho.mqtt.client as mqtt
import threading

# MQTT配置
mqtt_broker = "192.168.99.36"  # 替换为你的MQTT代理地址
mqtt_port = 1883               # 默认MQTT端口
mqtt_topic = "data/time"       # 替换为你的MQTT主题
mqtt_username = "admin" # 替换为你的MQTT用户名
mqtt_password = "szu123456" # 替换为你的MQTT密码

# 统计接收到的数据量
received_data_count = 0
received_data_lock = threading.Lock()

# 当客户端连接到MQTT代理时调用
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(f"Connected successfully with result code {rc}")
        client.subscribe(mqtt_topic, qos=0)  # 订阅主题并设置QoS为0
    else:
        print(f"Failed to connect, return code {rc}")

# 当客户端接收到消息时调用
def on_message(client, userdata, msg):
    global received_data_count
    with received_data_lock:
        received_data_count += 1

# 初始化MQTT客户端
client = mqtt.Client(protocol=mqtt.MQTTv311)  # 使用MQTT版本3.1.1
client.username_pw_set(mqtt_username, mqtt_password)
client.on_connect = on_connect
client.on_message = on_message

# 连接MQTT Broker
client.connect(mqtt_broker, mqtt_port, 60)

# 启动一个线程来处理MQTT消息
mqtt_thread = threading.Thread(target=client.loop_forever)
mqtt_thread.start()

# 监听键盘输入
def listen_for_input():
    while True:
        user_input = input()
        if user_input.lower() == 'c':
            with received_data_lock:
                print(f"Total messages received: {received_data_count}")
        elif user_input.lower() == 'q':
            print("Exiting program.")
            client.disconnect()
            influxdb_client.close()
            sys.exit(0)

# 启动一个线程来监听键盘输入
input_thread = threading.Thread(target=listen_for_input)
input_thread.daemon = True  # 使线程在主线程结束时自动退出
input_thread.start()

# 保持主线程运行
mqtt_thread.join()

