package meshhandle.model.scene;

public class LightAttenuation
{
	private Float mRange;
	private Float mConstantParameter;
	private Float mLinearParameter;
	private Float mQuadraticParameter;

	public Float getRange()
	{
		return mRange;
	}

	public void setRange(Float range)
	{
		mRange = range;
	}

	public Float getConstantParameter()
	{
		return mConstantParameter;
	}

	public void setConstantParameter(Float constantParameter)
	{
		mConstantParameter = constantParameter;
	}

	public Float getLinearParameter()
	{
		return mLinearParameter;
	}

	public void setLinearParameter(Float linearParameter)
	{
		mLinearParameter = linearParameter;
	}

	public Float getQuadraticParameter()
	{
		return mQuadraticParameter;
	}

	public void setQuadraticParameter(Float quadraticParameter)
	{
		mQuadraticParameter = quadraticParameter;
	}

	public String toXML(String nodename)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<lightAttenuation");
		buf.append(" range=\"" + mRange + "\"");
		buf.append(" constant=\"" + mConstantParameter + "\"");
		buf.append(" linear=\"" + mLinearParameter + "\"");
		buf.append(" quadratic=\"" + mQuadraticParameter + "\"");
		buf.append("/>");
		return buf.toString();
	}
	

}
