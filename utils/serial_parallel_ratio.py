import matplotlib.pyplot as plt
import glob
import os
import json


data={}
for folder in glob.glob("D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\newbenchmarks\\*"):
    print(folder)
    last_folder = os.path.basename(os.path.normpath(folder))
    for filename in glob.glob(folder+'\\*P1_*.txt'):
        p=int(filename.split("_")[5][1:])
        if not last_folder in data:
            data[last_folder]={"serial":[],"parallel":[]}
        print(filename)
        f=open(filename,"r")
        lines = [next(f).strip().split(" time ")[1] for _ in range(3)]
        data[last_folder]["serial"].append(float(lines[1]))
    for filename in glob.glob(folder+'\\*P100_*.txt'):
        p=int(filename.split("_")[5][1:])
        if not last_folder in data:
            data[last_folder]={"serial":[],"parallel":[]}
        print(filename)
        f=open(filename,"r")
        lines = [next(f).strip().split(" time ")[1] for _ in range(3)]
        data[last_folder]["parallel"].append(float(lines[1]))
        #print(data)
    print(json.dumps(data))
x=[]
y_serial=[]
y_parallel=[]
for key, value in data.items():
    x.append(key)
    y_serial.append(sum(value["serial"])/len(value["serial"]))
    y_parallel.append(sum(value["parallel"])/len(value["parallel"]))
plt.plot(x, y_serial, 'ro', label='serial',linestyle="-")
plt.plot(x, y_parallel, 'go', label='parallel',linestyle="-")

# Add axis labels and title
plt.xlabel("Number of workers")
plt.ylabel("Average execution time")

# Add a legend
plt.legend()

# Show the plot
plt.show()
"""
x=[]
y0=[]
y1=[]
y2=[]
for key, value in data.items():
    print(key)
    x.append(int(key.split("_")[-1]))
    root_time_avg=0
    worker_time_avg=0
    total_time_avg=0
    for mode in ["default","pack","scatter"] :
        root_time_avg+=sum([float(value[mode][i][0]) for i in range(len(value[mode]))])/len(value[mode])
        worker_time_avg+=sum([float(value[mode][i][1]) for i in range(len(value[mode]))])/len(value[mode])
        total_time_avg+=sum([float(value[mode][i][2]) for i in range(len(value[mode]))])/len(value[mode])
    y0.append(root_time_avg/3)
    y1.append(worker_time_avg/3)
    y2.append(total_time_avg/3)
    print(root_time_avg,worker_time_avg,total_time_avg)

sorted_x,sorted_y0,sorted_y1,sorted_y2=zip(*sorted(zip(x, y0, y1,y2), key=lambda x: x[0]))

print(sorted_x)
print(sorted_y0)
plt.plot(sorted_x, sorted_y0, 'ro', label='root',linestyle="-")
plt.plot(sorted_x, sorted_y1, 'go', label='worker',linestyle="-")
plt.plot(sorted_x, sorted_y2, 'bo', label='total',linestyle="-")

# Add axis labels and title
plt.xlabel("Number of workers")
plt.ylabel("Average execution time")

# Add a legend
plt.legend()

# Show the plot
plt.show()
"""



print("end")