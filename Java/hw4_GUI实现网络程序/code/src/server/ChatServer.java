package src.server;
import java.net.*;
import java.io.*;
import java.util.*;

import src.server.ClientHandler;
import src.client.ChatMessage;

public class ChatServer {
    private static final int PORT = 12345;
    private List<ClientHandler> clients = new ArrayList<>();
    private ServerSocket serverSocket;
    
    public static void main(String[] args) {
        try {
            ChatServer chatServer = new ChatServer();
            chatServer.serverSocket = new ServerSocket(PORT);
            System.out.println("聊天服务器启动在端口: " + PORT);
            chatServer.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    public void start() {
        try {
            while (true) {
                Socket socket = serverSocket.accept();
                // System.out.println("新客户端连接: " + socket.getInetAddress());
                ClientHandler clientHandler = new ClientHandler(socket, this);
                clients.add(clientHandler);
                new Thread(clientHandler).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    public void broadcast(ChatMessage message) {
        List<ClientHandler> clientsCopy = new ArrayList<>(clients);
        for (ClientHandler client : clientsCopy) {
            client.sendMessage(message);
        }
    }
    
    public synchronized void removeClient(ClientHandler client) {
        clients.remove(client);
        System.out.println("当前在线用户数: " + clients.size());
        updateUserList();
    }
    
    public synchronized void updateUserList() {
        List<String> usernames = new ArrayList<>();
        List<ClientHandler> clientsCopy = new ArrayList<>(clients);
        for (ClientHandler client : clientsCopy) {
            if (client.getUsername() != null) {
                usernames.add(client.getUsername());
            }
        }
        broadcast(new ChatMessage("SERVER", "USERLIST", String.join(",", usernames)));
    }
    
    public void sendPrivateMessage(String sender, String targetUser, String content) {
        List<ClientHandler> clientsCopy = new ArrayList<>(clients);
        for (ClientHandler client : clientsCopy) {
            if (client.getUsername() != null && 
                (client.getUsername().equals(targetUser) || client.getUsername().equals(sender))) {
                client.sendMessage(new ChatMessage(sender, "PRIVATE", targetUser + "|" + content));
            }
        }
    }
} 