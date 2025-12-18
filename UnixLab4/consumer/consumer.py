#!/usr/bin/env python3
import time
import signal
import sys
import socket
import math
from kafka import KafkaConsumer

time.sleep(30)

def shutdown(_signo, _stack_frame):
    consumer.close()
    sys.exit(0)
signal.signal(signal.SIGINT, shutdown)
signal.signal(signal.SIGTERM, shutdown)

def dosmth (arg):
    return 1/(1+math.exp(-float(arg)))

consumer_id = socket.gethostname()
print(f"Consumer {consumer_id} started")

consumer = KafkaConsumer(
    'events',
    bootstrap_servers=['kafka:9092', 'localhost:9092'],
    group_id='SomeGroup', 
    auto_offset_reset='latest',
    enable_auto_commit=False
)

for message in consumer:
    print(f"Consumer {consumer_id} received: {message.value.decode()}",flush=True)
    print(f"Something happened with message: {dosmth(message.value.decode())}",flush=True)
    consumer.commit()
    time.sleep(10)
