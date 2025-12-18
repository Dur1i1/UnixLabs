#!/usr/bin/env python3
import time
import signal
import sys
from kafka import KafkaProducer

time.sleep(30)

def shutdown(_signo, _stack_frame):
    producer.close()
    sys.exit(0)
signal.signal(signal.SIGINT, shutdown)
signal.signal(signal.SIGTERM, shutdown)

producer = KafkaProducer(bootstrap_servers=['kafka:9092', 'localhost:9092'])

somenum = 0
while True:
    msg = str(somenum)
    producer.send('events', msg.encode())
    print(f"Producer sent: {msg}",flush=True)
    somenum += 0.01
    time.sleep(1)
