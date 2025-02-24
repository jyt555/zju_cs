package src.server;

import java.net.*;
import java.io.*;
import src.client.ChatMessage;
import src.database.DatabaseUtil;

public class ClientHandler implements Runnable {
    private Socket socket;
    private ChatServer server;
    private ObjectOutputStream out;
    private ObjectInputStream in;
    private String username;
    
    public ClientHandler(Socket socket, ChatServer server) {
        this.socket = socket;
        this.server = server;
        try {
            out = new ObjectOutputStream(socket.getOutputStream());
            out.flush();
            in = new ObjectInputStream(socket.getInputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    public String getUsername() {
        return username;
    }
    
    public void sendMessage(ChatMessage message) {
        try {
            out.writeObject(message);
            System.out.println("发送消息: " + message.getType());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    @Override
    public void run() {
        try {
            while (true) {
                ChatMessage message = (ChatMessage) in.readObject();
                switch (message.getType()) {
                    case "LOGIN":
                        handleLogin(message);
                        break;
                    case "REGISTER":
                        handleRegister(message);
                        break;
                    case "CHAT":
                        server.broadcast(new ChatMessage(username, "CHAT", message.getContent()));
                        break;
                    case "PRIVATE":
                        handlePrivateMessage(message);
                        break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (username != null) {
                server.removeClient(this);
            }
        }
    }
    
    private void handleLogin(ChatMessage message) throws IOException {
        String[] credentials = message.getContent().split("\\|");
        if (credentials.length == 2 && DatabaseUtil.validateUser(credentials[0], credentials[1])) {
            username = credentials[0];
            System.out.println("新用户登录: " + username);
            sendMessage(new ChatMessage("SERVER", "LOGIN_SUCCESS", ""));
            server.broadcast(new ChatMessage("SERVER", "CHAT", username + " 加入了聊天室"));
            server.updateUserList();
        } else {
            sendMessage(new ChatMessage("SERVER", "LOGIN_FAILED", ""));
        }
    }
    
    private void handleRegister(ChatMessage message) throws IOException {
        String[] credentials = message.getContent().split("\\|");
        if (credentials.length == 2 && DatabaseUtil.registerUser(credentials[0], credentials[1])) {
            username = credentials[0];
            System.out.println("新用户注册: " + username);
            sendMessage(new ChatMessage("SERVER", "REGISTER_SUCCESS", ""));
            server.broadcast(new ChatMessage("SERVER", "CHAT", username + " 加入了聊天室"));
            server.updateUserList();
        } else {
            sendMessage(new ChatMessage("SERVER", "REGISTER_FAILED", ""));
        }
    }
    
    private void handlePrivateMessage(ChatMessage message) {
        String[] parts = message.getContent().split("\\|", 2);
        if (parts.length == 2) {
            String targetUser = parts[0];
            String content = parts[1];
            server.sendPrivateMessage(username, targetUser, content);
        }
    }
} 