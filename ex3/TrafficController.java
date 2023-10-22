import java.util.concurrent.Semaphore;
public class TrafficController {
  private Semaphore semaforo;
  
  public TrafficController(){
    this.semaforo = new Semaphore(1, true);
  }
  
  public void enterLeft() {
    try{
      semaforo.acquire();
    } catch (InterruptedException e){
      System.err.println("Excecao");
    }
  }
  
  public void enterRight() {
    try{
      semaforo.acquire();
    } catch (InterruptedException e){
      System.err.println("Excecao");
    }
  }
  
  public void leaveLeft() {
    semaforo.release();
  }

  public void leaveRight() {
    semaforo.release();
  }
}

