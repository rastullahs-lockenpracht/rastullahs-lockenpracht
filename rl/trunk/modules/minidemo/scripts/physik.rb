
Licht = $AF.createLightActor("EinsamesLeuchten");
Licht.setPosition(-128.0,128.0,130.0);

# Gummiball
Gummiball = $AF.createGameActor("Gummiball","sphere.mesh");
Gummiball.setPosition(-128.0,28.0,120.0);
Gummiball.setScale(0.1,0.1,0.1);
$phyBall = $PM.createSpherePhysicalThing( Gummiball, 1.45 );
$phyBall.setBounceParameters( 0.8, 0.1 );
$phyBall.setFriction( 20.0 );

size = 20.0;
cubemult = 1.0/(100.0/20.0);
rows = 3;

for row in 0..rows-1
	for cube in 0..(rows-1-row)
		# Eisenwuerfel
		name = "wuerfel_"+row.to_s()+"_"+cube.to_s();		
		wuerfel = $AF.createGameActor(name,"cube.mesh");
		wuerfel.setPosition(-128.0-((rows-1-row)*(size+1.5)/2)+(cube*(size+1.5)),128.0,32.5+(row*(size)));
		wuerfel.setScale(cubemult,cubemult,cubemult);
		wuerfelphys = $PM.createBoxPhysicalThing( wuerfel, 7.9 );
		wuerfelphys.stopDynamics();
	end
end


# statische Ebene
ebene = $AF.createGameActor("ebene","cube.mesh");
ebene.setPosition(-128.0,128.0,20.0);
ebene.setScale(1.0,1.0,0.05);
ebenePhys = $PM.createBoxPhysicalThing( ebene, 7.9 );
ebenePhys.setBounceParameters( 0.001, 0.1 );
ebenePhys.setSoftErp( 0.05 );
ebenePhys.setSoftness( 0.005 );
ebenePhys.setFriction( 0.5 );
ebenePhys.setDynamic( false );

# Erdenschwerkraft
$PM.setGravity( 0.0, 0.0, -9.80665 );
