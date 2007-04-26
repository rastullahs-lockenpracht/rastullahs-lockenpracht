package meshhandle.data;

public class ColourValue {
    private float mG, mB, mR;

    public ColourValue(float r, float g, float b) {
        mR = r;
        mG = g;
        mB = b;
    }

    public float getG() {
        return mG;
    }

    public void setG(float x) {
        mG = x;
    }

    public float getB() {
        return mB;
    }

    public void setB(float y) {
        mB = y;
    }

    public String toXML(String nodeName) {
        return "<" + nodeName + " r=\"" + round(mR) + "\"" + " g=\""
                + round(mG) + "\"" + " b=\"" + round(mB) + "\"/>";
    }

    private float round(float y) {
        return Math.abs(y) < 1E-5 ? 0 : y;
    }

    public float getR() {
        return mR;
    }

    public void setR(float w) {
        mR = w;
    }

}
