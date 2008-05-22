package meshhandle.io.xml;

import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import meshhandle.model.skeleton.Animation;
import meshhandle.model.skeleton.AnimationLink;
import meshhandle.model.skeleton.AnimationTrack;
import meshhandle.model.skeleton.Bone;
import meshhandle.model.skeleton.Keyframe;
import meshhandle.model.skeleton.Skeleton;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class SkeletonLoader extends XMLLoader {
    public Skeleton readSkeleton(String filename) throws SAXException,
            ParserConfigurationException, IOException {
        Document document = readDocument(filename);

        Skeleton skel = new Skeleton();

        processBones(skel, (Element) document.getElementsByTagName("bones")
                .item(0));
        processBoneHierarchy(skel, (Element) document.getElementsByTagName(
                "bonehierarchy").item(0));
        processAnimations(skel, (Element) document.getElementsByTagName(
        		"animations").item(0));        
        processAnimationLinks(skel, (Element) document.getElementsByTagName(
        		"animationlinks").item(0));

        return skel;
    }

	private void processBones(Skeleton skel, Element bonesNode) {
        NodeList bonesList = bonesNode.getElementsByTagName("bone");
        for (int idx = 0; idx < bonesList.getLength(); idx++) {
            Element boneNode = (Element) bonesList.item(idx);
            Bone bone = new Bone(Integer.parseInt(boneNode.getAttributes()
                    .getNamedItem("id").getNodeValue()), boneNode
                    .getAttributes().getNamedItem("name").getNodeValue());
            bone.setPosition(processVector3(boneNode
                    .getElementsByTagName("position").item(0)));
            bone.setRotationAxis(processVector3(boneNode
                    .getElementsByTagName("axis").item(0)));
            bone.setRotationAngle(Float.parseFloat(boneNode
                    .getElementsByTagName("rotation").item(0).getAttributes()
                    .getNamedItem("angle").getNodeValue()));
            skel.addBone(bone);
        }
    }

    private void processBoneHierarchy(Skeleton skel,
            Element hierarchyNode) {
        NodeList parentRelList = hierarchyNode
                .getElementsByTagName("boneparent");
        for (int idx = 0; idx < parentRelList.getLength(); idx++) {
            Node boneParentNode = parentRelList.item(idx);
            NamedNodeMap attrs = boneParentNode.getAttributes();
            skel.addBoneHierarchyRelation(attrs.getNamedItem("bone")
                    .getNodeValue(), attrs.getNamedItem("parent")
                    .getNodeValue());
        }
    }

    private void processAnimations(Skeleton skel, Element animationsNode) {
    	if (animationsNode != null)
    	{
	        NodeList animationList = animationsNode
	                .getElementsByTagName("animation");
	        for (int idx = 0; idx < animationList.getLength(); idx++) {
	            Element animation = (Element) animationList.item(idx);
	            Animation anim = new Animation(animation.getAttribute("name"),
	                    Float.parseFloat(animation.getAttribute("length")));
	            skel.addAnimation(anim);
	            processAnimationTracks(anim, animation
	                    .getElementsByTagName("track"));
	        }
    	}

    }

    private void processAnimationTracks(Animation anim,
            NodeList trackList) {
        for (int idx = 0; idx < trackList.getLength(); idx++) {
            Element trackElem = (Element) trackList.item(idx);
            AnimationTrack track = new AnimationTrack(trackElem
                    .getAttribute("bone"));
            anim.addTrack(track);
            processKeyFrames(track, trackElem.getElementsByTagName("keyframe"));
        }
    }

    private void processKeyFrames(AnimationTrack track,
            NodeList keyframeList) {
        for (int idx = 0; idx < keyframeList.getLength(); idx++) {
            Element keyframeElem = (Element) keyframeList.item(idx);

            Keyframe keyframe = new Keyframe();
            keyframe.setTime(Float
                    .parseFloat(keyframeElem.getAttribute("time")));

            Element rotateElem = (Element) keyframeElem.getElementsByTagName(
                    "rotate").item(0);

            if (rotateElem != null) {
                keyframe.setRotationAngle(Float.parseFloat(rotateElem
                        .getAttributes().getNamedItem("angle").getNodeValue()));
                keyframe.setRotationAxis(processVector3(keyframeElem
                        .getElementsByTagName("axis").item(0)));
            }

            Element translateElem = (Element) keyframeElem
                    .getElementsByTagName("translate").item(0);

            if (translateElem != null) {
                keyframe.setTranslation(processVector3(translateElem));
            }

            Element scaleElem = (Element) keyframeElem.getElementsByTagName(
                    "scale").item(0);

            if (scaleElem != null) {
                keyframe.setScale(processVector3(scaleElem));
            }
            track.addKeyframe(keyframe);
        }
    }

    private void processAnimationLinks(Skeleton skel, Element animationLinksElement) {
    	if (animationLinksElement != null)
    	{
    		NodeList keyframeList = animationLinksElement.getElementsByTagName("animationlink");
    		
	    	for (int idx = 0; idx < keyframeList.getLength(); idx++) {
	            Element keyframeElem = (Element) keyframeList.item(idx);
	            String skeletonName = keyframeElem.getAttribute("skeletonName");
	            String scale = keyframeElem.getAttribute("scale");
	            
	            AnimationLink link;
				if (scale == null)
	            	link = new AnimationLink(skeletonName);
	            else
	            	link = new AnimationLink(skeletonName, Float.parseFloat(scale));
	            
	            skel.addAnimationLink(link);
	    	}
    	}
    }
}

