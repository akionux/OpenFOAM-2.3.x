#!/usr/bin/env python
# create parcel injections
# http://www.geocities.co.jp/SiliconValley-SantaClara/1183/study/OpenFOAM/injection.html

from random import random

# input start
minx = 0
maxx = 0.032
miny = 0.0
maxy = 0.020
minz = 0
maxz = 0.017

nx = 32 
ny = 20
nz = 17

turbdx = 3.2e-5
turbdy = 2.0e-5
turbdz = 1.7e-5

d = 1e-3
v = (0, -1, 0)
rho = 964
mdot = 1
# input end


rangex = maxx - minx
rangey = maxy - miny
rangez = maxz - minz

dx = rangex/(nx + 1)
dy = rangey/(ny + 1)
dz = rangez/(nz + 1)

print('''/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  2.0.1                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.com                      |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       vectorField;
    object      kinematicCloudPositions;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

(''')

for i in range(0, nx):
	for j in range(0, ny):
		for k in range(0, nz):
			x = minx + dx*(i + 1) + turbdx*random()
			y = miny + dy*(j + 1) + turbdy*random()
			z = minz + dz*(k + 1) + turbdz*random()
			#print "(%f %f %f) (%f %f %f) %e %f %e" % \
			#	(x, y, z, v[0], v[1], v[2], d, rho, mdot)
                        print "(%f %f %f)" % (x, y, z)
print(''')

// ************************************************************************* //''')

