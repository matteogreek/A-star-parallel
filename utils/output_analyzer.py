import matplotlib.pyplot as plt
import glob
import os
import json
for folder in glob.glob("D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\newbenchmarks\\*"):
    
    data={}
    print(folder)
    last_folder = os.path.basename(os.path.normpath(folder))
    for filename in glob.glob(folder+'\\*.txt'):
        p=int(filename.split("_")[5][1:])
        if not last_folder+"_"+str(p) in data:
            data[last_folder+"_"+str(p)]={"default":[],"pack":[],"scatter":[]}
        f=open(filename,"r")
        lines = [next(f).strip().split(" time ")[1] for _ in range(3)]
        if("scatter" in filename):
            data[last_folder+"_"+str(p)]["scatter"].append(lines)
        elif("pack" in filename):
            data[last_folder+"_"+str(p)]["pack"].append(lines)
        elif("default" in filename):
            data[last_folder+"_"+str(p)]["default"].append(lines)
        #print(data)
    print(json.dumps(data))
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
    plt.xlabel("Number of processes")
    plt.ylabel("Average execution time (s)")

    # Add a legend
    plt.legend()

    # Show the plot
    plt.show()




print("end")