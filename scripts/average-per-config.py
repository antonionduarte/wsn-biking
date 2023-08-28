
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Directory where the files are stored
directory = 'results'

# Create empty DataFrame to store average RSSI and LQI values for each test configuration
df_avg = pd.DataFrame(columns=['Test Configuration', 'Average RSSI', 'Average LQI'])

# Iterate over all files in the directory
for filename in os.listdir(directory):
    if filename.endswith('.txt'):
        # Use filename (without .txt) as test configuration label
        test_config = filename[:-4]

        # Read file into a DataFrame
        df = pd.read_csv(os.path.join(directory, filename), names=['message_id', 'curr_time_in_seconds', 'rssi', 'lqi', 'ack'])
        df.replace('NaN', np.nan, inplace=True)
        df[['rssi', 'lqi']] = df[['rssi', 'lqi']].apply(pd.to_numeric, errors='coerce')

        # Calculate average RSSI and LQI, ignoring NaN values
        avg_rssi = df['rssi'].mean()
        avg_lqi = df['lqi'].mean()

        # Add averages to df_avg
        new_row = pd.DataFrame({'Test Configuration': [test_config], 'Average RSSI': [avg_rssi], 'Average LQI': [avg_lqi]})
        df_avg = pd.concat([df_avg, new_row], ignore_index=True)

# Plot average RSSI and LQI for each test configuration
fig, ax = plt.subplots(figsize=(10, 6))

df_avg.plot.bar(x='Test Configuration', y=['Average RSSI', 'Average LQI'], ax=ax)

ax.set_title('Average RSSI and LQI for each Test Configuration')
ax.set_xlabel('Test Configuration')
ax.set_ylabel('Average RSSI / LQI')
ax.legend(loc="upper right")
ax.tick_params(axis='x', rotation=45)

plt.tight_layout()
plt.savefig('average_rssi_lqi_per_test_config.pdf')
