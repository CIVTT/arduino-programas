package poro;

import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Button;

import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableModel;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.Table;

/*
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import java.io.OutputStream;
import java.util.Enumeration;
import javax.swing.JOptionPane;
import org.eclipse.swt.widgets.Table;
*/

public class solo {
	private Text text;
	private Text text_1;
	private Text text_2;
	private Text text_3;
	private Text text_4;
	private Text text_5;
	private Text text_6;
	private Table table;

	/**
	 * Launch the application.
	 * @param args
	 */
	// Variables de conexión.
    /*private OutputStream output = null;
    SerialPort serialPort;
    private final String puerto; //= "/dev/ttyUSB0";
    private BufferedReader input;
	private OutputStream output;
    private static final int TIMEOUT = 2000; // 2 segundos.
    private static final int DATA_RATE = 115200; // Baudios.
    implements SerialPortEventListener
    */
	


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
		shlMuestreoDeData.setLayout(null);
		
		Composite composite = new Composite(shlMuestreoDeData, SWT.BORDER);
		composite.setBounds(108, 54, 142, 142);
		composite.setLayout(null);
		
		Label lblNewLabel = new Label(composite, SWT.NONE);
		lblNewLabel.setBounds(24, 10, 105, 17);
		lblNewLabel.setText("roll, picht y yaw");
		
		text = new Text(composite, SWT.BORDER);
		text.setBounds(59, 41, 81, 29);
		
		text_1 = new Text(composite, SWT.BORDER);
		text_1.setBounds(59, 76, 81, 29);
		
		text_2 = new Text(composite, SWT.BORDER);
		text_2.setBounds(59, 111, 81, 29);
		
		Label lblRoll = new Label(composite, SWT.NONE);
		lblRoll.setBounds(10, 41, 33, 17);
		lblRoll.setText("roll:");
		
		Label lblPicth = new Label(composite, SWT.NONE);
		lblPicth.setBounds(10, 76, 43, 17);
		lblPicth.setText("picth:");
		
		Label lblYaw = new Label(composite, SWT.NONE);
		lblYaw.setBounds(10, 109, 33, 17);
		lblYaw.setText("yaw:");
		
		Button btnNewButton_1 = new Button(shlMuestreoDeData, SWT.NONE);
		btnNewButton_1.setBounds(252, 217, 67, 29);
		btnNewButton_1.setText("autoset");
		
		Button btnNewButton = new Button(shlMuestreoDeData, SWT.BORDER);
		btnNewButton.setBounds(456, 217, 45, 29);
		btnNewButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
			
				
			}
		});
		btnNewButton.setText("stop");
		
		Composite composite_1 = new Composite(shlMuestreoDeData, SWT.BORDER);
		composite_1.setBounds(311, 54, 136, 140);
		
		text_3 = new Text(composite_1, SWT.BORDER);
		text_3.setBounds(50, 41, 81, 29);
		
		text_4 = new Text(composite_1, SWT.BORDER);
		text_4.setBounds(50, 76, 81, 29);
		
		text_5 = new Text(composite_1, SWT.BORDER);
		text_5.setBounds(50, 109, 81, 29);
		
		Label lblX = new Label(composite_1, SWT.NONE);
		lblX.setBounds(22, 41, 23, 17);
		lblX.setText("x:");
		
		Label lblY = new Label(composite_1, SWT.NONE);
		lblY.setBounds(21, 76, 23, 17);
		lblY.setText("y:");
		
		Label lblZ = new Label(composite_1, SWT.NONE);
		lblZ.setBounds(21, 109, 23, 17);
		lblZ.setText("z:");
		
		Label lblNewLabel_1 = new Label(composite_1, SWT.NONE);
		lblNewLabel_1.setBounds(10, 10, 113, 17);
		lblNewLabel_1.setText("angulos de euler");
		
		Composite composite_2 = new Composite(shlMuestreoDeData, SWT.BORDER);
		composite_2.setBounds(503, 109, 136, 64);
		
		Label lblHm = new Label(composite_2, SWT.NONE);
		lblHm.setText("h:");
		lblHm.setBounds(10, 41, 22, 17);
		
		Label lblMetros = new Label(composite_2, SWT.NONE);
		lblMetros.setText("m");
		lblMetros.setBounds(96, 41, 28, 17);
		
		text_6 = new Text(composite_2, SWT.BORDER);
		text_6.setBounds(34, 33, 56, 29);
		
		Label lblAltura = new Label(composite_2, SWT.NONE);
		lblAltura.setBounds(34, 10, 56, 17);
		lblAltura.setText("altura");
		
		table = new Table(shlMuestreoDeData, SWT.BORDER | SWT.FULL_SELECTION);
		table.setBounds(58, 252, 629, 142);
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		// Arreglo del tipo Object
		final Object []object = new Object[6];
				
				// Creamos el modelo
		final DefaultTableModel modelo;
						
				// Inicializamos el modelo
		modelo = new DefaultTableModel();
						
				// Vinculamos nuestro modelo a la tabla
		//table.setMenu(modelo);
						
				// Columnas de la tabla
		modelo.addColumn("#");
		modelo.addColumn("Tipo");
		modelo.addColumn("Número Origen");
		modelo.addColumn("Número Destino");
		modelo.addColumn("Duración");
		modelo.addColumn("Franja");
		modelo.addColumn("Precio");
		
		shlMuestreoDeData.open();
		shlMuestreoDeData.layout();
		while (!shlMuestreoDeData.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
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
//}

