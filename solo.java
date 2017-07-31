package poro;

import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
//
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import java.io.OutputStream;
import java.util.Enumeration;
import javax.swing.JOptionPane;
import org.eclipse.swt.widgets.Table;
//

public class solo {
	private Text text;
	private Text text_1;
	private Text text_2;
	private Text text_3;
	private Text text_4;
	private Text text_5;
	private Text text_6;

	/**
	 * Launch the application.
	 * @param args
	 */
	// Variables de conexión.
    private OutputStream output = null;
    SerialPort serialPort;
    private final String puerto; //= "/dev/ttyUSB0";
    private BufferedReader input;
	private OutputStream output;
    private static final int TIMEOUT = 2000; // 2 segundos.
    private static final int DATA_RATE = 115200; // Baudios.
    implements SerialPortEventListener
    private Table table;


	public static void main(String[] args) {
		try {
			solo window = new solo();
			window.open();
		} catch (Exception e) {
			e.printStackTrace();		
		}
	}

	/**
	 * Open the window.
	 */
	public void open() {
		Display display = Display.getDefault();
		Shell shlMuestreoDeData = new Shell();
		shlMuestreoDeData.setText("Muestreo de data mpu5060");
		
		Composite composite = new Composite(shlMuestreoDeData, SWT.BORDER);
		composite.setBounds(44, 26, 157, 180);
		
		Button btnNewButton = new Button(composite, SWT.BORDER);
		btnNewButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
			
				
			}
		});
		
		searchForPorts();
		connect();
		if (connected == true){
			if (initIOStream() == true){
				initListener();
			}	
		}
				
		btnNewButton.setBounds(59, 33, 54, 29);
		btnNewButton.setText("show");
		
		Label lblNewLabel = new Label(composite, SWT.NONE);
		lblNewLabel.setBounds(24, 10, 105, 17);
		lblNewLabel.setText("roll, picht y yaw");
		
		text = new Text(composite, SWT.BORDER);
		text.setBounds(59, 68, 81, 29);
		
		text_1 = new Text(composite, SWT.BORDER);
		text_1.setBounds(59, 103, 81, 29);
		
		text_2 = new Text(composite, SWT.BORDER);
		text_2.setBounds(59, 138, 81, 29);
		
		Label lblRoll = new Label(composite, SWT.NONE);
		lblRoll.setBounds(10, 68, 33, 17);
		lblRoll.setText("roll:");
		
		Label lblPicth = new Label(composite, SWT.NONE);
		lblPicth.setBounds(10, 103, 43, 17);
		lblPicth.setText("picth:");
		
		Label lblYaw = new Label(composite, SWT.NONE);
		lblYaw.setBounds(10, 136, 33, 17);
		lblYaw.setText("yaw:");
		
		Composite composite_1 = new Composite(shlMuestreoDeData, SWT.BORDER);
		composite_1.setBounds(44, 224, 157, 180);
		
		Button btnNewButton_1 = new Button(composite_1, SWT.NONE);
		btnNewButton_1.setBounds(43, 33, 54, 29);
		btnNewButton_1.setText("show");
		
		Label lblNewLabel_1 = new Label(composite_1, SWT.NONE);
		lblNewLabel_1.setBounds(21, 10, 113, 17);
		lblNewLabel_1.setText("angulos de euler");
		
		text_3 = new Text(composite_1, SWT.BORDER);
		text_3.setBounds(53, 68, 81, 29);
		
		text_4 = new Text(composite_1, SWT.BORDER);
		text_4.setBounds(53, 103, 81, 29);
		
		text_5 = new Text(composite_1, SWT.BORDER);
		text_5.setBounds(53, 136, 81, 29);
		
		Label lblX = new Label(composite_1, SWT.NONE);
		lblX.setBounds(25, 68, 23, 17);
		lblX.setText("x:");
		
		Label lblY = new Label(composite_1, SWT.NONE);
		lblY.setBounds(24, 103, 23, 17);
		lblY.setText("y:");
		
		Label lblZ = new Label(composite_1, SWT.NONE);
		lblZ.setBounds(24, 136, 23, 17);
		lblZ.setText("z:");
		
		Composite composite_2 = new Composite(shlMuestreoDeData, SWT.BORDER);
		composite_2.setBounds(44, 425, 157, 72);
		
		Label lblAltura = new Label(composite_2, SWT.NONE);
		lblAltura.setText("altura");
		lblAltura.setBounds(52, 10, 56, 17);
		
		Label lblHm = new Label(composite_2, SWT.NONE);
		lblHm.setText("h:");
		lblHm.setBounds(10, 41, 22, 17);
		
		Label lblMetros = new Label(composite_2, SWT.NONE);
		lblMetros.setText("metros.");
		lblMetros.setBounds(96, 41, 51, 17);
		
		text_6 = new Text(composite_2, SWT.BORDER);
		text_6.setBounds(34, 33, 56, 29);
		
		table = new Table(shlMuestreoDeData, SWT.BORDER | SWT.FULL_SELECTION);
		table.setBounds(271, 68, 366, 338);
		table.setHeaderVisible(true);
		table.setLinesVisible(true);

		shlMuestreoDeData.open();
		shlMuestreoDeData.layout();
		while (!shlMuestreoDeData.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
	}
