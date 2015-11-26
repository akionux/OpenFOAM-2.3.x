/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    timeinfo

Description
    This simple application prints time informations.

Author
	Akio Nishimura

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

int main(int argc, char *argv[])
{
	#include "setRootCase.H"
	#include "createTime.H"

	Info << "controlDict name: " << runTime.controlDictName << endl;
    Info << "root path: " << runTime.rootPath() << endl;
    Info << "case name: " << runTime.caseName() << endl;
    Info << "path: " << runTime.path() << endl;
    Info << "time path: " << runTime.timePath() << endl;
	Info << "format: " << runTime.writeFormat() << endl;
	Info << "version: " << runTime.writeVersion() << endl;
	Info << "compression: " << runTime.writeCompression() << endl;
	Info << "start time: " << runTime.startTime() << endl;
	Info << "end time: " << runTime.endTime() << endl;
	Info << "deltaT: " << runTime.deltaT() << endl;

	runTime.writeNow();
	while(runTime.loop()){
		Info << "Time: " << runTime.timeName() << endl;
		runTime.write();
	}

	runTime.writeAndEnd();

	Info << "execution time: " << runTime.elapsedCpuTime() << " s" << endl;
	Info << "clock time: " << runTime.elapsedClockTime() << " s" << endl;
	return 0;
}
