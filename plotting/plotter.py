import matplotlib.pyplot as plt
from collections import defaultdict

def read_data(file_path):
    data = defaultdict(lambda: defaultdict(list))
    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split(',')
            group_id = (parts[0].split(': ')[1])
            thread_num = int(parts[1].split(': ')[1])
            time = float(parts[2].split(': ')[1])
            data[group_id][thread_num].append(time)

    averages = defaultdict(dict)
    for group_id, threads in data.items():
        for thread_num, times in threads.items():
            avg_time = sum(times) / len(times)
            averages[group_id][thread_num] = avg_time

    return averages

def plot_graph(data, save_path=None):
    plt.figure(figsize=(10, 6))
    for group_id, values in data.items():
        plt.plot(list(values.keys()), list(values.values()), marker='o', label=f'Group {group_id}')
        for thread_num, avg_time in values.items():
            plt.text(thread_num, avg_time, f'{avg_time:.2f}', ha='left', va='bottom')
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Time [s]')
    plt.title('Number of Threads vs Average Time for Each Group')
    plt.legend()
    plt.grid(True)
    if save_path:
        plt.savefig(save_path)
    else:
        plt.show()

def plot_single_thread_bar(data, thread_num, save_path=None):
    plt.figure(figsize=(6, 6))
    for group_id, values in data.items():
        if thread_num in values:
            plt.bar("", values[thread_num], label=f'Group {group_id}')
            plt.text("", values[thread_num], f'{values[thread_num]:.2f}', ha='center', va='bottom')
    plt.ylabel('Average Time')
    plt.title(f'Average Time for Thread {thread_num} Across Groups')
    plt.legend()
    plt.grid(True)
    if save_path:
        plt.savefig(save_path)
    else:
        plt.show()


if __name__ == "__main__":
    file_path = "data.txt"  # Path to your data file
    data = read_data(file_path)
    plot_graph(data, save_path="dynamic.png")
    # plot_single_thread_bar(data, 1, save_path="single_thread_bar.png")

