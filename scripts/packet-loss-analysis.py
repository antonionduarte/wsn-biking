import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.backends.backend_pdf
import os

def process_data(file_path):
    # Read the data file into a pandas DataFrame
    df = pd.read_csv(file_path, names=["message_id", "curr_time_in_seconds", "rssi", "lqi", "ack"])
    return df

def plot_rssi_vs_ack(data, pdf):
    plt.scatter(data['rssi'], data['ack'])
    plt.xlabel('RSSI')
    plt.ylabel('ACK')
    plt.title('RSSI vs ACK')
    pdf.savefig()
    plt.close()

if __name__ == "__main__":
    results_dir = "results"
    plots_dir = "plots"
    os.makedirs(plots_dir, exist_ok=True)

    for file_name in os.listdir(results_dir):
        if file_name.endswith(".txt"):
            file_path = os.path.join(results_dir, file_name)
            print(f"Processing file: {file_path}")
            data = process_data(file_path)

            pdf_scatter = matplotlib.backends.backend_pdf.PdfPages(os.path.join(plots_dir, file_name[:-4] + "-scatter.pdf"))
            plot_rssi_vs_ack(data, pdf_scatter)
            pdf_scatter.close()
            print(f"Saved scatter plot to: {os.path.join(plots_dir, file_name[:-4] + '-scatter.pdf')}")
