import matplotlib.pyplot as plt
from collections import defaultdict

def read_data(file_path):
    data = defaultdict(lambda: defaultdict(list))
    with open(file_path, 'r') as file:
        for line in file:
            group_id, thread_res, num, time = map(int, line.strip().split(','))
            data[group_id][num].append(time)
    
    averages = defaultdict(dict)
    for group_id, threads in data.items():
        for thread_num, times in threads.items():
            avg_time = sum(times) / len(times)
            averages[group_id][thread_num] = avg_time
    
    return averages

def plot_graph(data):
    plt.figure(figsize=(10, 6))
    for group_id, values in data.items():
        plt.plot(list(values.keys()), list(values.values()), label=f'Group {group_id}')
    plt.xlabel('Thread Num')
    plt.ylabel('Average Time')
    plt.title('Thread Num vs Average Time for Each Group')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    file_path = "input.txt"  # Path to your data file
    data = read_data(file_path)
    plot_graph(data)
