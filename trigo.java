//https://www.udoo.org/forum/threads/eclipse-with-udoobuntu-and-arduino-communication-tutorial.1512/
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;
import javax.swing.JScrollPane;
import javax.swing.JPanel;
import javax.swing.JFrame;
import java.awt.*;
import java.awt.event.*;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Enumeration;
import java.util.*;
//import javax.comm.*;

import java.util.Enumeration;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
      
public class trigo extends JFrame implements ActionListener{
    Enumeration puertos_libres;
    CommPortIdentifier port;
    SerialPort puerto_ser;
    Enumeration listport;
    CommPortIdentifier idport;
        private OutputStream out;
        private BufferedReader in;
        private static final int TIME_OUT = 2000;
        private static final int DATA_RATE = 9600;

        private JButton b1,b2,b3;
        private JTextField campo1,roll,picth,yao;
        private JTextField a,b,c,h;
        //private JTextField ;
        private JLabel etq1;
        private JLabel etq_picth, etq_roll, etq_yao;
        private JLabel eu_a,eu_b,eu_c,alt;
        private Panel panelbotones, panelcentral,aux1,aux2,aux3,aux4;
        private JPanel panelDeLaVentana;
        private JTable tabla;
        private JMenuBar menus;
        private JMenu file,conect;
        private JMenu port_com;
        Object[] objectaux= new Object[8];
        Thread t;


        public trigo(){
            //Creamos el boton
            //JFrame 
            //Objet[][] dato={};
            listaport=CommPortIdentifier.getPortIdentifiers();
            listapuerto();
            String[] columnaprincipal={"time","roll","picth","yao","alpha","beta","gama","altura"};
            Object[][] filas={{"time","roll","picth","yao","alpha","beta","gama","altura"},{"1","2","3","4","5","6","7","8"}};
            DefaultTableModel modelo = new DefaultTableModel(filas,columnaprincipal);
            tabla = new JTable (modelo);
            JScrollPane scrollpane = new JScrollPane(tabla);
            modelo.addRow(objectaux);
            //Registramos a la ventana como oyente
            b1 = new JButton("conectar");
            b1.setBounds(50,5,100,30);
            b1.addActionListener(this);
            b2 = new JButton("almacenar");
            b2.setBounds(400,5,100,30);
            b3=new JButton("exportar");
            b3.setBounds(600,5,100,30);
            //Creamos las etiquetas
            etq1 = new JLabel("Puerto: ");
            etq_roll =new JLabel("roll: ");
            etq_picth =new JLabel("picth: ");
            etq_yao =new JLabel("yao: ");
            eu_a=new JLabel("x: ");
            eu_b=new JLabel("y: ");
            eu_c=new JLabel("z: ");
            alt=new JLabel("h: ");
            //campo de barra de menu
            menus =new JMenuBar();
            setJMenuBar(menus);
            file=new JMenu("file");
            menus.add(file);
            conect=new JMenu("conectar");
            menus.add(conect);
            port_com=new JMenu("puertos");
            conect.add(port_com);
          //Creamos los campos de Texto
            campo1 = new JTextField();
            roll = new JTextField();
            picth = new JTextField();
            yao = new JTextField();
            a=new JTextField();
            b=new JTextField();
            c=new JTextField();
            h=new JTextField();
          //Cambiamos la propiedades de los TextFields
            campo1.setColumns(5);
            roll.setColumns(5);
            picth.setColumns(5);
            yao.setColumns(5);
            a.setColumns(5);
            b.setColumns(5);
            c.setColumns(5);
            h.setColumns(5);
          //Obtenemos la referencia al panel principal
            panelDeLaVentana = (JPanel)this.getContentPane();
            panelDeLaVentana.setLayout(null);            
          //Creamos los paneles auxiliares
            panelbotones = new Panel();
            panelbotones.setLayout(null);
            panelbotones.setBounds(200,320,700,100);
            panelcentral = new Panel();
            aux1=new Panel();
            aux1.setBounds(200,100,200,200);
            aux1.setLayout(new GridLayout(7,2));
            aux2=new Panel();
            aux2.setBounds(450,100,700,200);
            aux2.setLayout(null);
            tabla.setBounds(10,10,600,190);
            
            aux4=new Panel();
            aux4.setBounds(350,600,300,300);

            aux1.add(etq_roll);
            aux1.add(roll);
            aux1.add(etq_picth);
            aux1.add(picth);
            aux1.add(etq_yao);
            aux1.add(yao);
            aux1.add(eu_a);
            aux1.add(a);
            aux1.add(eu_b);
            aux1.add(b);
            aux1.add(eu_c);
            aux1.add(c);
            aux1.add(alt);
            aux1.add(h);
            aux2.add(tabla);
            panelbotones.add(b1);
            panelbotones.add(b2);
            panelbotones.add(b3);
            panelDeLaVentana.add(aux1);
            panelDeLaVentana.add(aux2);
            panelDeLaVentana.add(panelbotones);
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
        
        public class datarecivida implements Runnable{
            String dat;
            
            public void run(){

                while(true){
                    try{
                        dat=in.readLine();
                        //Thread.sleep(100);
                        String[] parts=dat.split("=");
                        String part1=parts[0];
                        String part2=parts[1];
                        if(parts[0].equalsIgnoreCase("Sensor1")){
                            etq_roll.setText(parts[1]);
                            objectaux[0]=etq_roll.getText();
                            }
                        if(parts[0].equalsIgnoreCase("Sensor2")){
                            etq_picth.setText(parts[1]);
                            objectaux[1]=etq_picth.getText();
                            }
                        if(parts[0].equalsIgnoreCase("Sensor3")){
                            etq_yao.setText(parts[1]);
                            objectaux[2]=etq_yao.getText();
                            }
                        if(parts[0].equalsIgnoreCase("Sensor1")){
                            eu_a.setText(parts[1]);
                            objectaux[3]=eu_a.getText();
                            }
                        if(parts[0].equalsIgnoreCase("Sensor2")){
                            eu_b.setText(parts[1]);
                            objectaux[5]=eu_b.getText();
                            }
                        if(parts[0].equalsIgnoreCase("Sensor3")){
                            eu_c.setText(parts[1]);
                            objectaux[6]=eu_c.getText();
                            }
                        if(parts[0].equalsIgnoreCase("Sensor3")){
                            alt.setText(parts[1]);
                            objectaux[7]=alt.getText();
                            }
                        
                    }catch (Exception e1){
                    }
                }
            }


        }
        public void listapuerto(){
            String lista="";
            lista +="Los puertos disponibles son:";
            while (listaPort.hasMoreElements()){
                idPort = (CommPortIdentifier) listaPort.nextElement();
                lista +="PUERTO: " + idPort.getName() + " ";

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
