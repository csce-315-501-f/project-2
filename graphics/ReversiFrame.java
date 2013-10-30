

// Import the basic graphics classes.
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.net.*;
import java.io.*;
import java.lang.*;

public class ReversiFrame extends JFrame{

    private ImageIcon black;
    private ImageIcon white;
    private ImageIcon dot;
    private JButton[][] buttons;
    private Socket socket;
    private BufferedReader in;
    private PrintWriter out;
    private JPanel game;
    private JPanel options;
    private JPanel results;
    private static String host;
    private static int port;
    private static String diff;
    private JTextField sname;
    private JTextField pname;
    private JComboBox dname;
    //private JTextField winbox;
    //private JTextField tiebox;
    //private JTextField losebox;

    public void updateButtons() {
        String str = null;
        try {
            str = in.readLine();
            while (!(str.contains("OK") || str.contains("ILLEGAL"))) {
                System.out.println(str);
                if (str.contains("Game started"))break;
                str = in.readLine();
            }
            if (str.contains("ILLEGAL")) in.readLine();
            out.println("RDISPLAY");
            System.out.println(str + "\nPRINT");
            String[] board = new String[8];
            for(int k = 0; k < 8; k++) {
                board[k] = in.readLine();
                System.out.println(board[k]);
            }
            if (board[0].contains("won")) {
                out.println("A1");
                updateButtons();
                JOptionPane.showMessageDialog(this,"YOU WON!");
            }
            else if (board[0].contains("lost")) {
                out.println("A1");
                updateButtons();
                JOptionPane.showMessageDialog(this,"you lost");
            }
            else if (board[0].contains("tie")) {
                out.println("A1");
                updateButtons();
                JOptionPane.showMessageDialog(this,"You tied!");
            }
            else {
                for(int k = 0; k < 8; k++) 
                    for(int n = 0; n < 8; n++)
                        switch (board[k].charAt(n)) {
                            case '.':
                                buttons[n][k].setIcon(null);
                                break;
                            case 'M':
                                buttons[n][k].setIcon(dot);
                                break;
                            case 'B':
                                buttons[n][k].setIcon(black);
                                break;
                            case 'W':
                                buttons[n][k].setIcon(white);
                                break;
                        }
            }
        }
        catch (Exception er) {}
    }

    private void setupConn() {

        try {
            socket.close();
        }
        catch (Exception e) {}
        try {
            socket = new Socket(host, port);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream(),true);
        }
        catch (UnknownHostException e) {
        }
        catch (Exception e) {}
        finally {}

        try{
            System.out.println(in.readLine());
            out.println(diff);
            System.out.println(in.readLine());
            out.println("HUMAN-AI");
            System.out.println(in.readLine());
        }
        catch(Exception e) {}

    }

    public ReversiFrame(){
        super("Reversi");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();


        JPanel options = new JPanel();
        options.setLayout(new GridBagLayout());
        c.gridx = 0;
        c.gridy = 1;
        c.gridwidth = 3;
        c.gridheight = 1;
        c.fill = GridBagConstraints.BOTH;
        JButton ok = new JButton("Start New Game");
        ok.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                host = sname.getText();
                port = Integer.parseInt(pname.getText());
                diff = (String)dname.getSelectedItem();
                setupConn();
                updateButtons();
            }
        });
        options.add(ok,c);
        sname = new JTextField("unix.cse.tamu.edu");
        c.gridx = 0;
        c.gridy = 0;
        c.gridwidth = 1;
        c.gridheight = 1;
        options.add(sname,c);
        c.gridx = 1;
        c.gridy = 0;
        c.gridwidth = 1;
        c.gridheight = 1;
        pname = new JTextField("8888");
        options.add(pname,c);
        String[] diffs = {"EASY","MEDIUM","HARD"};
        c.gridx = 2;
        c.gridy = 0;
        c.gridwidth = 1;
        c.gridheight = 1;
        dname = new JComboBox(diffs);
        options.add(dname,c);

        c.gridx = 0;
        c.gridy = 9;
        c.gridwidth = 8;
        c.gridheight = 1;
        c.fill = GridBagConstraints.BOTH;
        add(options,c);
        
        setupConn();

        JPanel game = new JPanel();
        game.setLayout(new GridLayout(8,8));
        buttons = new JButton[8][8];
        black = new ImageIcon("reversi_black.jpg");
        white = new ImageIcon("reversi_white.jpg");
        dot = new ImageIcon("dot.jpg");

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++) {
                buttons[i][j] = new JButton();
                buttons[i][j].setPreferredSize(new Dimension(60,60));
                final int ii = i;
                final int jj = j;
                buttons[i][j].addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        out.println((char)(jj+65)+""+(ii+1));
                        updateButtons();
                    }
                });
            }

        buttons[3][4].setIcon(black);
        buttons[4][3].setIcon(black);
        buttons[3][3].setIcon(white);
        buttons[4][4].setIcon(white);

        
        /*
        results = new JPanel();
        results.setLayout(new GridBagLayout());
        winbox = new JTextField("YOU WON!");
        winbox.setVisible(true);
        tiebox = new JTextField("YOU tied!");
        tiebox.setVisible(true);
        losebox = new JTextField("you lost.");
        losebox.setVisible(true);
        c.gridx = 0;
        c.gridy = 0;
        c.gridwidth = 1;
        c.gridheight = 1;
        results.add(winbox,c);
        results.add(tiebox,c);
        results.add(losebox,c);
        results.setVisible(true);
        c.gridx = 0;
        c.gridy = 11;
        c.gridwidth = 8;
        c.gridheight = 2;
        add(results,c);*/

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++) {
                game.add(buttons[i][j]);
            }
        c.gridx = 0;
        c.gridy = 0;
        c.gridwidth = 8;
        c.gridheight = 8;
        c.weightx = 1;
        c.weighty = 1;
        c.fill = GridBagConstraints.NONE;
        add(game,c);

    }

    public static void main(String arg[]) {
        host = "unix.cse.tamu.edu";
        port = 8888;
        diff = "easy";
        ReversiFrame frame = new ReversiFrame();
        frame.setLocation(500,500);
        frame.setSize(460,460);
        frame.setResizable(false);
        frame.setVisible(true);
        frame.updateButtons();
    }
}
