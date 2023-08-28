import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.backends.backend_pdf
import matplotlib.ticker as ticker
import os

def process_data(file_path):
    df = pd.read_csv(file_path, names=["message_id", "curr_time_in_seconds", "rssi", "lqi", "ack"])
    df.replace('NaN', np.nan, inplace=True)
    df[["rssi", "lqi"]] = df[["rssi", "lqi"]].apply(pd.to_numeric, errors='coerce')
    return df

def find_global_min_max(results_dir):
    global_rssi_min, global_rssi_max = float('inf'), float('-inf')
    global_lqi_min, global_lqi_max = float('inf'), float('-inf')
    
    for file_name in os.listdir(results_dir):
        if file_name.endswith(".txt"):
            file_path = os.path.join(results_dir, file_name)
            data = process_data(file_path)
            
            rssi_min, rssi_max = data['rssi'].min(), data['rssi'].max()
            lqi_min, lqi_max = data['lqi'].min(), data['lqi'].max()
            
            global_rssi_min = min(global_rssi_min, rssi_min)
            global_rssi_max = max(global_rssi_max, rssi_max)
            
            global_lqi_min = min(global_lqi_min, lqi_min)
            global_lqi_max = max(global_lqi_max, lqi_max)
            
    return global_rssi_min, global_rssi_max, global_lqi_min, global_lqi_max

def plot_time_series(data, pdf, rssi_min, rssi_max, lqi_min, lqi_max):
    fig, ax1 = plt.subplots()

    color = 'tab:red'
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('RSSI', color=color)
    ax1.plot(data['curr_time_in_seconds'][~data['rssi'].isna()], data['rssi'][~data['rssi'].isna()], color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    ax1.set_ylim(rssi_min, rssi_max)

    ax2 = ax1.twinx()  
    color = 'tab:blue'
    ax2.set_ylabel('LQI', color=color)  
    ax2.plot(data['curr_time_in_seconds'][~data['lqi'].isna()], data['lqi'][~data['lqi'].isna()], color=color)
    ax2.tick_params(axis='y', labelcolor=color)
    ax2.set_ylim(lqi_min, lqi_max)

    ax1.xaxis.set_major_locator(ticker.MaxNLocator(nbins=10))
    fig.tight_layout()  
    pdf.savefig(fig)
    plt.close()

def plot_histograms(data, pdf):
    fig, axs = plt.subplots(2, 1, tight_layout=True)
    
    # Exclude rows with NaN RSSI values
    axs[0].hist(data['rssi'][~data['rssi'].isna()].values, bins=30, color='red')  # Convert pandas Series to numpy array
    axs[0].set_title('RSSI')
    
    # Exclude rows with NaN LQI values
    axs[1].hist(data['lqi'][~data['lqi'].isna()].values, bins=30, color='blue')  # Convert pandas Series to numpy array
    axs[1].set_title('LQI')
    
    pdf.savefig(fig)
    plt.close()

if __name__ == "__main__":
    results_dir = "results"
    plots_dir = "plots"
    os.makedirs(plots_dir, exist_ok=True)
    
    rssi_min, rssi_max, lqi_min, lqi_max = find_global_min_max(results_dir)

    for file_name in os.listdir(results_dir):
        if file_name.endswith(".txt"):
            file_path = os.path.join(results_dir, file_name)
            print(f"Processing file: {file_path}")
            data = process_data(file_path)

            pdf_line = matplotlib.backends.backend_pdf.PdfPages(os.path.join(plots_dir, file_name[:-4] + "-line.pdf"))
            plot_time_series(data, pdf_line, rssi_min, rssi_max, lqi_min, lqi_max)
            pdf_line.close()
            print(f"Saved line plot to: {os.path.join(plots_dir, file_name[:-4] + '-line.pdf')}")

            pdf_histogram = matplotlib.backends.backend_pdf.PdfPages(os.path.join(plots_dir, file_name[:-4] + "-histogram.pdf"))
            plot_histograms(data, pdf_histogram) 
            pdf_histogram.close()
            print(f"Saved histogram plot to: {os.path.join(plots_dir, file_name[:-4] + '-histogram.pdf')}")
