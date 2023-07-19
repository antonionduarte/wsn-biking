import os
import serial
import time

ser = serial.Serial('/dev/tty.SLAB_USBtoUART', 115200, timeout=1)

filename = None
data = ""
reading_file = False

# Directory to save the results
results_dir = "unprocessed-results"
os.makedirs(results_dir, exist_ok=True)

while True:
    line = ser.readline().decode('utf-8').strip()
    if line:
        print("Received line: " + line)
        if line.startswith("FILENAME:"):
            # If we have data from a previous file, write it to disk
            if filename is not None and data:
                with open(os.path.join(results_dir, filename), 'a') as f:
                    print("Writing to file: " + filename)
                    print("Data: " + data)
                    f.write(data)
                print("Wrote data to file: " + filename)
            # Start a new file
            filename = line[9:]  # Remove the "FILENAME:" prefix
            data = ""
        elif line.startswith("[INFO: Client    ]"):
            # Append to the current file's data
            data_line = line[18:] 
            data += data_line + "\n"  # Add the newline back in
        else:
            print("Unrecognized line: " + line)
    else:
        time.sleep(1)  # Wait a second before checking for new data
