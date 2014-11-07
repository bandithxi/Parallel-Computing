
public class OompMachine extends Thread{
	
	private String name;
	private int quantity;
	
	public OompMachine(String name, int quantity){
		this.name = name;
		this.quantity = quantity;
	}
	
	
	public void run() {
		int i = 0;
		while (i++ < this.quantity) {
			try {
				sleep(100);
			} catch (InterruptedException e) {}
		
			ChocolateBar var = OompaloompaChocolateFactory.getInstance().createBar();
			System.out.println(name + " Created " + var.getName() + " with id: " + var.getId());
		}
	}
}
