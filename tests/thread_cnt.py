import matplotlib.pyplot as plt 
  
thread_idx = []
times = [] 

f = open('../out/release/thread_times.txt','r') 
for row in f: 
    row = row.split(' ') 
    thread_idx.append(int(row[0])) 
    times.append(float(row[1])) 

      
plt.title('Avg render time for given thread count. 20 runs per thread num') 
plt.xlabel('Thread Num') 
plt.ylabel('Time (ms)') 

min_x = min(thread_idx)  
max_x = max(thread_idx)  
tick_positions = range(min_x, max_x + 1, 5)  # Generate tick positions spaced by 5
plt.xticks(tick_positions)

plt.plot(thread_idx, times, marker = 'o', c = 'b')  
plt.show()
