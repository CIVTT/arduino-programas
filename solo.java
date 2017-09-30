
import java.awt.EventQueue;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.*;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;
import javax.swing.JScrollPane;
import javax.swing.JPanel;
import javax.swing.JFrame;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
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
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JFileChooser;

import java.util.Enumeration;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import jxl.write.*;
import jxl.*;
      
public class trigo extends JFrame{
    Enumeration puertos_libres;
    CommPortIdentifier port;
    SerialPort puerto_ser;
    Enumeration listport;
    CommPortIdentifier idport;
    /*
    */
        private OutputStream out;
        private BufferedReader in;
        private static final int TIME_OUT = 2000;
        private static final int DATA_RATE = 9600;

        private JButton b1,b3;
        private JToggleButton b2;
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
        Thread capture;
        private File files;
        DefaultTableModel modelo;
        String dat;
        public trigo(){
            //Creamos el boton
            //JFrame 
            //Objet[][] dato={};
            //listaport=CommPortIdentifier.getPortIdentifiers();
            //listapuerto();
            t = new Thread(new datarecivida());
            capture =new Thread(new capturastabla());
            String[] columnaprincipal={"time","roll","picth","yao","alpha","beta","gama","altura"};
            Object[][] filas={{"time","roll","picth","yao","alpha","beta","gama","altura"},{"1","2","3","4","5","6","7","8"}};
             modelo = new DefaultTableModel(filas,columnaprincipal);
            tabla = new JTable (modelo);
            JScrollPane scrollpane = new JScrollPane(tabla);
            tabla.setFocusable(false);
            //Registramos a la ventana como oyente
            b1 = new JButton("conectar");
            b1.setBounds(50,5,100,30);
            //b1.addActionListener(this);
            b2 = new JToggleButton("almacenar");
            b2.setBounds(400,5,100,30);
            b3=new JButton("exportar");
            b3.setBounds(600,5,100,30);
            //Creamos las etiquetas
            etq1 = new JLabel("Puerto: ");
            etq1.setBounds(200,5,100,30);
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
            campo1.setBounds(250,5,100,30);
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
            roll.setFocusable(false);
            picth.setColumns(5);
            picth.setFocusable(false);
            yao.setColumns(5);
            yao.setFocusable(false);
            a.setColumns(5);
            a.setFocusable(false);
            b.setColumns(5);
            b.setFocusable(false);
            c.setColumns(5);
            c.setFocusable(false);
            h.setColumns(5);
            h.setFocusable(false);
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
            aux2.add(scrollpane);
            panelbotones.add(b1);
            panelbotones.add(b2);
            panelbotones.add(b3);
            panelbotones.add(etq1);
            panelbotones.add(campo1);
            panelDeLaVentana.add(aux1);
            panelDeLaVentana.add(aux2);
            panelDeLaVentana.add(panelbotones);
            b1.addActionListener(new ActionListener(){
            //super();
            public void actionPerformed(ActionEvent ee){
                dat="Sen4=29";
                try {
                    t.start();
                    capture.start();
                    capture.suspend();
                }catch(Exception e24){

                }}});
                /*puertos_libres = CommPortIdentifier.getPortIdentifiers();
                int aux=0;
                //t=new Thread(new datarecivida());
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
                                    //t.start();
                            } catch (  Exception e1) {
                            }
                         break;
                     }}}*/
            b2.addItemListener(new ItemListener(){
               public void itemStateChanged(ItemEvent e){
                    //AbstractButton abstractButton = (AbstractButton) e.getSource();
                    //boolean truue = abstractButton.getModel().isSelected();
                    //while(truue){
                    int state = e.getStateChange();
                    if (state==ItemEvent.SELECTED){
                        b2.setText("parar");
                        capture.resume();
                    }
                    else{
                        //break;
                        b2.setText("almacenar");
                        capture.suspend();
                    }
                //}
                //while(true){ 
                    /*AbstractButton abstractButton = (AbstractButton) e.getSource();
                    boolean selected = abstractButton.getModel().isSelected();
                    System.out.println("Action - selected=" + selected + "\n");*/
                    //System.out.println("botonb2");
                    
                    //Thread.sleep(100);

                 //   }
                }});
            b3.addActionListener(new ActionListener(){
               public void actionPerformed(ActionEvent e1){
                
                
            JFileChooser chooser = new JFileChooser();
            FileNameExtensionFilter filter = new FileNameExtensionFilter("Archivos de excel", "xls");
            chooser.setFileFilter(filter);
            chooser.setDialogTitle("Guardar archivo");
            chooser.setAcceptAllFileFilterUsed(false);
            if (chooser.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
                List<JTable> tb = new ArrayList<JTable>();
                List<String> nom = new ArrayList<String>();
                tb.add(tabla);
                nom.add("Compras por factura");
                String file = chooser.getSelectedFile().toString().concat(".xls");
                try {
                    Exporter e2 = new Exporter(new File(file), tb, nom);
                    if (e2.export()) {
                        JOptionPane.showMessageDialog(null, "Los datos fueron exportados a excel en el directorio seleccionado", "Mensaje de Informacion", JOptionPane.INFORMATION_MESSAGE);
                    }
                } catch (Exception e2) {
                    JOptionPane.showMessageDialog(null, "Hubo un error " + e2.getMessage(), " Error", JOptionPane.ERROR_MESSAGE);
                }}
    
                
                    System.out.println("interesante");
                    //Thread.sleep(100);
                 //   }
                }});
    }
        
        public class datarecivida implements Runnable{
        //public synchronized void serialEvent(SerialPortEvent oEvent) {

            
            //if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            public void run(){
                while(true){
                try {
                //dat=in.readLine();
                String[] parts=dat.split("=");
               String part1=parts[0];
               String part2=parts[1];

               if(part1.equalsIgnoreCase("Sen1")){
                   roll.setText(part2);
                   }
               if(part1.equalsIgnoreCase("Sen2")){
                   picth.setText(part2);
                   }
               if(part1.equalsIgnoreCase("Sen3")){
                   yao.setText(part2);
                   }
               if(part1.equalsIgnoreCase("Sen4")){
                   a.setText(part2);
                   }
               if(part1.equalsIgnoreCase("Sen5")){
                   b.setText(part2);
                   }
               if(part1.equalsIgnoreCase("Sen6")){
                   c.setText(part2);
                   }
               if(part1.equalsIgnoreCase("Sen7")){
                   alt.setText(part2);
                }
            }catch (Exception e1){
                }
    }}}
/*        public void listapuerto(){
            String lista="";
            lista +="Los puertos disponibles son:";
            while (listaPort.hasMoreElements()){
                idPort = (CommPortIdentifier) listaPort.nextElement();
                lista +="PUERTO: " + idPort.getName() + " ";
                }
        }*/
        public class capturastabla implements Runnable{
                            public void run(){
                                while(true){
                                try{
                                objectaux[0]=roll.getText();
                                objectaux[1]=picth.getText();
                                objectaux[2]=yao.getText();
                                objectaux[3]=a.getText();
                                objectaux[4]=b.getText();
                                objectaux[5]=c.getText();
                                objectaux[6]=h.getText();
                                modelo.addRow(objectaux);
                                capture.sleep(200);
                            }catch(Exception e3){
                                //System.out.println("look");
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
