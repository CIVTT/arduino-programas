import javax.swing.table.DefaultTableModel;

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
        static BufferedReader in;
        private static final int TIME_OUT = 2000;
        private static final int DATA_RATE = 9600;

        private JButton b1;
        private JTextField campo1,roll,picth,yao;
        private JTextField a,b,c;
        //private JTextField ;
        private JLabel etq1;
        private JLabel etq_picth, etq_roll, etq_yao,alt;
        private JLabel eu_a,eu_b,eu_c,h;
        private Panel panelEntrada, panelcentral,aux1,aux2,aux3,aux4;

        private JPanel panelDeLaVentana;
        Thread t;
        public trigo(){
            //Creamos el boton
            //JFrame 
            b1 = new JButton("conectar");
            //Registramos a la ventana como oyente
            b1.addActionListener(this);
            
            //Creamos las etiquetas
            etq1 = new JLabel("Puerto: ");
            etq_roll =new JLabel("roll: ");
            etq_picth =new JLabel("picth: ");
            etq_yao =new JLabel("yao: ");
            eu_a=new JLabel("x: ");
            eu_b=new JLabel("y: ");
            eu_c=new JLabel("z: ");

          //Creamos los campos de Texto
            campo1 = new JTextField();
            roll = new JTextField();
            picth = new JTextField();
            yao = new JTextField();
            a=new JTextField();
            b=new JTextField();
            c=new JTextField();

          //Cambiamos la propiedades de los TextFields
            campo1.setColumns(5);
            roll.setColumns(5);
            picth.setColumns(5);
            yao.setColumns(5);
            a.setColumns(5);
            b.setColumns(5);
            c.setColumns(5);
          //Creamos los paneles auxiliares
            panelEntrada = new Panel();
            panelcentral = new Panel();
            aux1=new Panel();
            aux2=new Panel();
            aux3=new Panel();
              aux4=new Panel();
          //Obtenemos la referencia al panel principal
            panelDeLaVentana = (JPanel)this.getContentPane();
            panelEntrada.setLayout(new BoxLayout(panelEntrada,BoxLayout.Y_AXIS));
          //Agregamos los componentes del panel de entrada

            aux1.setLayout(new FlowLayout());
            aux1.add(etq1);
            aux1.add(campo1);
            aux1.add(b1);

            aux2.setLayout(new FlowLayout());
            aux2.add(etq_roll);
            aux2.add(roll);
            aux2.add(etq_picth);
            aux2.add(picth);
            aux2.add(etq_yao);
            aux2.add(yao);

            aux3.setLayout(new FlowLayout());
            aux3.add(eu_a);
            aux3.add(a);
            aux3.add(eu_b);
            aux3.add(b);
            aux3.add(eu_c);
            aux3.add(c);
            aux4.add(h);
            
            panelEntrada.add(aux1);
            panelEntrada.add(aux2);
            panelEntrada.add(aux3);
            panelEntrada.add(aux4);
            panelDeLaVentana.add(panelEntrada);
            panelDeLaVentana.add(panelcentral);
            
        }
     
        public void actionPerformed(ActionEvent e){
            //super();
            puertos_libres = CommPortIdentifier.getPortIdentifiers();
                int aux=0;
                t=new Thread(new datarecivida());
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
                                    //out = puerto_ser.getOutputStream();//salida de java
                                    in = new BufferedReader(new InputStreamReader(puerto_ser.getInputStream()));; // entrada de java
                                    t.start();

                            } catch (  Exception e1) {
                            }
 
                         break;
                     }
                    }
        }
        
        public static class datarecivida implements Runnable{
            int dat;
            public void run(){
                while(true){
                    try{
                        String dat=in.readLine();
                        //Thread.sleep(100);
                        String[] parts=dat.split("=");
                        String part1=parts[0];
                        String part2=parts[1];
                        if(parts[0].equalsIgnoreCase("Sensor1")){
                            etq_roll.setText(parts[1]);
                            
                            }
                        if(parts[0].equalsIgnoreCase("Sensor2")){
                            etq_picth.setText(parts[1]);
                            
                            }
                        if(parts[0].equalsIgnoreCase("Sensor3")){
                            etq_yao.setText(parts[1]);
                            
                            }
                        if(parts[0].equalsIgnoreCase("Sensor1")){
                            eu_a.setText(parts[1]);
                            
                            }
                        if(parts[0].equalsIgnoreCase("Sensor2")){
                            eu_b.setText(parts[1]);
                            
                            }
                        if(parts[0].equalsIgnoreCase("Sensor3")){
                            eu_c.setText(parts[1]);
                            
                            }
                    }catch (Exception e1){
                    }
                }
            }


        }
        public static void main(String[] arg){
            trigo miAplicacion = new trigo();
            miAplicacion.setTitle("   Muestreo de sensor   ");
            miAplicacion.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            miAplicacion.setBounds(50,50,600,800);
            miAplicacion.pack();
            miAplicacion.setVisible(true);
        }
}   
