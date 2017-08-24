import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Enumeration;

import java.util.Enumeration;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
      
    public class trigo extends JFrame implements ActionListener{
        Enumeration puertos_libres;
        CommPortIdentifier port;
        SerialPort puerto_ser;
        private OutputStream out;
        private BufferedReader in;
        private static final int TIME_OUT = 2000;
        private static final int DATA_RATE = 9600;

        private JButton b1;
        private JTextField campo1;
        private JTextField campo2;
        private JLabel etq1;
        private JLabel etq2;
        private Panel panelEntrada, panelSalida, panelcentral;
        private JPanel panelDeLaVentana;
     
        public trigo(){
            //Creamos el boton
            //JFrame 
            b1 = new JButton("conectar");
            //Registramos a la ventana como oyente
            b1.addActionListener(this);
            
            //Creamos las etiquetas
            etq1 = new JLabel("Puerto: ");
          
            etq2 = new JLabel("Resultado: ");
     
          //Creamos los campos de Texto
            campo1 = new JTextField();
            campo2 = new JTextField();
            
          //Cambiamos la propiedades de los TextFields
            campo2.setEditable(false);
            campo2.setColumns(10);
            campo2.setBackground(Color.lightGray);
            campo1.setColumns(10);
            
          //Creamos los paneles auxiliares
            panelEntrada = new Panel();
            panelcentral = new Panel();
            panelSalida = new Panel();
          //Obtenemos la referencia al panel principal
            panelDeLaVentana = (JPanel)this.getContentPane();
            panelDeLaVentana.setLayout(new GridLayout(1,3));
          //Agregamos los componentes del panel de entrada
            panelEntrada.add(campo1,BoxLayout.X_AXIS);
            panelEntrada.add(etq1,BoxLayout.X_AXIS);

            panelcentral.add(b1,BoxLayout.X_AXIS);
          //Agregamos los componentes del panel de salida
            panelSalida.add(campo2,BoxLayout.X_AXIS);
            panelSalida.add(etq2,BoxLayout.X_AXIS);
            //panelSalida.setVisible(true);
            //Agregamos todo al panel Principal
            //panelEntrada.setBounds(250,50,100,100);
            //b1.setBounds(500,100,100,50);
            //panelSalida.setBounds(500,500,100,100);
            //panelDeLaVentana.add(panelEntrada,BorderLayout.NORTH);
            //panelDeLaVentana.add(b1,BorderLayout.CENTER);
            //panelDeLaVentana.add(panelSalida,BorderLayout.SOUTH);
            panelDeLaVentana.add(panelEntrada);
            panelDeLaVentana.add(panelcentral);
            panelDeLaVentana.add(panelSalida);
        }
     
        public void actionPerformed(ActionEvent e){
            puertos_libres = CommPortIdentifier.getPortIdentifiers();
                int aux=0;
                while (puertos_libres.hasMoreElements())
                    {
                     port = (CommPortIdentifier) puertos_libres.nextElement();
                     int type = port.getPortType();
                     if (port.getName().equals(campo1.getText()))
                     {
                            try {
                                puerto_ser = (SerialPort) port.open(this.getClass().getName(), 2000);
                                   int baudRate = 9600; // 9600bps
                                   //configuracion de arduino
                                    puerto_ser.setSerialPortParams(
                                            baudRate,
                                            SerialPort.DATABITS_8,
                                            SerialPort.STOPBITS_1,
                                            SerialPort.PARITY_NONE);
                                    puerto_ser.setDTR(true);
                 ////////////////////////////////////////////////////////////////
                                    out = puerto_ser.getOutputStream();//salida de java
                                    in = new BufferedReader(new InputStreamReader(puerto_ser.getInputStream()));; // entrada de java

                            } catch (  Exception e1) {
                            }
 
                         break;
                     }
                    }
        }
     
        public static void main(String[] arg){
            trigo miAplicacion = new trigo();
            miAplicacion.setTitle("   Muestreo de sensor   ");
            miAplicacion.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            miAplicacion.setBounds(50,50,500,500);
            miAplicacion.pack();
            miAplicacion.setVisible(true);
        }
}   