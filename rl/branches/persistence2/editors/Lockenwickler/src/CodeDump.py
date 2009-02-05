    #THIS IS A GENERAL CODE DUMB FOR DEPRECATED CODE


    # parses the moduleconfig.rb and searches for all loader.loadmap(" statements in the ruby script
    # and parses them too

class old():



    def setResourcePaths(self, path, moduleName):
        for file in os.listdir(path):
            curFile = path + "/" + file

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if isdir(curFile):
                og.ResourceGroupManager.getSingleton().addResourceLocation(curFile, "FileSystem", moduleName, False)
                self.setResourcePaths(curFile, moduleName)
                continue
            if isfile(curFile):
                pass



    def load(self,  moduleName,  mapFiles,  gofFiles):
        self.moduleName = moduleName
        self.mapFiles = mapFiles
        self.gofFiles = gofFiles

        self.gocManager.parseGOFFiles(self.gofFiles)

        for a in self.mapFiles:
            doc = xml.parse(a)
            node = doc.getElementsByTagName("entity")
            if node != None:
                self.parseSceneNodes(node)

        self.pivot = Pivot(self.sceneManager)
        self.pivot.hide()

    def parseSceneNodes(self,  nodeList):
        for ent in nodeList:
            entityName = ent.attributes["name"].nodeValue # get the name of the ent
            meshFile = ent.attributes["meshfile"].nodeValue # meshfile
            nodePosition = None
            nodeRotation = None
            nodeScale = None

            for cn in ent.childNodes:
                if cn.nodeType == cn.ELEMENT_NODE:
                    if cn.localName == "position":
                        px = float(cn.attributes["x"].nodeValue)
                        py = float(cn.attributes["y"].nodeValue)
                        pz = float(cn.attributes["z"].nodeValue)
                        nodePosition = og.Vector3(px, py, pz)
                        continue

                    if cn.localName == "rotation":
                        qw = float(cn.attributes["qw"].nodeValue)
                        qx = float(cn.attributes["qx"].nodeValue)
                        qy = float(cn.attributes["qy"].nodeValue)
                        qz = float(cn.attributes["qz"].nodeValue)
                        nodeRotation = og.Quaternion(qw,  qx, qy, qz)
                        continue

                    if cn.localName == "scale":
                        px = float(cn.attributes["x"].nodeValue)
                        py = float(cn.attributes["y"].nodeValue)
                        pz = float(cn.attributes["z"].nodeValue)
                        nodeScale = og.Vector3(px, py, pz)
                        continue

            try:
                e = self.sceneManager.createEntity(entityName, meshFile)
            except:
                print "Warning: Meshfile " + meshFile + " could not be found."
                return

            n = self.sceneManager.getRootSceneNode().createChild(entityName + "_node")
            n.attachObject(e)
            n.setPosition(nodePosition)
            #n.setOrientation(nodeRotation)
            n.setScale(nodeScale)

        pass
