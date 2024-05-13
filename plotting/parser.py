import csv
from collections import defaultdict

import matplotlib.pyplot as plt

# Input data
input_data = open('input.txt', 'r')

# Dictionary to store accumulated values for each num_threads
thread_data = defaultdict(lambda: {'res_sum': 0, 'time_sum': 0, 'count': 0})

# Parse input data
for line in input_data:
    # Extract num_threads, res, and time
    num_threads = int(line.split(',')[0].split()[-1])
    res = int(line.split(',')[1].split()[-1])
    time = float(line.split(',')[2].split()[-1])

    # Accumulate values for each num_threads
    thread_data[num_threads]['res_sum'] += res
    thread_data[num_threads]['time_sum'] += time
    thread_data[num_threads]['count'] += 1

# Calculate averages and write to CSV
with open('output.csv', 'w', newline='') as csvfile:
    fieldnames = ['num_threads', 'average_res', 'average_time']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()

    for num_threads, data in thread_data.items():
        average_res = data['res_sum'] / data['count']
        average_time = data['time_sum'] / data['count']
        writer.writerow({'num_threads': num_threads, 'average_res': average_res, 'average_time': average_time})

print("CSV file 'output.csv' has been created.")

# Lists to store data
num_threads_list = []
time_sum_list = []

# Read data from CSV file
with open('output.csv', 'r') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    for row in csv_reader:
        num_threads_list.append(int(row['num_threads']))
        time_sum_list.append(float(row['average_time']))

# Plotting
plt.figure(figsize=(10, 6))
plt.bar(num_threads_list, time_sum_list, color='skyblue')
plt.xlabel('num_threads')
plt.ylabel('Average Time in seconds')
plt.title('Average Time vs Number threads')
plt.xticks(num_threads_list)
plt.grid(True)
plt.savefig('output.png')
plt.show()
print("Figure saved.")
