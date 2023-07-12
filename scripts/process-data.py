import pandas as pd
import matplotlib.pyplot as plt
import random
import time


def process_data(file_path):
    # Read the data file into a pandas DataFrame
    df = pd.read_csv(file_path, names=["message_id", "curr_time_in_seconds", "rssi", "lqi"])
    return df


def generate_mock_data(file_path, num_entries=1000):
    with open(file_path, 'w') as f:
        curr_time = 1000000  # Start from a base time of 1,000,000 seconds since the epoch
        for message_id in range(1, num_entries + 1):
            rssi = random.randint(-100, -30)  # Random RSSI between -100 and -30
            lqi = random.randint(0, 255)  # Random LQI between 0 and 255
            line = f"{message_id}, {curr_time}, {rssi}, {lqi}\n"
            f.write(line)
            curr_time += 1  # Increment the time by one second


def plot_time_series(data):
    ax1 = plt.subplot(1, 1, 1)

    color = 'tab:red'
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('RSSI', color=color)
    ax1.plot(data['curr_time_in_seconds'], data['rssi'], color=color)
    ax1.tick_params(axis='y', labelcolor=color)

    ax2 = ax1.twinx()  
    color = 'tab:blue'
    ax2.set_ylabel('LQI', color=color)  
    ax2.plot(data['curr_time_in_seconds'], data['lqi'], color=color)
    ax2.tick_params(axis='y', labelcolor=color)

    plt.tight_layout()  
    plt.show()


def plot_histograms(data):
    fig, axs = plt.subplots(2, 1, tight_layout=True)
    
    axs[0].hist(data['rssi'].values, bins=30, color='red')  # Convert pandas Series to numpy array
    axs[0].set_title('RSSI')
    
    axs[1].hist(data['lqi'].values, bins=30, color='blue')  # Convert pandas Series to numpy array
    axs[1].set_title('LQI')
    
    plt.show()


if __name__ == "__main__":
    # generate_mock_data("./mock-data.csv")
    data = process_data("./1-256-10-16.txt")
    plot_time_series(data)
    # plot_histograms(data)
    
