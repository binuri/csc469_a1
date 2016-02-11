#!/usr/bin/env python
import shlex, subprocess, os, sys

nodebr = {}
nodeat = {}
nodemint = {}
nodemaxt = {}

for membind in [0,2,4,6]:

	for cpunode in [0,2,4,6]:
		nodebr[membind/2,cpunode /2] = 0
		nodeat[membind/2,cpunode /2] = 0
		nodemint[membind/2,cpunode /2] = 0
		nodemaxt[membind/2,cpunode /2] = 0
	
		print("\nTesting memory bandwifth from CPUs in node " + str(cpunode) + " to memory in node "+str(membind))		
		

		for physcpubind in [0,1,2,3,4,5,6,7,8,9,10,11]:
			bestrate = 0
			avgtime = 0
			mintime = 0
			maxtime = 0
			print("\tTesting processor " + str(physcpubind) + " in node " + str(cpunode))
			args = shlex.split("numactl --membind " + str(membind) + " --physcpubind " + str(physcpubind) + " /u/csc469h/winter/pub/assgn1/bin/mccalpin-stream")
			output = subprocess.check_output(args).splitlines()[22:27]
	
		
			for line in output[1:]:
				line = line.split()
				bestrate +=  float(line[1])
				avgtime += float(line[2])
				mintime += float(line[3])
				maxtime += float(line[4])
	
			bestrate = bestrate/4
			avgtime = avgtime /4
			mintime = mintime/4
			maxtime = maxtime/4
		
			nodebr[membind/2,cpunode /2] += bestrate
			nodeat[membind/2,cpunode /2] += avgtime
			nodemint[membind/2,cpunode /2] += mintime
			nodemaxt[membind/2,cpunode /2] += maxtime
			print '\t' + '\n\t'.join(output)
			print ("\tAvg for processor: Best rate:" + str(bestrate) + "MB/s\tAvg time:" + str(avgtime) + "\n\t\t\tMin time:" + str(mintime) + "\tMax time:" + str(maxtime)+"\n")		
		
		nodebr[membind/2,cpunode /2] = nodebr[membind/2,cpunode /2]/12
		nodeat[membind/2,cpunode /2] = nodeat[membind/2,cpunode /2]/12
		nodemint[membind/2,cpunode /2] = nodemint[membind/2,cpunode /2] /12
		nodemaxt[membind/2,cpunode /2] = nodemaxt[membind/2,cpunode /2]/12

		print ("Avg for processors in node " + str(cpunode) + " to acess memory in node " + str(membind) +":\n Best rate:" + str(nodebr[membind/2,cpunode /2]) + "MB/s\tAvg time:" + str(nodeat[membind/2,cpunode /2]) + "\n\t\tMin time:" + str(nodemint[membind/2,cpunode /2]) + "\tMax time:" + str(nodemaxt[membind/2,cpunode /2])+"\n") 
		
print("Memory bandwidths are (in MB/s):")
print("CPU Node\t0:\t2:\t4:\t6:")
for i in range(4):
	print "Mem Node "+str(i*2) + ":\t" + str(nodebr[i,0]) +"\t"+ str(nodebr[i,1]) +"\t"+ str(nodebr[i,2]) +"\t"+ str(nodebr[i,3])
