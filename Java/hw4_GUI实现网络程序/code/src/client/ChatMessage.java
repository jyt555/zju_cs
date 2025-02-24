package src.client;

import java.io.Serializable;

public class ChatMessage implements Serializable {
    private String sender;
    private String type;
    private String content;
    
    public ChatMessage(String sender, String type, String content) {
        this.sender = sender;
        this.type = type;
        this.content = content;
    }
    
    public String getSender() { return sender; }
    public String getType() { return type; }
    public String getContent() { return content; }
}
 