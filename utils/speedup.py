import matplotlib.pyplot as plt
import glob
import os
import json
data={}
style={}
for folder in glob.glob("D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\newbenchmarks\\*"):
    
    print(folder)
    last_folder = os.path.basename(os.path.normpath(folder))
    for filename in glob.glob(folder+'\\*40x40_h*.txt'):
        p=filename.split("_")[5][1:]
        if not last_folder in data:
            data[last_folder]={"1":[],"4":[],"16":[],"25":[],"64":[],"100":[],"400":[]}
            style[last_folder]="ro"
        print(filename)
        f=open(filename,"r")
        lines = [next(f).strip().split(" time ")[1] for _ in range(3)]
        data[last_folder][p].append(float(lines[1]))
    for filename in glob.glob(folder+'\\*200x200_h*.txt'):
        p=filename.split("_")[5][1:]
        if not last_folder in data:
            data[last_folder]={"1":[],"4":[],"16":[],"25":[],"64":[],"100":[],"400":[]}
            style[last_folder]="go"
        print(filename)
        f=open(filename,"r")
        lines = [next(f).strip().split(" time ")[1] for _ in range(3)]
        data[last_folder][p].append(float(lines[1]))
    for filename in glob.glob(folder+'\\*400x400_h*.txt'):
        p=filename.split("_")[5][1:]
        if not last_folder in data:
            data[last_folder]={"1":[],"4":[],"16":[],"25":[],"64":[],"100":[],"400":[]}
            style[last_folder]="bo"
        print(filename)
        f=open(filename,"r")
        lines = [next(f).strip().split(" time ")[1] for _ in range(3)]
        data[last_folder][p].append(float(lines[1]))
        #print(data)
    print(json.dumps(data))


for key, value in data.items():
    for p_number,times in value.items():
        print("values for "+key+ "["+p_number+"]: ", times)
        print("sum is ", sum(times))
        print("number of elements is ", len(times))
        data[key][p_number]=sum( times)/len(times)
        print("average for "+p_number+" is "+ str(data[key][p_number]))
        if(p_number!="1"):
            data[key][p_number]=data[key]["1"]/data[key][p_number]
        print("speed up for "+p_number+" is "+ str(data[key][p_number]))
    data[key]["1"]=0
    plt.plot([k for k in value.keys()][1:], [v for k,v in value.items()][1:], style[key], label=key,linestyle="-")
print(json.dumps(data))
"""for i in range(3)
plt.plot(x, y200, 'go', label='200x200',linestyle="-")
plt.plot(x, y400, 'bo', label='400x400',linestyle="-")"""

# Add axis labels and title
plt.xlabel("Number of processes")
plt.ylabel("SpeedUp")

# Add a legend
plt.legend()

# Show the plot
plt.show()




print("end")