//-------------------------comunicacion ardino java-----------------------
	public void searchForPorts(){
		System.out.println("Puertos Disponibles:");
		ports = CommPortIdentifier.getPortIdentifiers();
		while (ports.hasMoreElements()){
			CommPortIdentifier curPort = (CommPortIdentifier)ports.nextElement();
			if (curPort.getPortType() == CommPortIdentifier.PORT_SERIAL){
			System.out.println(curPort.getName());
			portMap.put(curPort.getName(), curPort);
			}
		}
		System.out.println("----------------------");
	}
	public void connect(){
		//String puerto = "COM4";
		selectedPortIdentifier = (CommPortIdentifier)portMap.get(puerto);

		CommPort commPort = null;

		try{
			commPort = selectedPortIdentifier.open("Send Sms Java", TIMEOUT);
			serialPort = (SerialPort)commPort;
			setSerialPortParameters();
			connected = true;
			System.out.println("conectado exitosamente a puerto "+puerto);
		}catch (PortInUseException e){
			System.out.println("Puerto en uso.");
		}catch (Exception e){
			System.out.println("Error al abrir puerto.");
		}
	}
	private void setSerialPortParameters() throws IOException {
		//int baudRate = 115200;
		try {
			serialPort.setSerialPortParams(DATA_RATE,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
			serialPort.setFlowControlMode(
			SerialPort.FLOWCONTROL_NONE);
		} catch (UnsupportedCommOperationException ex) {
			throw new IOException("Unsupported serial port parameter");
		}
	}
	public boolean initIOStream(){
		boolean successful = false;
		try {
				//
			input = serialPort.getInputStream();
			output = serialPort.getOutputStream();
			writeData("HELLO");

			successful = true;
			return successful;
		}catch(IOException e) {
			System.out.println("Error al abrir Stream.");
			return successful;
		}
	}
	public void initListener(){
		try{
			serialPort.addEventListener(this);
			serialPort.notifyOnDataAvailable(true);
			System.out.println("Listo..");
		}catch(TooManyListenersException e){
			System.out.println("Demasiados escuchas.");
		}
	}
	public void writeData(String aenviar){
		try {
				output.write(aenviar.getBytes());
			} catch (IOException ex) {
			System.out.println("Error al enviar informacion.");
		}
	}
	@Override
	public void serialEvent(SerialPortEvent spe) {
		if (spe.getEventType() == SerialPortEvent.DATA_AVAILABLE){
			byte[] readBuffer = new byte[20];
			try {
				while (input.available() > 0) {
					int numBytes = input.read(readBuffer);
				}
				System.out.print(new String(readBuffer));
			} catch (IOException e) {System.out.println(e);}
		}
	}
	public void disconnect(){
		try{
			//writeData("GOODBYE");
			serialPort.removeEventListener();
			serialPort.close();
			input.close();
			output.close();
			connected = false;
			//System.out.println("Desconectado.");
		}catch (Exception e){
			System.out.println("Error al desconectar.");
		}
	}
/**
*searchForPorts();
*connect();
*if (connected == true){
*if (initIOStream() == true){
*initListener();
*writeData("comandos a puerto serial");
*/
//---------------------------------------------------------

}
