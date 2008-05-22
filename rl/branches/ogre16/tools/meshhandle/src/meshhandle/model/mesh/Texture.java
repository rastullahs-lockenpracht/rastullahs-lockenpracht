package meshhandle.model.mesh;

public class Texture {
String alias;
String name;

public Texture(){
	this.alias="";
	this.name="";
}

public String getAlias() {
	return alias;
}
public String getName() {
	return name;
}
public void setAlias(String alias) {
	this.alias = alias;
}
public void setName(String name) {
	this.name = name;
}
}
