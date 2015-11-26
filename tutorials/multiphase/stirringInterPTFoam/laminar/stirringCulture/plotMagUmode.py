#!/usr/bin/env python
# coding: UTF-8

import os.path
import numpy as np
import matplotlib.pyplot as plt

t=0.1
dt=0.1
nTime=0
while os.path.exists("{0:g}/lagrangian/kinematicCloud/U".format(t)):
	print("Found lagrangian data at t={0:g}".format(t))
	t += dt
	nTime+=1

t=0.1
lagDataT = []
for i in range(nTime):
	d = {}
	lagDataT.append(d)
for i in range(nTime):
	f = open("{0:g}/lagrangian/kinematicCloud/U".format(t))
	l = f.readline()
	while l:
		try:
			int(l)
		except ValueError:
			l = f.readline()
			continue
		break
	lagDataT[i].update({"U":np.zeros((int(l),3),dtype=np.float)})
	lagDataT[i].update({"magU":np.zeros(int(l),dtype=np.float)})
	#lagDataT[i].append({"U":np.zeros((int(l),3),dtype=np.float)}.copy())
	#lagDataT[i].append({"magU":np.zeros(int(l),dtype=np.float)}.copy())
	f.readline();
	for j in range(lagDataT[i]["U"].shape[0]):
		Ui=f.readline().replace("(","").replace(")\n","").split(" ")
		lagDataT[i]["U"][j,0] = np.float(Ui[0])
		lagDataT[i]["U"][j,1] = np.float(Ui[1])
		lagDataT[i]["U"][j,2] = np.float(Ui[2])
		lagDataT[i]["magU"][j] = np.linalg.norm(lagDataT[i]["U"][j])
	f.close()
	print "t={0:g}, number of particles:{1:g}".format(t,lagDataT[i]["U"].shape[0])
	t += dt

lagDatai = []
for i in range(lagDataT[0]["U"].shape[0]):
	d = {}
	lagDatai.append(d)
for i in range(lagDataT[0]["U"].shape[0]):
	lagDatai[i].update({"U":np.zeros((nTime,3),dtype=np.float)})
	lagDatai[i].update({"magU":np.zeros(nTime,dtype=np.float)})
	for j in range(nTime):
		lagDatai[i]["U"][j,0]=lagDataT[j]["U"][i,0]
		lagDatai[i]["U"][j,1]=lagDataT[j]["U"][i,1]
		lagDatai[i]["U"][j,2]=lagDataT[j]["U"][i,2]
		lagDatai[i]["magU"][j]=lagDataT[j]["magU"][i]

Umodes=np.zeros(lagDataT[0]["U"].shape[0])
for i in range(lagDataT[0]["U"].shape[0]):
	print("Calculating the histgram of particle #{0:d}...\n".format(i))
	n, bins, patches = plt.hist(lagDatai[i]["magU"], bins=[0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09], normed=1)
	Umodes[i]=np.float(bins[np.argmax(n)])
	plt.clf()

plt.hist(Umodes)
plt.xlim(0,0.1)
#plt.ylim(0,100)
plt.savefig("magUmode.png")
#plt.savefig("lagData{0:d}_magU.png".format(i))
	#plt.clf()
