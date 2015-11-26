{
    volScalarField rAU("rAU", 1.0/UEqn.A());
    surfaceScalarField rAUf("rAUf", fvc::interpolate(rAU));

    volVectorField HbyA("HbyA", U);
    HbyA = rAU*UEqn.H();

    surfaceScalarField phiHbyA
    (
        "phiHbyA",
        (fvc::interpolate(HbyA) & mesh.Sf())
      + fvc::interpolate(rho*rAU)*fvc::ddtCorr(U, phi)
    );
    adjustPhi(phiHbyA, U, p_rgh);
    fvOptions.makeRelative(phiHbyA);

	dimensionedScalar t("t", dimTime, runTime.value());
	/*
	//Debug only
	dimensionedScalar r0("r0",dimLength,0.002);
	scalar rps(scalar(2)); // revolutions per second
	dimensionedScalar omega("omega", dimensionSet(0, 0, -1, 0, 0, 0, 0), rps*2*constant::mathematical::pi); // 120rpm=2rps
	dimensionedVector xr1("xr1", dimLength, vector(0.016,0.004,0.0085));
	*/

	dimensionedVector rC("rC", dimLength, vector((r0*cos(omega*t)).value(),scalar(0),(r0*sin(omega*t)).value()));// rotation center at this time
	surfaceScalarField rOmegaf(((mesh.Cf()-xr1)+rC)*sqr(omega)&mesh.Sf()/mesh.magSf()); // centrifugal force

    surfaceScalarField phig
    (
		(
            mixture.surfaceTensionForce()
          - ghf*fvc::snGrad(rho)
		  + rOmegaf*fvc::interpolate(rho)
        )*rAUf*mesh.magSf()
    );

    phiHbyA += phig;

    // Update the fixedFluxPressure BCs to ensure flux consistency
    setSnGrad<fixedFluxPressureFvPatchScalarField>
    (
        p_rgh.boundaryField(),
        (
            phiHbyA.boundaryField()
          - fvOptions.relative(mesh.Sf().boundaryField() & U.boundaryField())
        )/(mesh.magSf().boundaryField()*rAUf.boundaryField())
    );

    while (pimple.correctNonOrthogonal())
    {
        fvScalarMatrix p_rghEqn
        (
            fvm::laplacian(rAUf, p_rgh) == fvc::div(phiHbyA)
        );

        p_rghEqn.setReference(pRefCell, getRefCellValue(p_rgh, pRefCell));

        p_rghEqn.solve(mesh.solver(p_rgh.select(pimple.finalInnerIter())));

        if (pimple.finalNonOrthogonalIter())
        {
            phi = phiHbyA - p_rghEqn.flux();

            p_rgh.relax();

            U = HbyA + rAU*fvc::reconstruct((phig - p_rghEqn.flux())/rAUf);
            U.correctBoundaryConditions();
            fvOptions.correct(U);
        }
    }

    #include "continuityErrs.H"

    p == p_rgh + rho*gh;

    if (p_rgh.needReference())
    {
        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pRefCell)
        );
        p_rgh = p - rho*gh;
    }
}
