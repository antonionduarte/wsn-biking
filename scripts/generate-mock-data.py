import pandas as pd
import numpy as np
import random

def generate_mock_data(file_path, num_entries=1000):
    with open(file_path, 'w') as f:
        curr_time = 1000000  # Start from a base time of 1,000,000 seconds since the epoch
        message_id = 1
        while message_id <= num_entries:
            # Occasionally skip a message ID to simulate missing messages
            if random.random() < 0.05:
                message_id += 1
            
            # Random RSSI between -100 and -30 or NaN
            rssi = random.choice([random.randint(-100, -30), "NaN"])
            # Random LQI between 0 and 255 or NaN
            lqi = random.choice([random.randint(0, 255), "NaN"])
            
            # Ack value 1 if message_id is not skipped, otherwise 0
            ack = 1 if (rssi != "NaN" and lqi != "NaN") else 0
            
            line = f"{message_id}, {curr_time}, {rssi}, {lqi}, {ack}\n"
            f.write(line)
            curr_time += 1  # Increment the time by one second
            message_id += 1


# Use the function to generate a mock data file
generate_mock_data("results/mock_data.txt")
