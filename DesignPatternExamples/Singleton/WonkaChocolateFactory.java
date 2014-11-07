
public class WonkaChocolateFactory{
	private volatile static WonkaChocolateFactory uniqueInstance;
	private volatile static int id = 0; 
	
	private WonkaChocolateFactory() {}
	
	public static WonkaChocolateFactory getInstance() {
		if(uniqueInstance == null) {
			synchronized(WonkaChocolateFactory.class) {
				if(uniqueInstance == null) {
					uniqueInstance = new WonkaChocolateFactory();
				}
			}
		}
		return uniqueInstance;
	}
	
	public synchronized ChocolateBar createBar(){
		id++; 
		
		return new WonkaBar(id);
	}
	
}
