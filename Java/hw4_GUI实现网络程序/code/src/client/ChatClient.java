package src.client;
import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.util.HashMap;
import java.util.Map;
import java.io.*;

public class ChatClient extends JFrame {
    private static final int PORT = 12345;
    private JTextArea chatArea;
    private JTextField messageField;
    private JTextField usernameField;
    private JPasswordField passwordField;
    private PrintWriter writer;
    private BufferedReader reader;
    private Socket socket;
    private JList<String> userList;
    private DefaultListModel<String> userListModel;
    private Map<String, PrivateChatWindow> privateChats = new HashMap<>();
    private ObjectOutputStream out;
    private ObjectInputStream in;
    
    // 定义颜色常量
    private static final Color LIGHT_BLUE = new Color(230, 240, 255); // 浅蓝色
    private static final Color LIGHT_PINK = new Color(255, 240, 245); // 浅粉色
    private static final Color BUTTON_COLOR = new Color(180, 200, 255); // 浅蓝色
    private static final Color TEXT_COLOR = new Color(70, 70, 70); // 深灰色
    private static final Color USER_COLOR = new Color(255, 105, 180); // 深粉色
    private static final Color SERVER_COLOR = new Color(128, 0, 128); // 深紫色
    private static final Color LIGHT_PURPLE = new Color(230, 230, 250); // 淡紫色
    private static final Color LIGHT_YELLOW = new Color(255, 250, 205); // 淡黄色
    
    public ChatClient() {
        setTitle("聊天室");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(400, 600);
        setLocationRelativeTo(null);
        
        // 设置整体背景色
        getContentPane().setBackground(LIGHT_BLUE);
        
        // 登录面板
        JPanel mainLoginPanel = new JPanel(new GridBagLayout());
        mainLoginPanel.setBackground(LIGHT_BLUE);
        
        JPanel loginPanel = new JPanel(new GridBagLayout());
        loginPanel.setBackground(LIGHT_BLUE);
        loginPanel.setBorder(new EmptyBorder(20, 40, 20, 40));
        
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(5, 5, 5, 5);
        
        // 创建和设置组件
        JLabel titleLabel = new JLabel("欢迎登录聊天室", SwingConstants.CENTER);
        titleLabel.setFont(new Font("微软雅黑", Font.BOLD, 24));
        titleLabel.setForeground(TEXT_COLOR);
        
        usernameField = new JTextField(20);
        passwordField = new JPasswordField(20);
        styleTextField(usernameField);
        styleTextField(passwordField);
        
        JButton loginButton = new JButton("登录");
        JButton registerButton = new JButton("注册");
        styleButton(loginButton);
        styleButton(registerButton);
        
        // 添加组件
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        loginPanel.add(titleLabel, gbc);
        
        gbc.gridy = 1;
        gbc.gridwidth = 2;
        gbc.insets = new Insets(20, 5, 5, 5);
        loginPanel.add(createStyledLabel("用户名"), gbc);
        
        gbc.gridy = 2;
        gbc.insets = new Insets(5, 5, 15, 5);
        loginPanel.add(usernameField, gbc);
        
        gbc.gridy = 3;
        gbc.insets = new Insets(5, 5, 5, 5);
        loginPanel.add(createStyledLabel("密码"), gbc);
        
        gbc.gridy = 4;
        gbc.insets = new Insets(5, 5, 20, 5);
        loginPanel.add(passwordField, gbc);
        
        gbc.gridy = 5;
        gbc.gridwidth = 1;
        gbc.weightx = 0.5;
        gbc.insets = new Insets(5, 5, 5, 10);
        loginPanel.add(loginButton, gbc);
        
        gbc.gridx = 1;
        gbc.insets = new Insets(5, 10, 5, 5);
        loginPanel.add(registerButton, gbc);
        
        mainLoginPanel.add(loginPanel);
        getContentPane().add(mainLoginPanel);
        
        // 聊天面板
        chatArea = new JTextArea();
        chatArea.setEditable(false);
        chatArea.setFont(new Font("微软雅黑", Font.PLAIN, 14));
        chatArea.setBackground(LIGHT_PINK);
        chatArea.setForeground(TEXT_COLOR);
        
        messageField = new JTextField();
        styleTextField(messageField);
        
        JButton sendButton = new JButton("发送");
        styleButton(sendButton);
        
        // 添加事件监听
        loginButton.addActionListener(e -> login(false));
        registerButton.addActionListener(e -> login(true));
        sendButton.addActionListener(e -> sendMessage());
        messageField.addActionListener(e -> sendMessage());
        
        setVisible(true);
    }
    
    private void styleTextField(JTextField field) {
        field.setPreferredSize(new Dimension(200, 35));
        field.setFont(new Font("微软雅黑", Font.PLAIN, 14));
        field.setBorder(BorderFactory.createCompoundBorder(
            new LineBorder(new Color(200, 200, 200)),
            new EmptyBorder(5, 10, 5, 10)));
    }
    
