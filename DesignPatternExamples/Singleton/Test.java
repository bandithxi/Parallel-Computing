
public class Test {
	
	public static void main(String[] args) {
		OompMachine o1 = new OompMachine("Machine 1", 10);
		OompMachine o2 = new OompMachine("Machine 2", 5);

		WonkaMachine w1 = new WonkaMachine("Machine 3", 5);
		WonkaMachine w2 = new WonkaMachine("Machine 4", 10);
		
		o1.start();
		//one.interrupt();
		o2.start();

		
		
		w1.start();
		
		w2.start();
		
	}

}
