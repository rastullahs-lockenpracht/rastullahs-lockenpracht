#!/usr/bin/bash
# Erstes Argument: Skalierungsfaktor
# Zweites Argument: Zielverzeichnis (muss bereits exisiteren)
find . -name "*.skeleton" -exec OgreXmlConverter \{\} \;
find . -name "*.mesh" -exec OgreXmlConverter \{\} \;

find . -maxdepth 1 -name "*.skeleton.xml" -print -exec java -cp . meshhandle.SkeletonScaler \{\} $1 $2/\{\} \;
find . -maxdepth 1 -name "*.mesh.xml" -print -exec java -cp . meshhandle.MeshScaler \{\} $1 $2/\{\} \;
find . -maxdepth 1 -name "*.scene" -print -exec java -cp . meshhandle.SceneScaler \{\} $1 $2/\{\} \;

cd $2
find . -name "*.skeleton.xml" -exec OgreXmlConverter \{\} \;
find . -name "*.mesh.xml" -exec OgreXmlConverter \{\} \;
cd ..
