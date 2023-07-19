import os

def process_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    new_lines = []
    expected_message_id = None

    for line in lines:
        message_id, timestamp, rssi, lqi = line.split(', ')
        message_id = int(message_id)

        if expected_message_id is None:
            expected_message_id = message_id

        while message_id > expected_message_id:
            new_lines.append(f"{expected_message_id}, NaN, NaN, NaN, 0\n")
            expected_message_id += 1

        new_lines.append(f"{message_id}, {timestamp}, {rssi}, {lqi.strip()}, 1\n")
        expected_message_id += 1

    return new_lines

def process_files():
    input_directory = 'unprocessed-results'
    output_directory = 'results'

    os.makedirs(output_directory, exist_ok=True)

    for filename in os.listdir(input_directory):
        new_lines = process_file(os.path.join(input_directory, filename))

        with open(os.path.join(output_directory, filename), 'w') as f:
            f.writelines(new_lines)

process_files()
