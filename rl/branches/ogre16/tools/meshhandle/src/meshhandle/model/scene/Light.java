package meshhandle.model.scene;

import meshhandle.data.ColourValue;
import meshhandle.data.Vector3;

public class Light extends SceneNode
{
	public static class SpotLightSettings
	{
		private Float mInnerAngle;
		private Float mOuterAngle;
		private Float mFallOff;

		public Float getInnerAngle()
		{
			return mInnerAngle;
		}

		public void setInnerAngle(Float innerAngle)
		{
			mInnerAngle = innerAngle;
		}

		public Float getOuterAngle()
		{
			return mOuterAngle;
		}

		public void setOuterAngle(Float outerAngle)
		{
			mOuterAngle = outerAngle;
		}

		public Float getFallOff()
		{
			return mFallOff;
		}

		public void setFallOff(Float fallOff)
		{
			mFallOff = fallOff;
		}

		public String toXML()
		{
			return "<spotlightrange inner=\"" + mInnerAngle + "\" outer=\""
					+ mOuterAngle + "\" falloff=\"" + mFallOff + "\" />";
		}
	}

	private Boolean mCastShadows;
	private ColourValue mDiffuseColour;
	private ColourValue mSpecularColour;
	private String mType;
	private SpotLightSettings mSpotLightSettings;
	private Vector3 mDirection;
	private LightAttenuation mAttenuation;
	private Boolean mVisible;

	public Light(String name)
	{
		super(name);
	}

	public void setCastShadows(boolean cast)
	{
		mCastShadows = cast;
	}

	public Boolean getCastShadows()
	{
		return mCastShadows;
	}

	public ColourValue getDiffuseColour()
	{
		return mDiffuseColour;
	}

	public void setDiffuseColour(ColourValue diffuseColour)
	{
		mDiffuseColour = diffuseColour;
	}

	public ColourValue getSpecularColour()
	{
		return mSpecularColour;
	}

	public void setSpecularColour(ColourValue specularColour)
	{
		mSpecularColour = specularColour;
	}

	public String getType()
	{
		return mType;
	}

	public void setType(String type)
	{
		mType = type;
	}

	public SpotLightSettings getSpotLightSettings()
	{
		return mSpotLightSettings;
	}

	public void setSpotLightSettings(SpotLightSettings spotLightSettings)
	{
		mSpotLightSettings = spotLightSettings;
	}

	public Vector3 getDirection()
	{
		return mDirection;
	}

	public void setDirection(Vector3 direction)
	{
		mDirection = direction;
	}

	public void setCastShadows(Boolean castShadows)
	{
		mCastShadows = castShadows;
	}

	public LightAttenuation getAttenuation()
	{
		return mAttenuation;
	}

	public void setAttenuation(LightAttenuation att)
	{
		mAttenuation = att;
	}

	public void setVisible(boolean visible)
	{
		mVisible = visible;		
	}
	
	public Boolean getVisible()
	{
		return mVisible;
	}
}
