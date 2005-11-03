



# Kreisender Vogel
# TODO: Abwechelnde Animationen, neuer Pfad

greifvogel = $AM.createMeshActor("Greifvogel", "tie_greifvogel.mesh" );
# , PhysicsManager::GT_CAPSULE);
greifvogel.placeIntoScene(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
$CORE.log("Fackel plaziert.");

# torch.getActor().setScale( 1.0, 1.0, 1.0 );

greifvogel.getControlledObject().startAnimation( "fliegen" );

trackAnim = $AnimMgr.createTrackAnimation(  greifvogel, "Patrouilleflug", 16.0 );

trackAnim.addKeyFrame( 0.0 );
trackAnim.setKeyFrameTranslation( 0.0, -402.735565185547, 2128.517913818359, 25.9572200775146)
# Erstes Frame keine ROTATION einfügen - Drehung sonst zuviel
#trackAnim.setKeyFrameRotationQuaternion( 0.0, -0.998850047588348, 0.0, 0.0478907190263271, 0.0)
trackAnim.addKeyFrame( 1.0 );
trackAnim.setKeyFrameTranslation( 1.0, -403.404174804688, 2128.617431640625, -104.758270263672)
trackAnim.setKeyFrameRotationQuaternion( 1.0, 0.989310145378113, 0.0, 0.145824044942856, 0.0)
trackAnim.addKeyFrame( 2.0 );
trackAnim.setKeyFrameTranslation( 20.0, -438.427459716797, 2133.843383789063, -216.583953857422)
trackAnim.setKeyFrameRotationQuaternion( 2.0, 0.955484211444855, 0.0, 0.295041054487228, 0.0)
trackAnim.addKeyFrame( 3.0 );
trackAnim.setKeyFrameTranslation( 3.0, -530.476684570313, 2147.57844543457, -336.240875244141)
trackAnim.setKeyFrameRotationQuaternion( 3.0, 0.882290780544281, 0.0, 0.470703780651093, 0.0)
trackAnim.addKeyFrame( 4.0 );
trackAnim.setKeyFrameTranslation( 4.0, -649.519775390625, 2165.34130859375, -390.117767333984)
trackAnim.setKeyFrameRotationQuaternion( 4.0, 0.787741661071777, 0.0, 0.616005063056946, 0.0)
trackAnim.addKeyFrame( 5.0 );
trackAnim.setKeyFrameTranslation( 5.0, -773.101867675781, 2183.781524658203, -405.283477783203)
trackAnim.setKeyFrameRotationQuaternion( 5.0, 0.642118394374847, 0.0, 0.766604542732239, 0.0)
trackAnim.addKeyFrame( 6.0 );
trackAnim.setKeyFrameTranslation( 6.0, -907.570007324219, 2201.812774658203, -358.483245849609)
trackAnim.setKeyFrameRotationQuaternion( 6.0, 0.507913887500763, 0.0, 0.861406981945038, 0.0)
trackAnim.addKeyFrame( 7.0 );
trackAnim.setKeyFrameTranslation( 7.0, -1014.51361083984, 2214.484481811523, -258.786376953125)
trackAnim.setKeyFrameRotationQuaternion( 7.0, 0.357878595590591, 0.0, 0.933766841888428, 0.0)
trackAnim.addKeyFrame( 8.0 );
trackAnim.setKeyFrameTranslation( 8.0, -1069.45080566406, 2214.673858642578, -141.598129272461)
trackAnim.setKeyFrameRotationQuaternion( 8.0, 0.167800903320313, 0.0, 0.985819756984711, 0.0)
trackAnim.addKeyFrame( 9.0 );
trackAnim.setKeyFrameTranslation( 9.0, -1067.86804199219, 2227.764785766602, 0.760276734828949)
trackAnim.setKeyFrameRotationQuaternion( 9.0, -0.126025766134262, 0.0, 0.992025673389435, 0.0)
trackAnim.addKeyFrame( 10.0 );
trackAnim.setKeyFrameTranslation( 10.0, -1016.34417724609, 2220.076690673828, 144.0986328125)
trackAnim.setKeyFrameRotationQuaternion( 10.0, -0.339558452367783, 0.0, 0.940583467483521, 0.0)
trackAnim.addKeyFrame( 11.0 );
trackAnim.setKeyFrameTranslation( 11.0, -947.081604003906, 2209.560516357422, 220.864562988281)
trackAnim.setKeyFrameRotationQuaternion( 11.0, -0.47907418012619, 0.0, 0.877772748470306, 0.0)
trackAnim.addKeyFrame( 12.0 );
trackAnim.setKeyFrameTranslation( 12.0, -811.8046875, 2189.556579589844, 283.753234863281)
trackAnim.setKeyFrameRotationQuaternion( 12.0, -0.665098965167999, 0.0, 0.746753036975861, 0.0)
trackAnim.addKeyFrame( 13.0 );
trackAnim.setKeyFrameTranslation(13.0, -680.105529785156, 2169.905258178711, 286.176727294922)
trackAnim.setKeyFrameRotationQuaternion( 13.0, -0.78195458650589, 0.0, 0.623332798480988, 0.0)
trackAnim.addKeyFrame( 14.0 );
trackAnim.setKeyFrameTranslation( 14.0, -560.457214355469, 2152.052108764648, 232.767608642578)
trackAnim.setKeyFrameRotationQuaternion( 14.0, -0.897179543972015, 0.0, 0.441661357879639, 0.0)
trackAnim.addKeyFrame( 15.0 );
trackAnim.setKeyFrameTranslation( 15.0, -450.603240966797, 2135.660400390625, 134.022109985352)
trackAnim.setKeyFrameRotationQuaternion( 15.0, -0.971691370010376, 0.0, 0.236243948340416, 0.0)
trackAnim.addKeyFrame( 16.0 );
trackAnim.setKeyFrameTranslation( 16.0, -402.735565185547, 2128.517913818359, 25.9572200775146)
trackAnim.setKeyFrameRotationQuaternion( 16.0, -0.998850047588348, 0.0, 0.0478907190263271, 0.0)

trackAnim.setInterpolationMode( AnimationManager::IM_SPLINE );


trackAnim.setPaused( false );
