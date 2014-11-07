
public class OompaloompaChocolateFactory  {
	private volatile static OompaloompaChocolateFactory uniqueInstance;
	private volatile static int id = 0; 
	private OompaloompaChocolateFactory() {}
	
	public static OompaloompaChocolateFactory getInstance() {
		if(uniqueInstance == null) {
			synchronized(OompaloompaChocolateFactory.class) {
				if(uniqueInstance == null) {
					uniqueInstance = new OompaloompaChocolateFactory();
				}
			}
		}
		return uniqueInstance;
	}
	
	public synchronized ChocolateBar createBar(){
		id++; 
		
		return new OompaloompaBar(id);
	}
	
	
}
