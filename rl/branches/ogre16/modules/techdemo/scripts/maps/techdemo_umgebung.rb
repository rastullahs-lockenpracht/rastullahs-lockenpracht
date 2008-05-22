



# Kreisender Vogel
# TODO: Abwechelnde Animationen, neuer Pfad

greifvogel = $AM.createMeshActor("Greifvogel", "tie_greifvogel.mesh" );
# , PhysicsManager::GT_CAPSULE);
greifvogel.placeIntoScene(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Greifvogel plaziert.");

# torch.getActor().setScale( 1.0, 1.0, 1.0 );

greifvogel.getControlledObject().startAnimation( "fliegen" );

trackAnim = $AnimMgr.createTrackAnimation(  greifvogel, "Patrouilleflug", 16.0 );

trackAnim.addKeyFrame( 0.0 );
trackAnim.setKeyFrameTranslation( 0.0, -4.02735565185547, 21.28517913818359, 0.259572200775146)
# Erstes Frame keine ROTATION einfügen - Drehung sonst zuviel
#trackAnim.setKeyFrameRotationQuaternion( 0.0, -0.998850047588348, 0.0, 0.0478907190263271, 0.0)
trackAnim.addKeyFrame( 1.0 );
trackAnim.setKeyFrameTranslation( 1.0, -4.03404174804688, 21.28617431640625, -1.04758270263672)
trackAnim.setKeyFrameRotationQuaternion( 1.0, 0.989310145378113, 0.0, 0.145824044942856, 0.0)
trackAnim.addKeyFrame( 2.0 );
trackAnim.setKeyFrameTranslation( 20.0, -4.38427459716797, 21.33843383789063, -2.16583953857422)
trackAnim.setKeyFrameRotationQuaternion( 2.0, 0.955484211444855, 0.0, 0.295041054487228, 0.0)
trackAnim.addKeyFrame( 3.0 );
trackAnim.setKeyFrameTranslation( 3.0, -5.30476684570313, 21.4757844543457, -3.36240875244141)
trackAnim.setKeyFrameRotationQuaternion( 3.0, 0.882290780544281, 0.0, 0.470703780651093, 0.0)
trackAnim.addKeyFrame( 4.0 );
trackAnim.setKeyFrameTranslation( 4.0, -6.49519775390625, 21.6534130859375, -3.90117767333984)
trackAnim.setKeyFrameRotationQuaternion( 4.0, 0.787741661071777, 0.0, 0.616005063056946, 0.0)
trackAnim.addKeyFrame( 5.0 );
trackAnim.setKeyFrameTranslation( 5.0, -7.73101867675781, 21.83781524658203, -4.05283477783203)
trackAnim.setKeyFrameRotationQuaternion( 5.0, 0.642118394374847, 0.0, 0.766604542732239, 0.0)
trackAnim.addKeyFrame( 6.0 );
trackAnim.setKeyFrameTranslation( 6.0, -9.07570007324219, 22.01812774658203, -3.58483245849609)
trackAnim.setKeyFrameRotationQuaternion( 6.0, 0.507913887500763, 0.0, 0.861406981945038, 0.0)
trackAnim.addKeyFrame( 7.0 );
trackAnim.setKeyFrameTranslation( 7.0, -10.1451361083984, 22.14484481811523, -2.58786376953125)
trackAnim.setKeyFrameRotationQuaternion( 7.0, 0.357878595590591, 0.0, 0.933766841888428, 0.0)
trackAnim.addKeyFrame( 8.0 );
trackAnim.setKeyFrameTranslation( 8.0, -10.6945080566406, 22.14673858642578, -1.41598129272461)
trackAnim.setKeyFrameRotationQuaternion( 8.0, 0.167800903320313, 0.0, 0.985819756984711, 0.0)
trackAnim.addKeyFrame( 9.0 );
trackAnim.setKeyFrameTranslation( 9.0, -10.6786804199219, 22.27764785766602, 0.760276734828949)
trackAnim.setKeyFrameRotationQuaternion( 9.0, -0.126025766134262, 0.0, 0.992025673389435, 0.0)
trackAnim.addKeyFrame( 10.0 );
trackAnim.setKeyFrameTranslation( 10.0, -10.1634417724609, 22.20076690673828, 1.440986328125)
trackAnim.setKeyFrameRotationQuaternion( 10.0, -0.339558452367783, 0.0, 0.940583467483521, 0.0)
trackAnim.addKeyFrame( 11.0 );
trackAnim.setKeyFrameTranslation( 11.0, -9.47081604003906, 22.09560516357422, 2.20864562988281)
trackAnim.setKeyFrameRotationQuaternion( 11.0, -0.47907418012619, 0.0, 0.877772748470306, 0.0)
trackAnim.addKeyFrame( 12.0 );
trackAnim.setKeyFrameTranslation( 12.0, -8.118046875, 21.89556579589844, 2.83753234863281)
trackAnim.setKeyFrameRotationQuaternion( 12.0, -0.665098965167999, 0.0, 0.746753036975861, 0.0)
trackAnim.addKeyFrame( 13.0 );
trackAnim.setKeyFrameTranslation(13.0, -6.80105529785156, 21.69905258178711, 2.86176727294922)
trackAnim.setKeyFrameRotationQuaternion( 13.0, -0.78195458650589, 0.0, 0.623332798480988, 0.0)
trackAnim.addKeyFrame( 14.0 );
trackAnim.setKeyFrameTranslation( 14.0, -5.60457214355469, 21.52052108764648, 2.32767608642578)
trackAnim.setKeyFrameRotationQuaternion( 14.0, -0.897179543972015, 0.0, 0.441661357879639, 0.0)
trackAnim.addKeyFrame( 15.0 );
trackAnim.setKeyFrameTranslation( 15.0, -4.50603240966797, 21.35660400390625, 1.34022109985352)
trackAnim.setKeyFrameRotationQuaternion( 15.0, -0.971691370010376, 0.0, 0.236243948340416, 0.0)
trackAnim.addKeyFrame( 16.0 );
trackAnim.setKeyFrameTranslation( 16.0, -4.02735565185547, 21.28517913818359, 0.259572200775146)
trackAnim.setKeyFrameRotationQuaternion( 16.0, -0.998850047588348, 0.0, 0.0478907190263271, 0.0)

trackAnim.setInterpolationMode( AnimationManager::IM_SPLINE );


trackAnim.setPaused( false );