    private void styleButton(JButton button) {
        button.setFont(new Font("微软雅黑", Font.BOLD, 14));
        button.setBackground(BUTTON_COLOR);
        button.setForeground(Color.WHITE);
        button.setBorder(new EmptyBorder(8, 20, 8, 20));
        button.setFocusPainted(false);
        button.setCursor(new Cursor(Cursor.HAND_CURSOR));
        
        button.addMouseListener(new MouseAdapter() {
            public void mouseEntered(MouseEvent e) {
                button.setBackground(BUTTON_COLOR.darker());
            }
            public void mouseExited(MouseEvent e) {
                button.setBackground(BUTTON_COLOR);
            }
        });
    }
    
    private JLabel createStyledLabel(String text) {
        JLabel label = new JLabel(text);
        label.setFont(new Font("微软雅黑", Font.PLAIN, 14));
        label.setForeground(TEXT_COLOR);
        return label;
    }
    
    private void login(boolean isRegister) {
        try {
            socket = new Socket("localhost", PORT);
            out = new ObjectOutputStream(socket.getOutputStream());
            out.flush();
            in = new ObjectInputStream(socket.getInputStream());
            
            String command = isRegister ? "REGISTER" : "LOGIN";
            out.writeObject(new ChatMessage("", command, 
                usernameField.getText() + "|" + new String(passwordField.getPassword())));
            out.flush();
            
            ChatMessage response = (ChatMessage) in.readObject();
            System.out.println("response: " + response.getType()); //
            if (response.getType().endsWith("SUCCESS")) {
                setupChatInterface();
            } else {
                JOptionPane.showMessageDialog(this, isRegister ? "注册失败" : "登录失败");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private void sendMessage() {
        String message = messageField.getText();
        if (!message.trim().isEmpty()) {
            try {
                out.writeObject(new ChatMessage(usernameField.getText(), "CHAT", message));
                out.flush();
                messageField.setText("");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    
    private void receiveMessages() {
        try {
            while (true) {
                ChatMessage message = (ChatMessage) in.readObject();
                SwingUtilities.invokeLater(() -> handleMessage(message));
            }
        } catch (Exception e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(this, "与服务器的连接已断开");
        }
    }
    
    private void handleMessage(ChatMessage message) {
        switch (message.getType()) {
            case "USERLIST":
                updateUserList(message.getContent().split(","));
                break;
            case "CHAT":
                displayMessage(message);
                break;
            case "PRIVATE":
                handlerPrivateMessage(message);
                break;
            case "LOGIN_SUCCESS":
                System.out.println("登录成功");
            case "REGISTER_SUCCESS":
                System.out.println("注册成功");
                setupChatInterface();
                break;
            case "LOGIN_FAILED":
                JOptionPane.showMessageDialog(this, "登录失败：用户名或密码错误");
                break;
            case "REGISTER_FAILED":
                JOptionPane.showMessageDialog(this, "注册失败：用户名已存在");
                break;
        }
    }
    
    private void updateUserList(String[] users) {
        userListModel.clear();
        for (String user : users) {
            userListModel.addElement(user);
        }
    }

    
    // 添加PrivateChatWindow内部类
    private class PrivateChatWindow extends JFrame {
        private JTextArea privateChatArea;
        private JTextField privateMessageField;
        private String targetUser;

        public PrivateChatWindow(String targetUser) {
            this.targetUser = targetUser;
            setTitle("与 " + targetUser + " 私聊");
            setSize(300, 400);
            setLayout(new BorderLayout());

            // 创建并设置私聊区域
            privateChatArea = new JTextArea();
            privateChatArea.setEditable(false);
            privateChatArea.setFont(new Font("微软雅黑", Font.PLAIN, 14));
            privateChatArea.setBackground(LIGHT_PINK);
            privateChatArea.setMargin(new Insets(10, 10, 10, 10));

            privateMessageField = new JTextField();
            styleTextField(privateMessageField);

            JButton privateSendButton = new JButton("发送");
            styleButton(privateSendButton);

            // 添加发送按钮事件
            privateSendButton.addActionListener(e -> sendPrivateMessage());
            privateMessageField.addActionListener(e -> sendPrivateMessage());

            add(new JScrollPane(privateChatArea), BorderLayout.CENTER);
            JPanel bottomPanel = new JPanel(new BorderLayout(5, 0));
            bottomPanel.setBackground(LIGHT_BLUE);
            bottomPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
            bottomPanel.add(privateMessageField, BorderLayout.CENTER);
            bottomPanel.add(privateSendButton, BorderLayout.EAST);
            add(bottomPanel, BorderLayout.SOUTH);

            // 窗口关闭时只隐藏不销毁
            setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
            setLocationRelativeTo(null);
            setVisible(true);
        }

        private void sendPrivateMessage() {
            String message = privateMessageField.getText();
            if (!message.trim().isEmpty()) {
                try {
                    out.writeObject(new ChatMessage(usernameField.getText(), "PRIVATE", 
                        targetUser + "|" + message));
                    out.flush();
                    privateMessageField.setText("");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        public void appendMessage(String sender, String message) {
            SwingUtilities.invokeLater(() -> {
                privateChatArea.append(sender + ": " + message + "\n");
                privateChatArea.setCaretPosition(privateChatArea.getDocument().getLength());
            });
        }
    }

    
    private void createPrivateChat(String targetUser) {
        // 如果已经存在与该用户的私聊窗口，则直接显示
        if (privateChats.containsKey(targetUser)) {
            privateChats.get(targetUser).setVisible(true);
            return;
        }
    
        PrivateChatWindow privateWindow = new PrivateChatWindow(targetUser);
        privateChats.put(targetUser, privateWindow);
    }

    private void handlerPrivateMessage(ChatMessage message) {
        String[] parts = message.getContent().split("\\|", 2);
        if (parts.length == 2) {
            String targetUser = parts[0];
            String content = parts[1];
            String sender = message.getSender();
            String chatPartner = sender.equals(usernameField.getText()) ? targetUser : sender;

            // 如果私聊窗口不存在，则创建一个
            if (!privateChats.containsKey(chatPartner)) {
                createPrivateChat(chatPartner);
            }

            // 显示消息在私聊窗口中
            PrivateChatWindow privateWindow = privateChats.get(chatPartner);
            privateWindow.setVisible(true);
            
            // 如果是自己发送的消息，显示"我"，否则显示发送者的用户名
            String displayName = sender.equals(usernameField.getText()) ? "我" : sender;
            privateWindow.appendMessage(displayName, content);
        }
    }
    
    private void displayMessage(ChatMessage message) {
        String sender = message.getSender();
        // 如果是自己发送的消息，显示"我"
        if (sender.equals(usernameField.getText())) {
            sender = "我";
        }
        chatArea.append(sender + ": " + message.getContent() + "\n");
        chatArea.setCaretPosition(chatArea.getDocument().getLength());
    }
    
    private void setupChatInterface() {
        getContentPane().removeAll();
        
        // 修改窗口大小为高度的4/3倍
        int height = getHeight();
        setSize((int)(height * 1.33), height);
        setLocationRelativeTo(null);
        
        // 创建用户列表
        userListModel = new DefaultListModel<>();
        userList = new JList<>(userListModel);
        userList.setCellRenderer(new UserListRenderer());
        userList.setBackground(LIGHT_PURPLE); // 设置用户列表背景色为淡紫色
        userList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    String selectedUser = userList.getSelectedValue();
                    if (selectedUser != null) {
                        createPrivateChat(selectedUser);
                    }
                }
            }
        });
        JScrollPane userListScroll = new JScrollPane(userList);
        userListScroll.setPreferredSize(new Dimension(150, 400));
        
        // 重新设计聊天界面
        JPanel chatPanel = new JPanel(new BorderLayout(0, 5));
        chatPanel.setBackground(LIGHT_BLUE);
        chatPanel.setBorder(new EmptyBorder(10, 10, 10, 10));
        
        chatArea.setMargin(new Insets(10, 10, 10, 10));
        JScrollPane scrollPane = new JScrollPane(chatArea);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        
        JPanel bottomPanel = new JPanel(new BorderLayout(5, 0));
        bottomPanel.setBackground(LIGHT_BLUE);
        bottomPanel.add(messageField, BorderLayout.CENTER);
        
        JButton sendButton = new JButton("发送");
        styleButton(sendButton);
        sendButton.addActionListener(e -> sendMessage());
        bottomPanel.add(sendButton, BorderLayout.EAST);
        
        chatPanel.add(scrollPane, BorderLayout.CENTER);
        chatPanel.add(bottomPanel, BorderLayout.SOUTH);
        
        // 添加用户列表和聊天面板
        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(userListScroll, BorderLayout.WEST);
        mainPanel.add(chatPanel, BorderLayout.CENTER);
        
        getContentPane().add(mainPanel);
        revalidate();
        repaint();
        
        // 启动接收消息的线程
        new Thread(this::receiveMessages).start();
    }
    
    private class UserListRenderer extends DefaultListCellRenderer {
        @Override
        public Component getListCellRendererComponent(JList<?> list, Object value, 
                int index, boolean isSelected, boolean cellHasFocus) {
            JLabel label = (JLabel) super.getListCellRendererComponent(
                list, value, index, isSelected, cellHasFocus);
            
            // 设置标签背景为淡紫色
            label.setBackground(LIGHT_PURPLE);
            if (!isSelected) {
                label.setBackground(LIGHT_PURPLE);
            }
            
            // 如果是当前用户，显示为"我"
            if (value.toString().equals(usernameField.getText())) {
                label.setText("我");
            }
            
            // 添加一些内边距使显示更美观
            label.setBorder(BorderFactory.createEmptyBorder(5, 10, 5, 10));
            
            return label;
        }
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new ChatClient());
    }
} 