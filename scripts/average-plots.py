import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Directory where the files are stored
directory = 'results'

# Create empty DataFrame to store average RSSI and LQI values for each TX Power
df_avg = pd.DataFrame(columns=['TX Power', 'Average RSSI', 'Average LQI'])

# Iterate over all files in the directory
for filename in os.listdir(directory):
    if filename.endswith('.txt'):
        # Extract TX Power from filename
        filename_parts = filename.split('-')
        if len(filename_parts) == 5:  # Negative TX Power
            tx_power = float(filename_parts[2] + filename_parts[3])
        else:  # Non-negative TX Power
            tx_power = float(filename_parts[2])

        # Read file into a DataFrame
        df = pd.read_csv(os.path.join(directory, filename), names=['message_id', 'curr_time_in_seconds', 'rssi', 'lqi', 'ack'])
        df.replace('NaN', np.nan, inplace=True)
        df[['rssi', 'lqi']] = df[['rssi', 'lqi']].apply(pd.to_numeric, errors='coerce')

        # Calculate average RSSI and LQI, ignoring NaN values
        avg_rssi = df['rssi'].mean()
        avg_lqi = df['lqi'].mean()

        # Add averages to df_avg
        new_row = pd.DataFrame({'TX Power': [tx_power], 'Average RSSI': [avg_rssi], 'Average LQI': [avg_lqi]})
        df_avg = pd.concat([df_avg, new_row], ignore_index=True)

# Plot average RSSI and LQI for each TX Power
fig, ax = plt.subplots()

df_avg.plot.bar(x='TX Power', y=['Average RSSI', 'Average LQI'], ax=ax)

ax.set_title('Average RSSI and LQI for each TX Power')
ax.set_xlabel('TX Power')
ax.set_ylabel('Average RSSI / LQI')

plt.tight_layout()
plt.savefig('average_rssi_lqi.pdf')
